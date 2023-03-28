#ifndef AIANTS_WORLD_H
#define AIANTS_WORLD_H

#include <memory>
#include <string>
#include <vector>

#include "BS_thread_pool.hpp"
#include "FastNoise/FastNoise.h"
#include "Tile.h"
#include "aints_types.h"
#include "raylib.h"

class World
    : public std::enable_shared_from_this<World>
{
 public:
  World(bool debug= false);
  ~World();
  World(const World &other)= default;
  World &operator=(const World &other)= default;

  static BlockTypes GetBlockAtPos(PosXY &blockpos, std::shared_ptr<World> world);
  PosXY FindNearestBlockOfType(PosXY center, BlockTypes type, uint64_t range);
  void SetBlockAtPos(PosXY const &position, BlockTypes type);
  bool PlaceBlockAtPos(PosXY const &my_position, PosXY &place_position,
                       BlockTypes &type);
  bool PickupBlockAtPos(PosXY const &my_position, PosXY &place_position,
                        BlockTypes &type);
    void AddFood(PosXY position, int64_t const size);
  // PosToTile returns a tile based on the world coordinates, not tile
  // coordinates It's also useful as it's static
  //  static std::shared_ptr<Tile> PosToTile(PosXY position, World *world);
  static bool OneBlockAway(PosXY center, PosXY block);
  //  bool XBlocksAway(PosXY center, PosXY block, uint16_t distance);

  // GetTile returns a tile based on the tile coordinates, not world coordinates
  // Use PosToTile to get a tile based on world coordinates
  std::shared_ptr<Tile> GetTile(uint64_t index)
  {
    return world_tiles_.at(index);
  }
  // Tile GetTile(uint64_t index) {  return world_tiles_.at(index); }

 private:
  std::vector<std::shared_ptr<Tile>> world_tiles_{};
  bool debug_{false};
};

 void GenerateTileNoise(FastNoise::SmartNode<> &noise_generator,
                        std::vector<float> &noise_data, uint16_t x_position,
                        uint16_t y_position);

#endif