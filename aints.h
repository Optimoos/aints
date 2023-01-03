//
// Created by Mark on 2022-07-18.
//

#ifndef AIANTS_AINTS_H
#define AIANTS_AINTS_H

#include <cstdint>
#include <vector>

class world
{
public:
    uint32_t get_world_position(uint32_t pos);

    std::vector<float> noise_output;

    world();
    ~world();

private:
    const uint16_t BLOCK_AIR = 0;
    const uint16_t BLOCK_DIRT = 1;
    const uint16_t BLOCK_GRASS = 2;
    const uint16_t BLOCK_FOOD = 3;
    const uint16_t BLOCK_STONE = 4;
    const uint16_t BLOCK_WATER = 5;
    const uint16_t BLOCK_SAND = 6;
    const uint16_t BLOCK_UNDERGROUND = 7;

    // Start with 8192 * 2048 world
    std::vector<uint16_t> worldspace;

};

class neuron
{
public:
private:
protected:
    float threshold = 0.0f;
};

class move_neuron: public neuron
{
public:
    move_neuron();
    void tick(float threshold, int64_t* x, int64_t* y);
private:
};

class aints
{
public:
    int setId(int newId);
    int getId();
    bool updateLocation(int64_t x, int64_t y);
    int64_t getX();
    int64_t getY();
    //std::vector<*neuron> neurons;

    aints();
    ~aints();


    void tick();

private:
    int id = 0;
    int64_t locX = 0;
    int64_t locY = 0;

    //move_neuron* mn = new move_neuron;
    move_neuron mn;
};

#endif //AIANTS_AINTS_H
