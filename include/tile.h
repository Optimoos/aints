#ifndef AIANTS_TILE_H
#define AIANTS_TILE_H

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <array>

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

  std::vector<float> *NoiseData() { return &noise_data_;};
  PosXY *TileLocation() { return &tile_location_;};
  Image *TilePixels() { return tile_pixels_.get();};
  Texture2D *TileTexture() { return tile_texture_.get();};

  // Tile interface to blocks. Tiles need to be able to get color information
  // from blocks for rendering. As tiles are primarily a structure for splitting
  // up rendering tasks, they shouldn't need to modify individual blocks.

  using IBlock = std::variant<AirBlock,DirtBlock,UndergroundBlock,StoneBlock>;

  class IBlockGetColor
  {
   public:
    BlockColor operator()( AirBlock const &a ) const
    {
      return a.GetBlockColor();
    }
    BlockColor operator()( DirtBlock const& a ) const
    {
      return a.GetBlockColor();
    }
    BlockColor operator()( StoneBlock const& a ) const
    {
      return a.GetBlockColor();
    }
    BlockColor operator()( UndergroundBlock const& a ) const
    {
      return a.GetBlockColor();
    }
  };

  class IBlockGetRayColor
  {
   public:
    Color operator()( AirBlock const &a ) const
    {
      return Color{a.GetBlockColor().r, a.GetBlockColor().g, a.GetBlockColor().b, a.GetBlockColor().a};
    }
    Color operator()( DirtBlock const& d ) const
    {
      return Color{d.GetBlockColor().r, d.GetBlockColor().g, d.GetBlockColor().b, d.GetBlockColor().a};
    }
    Color operator()( StoneBlock const& s ) const
    {
      return Color{s.GetBlockColor().r, s.GetBlockColor().g, s.GetBlockColor().b, s.GetBlockColor().a};
    }
    Color operator()( UndergroundBlock const& u ) const
    {
      return Color{u.GetBlockColor().r, u.GetBlockColor().g, u.GetBlockColor().b, u.GetBlockColor().a};
    }
  };

  class IBlockGetBlockType
  {
   public:
    BlockTypes operator()( AirBlock const &a ) const
    {
      return kBlockAir;
    }
    BlockTypes operator()( DirtBlock const& a ) const
    {
      return kBlockDirt;
    }
    BlockTypes operator()( StoneBlock const& a ) const
    {
      return kBlockStone;
    }
    BlockTypes operator()( UndergroundBlock const& a ) const
    {
      return kBlockUnderground;
    }
  };

  void IBlockSetBlockType(uint64_t tile_int, BlockTypes block_type)
  {
    switch (block_type)
    {
      case kBlockAir:
        blocks_->at(tile_int) = AirBlock{};
        break;
      case kBlockDirt:
        blocks_->at(tile_int) = DirtBlock{};
        break;
      case kBlockStone:
        blocks_->at(tile_int) = StoneBlock{};
        break;
      case kBlockUnderground:
        blocks_->at(tile_int) = UndergroundBlock{};
        break;
    }
  };

  IBlock IBlockAt(uint64_t tile_int) { return blocks_->at(tile_int);};

 private:
  std::unique_ptr<std::vector<IBlock>> blocks_;
  std::vector<float> noise_data_{kTileX * kTileY};
  PosXY tile_location_{-1,-1};
  std::unique_ptr<Image> tile_pixels_;
  std::unique_ptr<Texture2D> tile_texture_;

};

#endif  // AIANTS_TILE_H