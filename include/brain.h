//
// Created by Mark on 2023-03-22.
//

#ifndef AIANTS_BRAIN_H
#define AIANTS_BRAIN_H

#include <chrono>

#include "aints_types.h"
#include "world.h"

class Brain
{
 public:
  class LocationInformation
  {
   public:
    PosXY position{0, 0};
    BlockTypes block{kBlockAir};

    void StartTimer(int64_t time_until_expiry, std::shared_ptr<World> world)
    {
      this->expired= false;
      this->block= World::GetBlockAtPos(position, world);
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

  Brain(std::shared_ptr<World> world);
  ~Brain()= default;

  bool invalid_state{false};
  LocationInformation adjacent_blocks[7];
  TaskType current_task{kTaskWandering};
  BlockTypes carrying{kBlockAir};
  std::vector<PosXY> path_to_target{};
  PosXY current_position{0, 0};
  PosXY food_stockpile{4000, 300};
  PosXY dropoff_position{0, 0};
  LocationInformation sensed_food;
  LocationInformation current_destination;
  std::shared_ptr<World> world;
};

#endif  // AIANTS_BRAIN_H
