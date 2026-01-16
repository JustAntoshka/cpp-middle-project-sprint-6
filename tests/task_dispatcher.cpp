#include <chrono>
#include <gtest/gtest.h>
#include <thread>

#include "task_dispatcher.hpp"

using dispatcher::TaskDispatcher;
using dispatcher::TaskPriority;

TEST(TaskDispatcherTest, ExecutesTasks) {
    TaskDispatcher dispatcher(2);

    std::atomic<int> counter{0};

    for (int i = 0; i < 10; ++i) {
        dispatcher.schedule(TaskPriority::Normal, [&] { counter.fetch_add(1); });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    EXPECT_EQ(counter.load(), 10);
}

TEST(TaskDispatcherTest, HighPriorityExecutedFirst) {
    TaskDispatcher dispatcher(1);

    std::vector<std::string> order;
    std::mutex m;

    dispatcher.schedule(TaskPriority::Normal, [&] {
        std::lock_guard<std::mutex> lock(m);
        order.push_back("Normal");
    });

    dispatcher.schedule(TaskPriority::High, [&] {
        std::lock_guard<std::mutex> lock(m);
        order.push_back("High");
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    ASSERT_EQ(order.size(), 2);
    EXPECT_EQ(order[0], "High");
    EXPECT_EQ(order[1], "Normal");
}
