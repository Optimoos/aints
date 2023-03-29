#include "MapSearchNode.h"

void FindPath(PosXY const origin, PosXY const destination,
              std::vector<PosXY> &results, std::shared_ptr<World> world)
{
  AStarSearch<MapSearchNode> astarsearch;

  unsigned int SearchCount= 0;

  const unsigned int NumSearches= 1;

  while (SearchCount < NumSearches)
  {
    // Create a start state
    MapSearchNode nodeStart{};
    nodeStart.position= origin;
    nodeStart.world= world;

    // Define the goal state
    MapSearchNode nodeEnd{};
    nodeEnd.position= destination;
    nodeEnd.world= world;

    // Set Start and goal states

    astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);

    unsigned int SearchState;
    unsigned int SearchSteps= 0;

    do
    {
      SearchState= astarsearch.SearchStep();

      SearchSteps++;
#if DEBUG_LISTS

      std::cout << "Steps:" << SearchSteps << "\n";

      int len= 0;

      std::cout << "Open:\n";
      MapSearchNode *p= astarsearch.GetOpenListStart();
      while (p)
      {
        len++;
#if !DEBUG_LIST_LENGTHS_ONLY
        ((MapSearchNode *)p)->PrintNodeInfo();
#endif
        p= astarsearch.GetOpenListNext();
      }

      std::cout << "Open list has " << len << " nodes\n";

      len= 0;

      std::cout << "Closed:\n";
      p= astarsearch.GetClosedListStart();
      while (p)
      {
        len++;
#if !DEBUG_LIST_LENGTHS_ONLY
        p->PrintNodeInfo();
#endif
        p= astarsearch.GetClosedListNext();
      }

      std::cout << "Closed list has " << len << " nodes\n";
#endif

    } while (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING);

    if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED)
    {
      std::cout << "Search found goal state\n";

      MapSearchNode *node= astarsearch.GetSolutionStart();

#if DISPLAY_SOLUTION
      cout << "Displaying solution\n";
#endif
      int steps= 0;

      // node->PrintNodeInfo();
      node->block_type= World::GetBlockAtPos(node->position, world);
      results.push_back(node->position);
      for (;;)
      {
        node= astarsearch.GetSolutionNext();

        if (!node)
        {
          break;
        }

        node->block_type= World::GetBlockAtPos(node->position, world);

        results.push_back(node->position);
        steps++;
      };

      std::cout << "Solution steps " << steps << std::endl;

      // Once you're done with the solution you can free the nodes up
      astarsearch.FreeSolutionNodes();
    }
    else if (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED)
    {
      std::cout << "Search terminated. Did not find goal state\n";
    }

    // Display the number of loops the search went through
    // std::cout << "SearchSteps : " << SearchSteps << "\n";

    SearchCount++;

    astarsearch.EnsureMemoryFreed();
  }
}