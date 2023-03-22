#ifndef AIANTS_NEURON_H
#define AIANTS_NEURON_H

#include "world.h"
#include <vector>

class Neuron
{
public:
private:
protected:
    float threshold{0.0f};
    float strength{0.0f};
    std::vector<Neuron> outputs;

    void SendSignal(Neuron& neuron, float weight);
    virtual void ReceiveSignal(float weight);
};

class timer_neuron: public Neuron
{
public:
    timer_neuron();
    void tick();
private:
    uint16_t delay{1000};
};

class move_neuron: public Neuron
{
public:
    move_neuron();
    void tick(float threshold, int64_t* x, int64_t* y, World* world, World::PosXY desired);
    void ReceiveSignal(float weight) override;
private:
};

class detect_food_neuron: public Neuron
{
public:
    World::PosXY food_location{0,0};
    detect_food_neuron();
    void tick(World::PosXY origin, float distance, World* world);
private:

};

#endif