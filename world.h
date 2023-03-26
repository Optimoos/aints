#ifndef AIANTS_WORLD_H
#define AIANTS_WORLD_H

#include "BS_thread_pool.hpp"
#include "FastNoise/FastNoise.h"
#include "raylib.h"
#include "stlastar.h"

class World
{
 public:
  enum BlockTypes
  {
    kBlockAir,
    kBlockDirt,
    kBlockGrass,
    kBlockFood,
    kBlockStone,
    kBlockWater,
    kBlockSand,
    kBlockUnderground,
    kBlockStockpiledFood
  };

  struct PosXY
  {
    int64_t x{0};
    int64_t y{0};

    bool operator==(const PosXY &rhs) const { return x == rhs.x && y == rhs.y; }
  };

  class Tile
  {
   public:
    static const uint16_t kTileX{256};
    static const uint16_t kTileY{256};
    std::vector<World::BlockTypes> blocks;
    std::vector<float> noise_data_;
    Texture2D tile_texture_;
    Image tile_pixels[kTileX * kTileY]{
        GenImageColor(World::Tile::kTileX, World::Tile::kTileY, BLANK)};

    void GenerateTilePixels();

    void GenerateTileTexture(bool update= false);

    ~Tile();

   private:
  };

  World();

  ~World();

  static BlockTypes GetBlockAtPos(PosXY blockpos);

  //    PosXY FindNearestBlockOfType(BlockTypes);
  PosXY FindNearestBlockOfType(PosXY center, BlockTypes type, uint64_t range);

  void SetBlockAtPos(PosXY position, BlockTypes type);

  bool PlaceBlockAtPos(PosXY const &my_position, PosXY &place_position,
                       BlockTypes &type);

  bool PickupBlockAtPos(PosXY const &my_position, PosXY &place_position,
                        BlockTypes &type);

  void AddFood(int64_t x_pos, int64_t y_pos, int64_t size);

  static Tile *PosToTile(int64_t x_pos, int64_t y_pos);

  static std::vector<std::vector<Tile>> world_tiles_;

  bool OneBlockAway(PosXY center, PosXY block);

  bool XBlocksAway(PosXY center, PosXY block, uint16_t distance);

  static void FindPath(PosXY origin, PosXY destination, std::vector<PosXY> &results);

  const uint16_t kWorldX{8192};
  const uint16_t kWorldY{2048};

 private:
};

void GenerateTileNoise(FastNoise::SmartNode<> &noise_generator,
                       std::vector<float> &noise_data, uint16_t x_position,
                       uint16_t y_position);

std::vector<World::BlockTypes> NoiseToBlock(std::vector<float> noise);

// FIXME: The below is mostly lifted directly from the findpath.cpp example for
// the A* library, this can probably
//        be cleaned up for our specific use case

// Definitions

class MapSearchNode
{
 public:
  World::PosXY position{0,0};  // the (x,y) positions of the node
  World::BlockTypes block_type;

  MapSearchNode() {block_type = World::GetBlockAtPos(position);}

  MapSearchNode(int64_t px, int64_t py)
  {
    position.x= px;
    position.y= py;
    block_type = World::GetBlockAtPos(position);
  }

  bool operator==(const MapSearchNode &rhs) const
  {
    return position == rhs.position;
  }

  inline float GoalDistanceEstimate(MapSearchNode &nodeGoal);

  inline bool IsGoal(MapSearchNode &nodeGoal);

  inline bool GetSuccessors(AStarSearch<MapSearchNode> *astarsearch,
                            MapSearchNode *parent_node);

  inline float GetCost(MapSearchNode &successor);

  inline bool IsSameState(MapSearchNode &rhs);

  inline void PrintNodeInfo();
};

bool MapSearchNode::IsSameState(MapSearchNode &rhs)
{
  // same state in a maze search is simply when (x,y) are the same
  if (position == rhs.position)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void MapSearchNode::PrintNodeInfo()
{
  char str[100];
  sprintf(str, "Node position : (%d,%d)\n", position.x, position.y);

  std::cout << str;
}

// Here's the heuristic function that estimates the distance from a Node
// to the Goal.

float MapSearchNode::GoalDistanceEstimate(MapSearchNode &nodeGoal)
{
  return abs(position.x - nodeGoal.position.x) + abs(position.y - nodeGoal.position.y);
}

bool MapSearchNode::IsGoal(MapSearchNode &nodeGoal)
{
  if (position == nodeGoal.position)
  {
    return true;
  }

  return false;
}

// This generates the successors to the given Node. It uses a helper function
// called AddSuccessor to give the successors to the AStar class. The A*
// specific initialisation is done for each node internally, so here you just
// set the state information that is specific to the application
bool MapSearchNode::GetSuccessors(AStarSearch<MapSearchNode> *astarsearch,
                                  MapSearchNode *parent_node)
{

  World::PosXY parent_position{-1,-1};

  if (parent_node)
  {
    parent_position= parent_node->position;
  }

  MapSearchNode NewNode;

  // push each possible move

  int dx[8]= {-1, -1, -1, 0, 0, 1, 1, 1};
  int dy[8]= {-1, 0, 1, -1, 1, -1, 0, 1};

  for (int i= 0; i < 8; i++)
  {
    int x= position.x + dx[i];
    int y= position.y + dy[i];
    if (World::GetBlockAtPos(World::PosXY{x, y}) == World::kBlockUnderground)
    {
      NewNode= MapSearchNode(x, y);
        astarsearch->AddSuccessor(NewNode);
    }
  }

  return true;
}

// given this node, what does it cost to move to successor. In the case
// of our map the answer is the map terrain value at this node since that is
// conceptually where we're moving

float MapSearchNode::GetCost(MapSearchNode &successor)
{
  // FIXME: We only support a single allowable block initially so we return a
  // constant cost
  float cost{99.0f};
  World::BlockTypes successor_block= World::GetBlockAtPos(successor.position);
  switch (successor_block) {
    case World::kBlockUnderground:
      cost = 1.0f;
      break;
    case World::kBlockFood:
      cost = 10.0f;
      break;
    case World::kBlockDirt:
        cost = 50.0f;
        break;
    default:
      break;
  }
  return cost;
}

#endif