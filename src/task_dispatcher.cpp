#include "task_dispatcher.hpp"

namespace dispatcher {

TaskDispatcher::TaskDispatcher(std::size_t thread_count, std::map<TaskPriority, queue::QueueOptions> options) {
    queue_ = std::make_shared<queue::PriorityQueue>(options);
    thread_pool_ = std::make_unique<thread_pool::ThreadPool>(thread_count, queue_);
}

TaskDispatcher::~TaskDispatcher() = default;

void TaskDispatcher::schedule(TaskPriority priority, std::function<void()> task) {
    queue_->push(priority, std::move(task));
}

}  // namespace dispatcher
