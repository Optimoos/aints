//
// Created by Mark on 2022-07-18.
//

#ifndef AIANTS_AINTS_H
#define AIANTS_AINTS_H

#include <cstdint>
#include <iostream>
#include <vector>

#include "Brain.h"
#include "Neuron.h"
#include "raylib.h"
#include "world.h"

class aints
{
 public:
  int setId(int newId);

  int getId();

  bool updateLocation(int64_t x, int64_t y);

  int64_t getX();

  int64_t getY();
  // std::vector<*neuron> neurons;

  aints(World &world);

  ~aints();

  void tick();

  World *world;
  Brain brain;

  timer_neuron tn{brain};
  move_neuron mn{brain};
  detect_food_neuron fn{brain};
  connector_neuron cn{brain};
  task_neuron taskn{brain};
  gather_neuron gn{brain};
  detect_adjacent_neuron dan{brain};

 private:
  int id= 0;

  // move_neuron* mn = new move_neuron;
};

#endif  // AIANTS_AINTS_H
