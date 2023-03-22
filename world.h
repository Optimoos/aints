#ifndef AIANTS_WORLD_H
#define AIANTS_WORLD_H

#include "BS_thread_pool.hpp"
#include "FastNoise/FastNoise.h"
#include "raylib.h"

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

    struct PosXY {
        int64_t x{0};
        int64_t y{0};
    };

    class Tile {
    public:
        static const uint16_t kTileX{256};
        static const uint16_t kTileY{256};
        std::vector<World::BlockTypes> blocks;
        std::vector<float> noise_data_;
        Texture2D tile_texture_;
        void RegenerateTexture();

    private:

    };

    World();
    ~World();

    BlockTypes GetBlockAtPos(int64_t x_pos, int64_t y_pos);
//    PosXY FindNearestBlockOfType(BlockTypes);
    PosXY FindNearestBlockOfType(PosXY center, BlockTypes type, uint64_t range);
    void SetBlockAtPos(int64_t x_pos, int64_t y_pos, BlockTypes type);
    void AddFood(int64_t x_pos, int64_t y_pos, int64_t size);
    Tile* PosToTile(int64_t x_pos, int64_t y_pos);
    std::vector<std::vector<Tile>> world_tiles_;


    const uint16_t kWorldX{8192};
    const uint16_t kWorldY{2048};
private:

};

void GenerateTileNoise(FastNoise::SmartNode<>& noise_generator, std::vector<float>& noise_data, uint16_t x_position, uint16_t y_position);
std::vector<World::BlockTypes> NoiseToBlock(std::vector<float> noise);
Texture2D GenerateTileTexture(std::vector<World::BlockTypes>& blocks);

#endif