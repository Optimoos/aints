#include "world.h"

void GenerateTileNoise(FastNoise::SmartNode<> &noise_generator,
                       std::vector<float> &noise_data, uint16_t x_position,
                       uint16_t y_position)
{
  noise_generator->GenUniformGrid2D(
      noise_data.data(), x_position * World::Tile::kTileX,
      y_position * World::Tile::kTileY, World::Tile::kTileX,
      World::Tile::kTileY, 0.02f, 1337);
}

World::BlockTypes World::GetBlockAtPos(PosXY blockpos, World *world)
{
  const int64_t x_loc= blockpos.x % Tile::kTileX;
  const int64_t y_loc= blockpos.y % Tile::kTileY;
  auto tile = PosToTile(blockpos.x, blockpos.y, world);
  return tile->blocks.at(y_loc * Tile::kTileY + x_loc);
}

void World::SetBlockAtPos(PosXY const position, World::BlockTypes const type, World &world)
{
  const int64_t x_loc= position.x % Tile::kTileX;
  const int64_t y_loc= position.y % Tile::kTileY;
  auto tile= PosToTile(position.x, position.y, &world);
  std::cout << "Set block: " << position.x << ", " << position.y << std::endl;
  tile->blocks.at(y_loc * Tile::kTileY + x_loc)= type;
  tile->GenerateTilePixels();
  tile->GenerateTileTexture(true);
}

void World::Tile::NoiseToBlock()
{

  uint32_t location_counter= 0;
  for (float &block : this->noise_data_)
  {
    if ((block > 0.0f) && (block <= 0.1f))
    {
      blocks.at(location_counter)= World::kBlockStone;
    }
    else if ((block > 0.1f) && (block <= 0.5f))
    {
      blocks.at(location_counter)= World::kBlockDirt;
    }
    else if ((block > 0.5f) && (block <= 1.0f))
    {
      blocks.at(location_counter)= World::kBlockUnderground;
    }
    location_counter++;
  }
}

std::shared_ptr<World::Tile> World::PosToTile(int64_t const x_pos, int64_t const y_pos, World *world)
{
  const uint16_t x_tile= x_pos / Tile::kTileX;
  const uint16_t y_tile= y_pos / Tile::kTileY;
  return world->GetTile(x_tile, y_tile);
}

void World::Tile::GenerateTilePixels()
{
  for (uint16_t y_position= 0; y_position < World::Tile::kTileY; y_position++)
  {
    for (uint16_t x_position= 0; x_position < World::Tile::kTileX; x_position++)
    {
      switch (blocks.at((y_position * World::Tile::kTileY) + x_position))
      {
        case World::kBlockAir:
          ImageDrawPixel(this->tile_pixels, x_position, y_position, SKYBLUE);
          break;
        case World::kBlockDirt:
          ImageDrawPixel(this->tile_pixels, x_position, y_position, BROWN);
          break;
        case World::kBlockGrass:
          ImageDrawPixel(this->tile_pixels, x_position, y_position, GREEN);
          break;
        case World::kBlockFood:
          ImageDrawPixel(this->tile_pixels, x_position, y_position, RED);
          break;
        case World::kBlockStone:
          ImageDrawPixel(this->tile_pixels, x_position, y_position, DARKGRAY);
          break;
        case World::kBlockWater:
          ImageDrawPixel(this->tile_pixels, x_position, y_position, BLUE);
          break;
        case World::kBlockSand:
          ImageDrawPixel(this->tile_pixels, x_position, y_position, YELLOW);
          break;
        case World::kBlockUnderground:
          ImageDrawPixel(this->tile_pixels, x_position, y_position, DARKBROWN);
          break;
        case World::kBlockStockpiledFood:
          ImageDrawPixel(this->tile_pixels, x_position, y_position, RED);
          break;
        default:
          ImageDrawPixel(this->tile_pixels, x_position, y_position, PURPLE);
          break;
      }
    }
  }
};

void World::Tile::GenerateTileTexture(bool update)
{
  if (!update)
  {
    tile_texture_= LoadTextureFromImage(*tile_pixels);
  }
  else
  {
    UpdateTexture(tile_texture_, tile_pixels->data);
  }
}

void World::AddFood(int64_t x_pos, int64_t y_pos, int64_t size, World &world)
{
  for (int64_t x= x_pos - size; x <= x_pos + size; x++)
  {
    int64_t y_range= sqrt(pow(size, 2) - pow(x - x_pos, 2));
    for (int64_t y= y_pos - y_range; y <= y_pos + y_range; y++)
    {
      SetBlockAtPos(PosXY{x, y}, kBlockFood, world);
    }
  }
}

// World::PosXY World::FindNearestBlockOfType(BlockTypes) {}

World::PosXY World::FindNearestBlockOfType(PosXY center, BlockTypes type,
                                           uint64_t range)
{
  for (int64_t r= 1; r <= range; r++)
  {
    bool points_found= false;

    for (int64_t x= std::max(center.x - r, (int64_t)0);
         x <= std::min(center.x + r, (int64_t)kWorldX); x++)
    {
      int64_t y_range= sqrt(pow(r, 2) - pow(x - center.x, 2));
      for (int64_t y= std::max(center.y - y_range, (int64_t)0);
           y <= std::min(center.y + y_range, (int64_t)kWorldY); y++)
      {
        if (type == GetBlockAtPos(PosXY{x, y}, this) && (PosXY{x,y} != center))
        {
          return PosXY{x, y};
        }
        points_found= true;
      }
    }

    if (!points_found)
    {
      break;
    }
  }
  return PosXY{0, 0};
}

World::World()
{
  uint64_t x_tile_count{0};
  uint64_t y_tile_count{0};

  FastNoise::SmartNode<> noise_generator= FastNoise::NewFromEncodedNodeTree(
      "FwAAAIC/AACAPwAAAAAAAIA/DAABAAAAzczMPQkAAAAAgD8=");

  // Pre-size world storage and fill with initial data
  world_tiles_.clear();
  world_tiles_.reserve((kWorldX / Tile::kTileX) * (kWorldY / Tile::kTileY));
  BS::thread_pool pool;
  for (auto iter= 0; iter < (kWorldY / Tile::kTileY); iter++)
  {
    while (x_tile_count < (kWorldX / Tile::kTileX))
    {
      auto new_tile= std::make_shared<Tile>();

      //new_tile.get()->blocks.resize(Tile::kTileX * Tile::kTileY);
      new_tile.get()->noise_data_.resize(World::Tile::kTileX *
                                         World::Tile::kTileY);

      auto noise_future= pool.submit(
          GenerateTileNoise, std::ref(noise_generator),
          std::ref(new_tile.get()->noise_data_), x_tile_count, y_tile_count);

//      std::cout << "Generator: " << x_tile_count << ", " << y_tile_count << std::endl;
//      GenerateTileNoise(noise_generator, new_tile.get()->noise_data_, x_tile_count, y_tile_count);

      // FIXME: A vector of shared pointers may not stay consistent if the
      // vector is resized.
      world_tiles_.push_back(new_tile);
      x_tile_count++;
    }
    y_tile_count++;
    x_tile_count= 0;
  }
  std::cout << "Completed initial noise generation." << std::endl;

  pool.wait_for_tasks();

  std::cout << "Waiting for pool to close." << std::endl;

  for (auto& tile : world_tiles_)
  {
//    while (x_tile_count < (kWorldX / Tile::kTileX))
//    {
      tile->NoiseToBlock();
      tile->GenerateTilePixels();
      tile->GenerateTileTexture(true);
      std::cout << "Tile done" << std::endl;
//      x_tile_count++;
//    }
//    y_tile_count++;
//    x_tile_count= 0;
  }

//  for (auto row : world_tiles_)
//  {
//    for (auto tile : row)
//    {
//
//      tile.get()->GenerateTilePixels();
//      tile.get()->GenerateTileTexture();
//
//      x_tile_count++;
//    }
//    y_tile_count++;
//    x_tile_count= 0;
//  }

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
//      //            starting_area_generator->GenUniformGrid2D(starting_noise.at(noise_counter).data(),
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
//      // Could probably use some kind of GetTileAtCoordinate function here which
//      // will also be helpful in the future A function for updating the noise
//      // data may also be useful
//    }
//  }
}

bool World::XBlocksAway(PosXY center, PosXY block, uint16_t distance)
{
  int64_t absx= abs(block.x - center.x);
  int64_t absy= abs(block.y - center.y);
  if ((absx <= distance) && (absy <= distance))
  {
    return true;
  }
  else
  {
    return false;
  }
}

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

void World::FindPath(PosXY const origin, PosXY const destination, std::vector<World::PosXY> &results, World *world)
{
  AStarSearch<MapSearchNode> astarsearch;

  unsigned int SearchCount= 0;

  const unsigned int NumSearches= 1;

  while (SearchCount < NumSearches)
  {
    // Create a start state
    MapSearchNode nodeStart{};
    nodeStart.position= origin;

    // Define the goal state
    MapSearchNode nodeEnd{};
    nodeEnd.position= destination;

    // Set Start and goal states

    astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);

    unsigned int SearchState;
    unsigned int SearchSteps= 0;

    do
    {
      SearchState= astarsearch.SearchStep();

      SearchSteps++;
#if DEBUG_LISTS

      std::cout << "Steps:" << SearchSteps << "\n";

      int len= 0;

      std::cout << "Open:\n";
      MapSearchNode *p= astarsearch.GetOpenListStart();
      while (p)
      {
        len++;
#if !DEBUG_LIST_LENGTHS_ONLY
        ((MapSearchNode *)p)->PrintNodeInfo();
#endif
        p= astarsearch.GetOpenListNext();
      }

      std::cout << "Open list has " << len << " nodes\n";

      len= 0;

      std::cout << "Closed:\n";
      p= astarsearch.GetClosedListStart();
      while (p)
      {
        len++;
#if !DEBUG_LIST_LENGTHS_ONLY
        p->PrintNodeInfo();
#endif
        p= astarsearch.GetClosedListNext();
      }

      std::cout << "Closed list has " << len << " nodes\n";
#endif

    } while (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING);

    if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED)
    {
      //std::cout << "Search found goal state\n";

      MapSearchNode *node= astarsearch.GetSolutionStart();

#if DISPLAY_SOLUTION
      cout << "Displaying solution\n";
#endif
      int steps= 0;

      //node->PrintNodeInfo();
        node->block_type= World::GetBlockAtPos(node->position, world);
      results.push_back(node->position);
      for (;;)
      {
        node= astarsearch.GetSolutionNext();


        if (!node)
        {
          break;
        }

        node->block_type= World::GetBlockAtPos(node->position, world);

        results.push_back(node->position);
        steps++;
      };

      //std::cout << "Solution steps " << steps << std::endl;

      // Once you're done with the solution you can free the nodes up
      astarsearch.FreeSolutionNodes();
    }
    else if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED)
    {
      std::cout << "Search terminated. Did not find goal state\n";
    }

    // Display the number of loops the search went through
    //std::cout << "SearchSteps : " << SearchSteps << "\n";

    SearchCount++;

    astarsearch.EnsureMemoryFreed();
  }
}

World::Tile::~Tile() { UnloadImage(*tile_pixels); }

// Returns true if block placed successfully, false if it failed
bool World::PlaceBlockAtPos(PosXY const &my_position, PosXY &place_position,
                            BlockTypes &type)
{
  bool successfully_placed= false;
  // Check if we're within reach
  if (OneBlockAway(my_position, place_position))
  {
    // Is the place position empty
    if (GetBlockAtPos(place_position, this) == World::kBlockUnderground)
    {
      if (type == kBlockFood)
      {
        type= kBlockStockpiledFood;
      }
      this->SetBlockAtPos(place_position, type, *this);
      type = kBlockAir;
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
    if (GetBlockAtPos(place_position, this) != World::kBlockUnderground)
    {
      type= GetBlockAtPos(place_position, this);
      this->SetBlockAtPos(place_position, World::kBlockUnderground, *this);
      successfully_picked= true;
    }
  }
  else
  {
    successfully_picked= false;
  }
  return successfully_picked;
}