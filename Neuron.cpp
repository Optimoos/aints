#include "Neuron.h"

void move_neuron::tick(float threshold) {
    //FIXME: Update newX and newY to a PosXY object.
    int64_t newX{};
    int64_t newY{};
    if (threshold > this->threshold) {
        if ((brain.current_task == Brain::kTaskWandering) || (brain.current_task == Brain::kTaskSearchingFood)) {
            newX = brain.current_position.x + ((std::rand() % 3) -1);
            newY = brain.current_position.y + ((std::rand() % 3) -1);
        } else {
            if ((brain.current_destination.position.x != 0) && (brain.current_destination.position.y != 0)) {
                if (brain.current_destination.Expired() && (brain.carrying == World::kBlockAir)) {
                    brain.current_task = Brain::kTaskWandering;
                } else if ((brain.current_task == Brain::kTaskGatheringFood) &&
                           (!brain.world->OneBlockAway(brain.current_position, brain.current_destination.position))){
                    if (!brain.sensed_food.Expired()) { brain.current_destination.position = brain.sensed_food.position; };
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

                } else if ((brain.current_task == Brain::kTaskDeliveringFood) && (brain.carrying != World::kBlockAir) &&
                           (!brain.world->XBlocksAway(brain.current_position, brain.current_destination.position, 20))) {
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
    }
}


//void detect_food_neuron::tick(World::PosXY origin, float strength, World* world) {
void detect_food_neuron::tick(float threshold) {


    // FIXME: Initial fudge of threshold for manual task progression
    if (this->brain.current_task == Brain::kTaskSearchingFood) {
        this->threshold = 0.99f;
    } else {
        this->threshold = 0.0f;
    }

    // Arbitrary adjustment of strength float (expected between 0-1) to give a reasonable search distance
    // FIXME: This should be attached to a gene
    const float distance = threshold * 500;

    if ((this->brain.current_task == Brain::kTaskSearchingFood) && (this->brain.current_destination.Expired()) &&
        (threshold >= this->threshold)) {
        this->brain.sensed_food.position = this->brain.world->FindNearestBlockOfType(this->brain.current_position,
                                                                                     World::BlockTypes::kBlockFood,
                                                                                     static_cast<uint64_t>(distance));
        if ((brain.sensed_food.position.x != 0) && (brain.sensed_food.position.y != 0)) {
            // FIXME: Probably don't want arbitrary expiry times here
            this->brain.sensed_food.StartTimer(10000);
            std::cout << "Food found: " << this->brain.sensed_food.position.x << ", "
                      << this->brain.sensed_food.position.y << std::endl;
            this->brain.current_destination.position = this->brain.sensed_food.position;
            this->brain.current_destination.StartTimer(10000);
            std::cout << "Moving towards: " << this->brain.current_destination.position.x << ", "
                      << this->brain.current_destination.position.y << std::endl;
            // FIXME: Forcing a transition to gathering behavior here doesn't feel right but we need it for now
            //this->brain.current_task = Brain::kTaskGatheringFood;
        }
    } // else {
        // Didn't detect food, wander for a bit
        //this->brain.current_task = Brain::kTaskWandering;
    //}
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

    const auto now = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->last_measurement);
    //std::cout << "Time since last tick: " <<  (now - this->last_measurement) << " Duration: " << duration << std::endl;
    //this->delay_delta += duration.count();
    //std::cout << "Delay delta: " << std::to_string(this->delay_delta) << std::endl;
    float strength = std::min(static_cast<float>((float)duration.count() / this->delay), 1.0f);
    //std::cout << "Timer strength: " << std::to_string(strength) << std::endl;
    for (Neuron* neuron : this->outputs) {
        neuron->tick(strength);
    }
    if (duration.count() >= this->delay) {
        this->last_measurement = std::chrono::steady_clock::now();
    }
    //this->last_measurement = std::chrono::steady_clock::now();

    //std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() << " ms\n";

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
    this->threshold = 0.0f;
}

void task_neuron::tick(float threshold) {
    if (threshold > this->threshold) {
        switch (this->brain.current_task) {
            case Brain::kTaskWandering:
                this->brain.current_task = Brain::kTaskSearchingFood;
                break;
            case Brain::kTaskSearchingFood:
                std::cout << "Searching" << std::endl;
                if ((this->brain.current_task == Brain::kTaskSearchingFood) && (!this->brain.sensed_food.Expired())) {
                    this->brain.current_task = Brain::kTaskGatheringFood;
                } else {
                    //FIXME: Probably want a way to fall back to wandering but the below doesn't work as expected with
                    //       the current tests above
                    //this->brain.current_task = Brain::kTaskWandering;
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
                } else {
                    this->brain.current_destination.position = this->brain.food_stockpile;
                }
                break;
            default:
                std::cout << "No task match!!!" << std::endl;
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
            this->brain.world->SetBlockAtPos(this->brain.current_destination.position, World::kBlockUnderground);
            //this->brain.current_task = Brain::kTaskDeliveringFood;
            World::Tile* tile = this->brain.world->PosToTile(this->brain.current_destination.position.x, this->brain.current_destination.position.y);
            tile->RegenerateTexture();
            std::cout << "Picked up food" << std::endl;
        }
    }
    if ((threshold > this->threshold) && (this->brain.carrying == World::kBlockFood) &&
        //FIXME: dropping food at arbitrary distance to stockpile, this should be dynamic
        (this->brain.world->XBlocksAway(this->brain.current_position, this->brain.food_stockpile, 20))) {
        if (this->brain.world->GetBlockAtPos(World::PosXY{this->brain.current_position.x + 1, this->brain.current_position.y}) == World::kBlockUnderground) {
            this->brain.world->SetBlockAtPos(World::PosXY{this->brain.current_position.x + 1, this->brain.current_position.y}, World::kBlockStockpiledFood);
            World::Tile* tile = this->brain.world->PosToTile(this->brain.current_position.x + 1, this->brain.current_position.y);
            tile->RegenerateTexture();
            this->brain.carrying = World::kBlockAir;
            //this->brain.current_task = Brain::kTaskWandering;
            std::cout << "Dropped off food" << std::endl;
        }
    }
}