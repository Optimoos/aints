//
// Created by Mark on 2022-07-18.
//

#ifndef AIANTS_AINTS_H
#define AIANTS_AINTS_H

#include <cstdint>
#include <vector>
#include "raylib.h"
#include "world.h"


class neuron
{
public:
private:
protected:
    float threshold = 0.0f;
};

class move_neuron: public neuron
{
public:
    move_neuron();
    void tick(float threshold, int64_t* x, int64_t* y, World* world);
private:
};

class aints
{
public:
    int setId(int newId);
    int getId();
    bool updateLocation(int64_t x, int64_t y);
    int64_t getX();
    int64_t getY();
    World* world;
    //std::vector<*neuron> neurons;

    aints(World* world);
    ~aints();


    void tick();

private:
    int id = 0;
    int64_t locX = 0;
    int64_t locY = 0;

    //move_neuron* mn = new move_neuron;
    move_neuron mn;
};

#endif //AIANTS_AINTS_H
