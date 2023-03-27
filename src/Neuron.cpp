#include "Neuron.h"

void move_neuron::RandomMovement(World::PosXY &original_location,
                                 World::PosXY &new_location)
{
  new_location.x= original_location.x + ((std::rand() % 3) - 1);
  new_location.y= original_location.y + ((std::rand() % 3) - 1);
}

void move_neuron::GatherNavigate(Brain &brain, World::PosXY &next_coord)
{
  // Make sure we arrived at last coordinate
  if (!brain.path_to_target.empty())
  {
    World::PosXY position_to_check= *brain.path_to_target.begin();
    while (brain.current_position == position_to_check)
    {
      brain.path_to_target.erase(brain.path_to_target.begin());
      if (!brain.path_to_target.empty())
      {
        position_to_check= *brain.path_to_target.begin();
      }
      else
      {
        break;
      }
    }
  }
  // If path is empty, generate a new one
  if (brain.path_to_target.empty())
  {
    World::FindPath(brain.current_position,
                          brain.current_destination.position,
                          brain.path_to_target, brain.world);
    // Make sure path generation didn't fail
    // Remove our current position
    while (!brain.path_to_target.empty() &&
           (brain.current_position == *brain.path_to_target.begin()))
    {
      brain.path_to_target.erase(brain.path_to_target.begin());
    }
  }
  if (!brain.path_to_target.empty())
  {
    next_coord= *brain.path_to_target.begin();
  }
  else
  {
    brain.current_task= Brain::kTaskWandering;
  }
}

void move_neuron::tick(float threshold)
{
  World::PosXY newXY{0, 0};

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
        if (brain.carrying != World::kBlockAir)
        {
          brain.current_task= Brain::kTaskDeliveringFood;
        }
        RandomMovement(brain.current_position, newXY);
        break;
      case Brain::kTaskGatheringFood:
        // If the destination has expired and we're not carrying anything, go
        // back to wandering
        if (brain.current_destination.Expired() &&
            (brain.carrying == World::kBlockAir))
        {
          brain.current_task= Brain::kTaskWandering;
          break;
        }
        // Make sure our sensed destination hasn't expired
        if (!brain.sensed_food.Expired())
        {
          GatherNavigate(brain, newXY);
        }
        break;
      case Brain::kTaskDeliveringFood:

        if (brain.current_destination.Expired() &&
            (brain.carrying == World::kBlockAir))
        {
          brain.current_task= Brain::kTaskWandering;
          break;
        }

        if (this->brain.current_destination.Expired())
        {
          // FIXME: Arbitrarily searching 20 blocks around the stockpile, should be
          // dynamic
//          this->brain.dropoff_position= this->brain.world->FindNearestBlockOfType(
//              this->brain.food_stockpile, World::kBlockUnderground, 20);
          this->brain.current_destination.position=
              this->brain.world->FindNearestBlockOfType(
                  this->brain.food_stockpile, World::kBlockUnderground, 2);
          std::cout << "Delivering to: " << this->brain.current_destination.position.x << ", "
                    << this->brain.current_destination.position.y << std::endl;
          this->brain.current_destination.StartTimer(10000, brain.world);
        }
        else
        {
          GatherNavigate(brain, newXY);
        }
        break;
      default:
        std::cout << "Move neuron was given invalid task" << std::endl;
        break;
    }

    if (newXY != World::PosXY{0, 0} && newXY != brain.current_position)
    {
      World::BlockTypes block= World::GetBlockAtPos(newXY, brain.world);
      if (block == World::BlockTypes::kBlockUnderground)
      {
        // std::cout << "Path info: " << brain.current_position.x << ", " <<
        // brain.current_position.y << " New: " << newXY.x << ", " << newXY.y <<
        // " Block: " << brain.world->GetBlockAtPos(newXY) << std::endl;
        MoveOneTowards(brain.current_position, newXY);
      }
      else
      {
        std::cout << "Block: " << block << std::endl;
        if (!brain.path_to_target.empty())
        {
          brain.path_to_target.pop_back();
        }
        // Fixme: This is a panic fix to move to an open block 1 block away if
        // it exists.
        //        This should be handled by better pathfinding instead.
        // brain.current_position =
        // brain.world->FindNearestBlockOfType(brain.current_position,
        // World::kBlockUnderground, 3);
      }
    }
    else if (newXY == brain.current_position)
    {
      std::cout << "Ant is trying to move to the position it's already in"
                << std::endl;
    }
    else
    {
      std::cout << "THIS SHOULD NEVER HAPPEN" << std::endl;
    }
  }
}

void move_neuron::MoveOneTowards(World::PosXY &origin,
                                 World::PosXY &destination)
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
    World::PosXY next{};
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

void detect_food_neuron::tick(float threshold)
{
  // FIXME: Initial fudge of threshold for manual task progression
  if (this->brain.current_task == Brain::kTaskSearchingFood)
  {
    this->threshold= 0.0f;
  }
  else
  {
    this->threshold= 0.99f;
  }

  // Arbitrary adjustment of strength float (expected between 0-1) to give a
  // reasonable search distance
  // FIXME: This should be attached to a gene
  const float distance= threshold * 500;

  if ((this->brain.current_task == Brain::kTaskSearchingFood) &&
      (this->brain.sensed_food.Expired()) && (threshold >= this->threshold) &&
      (this->brain.carrying == World::kBlockAir))
  {
    this->brain.sensed_food.position= this->brain.world->FindNearestBlockOfType(
        this->brain.current_position, World::BlockTypes::kBlockFood,
        static_cast<uint64_t>(distance));
    if (brain.sensed_food.position != World::PosXY{0, 0})
    {
      // FIXME: Probably don't want arbitrary expiry times here
      this->brain.sensed_food.StartTimer(10000, this->brain.world);
//      std::cout << "Food found: " << this->brain.sensed_food.position.x << ", "
//                << this->brain.sensed_food.position.y << std::endl;
      this->brain.current_destination.position=
          this->brain.world->FindNearestBlockOfType(
              this->brain.sensed_food.position,
              World::BlockTypes::kBlockUnderground, static_cast<uint64_t>(20));
      if (this->brain.world->OneBlockAway(
              this->brain.current_destination.position,
              this->brain.sensed_food.position))
      {
        this->brain.current_destination.StartTimer(10000, this->brain.world);
      }
      else
      {
        this->brain.current_destination.SetExpired();
      }
      std::cout << "Moving towards: "
                << this->brain.current_destination.position.x << ", "
                << this->brain.current_destination.position.y << std::endl;
    }
  }  // else {
     // Didn't detect food, wander for a bit
     // this->brain.current_task = Brain::kTaskWandering;
  //}
  // this->food_location = world->FindNearestBlockOfType(origin,
  // World::BlockTypes::kBlockFood, distance);
}

move_neuron::move_neuron(Brain &brain) : Neuron(brain)
{
  // this->brain = brain;
  this->threshold= 0.9f;
}

detect_food_neuron::detect_food_neuron(Brain &brain) : Neuron(brain)
{
  // this->brain = brain;
  this->threshold= 0.7f;
  this->strength= 0.5f;
}

Neuron::Neuron(Brain &brain) : brain(brain) {}

void Neuron::tick(float threshold) {}

void Neuron::SendSignal(Neuron *neuron, float weight)
{
  neuron->ReceiveSignal(weight);
}

void Neuron::ReceiveSignal(float weight) {}

void move_neuron::ReceiveSignal(float weight) {}

timer_neuron::timer_neuron(Brain &brain) : Neuron(brain)
{
  // this->brain = brain;
  this->last_measurement= std::chrono::steady_clock::now();
}

void timer_neuron::tick(float threshold)
{
  const auto now= std::chrono::steady_clock::now();
  const auto duration= std::chrono::duration_cast<std::chrono::milliseconds>(
      now - this->last_measurement);
  float strength=
      std::min(static_cast<float>((float)duration.count() / this->delay), 1.0f);
  for (Neuron *neuron : this->outputs)
  {
    neuron->tick(strength);
  }
  if (duration.count() >= this->delay)
  {
    this->last_measurement= std::chrono::steady_clock::now();
  }
}

void Neuron::ConnectNeuron(Neuron &neuron) { this->outputs.push_back(&neuron); }

connector_neuron::connector_neuron(Brain &brain) : Neuron(brain)
{
  this->threshold= 0.5f;
}

void connector_neuron::tick(float threshold)
{
  if (threshold > this->threshold)
  {
    for (Neuron *neuron : this->outputs)
    {
      neuron->tick(threshold);
    }
  }
}

task_neuron::task_neuron(Brain &brain) : Neuron(brain)
{
  this->threshold= 0.0f;
}

void task_neuron::tick(float threshold)
{
  if (threshold > this->threshold)
  {
    switch (this->brain.current_task)
    {
      case Brain::kTaskWandering:
        this->brain.current_task= Brain::kTaskSearchingFood;
        break;
      case Brain::kTaskSearchingFood:
        // std::cout << "Searching, sensed food: " <<
        // std::to_string(this->brain.sensed_food.Expired()) << std::endl;
        if (this->brain.carrying == World::kBlockFood)
        {
          this->brain.current_task= Brain::kTaskDeliveringFood;
        }
        if (!this->brain.sensed_food.Expired())
        {
          this->brain.current_task= Brain::kTaskGatheringFood;
        }
        else
        {
          // FIXME: Probably want a way to fall back to wandering but the below
          // doesn't work as expected with
          //        the current tests above
          // this->brain.current_task = Brain::kTaskWandering;
        }
        break;
      case Brain::kTaskGatheringFood:
        // std::cout << "Gathering" << std::endl;
        if (this->brain.carrying == World::kBlockFood)
        {
          this->brain.current_task= Brain::kTaskDeliveringFood;
          break;
        }
        if (this->brain.current_destination.position == World::PosXY{0, 0})
        {
          this->brain.current_task= Brain::kTaskSearchingFood;
        }
        if ((this->brain.sensed_food.block != World::kBlockFood) ||
            (this->brain.sensed_food.Expired()))
        {
          this->brain.current_task= Brain::kTaskSearchingFood;
        }

        break;
      case Brain::kTaskDeliveringFood:
        // std::cout << "Delivering" << std::endl;
        if (this->brain.carrying == World::kBlockAir)
        {
          this->brain.current_task= Brain::kTaskWandering;
        }
        break;
      default:
        std::cout << "No task match!!!" << std::endl;
        break;
    }
  }
  // std::cout << "Current task: " << this->brain.current_task << std::endl;
}

detect_adjacent_neuron::detect_adjacent_neuron(Brain &brain) : Neuron(brain)
{
  this->threshold= 0.0f;
}

void detect_adjacent_neuron::tick(float threshold)
{
  if (threshold > this->threshold)
  {
    int dx[8]= {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8]= {-1, 0, 1, -1, 1, -1, 0, 1};

    std::cout << "Adjacent blocks: ";
    for (int i= 0; i < 8; i++)
    {
      int x= this->brain.current_position.x + dx[i];
      int y= this->brain.current_position.y + dy[i];
      this->brain.adjacent_blocks[i].block=
          World::GetBlockAtPos(World::PosXY{x, y}, this->brain.world);
      std::cout << this->brain.adjacent_blocks[i].block << " ";
      this->brain.adjacent_blocks[i].position= World::PosXY{x, y};
    }
    std::cout << std::endl;
  }
}

gather_neuron::gather_neuron(Brain &brain) : Neuron(brain)
{
  this->threshold= 0.0f;
}

void gather_neuron::tick(float threshold)
{
  // FIXME: Initial fudge of threshold for manual task progression
  if (this->brain.current_task == Brain::kTaskGatheringFood)
  {
    this->threshold= 0.0f;
  }
  else
  {
    this->threshold= 0.99f;
  }

  if ((threshold > this->threshold) &&
      (this->brain.carrying == World::kBlockAir))
  {
    if (this->brain.world->OneBlockAway(this->brain.current_position,
                                        this->brain.sensed_food.position))
    {
      if (this->brain.world->PickupBlockAtPos(this->brain.current_position,
                                              this->brain.sensed_food.position,
                                              this->brain.carrying))
      {
        this->brain.current_task= Brain::kTaskDeliveringFood;
        this->brain.path_to_target.clear();
        this->brain.current_destination.SetExpired();
        this->brain.sensed_food.SetExpired();
        std::cout << "Picked up food: "
                  << this->brain.current_destination.position.x << ", "
                  << this->brain.current_destination.position.y << std::endl;
      }
    }
  }
  if ((threshold > this->threshold) &&
      (this->brain.carrying == World::kBlockFood))
  {
    if (this->brain.world->PlaceBlockAtPos(this->brain.current_position,
                                           this->brain.current_destination.position,
                                           this->brain.carrying))
    {
      this->brain.current_task= Brain::kTaskWandering;
      this->brain.current_destination.SetExpired();
      this->brain.path_to_target.clear();
      std::cout << "Dropped off food" << std::endl;
    }
  }
}