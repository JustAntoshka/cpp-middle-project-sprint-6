#include "queue/priority_queue.hpp"
#include <optional>

namespace dispatcher::queue {

PriorityQueue::PriorityQueue(const std::map<TaskPriority, QueueOptions> &options) {
    for (const auto &[priority, opts] : options) {
        if (opts.bounded) {
            if (!opts.capacity.has_value()) {
                throw std::invalid_argument("Bounded queue requires capacity");
            }
            queues_[priority] = std::make_unique<BoundedQueue>(*opts.capacity);
        } else {
            queues_[priority] = std::make_unique<UnboundedQueue>(opts.capacity.value_or(0));
        }
    }
}

PriorityQueue::~PriorityQueue() { shutdown(); }

void PriorityQueue::push(TaskPriority priority, Task task) {
    {
        std::lock_guard lock(mtx_);

        auto it = queues_.find(priority);
        if (it == queues_.end()) {
            throw std::runtime_error("Unknown task priority");
        }

        it->second->push(std::move(task));
    }

    cv_.notify_one();
}

std::optional<Task> PriorityQueue::pop() {
    std::unique_lock lock(mtx_);

    auto try_pop_task = [this]() -> std::optional<Task> {
        for (const auto &[prio, queue] : queues_) {
            if (auto task = queue->try_pop()) {
                return task;
            }
        }
        return std::nullopt;
    };

    while (true) {
        auto task = try_pop_task();

        if (shutdown_.load()) {
            return task;
        } else if (task) {
            return task;
        } else {
            cv_.wait(lock);
        }
    }
}

void PriorityQueue::shutdown() {
    shutdown_.store(true);
    cv_.notify_all();
}

}  // namespace dispatcher::queue