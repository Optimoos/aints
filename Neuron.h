#ifndef AIANTS_NEURON_H
#define AIANTS_NEURON_H

#include <vector>
#include <chrono>
#include "world.h"
#include "Brain.h"

class Neuron
{
public:
    Neuron(Brain& brain);
    void ConnectNeuron(Neuron& neuron);
    virtual void tick(float threshold = 1.0f);
private:
protected:
    float threshold{0.0f};
    float strength{0.0f};
    std::vector<Neuron*> outputs{};
    Brain& brain;


    void SendSignal(Neuron* neuron, float weight);
    virtual void ReceiveSignal(float weight);
};

class timer_neuron: public Neuron
{
public:
    timer_neuron(Brain& brain);
    void tick(float threshold = 1.0f) override;
private:
    uint16_t delay{1000};
    uint16_t delay_delta{0};
    std::chrono::time_point<std::chrono::steady_clock> last_measurement;
};

class move_neuron: public Neuron
{
public:
    move_neuron(Brain& brain);
    //void tick(float threshold, Brain& brain);
    void tick(float threshold) override;
    void ReceiveSignal(float weight) override;
private:
};

class detect_food_neuron: public Neuron
{
public:
    World::PosXY food_location{0,0};
    detect_food_neuron(Brain& brain);
    //void tick(World::PosXY origin, float distance, World* world);
    void tick(float threshold) override;
private:

};

class connector_neuron: public Neuron
{
public:
    connector_neuron(Brain& brain);
    void tick(float threshold) override;
};

#endif