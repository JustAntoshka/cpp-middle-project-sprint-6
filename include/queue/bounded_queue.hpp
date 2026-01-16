#pragma once
#include "queue/queue.hpp"
#include <condition_variable>
#include <queue>

namespace dispatcher::queue {

class BoundedQueue : public IQueue {
public:
    explicit BoundedQueue(int capacity);

    void push(Task task) override;

    std::optional<Task> try_pop() override;

    ~BoundedQueue() override;

private:
    int capacity_;
    std::queue<Task> queue_;
    mutable std::mutex mtx_;
    std::condition_variable not_full_;
};

}  // namespace dispatcher::queue