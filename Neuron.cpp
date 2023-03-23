#include "Neuron.h"

void move_neuron::tick(float threshold) {
    int64_t newX{};
    int64_t newY{};
    if (threshold > this->threshold) {
//        if ((desired.x != 0) && (desired.y != 0)) {
//            if (*x - desired.x > 0) {
//                newX = *x - 1;
//            } else {
//                newX = *x + 1;
//            }
//            if (*y - desired.y > 0) {
//                newY = *y - 1;
//            } else {
//                newY = *y + 1;
//            }
//        } else {

            newX = brain.current_position.x + ((std::rand() % 3) -1);
            newY = brain.current_position.y + ((std::rand() % 3) -1);
//        }

//        int64_t newX = *x + 1;
//        int64_t newY = *y;
        World::BlockTypes block = brain.world->GetBlockAtPos(newX, newY);
        if (block == World::BlockTypes::kBlockUnderground) {
            brain.current_position.x = newX;
            brain.current_position.y = newY;
        }
    }
    //this->updateLocation(newX, newY);
}


//void detect_food_neuron::tick(World::PosXY origin, float strength, World* world) {
void detect_food_neuron::tick(float threshold) {

    // Arbitrary adjustment of strength float (expected between 0-1) to give a reasonable search distance
    const uint64_t distance = strength * 100;
    //this->food_location = world->FindNearestBlockOfType(origin, World::BlockTypes::kBlockFood, distance);
}

move_neuron::move_neuron(Brain& brain) : Neuron(brain) {
    //this->brain = brain;
    this->threshold = 0.7f;
}

detect_food_neuron::detect_food_neuron(Brain& brain) : Neuron(brain) {
    //this->brain = brain;
    this->threshold = 0.7f;
    this->strength = 0.5f;
}

Neuron::Neuron(Brain& brain) : brain(brain) {

}

void Neuron::tick(float threshold){

}

void Neuron::SendSignal(Neuron* neuron, float weight) {
    neuron->ReceiveSignal(weight);
}

void Neuron::ReceiveSignal(float weight) {

}

void move_neuron::ReceiveSignal(float weight) {

}

timer_neuron::timer_neuron(Brain& brain) : Neuron(brain) {
    //this->brain = brain;
    this->last_measurement = std::chrono::steady_clock::now();
}

void timer_neuron::tick(float threshold) {
    this->delay_delta += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - this->last_measurement).count();
    float strength = std::min(static_cast<float>((float)this->delay_delta / this->delay), 1.0f);
    for (Neuron* neuron : this->outputs) {
        neuron->tick(strength);
    }
    if (this->delay_delta > this->delay) {
        this->delay_delta = 0;
    }
    this->last_measurement = std::chrono::steady_clock::now();
}

void Neuron::ConnectNeuron(Neuron& neuron) {
    this->outputs.push_back(&neuron);
}