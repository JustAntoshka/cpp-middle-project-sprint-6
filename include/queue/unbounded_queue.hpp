#pragma once
#include "queue/queue.hpp"
#include <mutex>
#include <queue>

namespace dispatcher::queue {

class UnboundedQueue : public IQueue {
private:
    std::queue<Task> queue_;
    std::mutex mtx_;

public:
    explicit UnboundedQueue(int capacity);

    void push(Task task) override;

    std::optional<Task> try_pop() override;

    ~UnboundedQueue() override;
};

}  // namespace dispatcher::queue
