#include <gtest/gtest.h>
#include <windows.h>
#include <string>
#include "../header.h"

TEST(MessageQueueTest, CreateAndInitializeQueue)
{
    MessageQueue queue;
    EXPECT_TRUE(queue.Create("test_queue", 6));

    std::string message = "Hello";
    EXPECT_TRUE(queue.SendMessage(message));

    std::string received;
    EXPECT_TRUE(queue.ReceiveMessage(received));
    EXPECT_EQ(message, received);
}

TEST(MessageQueueTest, QueueFullBehavior)
{
    MessageQueue queue;
    EXPECT_TRUE(queue.Create("test_queue2", 3));

    EXPECT_TRUE(queue.SendMessage("Msg1"));
    EXPECT_TRUE(queue.SendMessage("Msg2"));
    EXPECT_TRUE(queue.IsFull());

    EXPECT_TRUE(queue.IsFull());

    std::string received;
    EXPECT_TRUE(queue.ReceiveMessage(received));
    EXPECT_FALSE(queue.IsFull());
}

TEST(MessageQueueTest, QueueEmptyBehavior)
{
    MessageQueue queue;
    EXPECT_TRUE(queue.Create("test_queue3", 4));

    EXPECT_TRUE(queue.IsEmpty());

    EXPECT_TRUE(queue.IsEmpty());

    EXPECT_TRUE(queue.SendMessage("Test"));
    EXPECT_FALSE(queue.IsEmpty());
}

TEST(MessageQueueTest, MessageTruncation)
{
    MessageQueue queue;
    EXPECT_TRUE(queue.Create("test_queue4", 4));

    std::string longMessage = "This is a very long message that exceeds the limit";
    EXPECT_TRUE(queue.SendMessage(longMessage));

    std::string received;
    EXPECT_TRUE(queue.ReceiveMessage(received));
    EXPECT_EQ(longMessage.substr(0, 20), received);
    EXPECT_EQ(20, received.length());
}