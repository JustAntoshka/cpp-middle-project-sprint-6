#include "thread_pool/thread_pool.hpp"

namespace dispatcher::thread_pool {

ThreadPool::ThreadPool(std::size_t thread_count, std::shared_ptr<dispatcher::queue::PriorityQueue> queue)
    : queue_(std::move(queue)) {

    workers_.reserve(thread_count);

    for (std::size_t i = 0; i < thread_count; ++i) {
        workers_.emplace_back(&ThreadPool::worker, this);
    }
}

ThreadPool::~ThreadPool() {
    if (queue_) {
        queue_->shutdown();
    }

    for (auto &w : workers_) {
        if (w.joinable()) {
            w.join();
        }
    }
}

void ThreadPool::worker() {
    while (true) {
        auto task = queue_->pop();
        if (!task) {
            return;
        }
        (*task)();
    }
}

}  // namespace dispatcher::thread_pool