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

    // Turns a PosXY into a single integer that can be used to index a 1D array
    // For coordinates "internal" to a tile, from 0,0 to Tile::kTileX,Tile::kTileY
    uint64_t XYTo16Bit() const { return (y * Tile::kTileY) + x; }
    // Turns a PosXY into a single integer that can be used to index a 1D array
    // such as those used in the Tile class.
    uint64_t ToTileInt() const { return ((y % World::WorldTileRatioY) * Tile::kTileY) + (x % World::WorldTileRatioX); };
    // Turns a PosXY into a single integer that can be used to index a 1D array
    // such as the one used in the World class.
    uint64_t ToWorldTile() const {return ((y / World::WorldTileRatioY) + (x / WorldTileRatioX));};
    // Turns a PosXY of a tile location into a single integer that can be used to index a 1D array
    uint64_t TilePosToInt() const {return (( y * World::WorldTileRatioY) + x);};
  };

  class Tile
  {
   public:
    static const uint16_t kTileX{256};
    static const uint16_t kTileY{256};

    std::vector<World::BlockTypes> blocks{kTileX * kTileY};
    std::vector<float> noise_data_{kTileX * kTileY};
    Image tile_pixels[kTileX * kTileY]
        {GenImageColor(World::Tile::kTileX, World::Tile::kTileY, BLANK)};
    Texture2D tile_texture_{LoadTextureFromImage(*tile_pixels)};

    void GenerateTilePixels();
    void GenerateTileTexture(bool update= false);

    void NoiseToBlock();

    Tile()= default;
    ~Tile();
    Tile(const Tile &other)= default;
    Tile &operator=(const Tile &other)= default;

   private:
  };

  World();
  ~World()= default;
  World(const World &other)= default;
  World &operator=(const World &other)= default;

  static BlockTypes GetBlockAtPos(PosXY blockpos, World *world);
  PosXY FindNearestBlockOfType(PosXY center, BlockTypes type, uint64_t range);
  void SetBlockAtPos(PosXY const &position, BlockTypes type);
  bool PlaceBlockAtPos(PosXY const &my_position, PosXY &place_position,
                       BlockTypes &type);
  bool PickupBlockAtPos(PosXY const &my_position, PosXY &place_position,
                        BlockTypes &type);
  void AddFood(PosXY &position, int64_t const size);
  // PosToTile returns a tile based on the world coordinates, not tile coordinates
  // It's also useful as it's static
  static std::shared_ptr<Tile> PosToTile(PosXY position, World *world);
  static bool OneBlockAway(PosXY center, PosXY block);
  bool XBlocksAway(PosXY center, PosXY block, uint16_t distance);
  static void FindPath(PosXY origin, PosXY destination,
                       std::vector<PosXY> &results, World *world);
  // GetTile returns a tile based on the tile coordinates, not world coordinates
  // Use PosToTile to get a tile based on world coordinates
  std::shared_ptr<Tile> GetTile(uint64_t index) {  return world_tiles_.at(index); }

  static constexpr uint16_t kWorldX{8192};
  static constexpr uint16_t kWorldY{2048};

  static constexpr int64_t WorldTileRatioY{ World::kWorldY / Tile::kTileY };
  static constexpr int64_t WorldTileRatioX{ World::kWorldX / Tile::kTileX };
  static constexpr int64_t TileTotalBlocks{ Tile::kTileX * Tile::kTileY };

 private:
  std::vector<std::shared_ptr<Tile>> world_tiles_{};
};

void GenerateTileNoise(FastNoise::SmartNode<> &noise_generator,
                       std::vector<float> &noise_data, uint16_t x_position,
                       uint16_t y_position);



// FIXME: The below is mostly lifted directly from the findpath.cpp example for
// the A* library, this can probably be cleaned up for our specific use case

// Definitions

class MapSearchNode
{
 public:
  World::PosXY position{0, 0};  // the (x,y) positions of the node
  World::BlockTypes block_type{};
  World *world;

  MapSearchNode() { block_type= World::kBlockAir; }

  MapSearchNode(World* world, int64_t px, int64_t py)
  {
    position.x= px;
    position.y= py;
    block_type= World::GetBlockAtPos(position, world);
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
  inline void PrintNodeInfo(){};
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

// Here's the heuristic function that estimates the distance from a Node
// to the Goal.

float MapSearchNode::GoalDistanceEstimate(MapSearchNode &nodeGoal)
{
  return abs(position.x - nodeGoal.position.x) +
         abs(position.y - nodeGoal.position.y);
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
  World::PosXY parent_position{-1, -1};

  if (parent_node)
  {
    parent_position= parent_node->position;
  }

  MapSearchNode NewNode{};

  // push each possible move

  int dx[8]= {-1, -1, -1, 0, 0, 1, 1, 1};
  int dy[8]= {-1, 0, 1, -1, 1, -1, 0, 1};

  for (int i= 0; i < 8; i++)
  {
    int x= position.x + dx[i];
    int y= position.y + dy[i];
    if (World::GetBlockAtPos(World::PosXY{x, y}, world) == World::kBlockUnderground)
    {
      NewNode= MapSearchNode(world, x, y);
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
  World::BlockTypes successor_block= World::GetBlockAtPos(successor.position, this->world);
  switch (successor_block)
  {
    case World::kBlockUnderground:
      cost= 1.0f;
      break;
    case World::kBlockFood:
      cost= 10.0f;
      break;
    case World::kBlockDirt:
      cost= 50.0f;
      break;
    default:
      break;
  }
  return cost;
}

#endif