#include "Neuron.h"

void move_neuron::tick(float tick_threshold, int64_t* x, int64_t* y, World* world, World::PosXY desired) {
    int64_t newX{};
    int64_t newY{};
    if (tick_threshold > this->threshold) {
        if ((desired.x != 0) && (desired.y != 0)) {
            if (*x - desired.x > 0) {
                newX = *x - 1;
            } else {
                newX = *x + 1;
            }
            if (*y - desired.y > 0) {
                newY = *y - 1;
            } else {
                newY = *y + 1;
            }
        } else {
            newX = *x + ((std::rand() % 3) -1);
            newY = *y + ((std::rand() % 3) -1);
        }

//        int64_t newX = *x + 1;
//        int64_t newY = *y;
        World::BlockTypes block = world->GetBlockAtPos(newX, newY);
        if (block == World::BlockTypes::kBlockUnderground) {
            *x = newX;
            *y = newY;
        }
    }
    //this->updateLocation(newX, newY);
}


void detect_food_neuron::tick(World::PosXY origin, float strength, World* world) {
    // Arbitrary adjustment of strength float (expected between 0-1) to give a reasonable search distance
    const uint64_t distance = strength * 100;
    //this->food_location = world->FindNearestBlockOfType(origin, World::BlockTypes::kBlockFood, distance);
}

move_neuron::move_neuron() {
    this->threshold = 0.7f;
}

detect_food_neuron::detect_food_neuron() {
    this->threshold = 0.7f;
    this->strength = 0.5f;
}

void Neuron::SendSignal(Neuron& neuron, float weight) {
    neuron.ReceiveSignal(weight);
}

void Neuron::ReceiveSignal(float weight) {

}

void move_neuron::ReceiveSignal(float weight) {

}