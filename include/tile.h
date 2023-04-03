#ifndef AIANTS_TILE_H
#define AIANTS_TILE_H

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "raylib.h"

#include "aints_types.h"
#include "block.h"

class Tile
{
 public:
  Tile();
  ~Tile();
  Tile(const Tile &other)= default;
  Tile &operator=(const Tile &other)= default;

  void GenerateTilePixels();
  void GenerateTilePixels(Color color);
  void GenerateTileTexture(bool update= false);
  void NoiseToBlock();

  BlockTypes Block(uint64_t tile_int) { return blocks_.at(tile_int);};
  std::vector<BlockTypes> *Blocks() { return &blocks_;};
  void SetBlock(uint64_t tile_int, BlockTypes type) { blocks_.at(tile_int) = type;};

  std::vector<float> *NoiseData() { return &noise_data_;};
  PosXY *TileLocation() { return &tile_location_;};
  Image *TilePixels() { return &tile_pixels_;};
  Texture2D *TileTexture() { return &tile_texture_;};


  class IBlock
  {
   public:
    IBlock();
    ~IBlock();
    IBlock(const IBlock &other)= default;
    IBlock &operator=(const IBlock &other)= default;

   private:

  };


 private:
  std::vector<BlockTypes> blocks_{kTileX * kTileY, kBlockDirt};
  std::vector<float> noise_data_{kTileX * kTileY};
  PosXY tile_location_{-1,-1};
  Image tile_pixels_;
  Texture2D tile_texture_;

};

#endif  // AIANTS_TILE_H