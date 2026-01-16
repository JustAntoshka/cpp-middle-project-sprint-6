#include <gtest/gtest.h>

#include "queue/bounded_queue.hpp"

using dispatcher::queue::BoundedQueue;

TEST(BoundedQueueTest, PushPopSingleThread) {
    BoundedQueue q(2);

    EXPECT_FALSE(q.try_pop().has_value());

    q.push([] {});

    EXPECT_TRUE(q.try_pop().has_value());
    EXPECT_FALSE(q.try_pop().has_value());

    q.push([] {});
    q.push([] {});

    EXPECT_TRUE(q.try_pop().has_value());
    EXPECT_TRUE(q.try_pop().has_value());
    EXPECT_FALSE(q.try_pop().has_value());
}

TEST(BoundedQueueTest, BlocksWhenFull) {
    BoundedQueue q(1);
    std::atomic<bool> pushed{false};

    q.push([] {});

    std::thread t([&] {
        // Expected to be blocked
        q.push([] {});
        pushed.store(true);
    });

    // Wait a bit to ensure thread t is locked
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_FALSE(pushed.load());

    q.try_pop();

    t.join();
    EXPECT_TRUE(pushed.load());
}