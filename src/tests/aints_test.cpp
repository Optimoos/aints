#include <gtest/gtest.h>

#include "world.h"

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
    pos_world_tile_eq_result_3_ = pos_world_tile_eq_2_.ToTileInt();
    pos_find_nearest_1_ = world->FindNearestBlockOfType(posxy1_, kBlockAir, 10);
    pos_find_nearest_2_ = world->FindNearestBlockOfType(pos_tile_1_, kBlockDirt, 5);
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

  std::shared_ptr<World> world= std::make_shared<World>(true);

  PosXY pos_tile_1_{255,0};
  PosXY pos_tile_2_{256, 0};

  PosXY pos_world_tile_eq_{4000,400};
  PosXY pos_world_tile_eq_2_{400,900};
  uint64_t pos_world_tile_eq_result_1_;
  uint64_t pos_world_tile_eq_result_answer_{47};
  uint64_t pos_world_tile_eq_result_2_;
  uint64_t pos_world_tile_eq_result_3_;
  uint64_t pos_tile_int_eq_result_answer_{37024};
  uint64_t pos_tile_int_eq_result_answer_2_{33936};

  PosXY pos_find_nearest_1_;
  PosXY pos_find_nearest_2_;


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
  ASSERT_EQ(World::GetBlockAtPos(posxy1_, world), kBlockAir);
  ASSERT_EQ(World::GetBlockAtPos(pos_tile_2_, world), kBlockDirt);
}

TEST_F(WorldTest, PositionHelpers)
{
  ASSERT_EQ(pos_world_tile_eq_result_1_, pos_world_tile_eq_result_answer_);
  ASSERT_EQ(pos_world_tile_eq_result_2_, pos_tile_int_eq_result_answer_);
  ASSERT_EQ(pos_world_tile_eq_result_3_, pos_tile_int_eq_result_answer_2_);

  ASSERT_EQ(pos_find_nearest_1_, posxy1_);
  ASSERT_EQ(pos_find_nearest_2_, pos_tile_2_);

}

// Test needed for:
//  World FindPath
// FindNearestBlockOfType
//  PickupBlockAtPos
//  PlaceBlockAtPos
//  SetBlockAtPos