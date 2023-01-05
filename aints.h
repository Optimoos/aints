//
// Created by Mark on 2022-07-18.
//

#ifndef AIANTS_AINTS_H
#define AIANTS_AINTS_H

#include <cstdint>
#include <vector>
#include "raylib.h"

class World
{
public:

    World();
    ~World();

    class WorldTile {
    public:
        static const uint16_t kTileX = 256;
        static const uint16_t kTileY = 256;
        std::vector<uint8_t> blocks;
        //std::vector<float> blocks;
        Texture2D tile_texture_;

        uint8_t NoiseToBlock(float noise);
        Texture2D GenerateTileTexture();
    private:
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
    };
    std::vector<std::vector<WorldTile>> world_tiles_;

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
