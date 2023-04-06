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


  // Variants - AirBlock, DirtBlock, StoneBlock, UndergroundBlock, GrassBlock, FoodBlock, WaterBlock, SandBlock, StockpiledFoodBlock, InvalidBlock
  using IBlock = Block;

  class IBlockGetColor
  {
   public:
    BlockColor operator()( auto const &a ) const
    {
      return a.GetBlockColor();
    }
  };

  class IBlockGetRayColor
  {
   public:
    Color operator()(auto const &a)
    {
      return Color{a.GetBlockColor().r, a.GetBlockColor().g, a.GetBlockColor().b, a.GetBlockColor().a};
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
    BlockTypes operator()( GrassBlock const& a ) const
    {
      return kBlockGrass;
    }
    BlockTypes operator()( FoodBlock const& a ) const
    {
      return kBlockFood;
    }
    BlockTypes operator()( WaterBlock const& a ) const
    {
      return kBlockWater;
    }
    BlockTypes operator()( SandBlock const& a ) const
    {
      return kBlockSand;
    }
    BlockTypes operator()( StockpiledFoodBlock const& a ) const
    {
      return kBlockStockpiledFood;
    }
    BlockTypes operator()( InvalidBlock const& a ) const
    {
      return kBlockInvalid;
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
      case kBlockGrass:
        blocks_->at(tile_int) = GrassBlock{};
        break;
      case kBlockFood:
        blocks_->at(tile_int) = FoodBlock{};
        break;
      case kBlockWater:
        blocks_->at(tile_int) = WaterBlock{};
        break;
      case kBlockSand:
        blocks_->at(tile_int) = SandBlock{};
        break;
      case kBlockStockpiledFood:
        blocks_->at(tile_int) = StockpiledFoodBlock{};
        break;
      case kBlockInvalid:
        blocks_->at(tile_int) = InvalidBlock{};
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