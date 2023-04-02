#include "neuron.h"

void DetectFoodNeuron::Tick(float threshold)
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
      (this->brain.carrying == kBlockAir))
  {
    this->brain.sensed_food.position= this->brain.world->FindNearestBlockOfType(
        this->brain.current_position, BlockTypes::kBlockFood,
        static_cast<uint64_t>(distance));
    if (brain.sensed_food.position.valid)
    {
      // FIXME: Probably don't want arbitrary expiry times here
      this->brain.sensed_food.StartTimer(10000, this->brain.world);
      this->brain.current_destination.position=
          this->brain.world->FindNearestBlockOfType(
              this->brain.sensed_food.position, BlockTypes::kBlockUnderground,
              static_cast<uint64_t>(20));
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
    }
  }  // else {
     // Didn't detect food, wander for a bit
     // this->brain.current_task = Brain::kTaskWandering;
  //}
  // this->food_location = world->FindNearestBlockOfType(origin,
  // BlockTypes::kBlockFood, distance);
}


DetectFoodNeuron::DetectFoodNeuron(Brain &brain) : Neuron(brain)
{
  // this->brain = brain;
  this->threshold= 0.7f;
  this->strength= 0.5f;
}

Neuron::Neuron(Brain &brain) : brain(brain) {}

void Neuron::Tick(float threshold) {}

void Neuron::SendSignal(Neuron *neuron, float weight)
{
  neuron->ReceiveSignal(weight);
}

void Neuron::ReceiveSignal(float weight) {}

TimerNeuron::TimerNeuron(Brain &brain) : Neuron(brain)
{
  // this->brain = brain;
  this->last_measurement= std::chrono::steady_clock::now();
}

void TimerNeuron::Tick(float threshold)
{
  const auto now= std::chrono::steady_clock::now();
  const auto duration= std::chrono::duration_cast<std::chrono::milliseconds>(
      now - this->last_measurement);
  float strength=
      std::min(static_cast<float>((float)duration.count() / this->delay), 1.0f);
  for (Neuron *neuron : this->outputs)
  {
    neuron->Tick(strength);
  }
  if (duration.count() >= this->delay)
  {
    this->last_measurement= std::chrono::steady_clock::now();
  }
}

void Neuron::ConnectNeuron(Neuron &neuron) { this->outputs.push_back(&neuron); }

ConnectorNeuron::ConnectorNeuron(Brain &brain) : Neuron(brain)
{
  this->threshold= 0.5f;
}

void ConnectorNeuron::Tick(float threshold)
{
  if (threshold > this->threshold)
  {
    for (Neuron *neuron : this->outputs)
    {
      neuron->Tick(threshold);
    }
  }
}

TaskNeuron::TaskNeuron(Brain &brain) : Neuron(brain)
{
  this->threshold= 0.0f;
}

void TaskNeuron::Tick(float threshold)
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
        if (this->brain.carrying == kBlockFood)
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
        if (this->brain.carrying == kBlockFood)
        {
          this->brain.current_task= Brain::kTaskDeliveringFood;
          break;
        }
        if ((this->brain.sensed_food.block != kBlockFood) ||
            (this->brain.sensed_food.Expired()))
        {
          this->brain.current_task= Brain::kTaskSearchingFood;
        }
        if (this->brain.current_destination.position == PosXY{0, 0} || this->brain.current_destination.Expired())
        {
          this->brain.current_task= Brain::kTaskSearchingFood;
          break;
        }
        break;
      case Brain::kTaskDeliveringFood:
        // std::cout << "Delivering" << std::endl;
        if (this->brain.carrying == kBlockAir)
        {
          this->brain.current_task= Brain::kTaskWandering;
        }

        // FIXME: This case should be a rare occurrence as the delivery task should be
        // changing the state when food is dropped off. Logging for now to see
        // how regularly this occurs.
        if (brain.current_destination.Expired() &&
            (brain.carrying == kBlockAir))
        {
          std::cout << "MoveNeuron::Tick() - Delivery task with no destination" << std::endl;
          brain.current_task= Brain::kTaskWandering;
          break;
        }

        break;
      default:
        std::cout << "No task match!!!" << std::endl;
        break;
    }
  }
  // std::cout << "Current task: " << this->brain.current_task << std::endl;
}

DetectAdjacentNeuron::DetectAdjacentNeuron(Brain &brain) : Neuron(brain)
{
  this->threshold= 0.0f;
}

void DetectAdjacentNeuron::Tick(float threshold)
{
  if (threshold > this->threshold)
  {
    int dx[8]= {-1, 0, 1, 1, 1, 0, -1, -1};
    int dy[8]= {-1, -1, -1, 0, 1, 1, 1, 0};

    std::cout << "Adjacent blocks: ";
    for (int i= 0; i < 8; i++)
    {
      int x= this->brain.current_position.x + dx[i];
      int y= this->brain.current_position.y + dy[i];
      auto pos= PosXY{x, y};
      this->brain.adjacent_blocks[i].block=
          World::GetBlockAtPos(pos, this->brain.world);
      std::cout << this->brain.adjacent_blocks[i].block << " ";
      this->brain.adjacent_blocks[i].position= PosXY{x, y};
    }
    std::cout << std::endl;
  }
}

GatherNeuron::GatherNeuron(Brain &brain) : Neuron(brain)
{
  this->threshold= 0.0f;
}

void GatherNeuron::Tick(float threshold)
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

  if ((threshold > this->threshold) && (this->brain.carrying == kBlockFood) && (this->brain.current_task == Brain::kTaskDeliveringFood))
  {
    if (this->brain.world->PlaceBlockAtPos(
            this->brain.current_position,
            this->brain.dropoff_position.position, this->brain.carrying))
    {
      this->brain.current_task= Brain::kTaskWandering;
      this->brain.current_destination.SetExpired();
      this->brain.path_to_target.clear();
    } else {
      this->brain.dropoff_position.SetExpired();
      //this->brain.current_destination.SetExpired();
    }
  }

  if ((threshold > this->threshold) && (this->brain.carrying == kBlockAir) && (this->brain.current_task == Brain::kTaskGatheringFood))
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
      } else {
        this->brain.sensed_food.SetExpired();
      }
    }
  }

}