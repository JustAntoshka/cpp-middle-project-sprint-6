#pragma once

#include "queue/priority_queue.hpp"
#include <memory>

namespace dispatcher::thread_pool {

class ThreadPool {
public:
    ThreadPool(std::size_t thread_count, std::shared_ptr<dispatcher::queue::PriorityQueue> queue);

    ~ThreadPool();

private:
    void worker();

private:
    std::shared_ptr<dispatcher::queue::PriorityQueue> queue_;
    std::vector<std::thread> workers_;
};

}  // namespace dispatcher::thread_pool