#include "bit_arr.hpp"
#include <gtest/gtest.h>


TEST(BitArrayTest, DefaultConstructor) {
    BitArray b;
    EXPECT_EQ(b.size(), 0);
    EXPECT_TRUE(b.empty());
}

TEST(BitArrayTest, ConstructorWithSize) {
    BitArray b(10);
    EXPECT_EQ(b.size(), 10);
    for (int i = 0; i < 10; ++i) EXPECT_FALSE(b[i]);
}

TEST(BitArrayTest, ConstructorWithValue) {
    BitArray b(8, 0b10101010);
    EXPECT_EQ(b.size(), 8);
    EXPECT_EQ(b.to_string(), "01010101");
}

TEST(BitArrayTest, CopyConstructor) {
    BitArray b1(8, 0b11001100);
    BitArray b2(b1);
    EXPECT_EQ(b1.size(), b2.size());
    for (int i = 0; i < 8; ++i) EXPECT_EQ(b1[i], b2[i]);
}

TEST(BitArrayTest, AssignmentOperator) {
    BitArray b1(8, 0b11110000);
    BitArray b2;
    b2 = b1;
    EXPECT_EQ(b1.size(), b2.size());
    EXPECT_EQ(b2.to_string(), "00001111");
}


TEST(BitArrayTest, SetSingleBit) {
    BitArray b(8);
    b.set(3, true);
    EXPECT_TRUE(b[3]);
    b.set(3, false);
    EXPECT_FALSE(b[3]);
}

TEST(BitArrayTest, SetAllBits) {
    BitArray b(8);
    b.set();
    for (int i = 0; i < 8; ++i) EXPECT_TRUE(b[i]);
}

TEST(BitArrayTest, ResetSingleBit) {
    BitArray b(8, 0b11111111);
    b.reset(4);
    EXPECT_FALSE(b[4]);
    for (int i = 0; i < 8; ++i) {
        if (i != 4) EXPECT_TRUE(b[i]);
    }
}

TEST(BitArrayTest, ResetAllBits) {
    BitArray b(8, 0b11111111);
    b.reset();
    for (int i = 0; i < 8; ++i) EXPECT_FALSE(b[i]);
}

TEST(BitArrayTest, PushBack) {
    BitArray b;
    b.push_back(true);
    b.push_back(false);
    EXPECT_EQ(b.size(), 2);
    EXPECT_TRUE(b[0]);
    EXPECT_FALSE(b[1]);
}


TEST(BitArrayTest, BitwiseAND) {
    BitArray b1(8, 0b10101010);
    BitArray b2(8, 0b11001100);
    BitArray result = b1 & b2;
    EXPECT_EQ(result.to_string(), "01000100");
}

TEST(BitArrayTest, BitwiseOR) {
    BitArray b1(8, 0b10101010);
    BitArray b2(8, 0b11001100);
    BitArray result = b1 | b2;
    EXPECT_EQ(result.to_string(), "11011110");
}

TEST(BitArrayTest, BitwiseXOR) {
    BitArray b1(8, 0b10101010);
    BitArray b2(8, 0b11001100);
    BitArray result = b1 ^ b2;
    EXPECT_EQ(result.to_string(), "10011010");
}

TEST(BitArrayTest, BitwiseNOT) {
    BitArray b(8, 0b10101010);
    BitArray result = ~b;
    EXPECT_EQ(result.to_string(), "10101010");
}


TEST(BitArrayTest, LeftShift) {
    BitArray b(8, 0b00001111);
    b <<= 2;
    EXPECT_EQ(b.to_string(), "00111100");
}

TEST(BitArrayTest, RightShift) {
    BitArray b(8, 0b11110000);
    b >>= 2;
    EXPECT_EQ(b.to_string(), "00111100");
}

TEST(BitArrayTest, ShiftBeyondSize) {
    BitArray b(8, 0b11111111);
    b <<= 10;
    EXPECT_EQ(b.count(), 0);
}


TEST(BitArrayTest, Any) {
    BitArray b1(8);
    EXPECT_FALSE(b1.any());
    BitArray b2(8, 0b00010000);
    EXPECT_TRUE(b2.any());
}

TEST(BitArrayTest, None) {
    BitArray b1(8);
    EXPECT_TRUE(b1.none());
    BitArray b2(8, 0b00000001);
    EXPECT_FALSE(b2.none());
}

TEST(BitArrayTest, Count) {
    BitArray b(8, 0b10101010);
    EXPECT_EQ(b.count(), 4);
}

TEST(BitArrayTest, InvalidSizeConstructor) {
    EXPECT_THROW(BitArray(-1), std::invalid_argument);
}

TEST(BitArrayTest, OutOfRangeAccess) {
    BitArray b(8);
    EXPECT_THROW(b[10], std::out_of_range);
    EXPECT_THROW(b.set(10), std::out_of_range);
    EXPECT_THROW(b.reset(10), std::out_of_range);
}

TEST(BitArrayTest, IncompatibleSizesBitwiseOp) {
    BitArray b1(8), b2(16);
    EXPECT_THROW(b1 &= b2, std::invalid_argument);
    EXPECT_THROW(b1 |= b2, std::invalid_argument);
    EXPECT_THROW(b1 ^= b2, std::invalid_argument);
}

TEST(BitArrayTest, NegativeShift) {
    BitArray b(8);
    EXPECT_THROW(b <<= -1, std::invalid_argument);
    EXPECT_THROW(b >>= -1, std::invalid_argument);
}

TEST(BitArrayTest, ToString) {
    BitArray b1(8, 0b00001111);
    EXPECT_EQ(b1.to_string(), "11110000");
    BitArray b2(5, 0b10101);
    EXPECT_EQ(b2.to_string(), "10101");
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}