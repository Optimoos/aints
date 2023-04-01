
#include "tile.h"

Tile::Tile()
{
  //  tile_pixels= std::make_shared<Image>(GenImageColor(kTileX, kTileY,
  //  GREEN)); Texture2D img_texture= LoadTextureFromImage(*tile_pixels);
  // tile_texture_= std::make_shared<Texture2D>(img_texture);

  tile_pixels= GenImageColor(kTileX, kTileY, GREEN);
  tile_texture_= LoadTextureFromImage(tile_pixels);

  //std::cout << "Tile created" << std::endl;

  // tile_pixels = std::make_shared<Image>(GenImageColor(World::Tile::kTileX,
  // World::Tile::kTileY, GREEN)); tile_texture_ =
  // std::make_shared<Texture2D>(LoadTextureFromImage(*tile_pixels));
}

Tile::~Tile()
{
  UnloadImage(tile_pixels);
  //std::cout << "Tile destroyed" << std::endl;
}

void Tile::NoiseToBlock()
{
  uint32_t location_counter= 0;
  for (float &block : this->noise_data_)
  {
    if ((block > 0.0f) && (block <= 0.1f))
    {
      this->blocks.at(location_counter)= kBlockStone;
    }
    else if ((block > 0.1f) && (block <= 0.5f))
    {
      this->blocks.at(location_counter)= kBlockDirt;
    }
    else if ((block > 0.5f) && (block <= 1.0f))
    {
      this->blocks.at(location_counter)= kBlockUnderground;
    }
    location_counter++;
  }
}

void Tile::GenerateTilePixels(Color color)
{
  for (uint16_t y_position= 0; y_position < kTileY; y_position++)
  {
    for (uint16_t x_position= 0; x_position < kTileX; x_position++)
    {
      ImageDrawPixel(&tile_pixels, x_position, y_position, color);
    }
  }
}

void Tile::GenerateTilePixels()
{
  for (uint16_t y_position= 0; y_position < kTileY; y_position++)
  {
    for (uint16_t x_position= 0; x_position < kTileX; x_position++)
    {
      switch (blocks.at(PosXY{x_position, y_position}.XYTo16Bit()))
      {
        case kBlockAir:
          ImageDrawPixel(&tile_pixels, x_position, y_position, SKYBLUE);
          break;
        case kBlockDirt:
          ImageDrawPixel(&tile_pixels, x_position, y_position, BROWN);
          break;
        case kBlockGrass:
          ImageDrawPixel(&tile_pixels, x_position, y_position, GREEN);
          break;
        case kBlockFood:
          ImageDrawPixel(&tile_pixels, x_position, y_position, RED);
          break;
        case kBlockStone:
          ImageDrawPixel(&tile_pixels, x_position, y_position, DARKGRAY);
          break;
        case kBlockWater:
          ImageDrawPixel(&tile_pixels, x_position, y_position, BLUE);
          break;
        case kBlockSand:
          ImageDrawPixel(&tile_pixels, x_position, y_position, YELLOW);
          break;
        case kBlockUnderground:
          ImageDrawPixel(&tile_pixels, x_position, y_position, DARKBROWN);
          break;
        case kBlockStockpiledFood:
          ImageDrawPixel(&tile_pixels, x_position, y_position, RED);
          break;
        default:
          ImageDrawPixel(&tile_pixels, x_position, y_position, PURPLE);
          break;
      }
    }
  }
}

void Tile::GenerateTileTexture(bool update)
{
  if (!update)
  {
    tile_texture_= LoadTextureFromImage(tile_pixels);
  }
  else
  {
    UpdateTexture(tile_texture_, tile_pixels.data);
  }
}