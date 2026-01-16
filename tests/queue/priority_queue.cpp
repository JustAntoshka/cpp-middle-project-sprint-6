#include <gtest/gtest.h>

#include "queue/priority_queue.hpp"

using dispatcher::TaskPriority;
using dispatcher::queue::PriorityQueue;
using dispatcher::queue::QueueOptions;

TEST(PriorityQueueTest, HighPriorityFirst) {
    PriorityQueue q({
        {TaskPriority::High, {false, std::nullopt}},
        {TaskPriority::Normal, {false, std::nullopt}},
    });

    std::vector<std::string> order;

    q.push(TaskPriority::Normal, [&] { order.push_back("Normal"); });
    q.push(TaskPriority::High, [&] { order.push_back("High"); });

    auto t1 = q.pop();
    auto t2 = q.pop();

    (*t1)();
    (*t2)();

    ASSERT_EQ(order.size(), 2);
    EXPECT_EQ(order[0], "High");
    EXPECT_EQ(order[1], "Normal");
}

TEST(PriorityQueueTest, ShutdownUnblocksPop) {
    PriorityQueue q({
        {TaskPriority::High, {false, std::nullopt}},
    });

    std::atomic<bool> finished{false};

    std::thread t([&] {
        auto task = q.pop();
        EXPECT_FALSE(task.has_value());
        finished.store(true);
    });

    // Wait a bit to ensure thread t is blocked
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_FALSE(finished.load());

    q.shutdown();
    t.join();
    EXPECT_TRUE(finished.load());
}