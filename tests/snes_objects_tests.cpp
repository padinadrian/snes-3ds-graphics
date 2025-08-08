// Unit tests for SNES objects.

#include <gtest/gtest.h>

#include "oam.h"

namespace {

TEST(ObjectAttributeMemory, SizeofOam)
{
    EXPECT_EQ(sizeof(ObjectAttributeMemory), SNES_OAM_SIZE);
}

TEST(SnesTest, ReadObjectFromOam1) {
    EXPECT_TRUE(true);
}

}   // namespace
