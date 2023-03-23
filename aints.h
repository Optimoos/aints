//
// Created by Mark on 2022-07-18.
//

#ifndef AIANTS_AINTS_H
#define AIANTS_AINTS_H

#include <cstdint>
#include <vector>
#include <iostream>
#include "raylib.h"
#include "world.h"
#include "Neuron.h"
#include "Brain.h"

class aints
{
public:
    int setId(int newId);
    int getId();
    bool updateLocation(int64_t x, int64_t y);
    int64_t getX();
    int64_t getY();
    //std::vector<*neuron> neurons;

    aints(World& world);
    ~aints();


    void tick();
    World* world;

private:
    Brain brain;

    int id = 0;

    //move_neuron* mn = new move_neuron;
    move_neuron mn;
    detect_food_neuron fn;
    timer_neuron tn;
};

#endif //AIANTS_AINTS_H
