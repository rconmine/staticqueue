#include "gtest/gtest.h"
extern "C" {
#include "queue.h"
}

TEST(CreateQueue, SimpleCreate) {
    uint8_t buffer[256] = {0};
    Queue q = Queue_Create(buffer, sizeof(buffer));

    ASSERT_EQ(q.buffer, buffer) << "Buffer of queue: Invalid init";
    ASSERT_EQ(q.capacity, 256) << "Capacity of queue calculated invalid";
    ASSERT_EQ(q.size, 0) << "Queue has been initialzed with non-zero size";
}

TEST(QueuePush, PushInEmpty) {
    uint8_t buffer[5] = {0};
    Queue q = Queue_Create(buffer, sizeof(buffer));

    ASSERT_EQ(q.size, 0) << "Queue has been initialzed with non-zero size";
    Boolean result = Queue_Push(&q, 5);
    ASSERT_EQ(result, TRUE) << "Expected result to be true when queue have some space";
    ASSERT_EQ(q.size, 1) << "After insert in queue size have not updated!";
}

TEST(QueuePush, PushInFull) {
    uint8_t buffer[1] = {0};
    Queue q = Queue_Create(buffer, sizeof(buffer));

    ASSERT_EQ(q.size, 0) << "Queue has been initialzed with non-zero size";
    Boolean result = Queue_Push(&q, 5);
    ASSERT_EQ(result, TRUE) << "Expected result to be true when queue have some space";
    ASSERT_EQ(q.size, 1) << "After insert in queue size have not updated!";

    result = Queue_Push(&q, 5);
    ASSERT_EQ(result, FALSE) << "Expected result to be false - No space for storage";
    ASSERT_EQ(q.size, 1) << "After failed push size must not change!";
}

TEST(QueuePop, PopHaveElements) {
    uint8_t buffer[5] = {0};
    Queue q = Queue_Create(buffer, sizeof(buffer));

    ASSERT_EQ(q.size, 0) << "Queue has been initialzed with non-zero size";
    Boolean result = Queue_Push(&q, 5);
    ASSERT_EQ(result, TRUE) << "Expected result to be true when queue have some space";
    ASSERT_EQ(q.size, 1) << "After insert in queue size have not updated!";

    uint8_t val = 0;
    result = Queue_Pop(&q, &val);
    ASSERT_EQ(result, TRUE) << "Expected result to be true: have elements for poping";
    ASSERT_EQ(val, 5) << "Expected that popped value equals inserted";
    ASSERT_EQ(q.size, 0) << "After pop from queue size have not updated!";
}

TEST(QueuePop, PopHaveElementsNull) {
    uint8_t buffer[5] = {0};
    Queue q = Queue_Create(buffer, sizeof(buffer));

    ASSERT_EQ(q.size, 0) << "Queue has been initialzed with non-zero size";
    Boolean result = Queue_Push(&q, 5);
    ASSERT_EQ(result, TRUE) << "Expected result to be true when queue have some space";
    ASSERT_EQ(q.size, 1) << "After insert in queue size have not updated!";

    result = Queue_Pop(&q, NULL);
    ASSERT_EQ(result, TRUE) << "Expected result to be true: have elements for poping";
    ASSERT_EQ(q.size, 0) << "After pop from queue size have not updated!";
}

TEST(QueuePop, PopNoElements) {
    uint8_t buffer[5] = {0};
    Queue q = Queue_Create(buffer, sizeof(buffer));

    ASSERT_EQ(q.size, 0) << "Queue has been initialzed with non-zero size";

    uint8_t val = 5;
    Boolean result = Queue_Pop(&q, &val);
    ASSERT_EQ(result, FALSE) << "Expected result to be true: have elements for poping";
    ASSERT_EQ(val, 5) << "Value must not change if popping failed";
}

TEST(QueueEmpty, Empty) {
    uint8_t buffer[5] = {0};
    Queue q = Queue_Create(buffer, sizeof(buffer));

    ASSERT_EQ(q.size, 0) << "Queue has been initialzed with non-zero size";
    ASSERT_EQ(Queue_Empty(&q), TRUE) << "Queue must be empty after init but Queue_Empty returns FALSE";

    Boolean result = Queue_Push(&q, 5);
    ASSERT_EQ(result, TRUE) << "Expected result to be true when queue have some space";
    ASSERT_EQ(q.size, 1) << "After insert in queue size have not updated!";
    ASSERT_EQ(Queue_Empty(&q), FALSE) << "Queue must be not empty after successful insert but Queue_Empty returns TRUE";

    result = Queue_Pop(&q, NULL);
    ASSERT_EQ(result, TRUE) << "Expected result to be true: have elements for poping";
    ASSERT_EQ(Queue_Empty(&q), TRUE) << "Queue must be empty after last one popping but Queue_Empty returns FALSE";
}

TEST(QueueSize, Size) {
    uint8_t buffer[5] = {0};
    Queue q = Queue_Create(buffer, sizeof(buffer));

    ASSERT_EQ(Queue_Size(&q), 0) << "Queue has been initialzed with non-zero size";

    Boolean result = Queue_Push(&q, 5);
    ASSERT_EQ(result, TRUE) << "Expected result to be true when queue have some space";
    ASSERT_EQ(Queue_Size(&q), 1) << "After insert in queue size have not updated!";

    result = Queue_Pop(&q, NULL);
    ASSERT_EQ(result, TRUE) << "Expected result to be true: have elements for poping";
    ASSERT_EQ(Queue_Size(&q), 0) << "After pop from queue size have not updated!";
}

TEST(QueueCapacity, Capacity) {
    uint8_t buffer[5] = {0};
    Queue q = Queue_Create(buffer, sizeof(buffer));

    ASSERT_EQ(Queue_Capacity(&q), sizeof(buffer)) << "Invalid capacity have been returned";
}

TEST(QueueOverflowed, Overflowed) {
    uint8_t buffer[1] = {0};
    Queue q = Queue_Create(buffer, sizeof(buffer));

    ASSERT_EQ(Queue_Overflowed(&q), FALSE) << "The queue just created but have already overflowed!";

    Boolean result = Queue_Push(&q, 5);
    ASSERT_EQ(result, TRUE) << "Expected result to be true when queue have some space";
    ASSERT_EQ(Queue_Overflowed(&q), TRUE) << "The queue have no space for new values, but Queue_Overflowed say that it have!";

    result = Queue_Pop(&q, NULL);
    ASSERT_EQ(result, TRUE) << "Expected result to be true: have elements for poping";
    ASSERT_EQ(Queue_Overflowed(&q), FALSE) << "The queue have space for new values, but Queue_Overflowed say that it is not have!";
}

TEST(QueueCycle, Cycle256) {
    uint8_t buffer[256] = {0};
    Queue q = Queue_Create(buffer, sizeof(buffer));

    ASSERT_EQ(q.buffer, buffer) << "Buffer of queue: Invalid init";
    ASSERT_EQ(q.capacity, 256) << "Capacity of queue calculated invalid";
    ASSERT_EQ(q.size, 0) << "Queue has been initialzed with non-zero size";

    // Push a lot of integers
    for (int i = 0; i < 256; i++) {
        Queue_Push(&q, i);
        ASSERT_EQ(Queue_Size(&q), i+1) << "Expected size of " << i << " on iteration #" << i;
    }

    // Grab some
    for (int i = 0; i < 256; i++) {
        Queue_Pop(&q, NULL);
        ASSERT_EQ(Queue_Size(&q), 255-i) << "Expected size of " << 255-i << " on iteration #" << i;;
    }

    // Push a lot of integers again. So queue must filled again
    for (int i = 0; i < 256; i++) {
        Queue_Push(&q, i);
        ASSERT_EQ(Queue_Size(&q), i+1) << "Expected size of " << i << " on iteration #" << i;
    }

}