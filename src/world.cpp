#include "world.h"

void GenerateTileNoise(FastNoise::SmartNode<> &noise_generator,
                       std::vector<float> &noise_data, uint16_t x_position,
                       uint16_t y_position)
{
  noise_generator->GenUniformGrid2D(noise_data.data(), x_position * kTileX,
                                    y_position * kTileY, kTileX, kTileY, 0.02f,
                                    1337);
}

BlockTypes World::GetBlockAtPos(PosXY &&blockpos, std::shared_ptr<World> world)
{
//  std::cout << "GetBlockAtPos: " << blockpos.x << ", " << blockpos.y
//            << std::endl;
  if (blockpos.x < 0 || blockpos.y < 0 || blockpos.x >= kWorldX ||
      blockpos.y >= kWorldY)
  {
    return kBlockInvalid;
  }
  auto tile= world->GetTile(blockpos.ToWorldTile());
  return tile->blocks.at(blockpos.ToTileInt());
}

BlockTypes World::GetBlockAtPos(PosXY &blockpos, std::shared_ptr<World> world)
{
//  std::cout << "GetBlockAtPos: " << blockpos.x << ", " << blockpos.y
//            << std::endl;
  if (blockpos.x < 0 || blockpos.y < 0 || blockpos.x >= kWorldX ||
      blockpos.y >= kWorldY)
  {
    return kBlockInvalid;
  }
  auto tile= world->GetTile(blockpos.ToWorldTile());
  return tile->blocks.at(blockpos.ToTileInt());
}

void World::SetBlockAtPos(PosXY const &position, BlockTypes const type)
{
  const auto tile= GetTile(position.ToWorldTile());
  tile->blocks.at(position.ToTileInt())= type;
  tile->GenerateTilePixels();
  tile->GenerateTileTexture(true);
}

void World::AddFood(PosXY position, int64_t const size)
{
  for (int64_t x= position.x - size; x <= position.x + size; x++)
  {
    int64_t y_range= sqrt(pow(size, 2) - pow(x - position.x, 2));
    for (int64_t y= position.y - y_range; y <= position.y + y_range; y++)
    {
      SetBlockAtPos(PosXY{x, y}, kBlockFood);
    }
  }
}

// World::PosXY World::FindNearestBlockOfType(BlockTypes) {}

PosXY World::FindNearestBlockOfType(PosXY &center, BlockTypes type,
                                    uint64_t range, bool at_least_one_away)
{
  PosXY result{-1, -1};
  result.valid= false;

  if (GetBlockAtPos(center, shared_from_this()) == type && !at_least_one_away)
  {
    result= center;
    result.valid= true;
  }
  else
  {
    bool found= false;
    for (int d= 1; d < range; d++)
    {
      for (int i= 0; i < d + 1; i++)
      {
        int x1= center.x - d + i;
        int y1= center.y - i;

        // Check point (x1, y1)
        if (GetBlockAtPos(PosXY{x1, y1}, shared_from_this()) == type)
        {
          result= PosXY{x1, y1};
          result.valid= true;
          found= true;
          break;
        }

        int x2= center.x + d - i;
        int y2= center.y + i;

        // Check point (x2, y2)
        if (GetBlockAtPos(PosXY{x2, y2}, shared_from_this()) == type)
        {
          result= PosXY{x2, y2};
          result.valid= true;
          found= true;
          break;
        }
      }
      if (found)
      {
        break;
      }
      for (int i= 1; i < d; i++)
      {
        int x1= center.x - i;
        int y1= center.y + d - i;

        // Check point (x1, y1)
        if (GetBlockAtPos(PosXY{x1, y1}, shared_from_this()) == type)
        {
          result= PosXY{x1, y1};
          result.valid=true;
          found= true;
          break;
        }

        int x2= center.x + i;
        int y2= center.y - d + i;

        // Check point (x2, y2)
        if (GetBlockAtPos(PosXY{x2, y2}, shared_from_this()) == type)
        {
          result= PosXY{x2, y2};
          result.valid= true;
          found= true;
          break;
        }
      }
      if (found)
      {
        break;
      }
    }
  }

  return result;
}

World::World(bool debug)
{
  uint64_t x_tile_count{0};
  uint64_t y_tile_count{0};

  if (debug)
  {
    debug_= true;
    InitWindow(800, 600, "Aints - Test");
  }

  // FIXME: As noise integration becomes more sophisticated, use noise to
  // generate test patterns instead of the current method.
  FastNoise::SmartNode<> noise_generator= FastNoise::NewFromEncodedNodeTree(
      "FwAAAIC/AACAPwAAAAAAAIA/DAABAAAAzczMPQkAAAAAgD8=");

  // Pre-size world storage and fill with initial data
  // This will hopefully prevent the vector from resizing and causing
  // issues with the shared pointers it contains
  world_tiles_.clear();
  world_tiles_.reserve((kWorldX / kTileX) * (kWorldY / kTileY));

  BS::thread_pool pool;

  for (auto iter= 0; iter < WorldTileRatioY; iter++)
  {
    while (x_tile_count < WorldTileRatioX)
    {
      //      auto new_tile= std::make_shared<Tile>();
      //      Tile *new_tile= new Tile();

      world_tiles_.emplace_back(std::make_shared<Tile>());
      auto &new_tile= *world_tiles_.back();

      if (debug)
      {
        if (x_tile_count % 2 == 0)
        {
          for (auto &block : new_tile.blocks)
          {
            block= kBlockAir;
          }
        }
      }
      new_tile.noise_data_.clear();
      new_tile.noise_data_.resize(kTileX * kTileY);

      new_tile.tile_location= PosXY{static_cast<int64_t>(x_tile_count),
                                    static_cast<int64_t>(y_tile_count)};

      auto noise_future= pool.submit(
          GenerateTileNoise, std::ref(noise_generator),
          std::ref(new_tile.noise_data_), x_tile_count, y_tile_count);

      // NOTE: This call is not happy with the default initialization of
      // noise_data_ in the Tile class.
      //            GenerateTileNoise(noise_generator, new_tile.noise_data_,
      //            x_tile_count, y_tile_count);

      x_tile_count++;
    }
    y_tile_count++;
    x_tile_count= 0;
  }

  pool.wait_for_tasks();

  uint64_t tmp_counter{0};
  for (auto &tile : world_tiles_)
  {
    if (!debug)
    {
      tile->NoiseToBlock();
    }
    tile->GenerateTilePixels();

    ImageDrawText(&tile->tile_pixels, std::to_string(tmp_counter).c_str(), 20,
                  20, 20, BLACK);

    tile->GenerateTileTexture(true);
    tmp_counter++;
  }

  // Generate starting area
  //  FastNoise::SmartNode<> starting_area_generator=
  //      FastNoise::NewFromEncodedNodeTree(
  //          "FwAAAIC/AACAPwAAAAAAAIA/EAAAAAA/EwAAAIA+IgAAAIA/"
  //          "zcxMPgUAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAACAPw==");
  //  std::vector<std::vector<float>> starting_noise;
  //  starting_noise.resize(4);
  //  for (std::vector<float> entry : starting_noise)
  //  {
  //    entry.resize(Tile::kTileX * Tile::kTileY);
  //  }
  //  uint8_t noise_counter= 0;
  //  for (uint8_t y_location= 0; y_location <= 1; y_location++)
  //  {
  //    for (uint8_t x_location= 0; x_location <= 1; x_location++)
  //    {
  //      //
  //      starting_area_generator->GenUniformGrid2D(starting_noise.at(noise_counter).data(),
  //      //                                              x_location,
  //      //                                              y_location,
  //      //                                              World::Tile::kTileX,
  //      //                                              World::Tile::kTileY,
  //      //                                              0.02f, 1337);
  //      noise_counter++;
  //
  //      // Once noise is generated, it needs to be added to the existing noise
  //      // data for the tiles in the middle of the map
  //
  //      // Could probably use some kind of GetTileAtCoordinate function here
  //      which
  //      // will also be helpful in the future A function for updating the
  //      noise
  //      // data may also be useful
  //    }
  //  }
}

World::~World()
{
  if (debug_)
  {
    CloseWindow();
  }
}

// bool World::XBlocksAway(PosXY center, PosXY block, uint16_t distance)
//{
//   int64_t absx= abs(block.x - center.x);
//   int64_t absy= abs(block.y - center.y);
//   if ((absx <= distance) && (absy <= distance))
//   {
//     return true;
//   }
//   else
//   {
//     return false;
//   }
// }

bool World::OneBlockAway(PosXY center, PosXY block)
{
  int64_t absx= abs(block.x - center.x);
  int64_t absy= abs(block.y - center.y);
  if (((absx == 1) && (absy == 1)) || ((absx == 1) && (absy == 0)) ||
      ((absx == 0) && (absy == 1)))
  {
    return true;
  }
  else
  {
    return false;
  }
}

// Returns true if block placed successfully, false if it failed
bool World::PlaceBlockAtPos(PosXY const &my_position, PosXY &place_position,
                            BlockTypes &type)
{
  bool successfully_placed= false;
  // Check if we're within reach
  if (OneBlockAway(my_position, place_position))
  {
    // Is the place position empty
    if (GetBlockAtPos(place_position, shared_from_this()) == kBlockUnderground)
    {
      if (type == kBlockFood)
      {
        this->SetBlockAtPos(place_position, kBlockStockpiledFood);
      } else {
        this->SetBlockAtPos(place_position, type);
      }
      type= kBlockAir;
      successfully_placed= true;
    }
  }
  else
  {
    successfully_placed= false;
  }
  return successfully_placed;
}

// Returns true if block picked up successfully, false if it failed
bool World::PickupBlockAtPos(PosXY const &my_position, PosXY &place_position,
                             BlockTypes &type)
{
  bool successfully_picked= false;
  // Check if we're within reach
  if (OneBlockAway(my_position, place_position))
  {
    // Is the place position valid for pickup
    // FIXME: This should be a property of the block rather than a static list
    if (GetBlockAtPos(place_position, shared_from_this()) != kBlockUnderground)
    {
      type= GetBlockAtPos(place_position, shared_from_this());
      this->SetBlockAtPos(place_position, kBlockUnderground);
      successfully_picked= true;
    }
  }
  else
  {
    successfully_picked= false;
  }
  return successfully_picked;
}