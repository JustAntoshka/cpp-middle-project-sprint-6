#include <gtest/gtest.h>

#include "queue/unbounded_queue.hpp"

using dispatcher::queue::UnboundedQueue;

TEST(UnboundedQueueTest, PushPopSingleThread) {
    UnboundedQueue q(0);

    q.push([] {});
    q.push([] {});

    EXPECT_TRUE(q.try_pop().has_value());
    EXPECT_TRUE(q.try_pop().has_value());
    EXPECT_FALSE(q.try_pop().has_value());
}

TEST(UnboundedQueueTest, NeverBlocksOnPush) {
    UnboundedQueue q(0);

    for (int i = 0; i < 10000; ++i) {
        q.push([] {});
    }

    int count = 0;
    while (q.try_pop()) {
        ++count;
    }

    EXPECT_EQ(count, 10000);
}