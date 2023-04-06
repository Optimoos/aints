
#include "tile.h"

Tile::Tile()
{
  tile_pixels_= std::make_unique<Image>(GenImageColor(kTileX, kTileY, GREEN));
  tile_texture_= std::make_unique<Texture2D>(LoadTextureFromImage(*tile_pixels_));
  blocks_= std::make_unique<std::vector<IBlock>>();
  // Don't resize the vector as we use emplace_back to add blocks
  blocks_->reserve(kTileX * kTileY);

  //std::cout << "Tile created" << std::endl;

}

Tile::~Tile()
{
  UnloadImage(*tile_pixels_);
  //std::cout << "Tile destroyed" << std::endl;
}

void Tile::NoiseToBlock()
{
  uint32_t location_counter= 0;
  for (float &block : noise_data_)
  {
    if ((block > 0.0f) && (block <= 0.1f))
    {
      blocks_->emplace_back(StoneBlock{});
    }
    else if ((block > 0.1f) && (block <= 0.5f))
    {
      blocks_->emplace_back(DirtBlock{});
    }
    else if ((block > 0.5f) && (block <= 1.0f))
    {
      blocks_->emplace_back(UndergroundBlock{});
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
      ImageDrawPixel(tile_pixels_.get(), x_position, y_position, color);
    }
  }
}

void Tile::GenerateTilePixels()
{
  for (uint16_t y_position= 0; y_position < kTileY; y_position++)
  {
    for (uint16_t x_position= 0; x_position < kTileX; x_position++)
    {
      ImageDrawPixel(tile_pixels_.get(), x_position, y_position, std::visit(IBlockGetRayColor{}, blocks_->at(PosXY{x_position, y_position}.XYTo16Bit())));
    }
  }
}

void Tile::GenerateTileTexture(bool update)
{

  if (!update)
  {
    *tile_texture_= LoadTextureFromImage(*tile_pixels_);
  }
  else
  {
    UpdateTexture(*tile_texture_, tile_pixels_->data);
  }
}



//Block<> Tile::IBlock::CreateAirBlock()
//{
//  AirBlock airblock{};
//  auto strategy = []( AirBlock const& c ){ /*...*/ };
//  return { airblock, strategy };
//}
