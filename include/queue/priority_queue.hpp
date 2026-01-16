#pragma once
#include "queue/bounded_queue.hpp"
#include "queue/unbounded_queue.hpp"
#include "types.hpp"

#include <atomic>
#include <condition_variable>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <unordered_map>

namespace dispatcher::queue {

class PriorityQueue {
private:
    std::map<TaskPriority, std::unique_ptr<IQueue>> queues_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::atomic<bool> shutdown_{false};

public:
    explicit PriorityQueue(const std::map<TaskPriority, QueueOptions> &options);

    void push(TaskPriority priority, Task task);
    // block on pop until shutdown is called
    // after that return std::nullopt on empty queue
    std::optional<Task> pop();

    void shutdown();

    ~PriorityQueue();
};

}  // namespace dispatcher::queue