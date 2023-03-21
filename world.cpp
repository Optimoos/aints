#include "world.h"

void GenerateTileNoise(FastNoise::SmartNode<>& noise_generator, std::vector<float>& noise_data, uint16_t x_position, uint16_t y_position) {

    noise_generator->GenUniformGrid2D(noise_data.data(),
                                      x_position * World::Tile::kTileX,
                                      y_position * World::Tile::kTileY,
                                      World::Tile::kTileX,
                                      World::Tile::kTileY,
                                      0.02f, 1337);

}

World::BlockTypes World::GetBlockAtPos(int64_t x_pos, int64_t y_pos) {
    const int64_t x_loc = x_pos % Tile::kTileX;
    const int64_t y_loc = y_pos % Tile::kTileY;
    const Tile* tile = PosToTile(x_pos, y_pos);
    return tile->blocks.at(y_loc * Tile::kTileY + x_loc);
}

void World::SetBlockAtPos(int64_t x_pos, int64_t y_pos, World::BlockTypes type) {
    const int64_t x_loc = x_pos % Tile::kTileX;
    const int64_t y_loc = y_pos % Tile::kTileY;
    Tile* tile = PosToTile(x_pos, y_pos);
    tile->blocks.at(y_loc * Tile::kTileY + x_loc) = type;
}

std::vector<World::BlockTypes> NoiseToBlock(std::vector<float> noise) {
    std::vector<World::BlockTypes> blocks;
    blocks.resize(noise.size());

    uint32_t location_counter = 0;
    for (float block : noise) {
        if ((block > 0.0f) && (block <= 0.1f)) {
            blocks.at(location_counter) = World::kBlockStone;
        } else if ((block > 0.1f) && (block <=0.5f)) {
            blocks.at(location_counter) = World::kBlockDirt;
        } else if ((block > 0.5f) && (block <=1.0f)) {
            blocks.at(location_counter) = World::kBlockUnderground;
        }
        location_counter++;
    }
    return blocks;
}

World::Tile* World::PosToTile(int64_t x_pos, int64_t y_pos) {
    const uint16_t x_tile = x_pos / Tile::kTileX;
    const uint16_t y_tile = y_pos / Tile::kTileY;
    return &this->world_tiles_.at(y_tile).at(x_tile);
}

void World::Tile::RegenerateTexture() {
    this->tile_texture_ = GenerateTileTexture(this->blocks);
}

Texture2D GenerateTileTexture(std::vector<World::BlockTypes>& blocks) {
    Image image_canvas = GenImageColor(World::Tile::kTileX, World::Tile::kTileY, BLANK);
    for (uint16_t y_position = 0; y_position < World::Tile::kTileY; y_position++) {
        for (uint16_t x_position = 0; x_position < World::Tile::kTileX; x_position++) {
            switch (blocks.at((y_position * World::Tile::kTileY) + x_position)){
                case 0:
                    ImageDrawPixel(&image_canvas, x_position, y_position, SKYBLUE);
                    break;
                case 1:
                    ImageDrawPixel(&image_canvas, x_position, y_position, BROWN);
                    break;
                case 2:
                    ImageDrawPixel(&image_canvas, x_position, y_position, GREEN);
                    break;
                case 3:
                    ImageDrawPixel(&image_canvas, x_position, y_position, RED);
                    break;
                case 4:
                    ImageDrawPixel(&image_canvas, x_position, y_position, DARKGRAY);
                    break;
                case 5:
                    ImageDrawPixel(&image_canvas, x_position, y_position, BLUE);
                    break;
                case 6:
                    ImageDrawPixel(&image_canvas, x_position, y_position, YELLOW);
                    break;
                case 7:
                    ImageDrawPixel(&image_canvas, x_position, y_position, DARKBROWN);
                    break;
                case 8:
                    ImageDrawPixel(&image_canvas, x_position, y_position, PURPLE);
                    break;
            }
        }
    }
    Texture2D texture = LoadTextureFromImage(image_canvas);
    UnloadImage(image_canvas);
    return texture;
}

void World::AddFood(int64_t x_pos, int64_t y_pos, int64_t size) {
    for (int64_t x = x_pos - size; x <= x_pos + size; x++)
    {
        int64_t y_range = sqrt(pow(size, 2) - pow(x - x_pos, 2));
        for (int64_t y = y_pos - y_range; y <= y_pos + y_range; y++)
        {
            SetBlockAtPos(x, y, kBlockFood);
            //FIXME: This happening as part of the loop is not efficient
            Tile* tile = PosToTile(x_pos, y_pos);
            tile->RegenerateTexture();
        }
    }
}


World::World() {

//    auto fnSimplex = FastNoise::New<FastNoise::OpenSimplex2>();
//    auto fnFractal = FastNoise::New<FastNoise::FractalFBm>();
//
//    fnFractal->SetSource(fnSimplex);
//    fnFractal->SetGain(1.820f);
//    fnFractal->SetOctaveCount(3);
//    fnFractal->SetLacunarity(0.3f);
//
//    auto fnDomainScale = FastNoise::New<FastNoise::DomainScale>();
//
//    fnDomainScale->SetSource(fnFractal);
//    fnDomainScale->SetScale(0.66f);
//
//    auto fnPositionOutput = FastNoise::New<FastNoise::PositionOutput>();
//    fnPositionOutput->Set<FastNoise::Dim::Y>(3.0f, -7.0f);
//
//    auto fnAdd1 = FastNoise::New<FastNoise::Add>();
//    fnAdd1->SetLHS(fnDomainScale);
//    fnAdd1->SetRHS(fnPositionOutput);
//
//    auto fnDomainWarp = FastNoise::New<FastNoise::DomainWarpGradient>();
//    fnDomainWarp->SetSource(fnAdd1);
//    fnDomainWarp->SetWarpAmplitude(0.86f);
//    fnDomainWarp->SetWarpFrequency(0.32f);
//
//    auto fnGenerator = FastNoise::New<FastNoise::Max>();
//    fnGenerator->SetLHS(fnDomainScale);
//    fnGenerator->SetRHS(fnDomainWarp);

//
//    fnGenerator->GenUniformGrid2D(noise_output.data(), 0, 0, 1600, 900, 0.02f, 1337);
//    fnGenerator->GenTileable2D(noise_output.data(), 256, 256, 0.02f, 1337);


    FastNoise::SmartNode<> noise_generator = FastNoise::NewFromEncodedNodeTree( "FwAAAIC/AACAPwAAAAAAAIA/DAABAAAAzczMPQkAAAAAgD8=" );
//    auto fnGenerator = FastNoise::New<FastNoise::Checkerboard>();
//    auto fnGenerator = FastNoise::New<FastNoise::OpenSimplex2>();
//    auto fnGenerator = FastNoise::New<FastNoise::PositionOutput>();
//    fnGenerator->Set<FastNoise::Dim::Y>(3.0f, 0.0f);
//    fnGenerator->Set<FastNoise::Dim::X>(0.1f, 1.0f);


    // Pre-size world storage and fill with initial data
    uint16_t y_tile_count = 0;
    uint16_t x_tile_count = 0;
    world_tiles_.resize(kWorldY / Tile::kTileY);
    BS::thread_pool pool;
    for (std::vector<Tile>& row : world_tiles_) {
        row.resize(kWorldX / Tile::kTileX);
        for (Tile& tile : row) {
            tile.blocks.resize(Tile::kTileX * Tile::kTileY);
            tile.noise_data_.resize(World::Tile::kTileX * World::Tile::kTileY);

            auto noise_future = pool.submit(GenerateTileNoise, std::ref(noise_generator), std::ref(tile.noise_data_), x_tile_count, y_tile_count);
            //GenerateTileNoise(std::ref(noise_generator), std::ref(tile.noise_data_), x_tile_count, y_tile_count);
            //noise_future.wait();


//            std::thread noise_thread(GenerateTileNoise, std::ref(tile), std::ref(noise_generator), x_tile_count, y_tile_count);
//            noise_thread.join();
            x_tile_count++;
        }
        y_tile_count++;
        x_tile_count = 0;
    }

    pool.wait_for_tasks();

    for (std::vector<Tile>& row : world_tiles_) {
        for (Tile& tile : row) {
            tile.blocks = NoiseToBlock(tile.noise_data_);

            x_tile_count++;
        }
        y_tile_count++;
        x_tile_count = 0;
    }

    for (std::vector<Tile>& row : world_tiles_) {
        for (Tile& tile : row) {
            //tile.tile_texture_ = GenerateTileTexture(tile.blocks);
            tile.RegenerateTexture();

            x_tile_count++;
        }
        y_tile_count++;
        x_tile_count = 0;
    }

    // Generate starting area
    FastNoise::SmartNode<> starting_area_generator = FastNoise::NewFromEncodedNodeTree( "FwAAAIC/AACAPwAAAAAAAIA/EAAAAAA/EwAAAIA+IgAAAIA/zcxMPgUAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAACAPw==" );
    std::vector<std::vector<float>> starting_noise;
    starting_noise.resize(4);
    for (std::vector<float> entry : starting_noise) {
        entry.resize(Tile::kTileX * Tile::kTileY);
    }
    uint8_t noise_counter = 0;
    for (uint8_t y_location = 0; y_location <=1; y_location++) {
        for (uint8_t x_location = 0; x_location <= 1; x_location++) {
//            starting_area_generator->GenUniformGrid2D(starting_noise.at(noise_counter).data(),
//                                              x_location,
//                                              y_location,
//                                              World::Tile::kTileX,
//                                              World::Tile::kTileY,
//                                              0.02f, 1337);
            noise_counter++;

            // Once noise is generated, it needs to be added to the existing noise data for the tiles in the middle of the map

            // Could probably use some kind of GetTileAtCoordinate function here which will also be helpful in the future
            // A function for updating the noise data may also be useful
        }
    }

}

World::~World() {

}