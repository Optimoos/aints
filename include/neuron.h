#ifndef AIANTS_NEURON_H
#define AIANTS_NEURON_H

#include <chrono>
#include <ctime>
#include <vector>

#include "brain.h"
#include "world.h"

class Neuron
{
 public:
  Neuron(Brain &brain);

  void ConnectNeuron(Neuron &neuron);

  virtual void Tick(float threshold= 1.0f);

 private:
 protected:
  float threshold{0.0f};
  float strength{0.0f};
  std::vector<Neuron *> outputs{};
  Brain &brain;

  void SendSignal(Neuron *neuron, float weight);

  virtual void ReceiveSignal(float weight);
};

class TimerNeuron : public Neuron
{
 public:
  TimerNeuron(Brain &brain);

  void Tick(float threshold= 1.0f) override;

 private:
  uint16_t delay{1000};
  std::chrono::time_point<std::chrono::steady_clock> last_measurement;
};



class DetectFoodNeuron : public Neuron
{
 public:
  PosXY food_location{0, 0};

  DetectFoodNeuron(Brain &brain);

  // void tick(PosXY origin, float distance, World* world);
  void Tick(float threshold) override;

 private:
};

class ConnectorNeuron : public Neuron
{
 public:
  ConnectorNeuron(Brain &brain);

  void Tick(float threshold) override;
};

class TaskNeuron : public Neuron
{
 public:
  TaskNeuron(Brain &brain);

  void Tick(float threshold) override;
};

class DetectAdjacentNeuron : public Neuron
{
 public:
  DetectAdjacentNeuron(Brain &brain);

  void Tick(float threshold) override;
};

class GatherNeuron : public Neuron
{
 public:
  GatherNeuron(Brain &brain);

  void Tick(float threshold) override;
};

#endif