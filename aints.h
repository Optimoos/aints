//
// Created by Mark on 2022-07-18.
//

#ifndef AIANTS_AINTS_H
#define AIANTS_AINTS_H

#include <cstdint>
#include <vector>
#include "raylib.h"
#include "FastNoise/FastNoise.h"
#include "BS_thread_pool.hpp"

class World
{
public:
    enum BlockTypes {
        kBlockAir,
        kBlockDirt,
        kBlockGrass,
        kBlockFood,
        kBlockStone,
        kBlockWater,
        kBlockSand,
        kBlockUnderground
    };

    class Tile {
    public:
        static const uint16_t kTileX = 256;
        static const uint16_t kTileY = 256;
        std::vector<World::BlockTypes> blocks;
        std::vector<float> noise_data_;
        Texture2D tile_texture_;
        void RegenerateTexture();

   private:

    };

    World();
    ~World();

    BlockTypes GetBlockAtPos(int64_t x_pos, int64_t y_pos);
    void SetBlockAtPos(int64_t x_pos, int64_t y_pos, World::BlockTypes type);
    void AddFood(int64_t x_pos, int64_t y_pos, int64_t size);
    Tile* PosToTile(int64_t x_pos, int64_t y_pos);
    std::vector<std::vector<Tile>> world_tiles_;


    const uint16_t kWorldX = 8192;
    const uint16_t kWorldY = 2048;
private:

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
    void tick(float threshold, int64_t* x, int64_t* y, World* world);
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
    World* world;
    //std::vector<*neuron> neurons;

    aints(World* world);
    ~aints();


    void tick();

private:
    int id = 0;
    int64_t locX = 0;
    int64_t locY = 0;

    //move_neuron* mn = new move_neuron;
    move_neuron mn;
};

void GenerateTileNoise(FastNoise::SmartNode<>& noise_generator, std::vector<float>& noise_data, uint16_t x_position, uint16_t y_position);
std::vector<World::BlockTypes> NoiseToBlock(std::vector<float> noise);
Texture2D GenerateTileTexture(std::vector<World::BlockTypes>& blocks);


#endif //AIANTS_AINTS_H
