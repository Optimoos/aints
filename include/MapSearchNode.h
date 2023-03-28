#ifndef AIANTS_MAPSEARCHNODE_H
#define AIANTS_MAPSEARCHNODE_H

#include "aints_types.h"
#include "stlastar.h"
#include "world.h"

// FIXME: The below is mostly lifted directly from the findpath.cpp example for
// the A* library, this can probably be cleaned up for our specific use case

// Definitions

class MapSearchNode
{
 public:
  PosXY position{0, 0};  // the (x,y) positions of the node
  BlockTypes block_type{};
  World *world;

  MapSearchNode() { block_type= kBlockAir; }

  MapSearchNode(World *world, int64_t px, int64_t py)
  {
    position.x= px;
    position.y= py;
    block_type= World::GetBlockAtPos(position, world);
  }

  bool operator==(const MapSearchNode &rhs) const
  {
    return position == rhs.position;
  }

  inline float GoalDistanceEstimate(MapSearchNode &nodeGoal);
  inline bool IsGoal(MapSearchNode &nodeGoal);
  inline bool GetSuccessors(AStarSearch<MapSearchNode> *astarsearch,
                            MapSearchNode *parent_node);
  inline float GetCost(MapSearchNode &successor);
  inline bool IsSameState(MapSearchNode &rhs);
  inline void PrintNodeInfo(){};
};

void FindPath(PosXY origin, PosXY destination, std::vector<PosXY> &results,
              World *world);

bool MapSearchNode::IsSameState(MapSearchNode &rhs)
{
  // same state in a maze search is simply when (x,y) are the same
  if (position == rhs.position)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// Here's the heuristic function that estimates the distance from a Node
// to the Goal.

float MapSearchNode::GoalDistanceEstimate(MapSearchNode &nodeGoal)
{
  return abs(position.x - nodeGoal.position.x) +
         abs(position.y - nodeGoal.position.y);
}

bool MapSearchNode::IsGoal(MapSearchNode &nodeGoal)
{
  if (position == nodeGoal.position)
  {
    return true;
  }

  return false;
}

// This generates the successors to the given Node. It uses a helper function
// called AddSuccessor to give the successors to the AStar class. The A*
// specific initialisation is done for each node internally, so here you just
// set the state information that is specific to the application
bool MapSearchNode::GetSuccessors(AStarSearch<MapSearchNode> *astarsearch,
                                  MapSearchNode *parent_node)
{
  PosXY parent_position{-1, -1};

  if (parent_node)
  {
    parent_position= parent_node->position;
  }

  MapSearchNode NewNode{};

  // push each possible move

  int dx[8]= {-1, -1, -1, 0, 0, 1, 1, 1};
  int dy[8]= {-1, 0, 1, -1, 1, -1, 0, 1};

  for (int i= 0; i < 8; i++)
  {
    int x= position.x + dx[i];
    int y= position.y + dy[i];
    auto pos= PosXY{x, y};
    if (World::GetBlockAtPos(pos, world) == kBlockUnderground)
    {
      NewNode= MapSearchNode(world, x, y);
      astarsearch->AddSuccessor(NewNode);
    }
  }

  return true;
}

// given this node, what does it cost to move to successor. In the case
// of our map the answer is the map terrain value at this node since that is
// conceptually where we're moving

float MapSearchNode::GetCost(MapSearchNode &successor)
{
  // FIXME: We only support a single allowable block initially so we return a
  // constant cost
  float cost{99.0f};
  BlockTypes successor_block=
      World::GetBlockAtPos(successor.position, this->world);
  switch (successor_block)
  {
    case kBlockUnderground:
      cost= 1.0f;
      break;
    case kBlockFood:
      cost= 10.0f;
      break;
    case kBlockDirt:
      cost= 50.0f;
      break;
    default:
      break;
  }
  return cost;
}

#endif  // AIANTS_MAPSEARCHNODE_H