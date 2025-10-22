#include <gtest/gtest.h>
#include "../header.h"

TEST(ArrayOperationsTest, FindMinMax_BasicArray)
{
    ArrayValue data(5);
    int testArray[] = {3, 1, 4, 1, 5};
    for (int i = 0; i < 5; i++) data.array[i] = testArray[i];
    findMinMax(&data);
    EXPECT_EQ(data.min, 1);
    EXPECT_EQ(data.max, 5);
    EXPECT_EQ(data.minIndexes.size(), 2);
}

TEST(ArrayOperationsTest, FindMinMax_SingleElement)
{
    ArrayValue data(1);
    data.array[0] = 42;
    findMinMax(&data);
    EXPECT_EQ(data.min, 42);
    EXPECT_EQ(data.max, 42);
    EXPECT_EQ(data.minIndexes.size(), 1);
    EXPECT_EQ(data.maxIndexes.size(), 1);
}

TEST(ArrayOperationsTest, CalculateAverage_BasicArray)
{
    ArrayValue data(5);
    int testArray[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) data.array[i] = testArray[i];
    calculateAverage(&data);
    EXPECT_DOUBLE_EQ(data.averageValue, 3.0);
}

TEST(ArrayOperationsTest, CalculateAverage_EmptyArray)
{
    ArrayValue data(0);
    calculateAverage(&data);
    EXPECT_DOUBLE_EQ(data.averageValue, 0.0);
}

TEST(ArrayOperationsTest, ReplaceMinMaxWithAverage)
{
    ArrayValue data(6);
    int testArray[] = {1, 5, 1, 5, 1, 5};
    for (int i = 0; i < 6; i++) data.array[i] = testArray[i];
    data.min = 1;
    data.max = 5;
    data.averageValue = 3.0;
    data.minIndexes = {0, 2, 4};
    data.maxIndexes = {1, 3, 5};
    data.replaceMinMaxWithAverage();
    EXPECT_EQ(data.array[0], 3);
    EXPECT_EQ(data.array[1], 3);
    EXPECT_EQ(data.array[2], 3);
    EXPECT_EQ(data.array[3], 3);
    EXPECT_EQ(data.array[4], 3);
    EXPECT_EQ(data.array[5], 3);
}

TEST(ArrayOperationsTest, InitializeMinMax_SetsExtremeValues)
{
    ArrayValue data;
    data.min = 10;
    data.max = 20;
    data.initializeMinMax();
    EXPECT_EQ(data.min, INT_MAX);
    EXPECT_EQ(data.max, INT_MIN);
}