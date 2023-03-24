#ifndef AIANTS_WORLD_H
#define AIANTS_WORLD_H

#include "BS_thread_pool.hpp"
#include "FastNoise/FastNoise.h"
#include "raylib.h"
#include "stlastar.h"

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
        kBlockUnderground,
        kBlockStockpiledFood
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

    static BlockTypes GetBlockAtPos(PosXY blockpos);
//    PosXY FindNearestBlockOfType(BlockTypes);
    PosXY FindNearestBlockOfType(PosXY center, BlockTypes type, uint64_t range);
    void SetBlockAtPos(PosXY position, BlockTypes type);
    void AddFood(int64_t x_pos, int64_t y_pos, int64_t size);
    static Tile* PosToTile(int64_t x_pos, int64_t y_pos);
    static std::vector<std::vector<Tile>> world_tiles_;
    bool OneBlockAway(PosXY center, PosXY block);
    bool XBlocksAway(PosXY center, PosXY block, uint16_t distance);
    std::vector<PosXY> FindPath(PosXY origin, PosXY destination);


    const uint16_t kWorldX{8192};
    const uint16_t kWorldY{2048};
private:

};

void GenerateTileNoise(FastNoise::SmartNode<>& noise_generator, std::vector<float>& noise_data, uint16_t x_position, uint16_t y_position);
std::vector<World::BlockTypes> NoiseToBlock(std::vector<float> noise);
Texture2D GenerateTileTexture(std::vector<World::BlockTypes>& blocks);


// FIXME: The below is mostly lifted directly from the findpath.cpp example for the A* library, this can probably
//        be cleaned up for our specific use case

// Definitions

class MapSearchNode
{
public:
    int x;	 // the (x,y) positions of the node
    int y;

    MapSearchNode() { x = y = 0; }
    MapSearchNode( int px, int py ) { x=px; y=py; }

    inline float GoalDistanceEstimate( MapSearchNode &nodeGoal );
    inline bool IsGoal( MapSearchNode &nodeGoal );
    inline bool GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node );
    inline float GetCost( MapSearchNode &successor );
    inline bool IsSameState( MapSearchNode &rhs );

    inline void PrintNodeInfo();


};

bool MapSearchNode::IsSameState( MapSearchNode &rhs )
{

    // same state in a maze search is simply when (x,y) are the same
    if( (x == rhs.x) &&
        (y == rhs.y) )
    {
        return true;
    }
    else
    {
        return false;
    }

}

void MapSearchNode::PrintNodeInfo()
{
    char str[100];
    sprintf( str, "Node position : (%d,%d)\n", x,y );

    std::cout << str;
}

// Here's the heuristic function that estimates the distance from a Node
// to the Goal.

float MapSearchNode::GoalDistanceEstimate( MapSearchNode &nodeGoal )
{
    return abs(x - nodeGoal.x) + abs(y - nodeGoal.y);
}

bool MapSearchNode::IsGoal( MapSearchNode &nodeGoal )
{

    if( (x == nodeGoal.x) &&
        (y == nodeGoal.y) )
    {
        return true;
    }

    return false;
}

// This generates the successors to the given Node. It uses a helper function called
// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
// is done for each node internally, so here you just set the state information that
// is specific to the application
bool MapSearchNode::GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node)
{

    int parent_x = -1;
    int parent_y = -1;

    if( parent_node )
    {
        parent_x = parent_node->x;
        parent_y = parent_node->y;
    }


    MapSearchNode NewNode;

    // push each possible move except allowing the search to go backwards

    if( (World::GetBlockAtPos( World::PosXY{x-1, y} ) == World::kBlockUnderground)
        && !((parent_x == x-1) && (parent_y == y))
            )
    {
        NewNode = MapSearchNode( x-1, y );
        astarsearch->AddSuccessor( NewNode );
    }

    if( (World::GetBlockAtPos( World::PosXY{ x, y-1 } ) == World::kBlockUnderground)
        && !((parent_x == x) && (parent_y == y-1))
            )
    {
        NewNode = MapSearchNode( x, y-1 );
        astarsearch->AddSuccessor( NewNode );
    }

    if( (World::GetBlockAtPos( World::PosXY{ x+1, y } ) == World::kBlockUnderground)
        && !((parent_x == x+1) && (parent_y == y))
            )
    {
        NewNode = MapSearchNode( x+1, y );
        astarsearch->AddSuccessor( NewNode );
    }


    if( (World::GetBlockAtPos( World::PosXY{ x, y+1 } ) == World::kBlockUnderground)
        && !((parent_x == x) && (parent_y == y+1))
            )
    {
        NewNode = MapSearchNode( x, y+1 );
        astarsearch->AddSuccessor( NewNode );
    }

    return true;
}

// given this node, what does it cost to move to successor. In the case
// of our map the answer is the map terrain value at this node since that is
// conceptually where we're moving

float MapSearchNode::GetCost( MapSearchNode &successor )
{
    //FIXME: We only support a single allowable block initially so we return a constant cost
    return (float) 1.0f;

}

#endif