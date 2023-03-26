//
// Created by Mark on 2023-03-22.
//

#ifndef AIANTS_BRAIN_H
#define AIANTS_BRAIN_H

#include <chrono>

#include "world.h"

class Brain
{
 public:
  class LocationInformation
  {
   public:
    World::PosXY position{0, 0};
    World::BlockTypes block{World::kBlockAir};

    void StartTimer(int64_t time_until_expiry)
    {
      this->expired= false;
      this->block= World::GetBlockAtPos(position);
      this->expiry= time_until_expiry;
      this->expiry_clock= std::chrono::steady_clock::now();
    }

    bool Expired()
    {
      auto now= std::chrono::steady_clock::now();
      auto duration= std::chrono::duration_cast<std::chrono::milliseconds>(
          now - this->expiry_clock);
      if (duration.count() >= this->expiry)
      {
        this->expired= true;
        std::cout << "Timer expired on block: " << this->block
                  << " at position: " << this->position.x << ", "
                  << this->position.y << std::endl;
      }
      return this->expired;
    };

    void SetExpired() { this->expired= true; }

   private:
    bool expired{true};
    int64_t expiry{5000};
    std::chrono::time_point<std::chrono::steady_clock> expiry_clock{
        std::chrono::steady_clock::now()};
  };

  enum TaskType
  {
    kTaskWandering,
    kTaskSearchingFood,
    kTaskGatheringFood,
    kTaskDeliveringFood

  };

  bool invalid_state{false};
  LocationInformation adjacent_blocks[7];
  TaskType current_task{kTaskWandering};
  World::BlockTypes carrying{World::kBlockAir};
  std::vector<World::PosXY> path_to_target{};
  World::PosXY current_position{0, 0};
  World::PosXY food_stockpile{4000, 300};
  World::PosXY dropoff_position{0, 0};
  LocationInformation sensed_food;
  LocationInformation current_destination;
  World *world;
};

#endif  // AIANTS_BRAIN_H
