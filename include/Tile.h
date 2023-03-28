#ifndef AIANTS_TILE_H
#define AIANTS_TILE_H

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "aints_types.h"
#include "raylib.h"

class Tile
{
 public:
  std::vector<BlockTypes> blocks{kTileX * kTileY, kBlockDirt};
  std::vector<float> noise_data_{kTileX * kTileY};

  PosXY tile_location{-1,-1};

  Image tile_pixels;
  Texture2D tile_texture_;

  void GenerateTilePixels();
  void GenerateTilePixels(Color color);
  void GenerateTileTexture(bool update= false);
  void NoiseToBlock();

  Tile();
  ~Tile();
  Tile(const Tile &other)= default;
  Tile &operator=(const Tile &other)= default;

 private:
};

#endif  // AIANTS_TILE_H