#include <gtest/gtest.h>

#include "world.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions)
{
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

class WorldTest : public ::testing::Test
{
 protected:
  void SetUp() override
  {
    posxy1_= PosXY{1, 1};
    posxy2_= PosXY{1, 1};
    posxy3_= PosXY{2, 2};

    pos_world_tile_eq_result_1_ = pos_world_tile_eq_.ToWorldTile();
    pos_world_tile_eq_result_2_ = pos_world_tile_eq_.ToTileInt();
  }

  //  void TearDown() override {
  //    delete world;
  //  }

  PosXY posxy1_;
  PosXY posxy2_;
  PosXY posxy3_;
  PosXY posxy4_{0, 0};
  PosXY posxy5_{1, 0};
  PosXY posxy6_{2, 0};
  PosXY posxy7_{0, 1};
  PosXY posxy8_{2, 1};
  PosXY posxy9_{0, 2};
  PosXY posxy10_{1, 2};
  PosXY posxy11_{-1, 0};

  World world{true};
  PosXY pos_tile_2_{300, 0};

  PosXY pos_world_tile_eq_{4000,400};
  uint64_t pos_world_tile_eq_result_1_;
  uint64_t pos_world_tile_eq_result_answer_{47};
  uint64_t pos_world_tile_eq_result_2_;
  uint64_t pos_tile_int_eq_result_answer_{37008};

};

TEST_F(WorldTest, PosXYEquality)
{
  ASSERT_EQ(posxy1_, posxy2_);
  ASSERT_NE(posxy1_, posxy3_);
}

TEST_F(WorldTest, PosXYOneBlockAway)
{
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

TEST_F(WorldTest, BlockFunctions)
{
  ASSERT_EQ(World::GetBlockAtPos(posxy1_, &world), kBlockAir);
  ASSERT_EQ(World::GetBlockAtPos(pos_tile_2_, &world), kBlockDirt);
}

TEST_F(WorldTest, PositionHelpers)
{
  ASSERT_EQ(pos_world_tile_eq_result_1_, pos_world_tile_eq_result_answer_);
  ASSERT_EQ(pos_world_tile_eq_result_2_, pos_tile_int_eq_result_answer_);

}

// Test needed for:
//  World FindPath
// FindNearestBlockOfType
//  PickupBlockAtPos
//  PlaceBlockAtPos
//  SetBlockAtPos