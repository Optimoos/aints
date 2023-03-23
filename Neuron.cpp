#include "Neuron.h"

void move_neuron::tick(float threshold) {
    //FIXME: Update newX and newY to a PosXY object.
    int64_t newX{};
    int64_t newY{};
    if (threshold > this->threshold) {
        if (brain.current_task == Brain::kTaskWandering) {
            newX = brain.current_position.x + ((std::rand() % 3) -1);
            newY = brain.current_position.y + ((std::rand() % 3) -1);
        } else {
            if ((brain.current_destination.position.x != 0) && (brain.current_destination.position.y != 0)) {
                if (brain.current_destination.Expired()) {
                    brain.current_task = Brain::kTaskWandering;
                } else if (brain.current_task == Brain::kTaskGatheringFood) {
                    if (brain.current_position.x - brain.current_destination.position.x > 0) {
                        newX = brain.current_position.x - 1;
                    } else {
                        newX = brain.current_position.x + 1;
                    }
                    if (brain.current_position.y - brain.current_destination.position.y > 0) {
                        newY = brain.current_position.y - 1;
                    } else {
                        newY = brain.current_position.y + 1;
                    }
                } else if ((brain.current_task == Brain::kTaskDeliveringFood) && (brain.carrying != World::kBlockAir)) {
                    if (brain.current_position.x - brain.current_destination.position.x > 0) {
                        newX = brain.current_position.x - 1;
                    } else {
                        newX = brain.current_position.x + 1;
                    }
                    if (brain.current_position.y - brain.current_destination.position.y > 0) {
                        newY = brain.current_position.y - 1;
                    } else {
                        newY = brain.current_position.y + 1;
                    }
                }
            }
        }
        World::BlockTypes block = brain.world->GetBlockAtPos(World::PosXY{newX, newY});
        if (block == World::BlockTypes::kBlockUnderground) {
            brain.current_position.x = newX;
            brain.current_position.y = newY;
        }
        if (brain.world->OneBlockAway(brain.current_position, brain.current_destination.position)) {
            brain.current_task = Brain::kTaskDeliveringFood;
        }
    }
}


//void detect_food_neuron::tick(World::PosXY origin, float strength, World* world) {
void detect_food_neuron::tick(float threshold) {

    // Arbitrary adjustment of strength float (expected between 0-1) to give a reasonable search distance
    const uint64_t distance = threshold * 100;

    // FIXME: Initial fudge of threshold for manual task progression
    if (this->brain.current_task == Brain::kTaskSearchingFood) {
        this->threshold = 0.0f;
    } else {
        this->threshold = 0.99f;
    }

    if ((this->brain.current_task == Brain::kTaskSearchingFood) && (this->brain.current_destination.Expired()) && (threshold >= this->threshold)) {
        this->brain.current_destination.position = this->brain.world->FindNearestBlockOfType(this->brain.current_position, World::BlockTypes::kBlockFood, distance);
        std::cout << "Food found: " << this->brain.current_destination.position.x << ", " << this->brain.current_destination.position.y << std::endl;
        // FIXME: Probably don't want arbitrary expiry times here
        this->brain.current_destination.StartTimer(10000);
    }
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
                this->brain.current_task = Brain::kTaskSearchingFood;
                break;
            case Brain::kTaskSearchingFood:
                if ((this->brain.current_task == Brain::kTaskSearchingFood) && (!this->brain.current_destination.Expired())) {
                    this->brain.current_task = Brain::kTaskGatheringFood;
                }
                break;
            case Brain::kTaskGatheringFood:
                if (this->brain.carrying == World::kBlockFood) {
                    this->brain.current_task = Brain::kTaskDeliveringFood;
                }
                break;
            case Brain::kTaskDeliveringFood:
                if (this->brain.carrying == World::kBlockAir) {
                    this->brain.current_task = Brain::kTaskWandering;
                }
                break;
            default:
                break;
        }
    }
    //std::cout << "Current task: " << this->brain.current_task << std::endl;
}

detect_adjacent_neuron::detect_adjacent_neuron(Brain& brain) : Neuron(brain) {
    this->threshold = 0.0f;
}

void detect_adjacent_neuron::tick(float threshold) {
    if (threshold > this->threshold) {

    }
}

gather_neuron::gather_neuron(Brain& brain) : Neuron(brain) {
    this->threshold = 0.0f;
}

void gather_neuron::tick(float threshold) {
    if ((threshold > this->threshold) && (this->brain.carrying == World::kBlockAir)) {
        if (this->brain.world->OneBlockAway(this->brain.current_position, this->brain.current_destination.position)) {
            this->brain.carrying = this->brain.world->GetBlockAtPos(this->brain.current_destination.position);
            this->brain.world->SetBlockAtPos(this->brain.current_destination.position, World::kBlockAir);
            World::Tile* tile = this->brain.world->PosToTile(this->brain.current_destination.position.x, this->brain.current_destination.position.y);
            tile->RegenerateTexture();
            std::cout << "Picked up food" << std::endl;
        }
    }
}