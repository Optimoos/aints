//
// Created by Mark on 2022-07-18.
//

#ifndef AIANTS_AINTS_H
#define AIANTS_AINTS_H

#include <cstdint>
#include <iostream>
#include <vector>

#include "brain.h"
#include "neuron.h"
#include "moveneuron.h"
#include "raylib.h"
#include "world.h"

class aints
{
 public:
  int setId(int newId);
  int getId() const;
  bool updateLocation(int64_t x, int64_t y);
  int64_t getX() const;
  int64_t getY() const;
  PosXY GetPosition() const;

  aints(std::shared_ptr<World> world);
  ~aints();

  void tick();

  std::shared_ptr<World> world;
  Brain brain{world};

  TimerNeuron tn{brain};
  MoveNeuron mn{brain};
  DetectFoodNeuron fn{brain};
  ConnectorNeuron cn{brain};
  TaskNeuron taskn{brain};
  GatherNeuron gn{brain};
  DetectAdjacentNeuron dan{brain};

 private:
  int id= 0;

  // MoveNeuron* mn = new MoveNeuron;
};

#endif  // AIANTS_AINTS_H
