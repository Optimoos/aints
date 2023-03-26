#include <gtest/gtest.h>
#include "world.h"


// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

class WorldTest : public ::testing::Test {
 protected:
  void SetUp() override {
    posxy1_ = World::PosXY{1,1};
    posxy2_ = World::PosXY{1,1};
    posxy3_ = World::PosXY{2,2};
  }

  // void TearDown() override {}

  World::PosXY posxy1_;
  World::PosXY posxy2_;
  World::PosXY posxy3_;
  World::PosXY posxy4_{0,0};
  World::PosXY posxy5_{1,0};
  World::PosXY posxy6_{2,0};
  World::PosXY posxy7_{0,1};
  World::PosXY posxy8_{2,1};
  World::PosXY posxy9_{0,2};
  World::PosXY posxy10_{1,2};
  World::PosXY posxy11_{-1,0};

};


TEST_F(WorldTest, PosXYEquality) {
  ASSERT_EQ(posxy1_, posxy2_);
  ASSERT_NE(posxy1_, posxy3_);
}

TEST_F(WorldTest, PosXYOneBlockAway) {
  ASSERT_TRUE(World::OneBlockAway(posxy1_, posxy4_));
  ASSERT_TRUE(World::OneBlockAway(posxy1_, posxy5_));
  ASSERT_TRUE(World::OneBlockAway(posxy1_, posxy6_));
  ASSERT_TRUE(World::OneBlockAway(posxy1_, posxy7_));
  ASSERT_TRUE(World::OneBlockAway(posxy1_, posxy8_));
  ASSERT_TRUE(World::OneBlockAway(posxy1_, posxy9_));
  ASSERT_TRUE(World::OneBlockAway(posxy1_, posxy10_));
  ASSERT_TRUE(World::OneBlockAway(posxy1_, posxy3_));
  ASSERT_TRUE(World::OneBlockAway(posxy4_, posxy11_));
  ASSERT_FALSE(World::OneBlockAway(posxy1_, posxy11_));
  ASSERT_FALSE(World::OneBlockAway(posxy6_, posxy9_));
  ASSERT_FALSE(World::OneBlockAway(posxy1_, posxy2_));
}