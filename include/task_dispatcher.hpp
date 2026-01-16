#pragma once

#include <memory>

#include "queue/priority_queue.hpp"
#include "thread_pool/thread_pool.hpp"
#include "types.hpp"

namespace dispatcher {

class TaskDispatcher {
private:
    std::shared_ptr<queue::PriorityQueue> queue_;
    std::unique_ptr<thread_pool::ThreadPool> thread_pool_;

public:
    explicit TaskDispatcher(std::size_t thread_count,
                            std::map<TaskPriority, queue::QueueOptions> options = {
                                {TaskPriority::High, queue::QueueOptions{true, 1000}},
                                {TaskPriority::Normal, queue::QueueOptions{false, std::nullopt}},
                            });

    void schedule(TaskPriority priority, std::function<void()> task);
    ~TaskDispatcher();
};

}  // namespace dispatcher
