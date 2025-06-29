#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include "utility.h"
}

namespace {

TEST(UtilityTests, SwapU8)
{
    uint8_t a = 10;
    uint8_t b = 19;
    swap_u8(&a, &b);
    EXPECT_EQ(a, 19);
    EXPECT_EQ(b, 10);
}

TEST(UtilityTests, ReverseBytes)
{
    uint8_t bytes[] = {1, 2, 3, 4, 5, 6, 7};
    reverse_bytes(bytes, sizeof(bytes));
    ASSERT_THAT(bytes, ::testing::ElementsAre(7, 6, 5, 4, 3, 2, 1));
}

}   // namespace
