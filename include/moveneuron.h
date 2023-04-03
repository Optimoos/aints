#ifndef AINTS_MOVENEURON_H
#define AINTS_MOVENEURON_H

#include <future>

#include "neuron.h"
#include "mapsearchnode.h"

class MoveNeuron : public Neuron
{
 public:
  MoveNeuron(Brain &brain);

  // void tick(float threshold, Brain& brain);
  void Tick(float threshold) override;

  void ReceiveSignal(float weight) override;

  void MoveOneTowards(PosXY &origin, PosXY &destination);

  void RandomMovement(PosXY &original_location, PosXY &new_location);

  void GatherNavigate(Brain &brain, PosXY &next_coord);

  void DeliverNavigate(Brain &brain, PosXY &next_coord);

  void ValidateLastMove();



  void SetFoodStockpileAsDestination();
  void DoMovement(PosXY &location);

 private:
};

#endif  // AIANTS_MOVENEURON_H