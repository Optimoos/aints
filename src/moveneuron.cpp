#include "moveneuron.h"

// Handles state machine for the neuron
void MoveNeuron::Tick(float threshold)
{
  PosXY newXY{0, 0};

  // FIXME: Temporarily setting threshold to 0 for full speed ants
  this->threshold= 0.0f;
  if (threshold > this->threshold)
  {
    switch (brain.current_task)
    {
      // Random movement if we're wandering or searching for food
      case Brain::kTaskWandering:
        RandomMovement(brain.current_position, newXY);
        break;
        // FIXME: food searching random movement will be replaced with pheromone
        // behavior
      case Brain::kTaskSearchingFood:

        // There is no movement action to take here at this time, the detect
        // neuron will set the destination and the task neuron will change the
        // state.
        break;
      case Brain::kTaskGatheringFood:
        // If the destination has expired go back to searching
        if (!brain.current_destination.Expired())
        {
          GatherNavigate(brain, newXY);
          break;
        }
        break;
      case Brain::kTaskDeliveringFood:

        if (this->brain.current_destination.Expired())
        {
          SetFoodStockpileAsDestination();
        }
        DeliverNavigate(brain, newXY);
        break;
      default:
        std::cout << "Move neuron was given invalid task" << std::endl;
        break;
    }
    //DoMovement(newXY);
  }
}

void MoveNeuron::RandomMovement(PosXY &original_location, PosXY &new_location)
{
  new_location.x= original_location.x + ((std::rand() % 3) - 1);
  new_location.y= original_location.y + ((std::rand() % 3) - 1);

  // FIXME: Something else should probably trigger the state change to searching
  if (brain.carrying != kBlockAir)
  {
    brain.current_task= Brain::kTaskGatheringFood;
  }
}

void MoveNeuron::GatherNavigate(Brain &brain, PosXY &next_coord)
{

  std::vector<std::future<void>> path_futures;

  // Make sure we arrived at last coordinate we tried to move to from the path
  ValidateLastMove();

  // If path is empty, go back to searching
  if (brain.path_to_target.empty())
  {
    brain.current_task= Brain::kTaskSearchingFood;
  }
  if (!brain.path_to_target.empty())
  {
    next_coord= *brain.path_to_target.begin();
    DoMovement(next_coord);
  } else {
//    auto search_future= path_pool.submit(
//        FindPath, brain.current_position, brain.current_destination.position,
//        std::ref(brain.path_to_target), brain.world);

    path_futures.push_back(std::async(std::launch::async, FindPath, brain.current_position, brain.current_destination.position,
            std::ref(brain.path_to_target), brain.world));

//    FindPath(brain.current_position, brain.current_destination.position,
//             brain.path_to_target, brain.world);
    // FIXME: Need to do something here if A* navigation fails
  }
}

void MoveNeuron::DeliverNavigate(Brain &brain, PosXY &next_coord)
{
  // Make sure we arrived at last coordinate
  ValidateLastMove();
  // If path is empty, go back to searching
  if (brain.path_to_target.empty())
  {
    // Leave us in delivering state but force state machine to find a new path
    brain.current_destination.SetExpired();
  }
  if (!brain.path_to_target.empty())
  {
    brain.current_task= Brain::kTaskGatheringFood;

    next_coord= *brain.path_to_target.begin();
    DoMovement(next_coord);
  } else {
    FindPath(brain.current_position, brain.current_destination.position,
             brain.path_to_target, brain.world);
    // FIXME: Need to do something here if A* navigation fails
  }

}

void MoveNeuron::ValidateLastMove()
{
  if (!brain.path_to_target.empty())
  {
    PosXY position_to_check= *brain.path_to_target.begin();
    while (brain.current_position == position_to_check && !brain.path_to_target.empty())
    {
      brain.path_to_target.erase(brain.path_to_target.begin());
      if (!brain.path_to_target.empty())
      {
        position_to_check= *brain.path_to_target.begin();
      }
      // This should be unnecessary and should be removed
      //      else
      //      {
      //        break;
      //      }
    }
  }
}

void MoveNeuron::SetFoodStockpileAsDestination()
{
  // FIXME: Arbitrarily searching 20 blocks around the stockpile, should
  // be dynamic
  this->brain.dropoff_position.position=
      this->brain.world->FindNearestBlockOfType(
          this->brain.food_stockpile, kBlockUnderground, 20);
  this->brain.current_destination.position=
      this->brain.world->FindNearestBlockOfType(
          this->brain.dropoff_position.position, kBlockUnderground,
          static_cast<uint64_t>(2), true);
  this->brain.current_destination.StartTimer(10000, brain.world);
  this->brain.dropoff_position.StartTimer(10000, brain.world);
}

void MoveNeuron::DoMovement(PosXY &location)
{
  if (location != PosXY{0, 0} && location != brain.current_position)
  {
    BlockTypes block= World::GetBlockAtPos(location, brain.world);
    if (block == BlockTypes::kBlockUnderground)
    {
      // std::cout << "Path info: " << brain.current_position.x << ", " <<
      // brain.current_position.y << " New: " << newXY.x << ", " << newXY.y <<
      // " Block: " << brain.world->GetBlockAtPos(newXY) << std::endl;
      MoveOneTowards(brain.current_position, location);
    }
    else
    {
      //std::cout << "Block: " << block << std::endl;
      if (!brain.path_to_target.empty())
      {
        brain.path_to_target.pop_back();
      }
    }
  }
  else if (location == brain.current_position)
  {
    std::cout << "Ant is trying to move to the position it's already in"
              << std::endl;
  }
  else
  {
    std::cout << "THIS SHOULD NEVER HAPPEN" << std::endl;
    std::cout << " location: " << location.x << ", " << location.y << std::endl;
    std::cout << " brain.current_position: " << brain.current_position.x
              << ", " << brain.current_position.y << std::endl;
  }
}


void MoveNeuron::MoveOneTowards(PosXY &origin, PosXY &destination)
{
  if (!(origin == destination))
  {
    // std::cout << "Next position origin: " << origin.x << ", " << origin.y <<
    // std::endl; std::cout << "Next position destination: " << destination.x <<
    // ", " << destination.y << std::endl;
    double distance= sqrt(pow(destination.x - origin.x, 2) +
                          pow(destination.y - origin.y, 2));
    double directionX= (destination.x - origin.x) / distance;
    double directionY= (destination.y - origin.y) / distance;
    PosXY next{};
    next.x= round(origin.x + directionX);
    next.y= round(origin.y + directionY);
    // std::swap(origin, next);
    origin= next;
    if ((next.x < 0) || (next.y < 0))
    {
      std::cout << "STOP HERE WTF IS GOING ON!" << std::endl;
    }
  }
  else
  {
    std::cout << "Is this happening a lot?" << std::endl;
  }
}


MoveNeuron::MoveNeuron(Brain &brain) : Neuron(brain)
{
  // this->brain = brain;
  this->threshold= 0.9f;
}

void MoveNeuron::ReceiveSignal(float weight) {}
