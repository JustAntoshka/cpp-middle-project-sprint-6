#include "queue/bounded_queue.hpp"
#include <mutex>
#include <optional>

namespace dispatcher::queue {

BoundedQueue::BoundedQueue(int capacity) : capacity_(capacity) {}

BoundedQueue::~BoundedQueue() = default;

void BoundedQueue::push(Task task) {
    std::unique_lock lock{mtx_};

    not_full_.wait(lock, [this]() { return queue_.size() < capacity_; });

    queue_.push(std::move(task));
}

std::optional<Task> BoundedQueue::try_pop() {
    std::lock_guard lock(mtx_);

    if (queue_.empty()) {
        return std::nullopt;
    }

    auto task = std::move(queue_.front());
    queue_.pop();

    not_full_.notify_one();

    return task;
}

}  // namespace dispatcher::queue