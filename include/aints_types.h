#ifndef AIANTS_TYPES_H
#define AIANTS_TYPES_H

#include <cstdint>

#include "block.h"

// FIXME: To be deleted once block class is refactored
const enum BlockTypes
{
  kBlockAir,
  kBlockDirt,
  kBlockGrass,
  kBlockFood,
  kBlockStone,
  kBlockWater,
  kBlockSand,
  kBlockUnderground,
  kBlockStockpiledFood,
  kBlockInvalid
};

static constexpr uint16_t kWorldX{8192};
static constexpr uint16_t kWorldY{2048};

static constexpr uint16_t kTileX{256};
static constexpr uint16_t kTileY{256};

static constexpr int64_t WorldTileRatioY{kWorldY / kTileY};
static constexpr int64_t WorldTileRatioX{kWorldX / kTileX};
static constexpr int64_t TileTotalBlocks{kTileX * kTileY};

struct PosXY
{
  int64_t x{0};
  int64_t y{0};
  bool valid{true};

  PosXY()= default;

  PosXY(int64_t x, int64_t y) : x(x), y(y) {}

  PosXY(const PosXY &other)
  {
        x= other.x;
        y= other.y;
        valid= other.valid;
  }

  PosXY(PosXY&& other) noexcept
  {
        x= other.x;
        y= other.y;

        other.x= 0;
        other.y= 0;
  }

  PosXY &operator=(const PosXY &other)
  {
          x= other.x;
          y= other.y;
          valid= other.valid;

          return *this;
  }

  bool operator==(const PosXY &rhs) const { return x == rhs.x && y == rhs.y; }

  bool operator<(const PosXY &rhs) const { return x < rhs.x || (x == rhs.x && y < rhs.y); }

  bool operator>(const PosXY &rhs) const { return x > rhs.x || (x == rhs.x && y > rhs.y); }

  // Turns a PosXY into a single integer that can be used to index a 1D array
  // For coordinates "internal" to a tile, from 0,0 to Tile::kTileX,Tile::kTileY
  uint64_t XYTo16Bit() const { return (y * kTileY) + x; }
  // Turns a PosXY into a single integer that can be used to index a 1D array
  // such as those used in the Tile class.
  uint64_t ToTileInt() const
  {
    return ((y % kTileY) * kTileY) + (x % kTileX);
  };
  // Turns a PosXY into a single integer that can be used to index a 1D array
  // such as the one used in the World class.
  uint64_t ToWorldTile() const
  {
    return (((y / kTileY) * WorldTileRatioX) + (x / kTileX));
  };
  // Turns a PosXY of a tile location into a single integer that can be used to
  // index a 1D array
  uint64_t TilePosToInt() const { return ((y * WorldTileRatioX) + x); };
};

#endif  // AIANTS_TYPES_H