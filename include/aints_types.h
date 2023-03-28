#ifndef AIANTS_TYPES_H
#define AIANTS_TYPES_H

#include <cstdint>

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

  bool operator==(const PosXY &rhs) const { return x == rhs.x && y == rhs.y; }

  // Turns a PosXY into a single integer that can be used to index a 1D array
  // For coordinates "internal" to a tile, from 0,0 to Tile::kTileX,Tile::kTileY
  uint64_t XYTo16Bit() const { return (y * kTileY) + x; }
  // Turns a PosXY into a single integer that can be used to index a 1D array
  // such as those used in the Tile class.
  uint64_t ToTileInt() const
  {
    return ((y % WorldTileRatioY) * kTileY) + (x % WorldTileRatioX);
  };
  // Turns a PosXY into a single integer that can be used to index a 1D array
  // such as the one used in the World class.
  uint64_t ToWorldTile() const
  {
    return ((y / WorldTileRatioY) + (x / WorldTileRatioX));
  };
  // Turns a PosXY of a tile location into a single integer that can be used to
  // index a 1D array
  uint64_t TilePosToInt() const { return ((y * WorldTileRatioX) + x); };
};

#endif  // AIANTS_TYPES_H