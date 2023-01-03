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
    worldspace.resize(16777216);

    // Create an array of floats to store the noise output in
    //std::vector<float> noiseOutput(8192*2048);
    noise_output.resize(1600*900);

    // Generate an 8192 * 2048 area of noise

    FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree( "CQA=" );

    fnGenerator->GenUniformGrid2D(noise_output.data(), 0, 0, 1600, 900, 0.02f, 1337);

    for (uint32_t index = 0; index < (1600*900); index++) {
        noise_output.at(index) = (noise_output.at(index) + 1) * 0.5;
        // Set top 200px to sky
//        if (index < (1600*5)) {
//            worldspace.at(index) = 0;
//        } else {
            if (noise_output.at(index) < 0.3f ) {
                worldspace.at(index) = BLOCK_STONE;
            } else if ((noise_output.at(index) >= 0.3f) && (noise_output.at(index) < 0.7f)) {
                worldspace.at(index) = BLOCK_DIRT;
            } else if ((noise_output.at(index) >= 0.7f) && (noise_output.at(index) < 0.8f)) {
                worldspace.at(index) = BLOCK_SAND;
            } else {
                worldspace.at(index) = BLOCK_UNDERGROUND;
            }
//        }

    }

}

world::~world()
{

}