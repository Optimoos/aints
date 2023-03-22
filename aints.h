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


class aints
{
public:
    int setId(int newId);
    int getId();
    bool updateLocation(int64_t x, int64_t y);
    int64_t getX();
    int64_t getY();
    //std::vector<*neuron> neurons;

    World::PosXY next_position{0,0};

    aints(World& world);
    ~aints();


    void tick();

private:
    World* world;
    int id = 0;
    int64_t locX = 0;
    int64_t locY = 0;

    World::PosXY desired_destination{0,0};

    //move_neuron* mn = new move_neuron;
    move_neuron mn;
    detect_food_neuron fn;
};

#endif //AIANTS_AINTS_H
