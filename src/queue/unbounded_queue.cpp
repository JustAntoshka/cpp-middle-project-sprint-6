#include "queue/unbounded_queue.hpp"

#include <functional>
#include <mutex>
#include <queue>
#include <semaphore>

namespace dispatcher::queue {

UnboundedQueue::UnboundedQueue(int /* capacity */) {}

UnboundedQueue::~UnboundedQueue() = default;

void UnboundedQueue::push(Task task) {
    std::lock_guard lock(mtx_);
    queue_.push(std::move(task));
}

std::optional<Task> UnboundedQueue::try_pop() {
    std::lock_guard lock(mtx_);

    if (queue_.empty()) {
        return std::nullopt;
    }

    auto task = std::move(queue_.front());
    queue_.pop();
    return task;
}

}  // namespace dispatcher::queue