//
// Created by Mark on 2022-07-18.
//

#include "aints.h"
#include <entt/entt.hpp>
#include <iostream>
#include "FastNoise/FastNoise.h"

void GenerateTileNoise(FastNoise::SmartNode<>& noise_generator, std::vector<float>& noise_data, uint16_t x_position, uint16_t y_position) {

    noise_generator->GenUniformGrid2D(noise_data.data(),
                                  x_position * World::Tile::kTileX,
                                  y_position * World::Tile::kTileY,
                                  World::Tile::kTileX,
                                  World::Tile::kTileY,
                                  0.02f, 1337);

}

int aints::setId(int newId) {
    this->id = newId;
    return this->id;
}

int aints::getId() {
    return this->id;
}

bool aints::updateLocation(int64_t x, int64_t y) {
    this->locX = x;
    this->locY = y;
    if ((this->locX == x) && (this->locY == y))
    {
        return true;
    } else
        return false;
}

int64_t aints::getX() {
    return this->locX;
}

int64_t aints::getY() {
    return this->locY;
}

aints::aints() {

}

aints::~aints() {

}

void move_neuron::tick(float tick_threshold, int64_t* x, int64_t* y) {
    //int64_t newX = this->getX() + ((std::rand() % 3) -1);
    //int64_t newY = this->getY() + ((std::rand() % 3) -1);
    if (tick_threshold > this->threshold) {
        *x += ((std::rand() % 3) -1);
        *y += ((std::rand() % 3) -1);
    }
    //this->updateLocation(newX, newY);
}

void aints::tick() {
    this->mn.tick(1.0f, &this->locX, &this->locY);
    this->updateLocation(this->getX(), this->getY());
}

move_neuron::move_neuron() {
    this->threshold = 0.7f;
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
            tile.tile_texture_ = GenerateTileTexture(tile.blocks);

            x_tile_count++;
        }
        y_tile_count++;
        x_tile_count = 0;
    }

}

World::~World() {

}

std::vector<uint8_t> NoiseToBlock(std::vector<float> noise) {
    std::vector<uint8_t> blocks;
    blocks.resize(noise.size());

    uint32_t location_counter = 0;
    for (float block : noise) {
        if ((block > 0.0f) && (block <= 0.1f)) {
            blocks.at(location_counter) = World::Tile::kBlockStone;
        } else if ((block > 0.1f) && (block <=0.5f)) {
            blocks.at(location_counter) = World::Tile::kBlockDirt;
        } else if ((block > 0.5f) && (block <=1.0f)) {
            blocks.at(location_counter) = World::Tile::kBlockUnderground;
        }
        location_counter++;
    }
    return blocks;
}


Texture2D GenerateTileTexture(std::vector<uint8_t>& blocks) {
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



