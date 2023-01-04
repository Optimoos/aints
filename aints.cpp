//
// Created by Mark on 2022-07-18.
//

#include "aints.h"
#include <entt/entt.hpp>
#include <iostream>
#include "FastNoise/FastNoise.h"


int aints::setId(int newId)
{
    this->id = newId;
    return this->id;
}

int aints::getId()
{
    return this->id;
}

bool aints::updateLocation(int64_t x, int64_t y)
{
    this->locX = x;
    this->locY = y;
    if ((this->locX == x) && (this->locY == y))
    {
        return true;
    } else
        return false;

};
int64_t aints::getX()
{
    return this->locX;
};
int64_t aints::getY()
{
    return this->locY;
};


aints::aints()
{

};

aints::~aints()
{

};

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

uint32_t world::get_world_position(uint32_t pos){
    return worldspace.at(pos);
}

world::world()
{

//    auto fnSimplex = FastNoise::New<FastNoise::Simplex>();
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
//    FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree( "HgAQAArXoz4ZABMAw/UoPw0AAwAAAJqZmT4JAADD9eg/AAAAgD8BBAAAAAAAAABAQAAAAAAAAAAAAAAAAAAA4MAAAAAAAAAAAAD2KFw/Af//AgA=" );
//
//    fnGenerator->GenUniformGrid2D(noise_output.data(), 0, 0, 1600, 900, 0.02f, 1337);
//    fnGenerator->GenTileable2D(noise_output.data(), 256, 256, 0.02f, 1337);


    //auto fnGenerator = FastNoise::New<FastNoise::Checkerboard>();
    auto fnGenerator = FastNoise::New<FastNoise::OpenSimplex2>();


    // Pre-size world storage and fill with initial data
    uint16_t y_tile_count = 0;
    uint16_t x_tile_count = 0;
    worldtiles.resize(WORLD_Y/worldtile::TILE_Y);
    for (std::vector<worldtile>& x_tiles : worldtiles) {
        x_tiles.resize(WORLD_X/worldtile::TILE_X);
        for (worldtile& tile : x_tiles) {
            tile.blocks.resize(worldtile::TILE_X * worldtile::TILE_Y);

            fnGenerator->GenUniformGrid2D(tile.blocks.data(),
                                        x_tile_count * worldtile::TILE_X,
                                        y_tile_count * worldtile::TILE_Y,
                                        worldtile::TILE_X,
                                        worldtile::TILE_Y,
                                        0.02f, 1337);
            x_tile_count++;
        }
        y_tile_count++;
        x_tile_count = 0;
    }
}

world::~world()
{

}