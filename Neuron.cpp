#include "Neuron.h"

void move_neuron::tick(float threshold) {
    int64_t newX{};
    int64_t newY{};
    if (threshold > this->threshold) {
        if (brain.current_task == Brain::kTaskWandering) {
            newX = brain.current_position.x + ((std::rand() % 3) -1);
            newY = brain.current_position.y + ((std::rand() % 3) -1);
        } else {
            if ((brain.current_destination.x != 0) && (brain.current_destination.y != 0)) {
                if (brain.current_position.x - brain.current_destination.x > 0) {
                    newX = brain.current_position.x - 1;
                } else {
                    newX = brain.current_position.x + 1;
                }
                if (brain.current_position.y - brain.current_destination.y > 0) {
                    newY = brain.current_position.y - 1;
                } else {
                    newY = brain.current_position.y + 1;
                }
            }
        }
        World::BlockTypes block = brain.world->GetBlockAtPos(newX, newY);
        if (block == World::BlockTypes::kBlockUnderground) {
            brain.current_position.x = newX;
            brain.current_position.y = newY;
        }
    }
}


//void detect_food_neuron::tick(World::PosXY origin, float strength, World* world) {
void detect_food_neuron::tick(float threshold) {

    // Arbitrary adjustment of strength float (expected between 0-1) to give a reasonable search distance
    const uint64_t distance = strength * 100;
    //this->food_location = world->FindNearestBlockOfType(origin, World::BlockTypes::kBlockFood, distance);
}

move_neuron::move_neuron(Brain& brain) : Neuron(brain) {
    //this->brain = brain;
    this->threshold = 0.9f;
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

connector_neuron::connector_neuron(Brain& brain) : Neuron(brain) {
    this->threshold = 0.5f;
}

void connector_neuron::tick(float threshold) {
    if (threshold > this->threshold) {
        for (Neuron* neuron : this->outputs) {
            neuron->tick(threshold);
        }
    }
}

task_neuron::task_neuron(Brain& brain) : Neuron(brain) {
    this->threshold = 0.99f;
}

void task_neuron::tick(float threshold) {
    if (threshold > this->threshold) {
        switch (this->brain.current_task) {
            case Brain::kTaskWandering:
                //this->brain.current_task =
                break;
            case Brain::kTaskSearchingFood:
                break;
            case Brain::kTaskGatheringFood:
                break;
            case Brain::kTaskDeliveringFood:
                break;
            default:
                break;
        }
    }
}