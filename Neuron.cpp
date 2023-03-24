#include "Neuron.h"

void move_neuron::tick(float threshold) {
    World::PosXY newXY{0,0};




    //FIXME: Temporarily setting threshold to 0 for full speed ants
    this->threshold = 0.0f;
    if (threshold > this->threshold) {
        // Random movement if we're wandering or searching for food
        // FIXME: food searching random movement will be replaced with pheromone behavior
        if ((brain.current_task == Brain::kTaskWandering) || (brain.current_task == Brain::kTaskSearchingFood)) {
            newXY.x = brain.current_position.x + ((std::rand() % 3) -1);
            newXY.y = brain.current_position.y + ((std::rand() % 3) -1);
        } else {
            // Destination is valid
            // FIXME: The position data structure should have an internal "valid" test
            if ((brain.current_destination.position.x != 0) && (brain.current_destination.position.y != 0)) {
                // If the destination has expired and we're not carrying anything, go back to wandering
                if (brain.current_destination.Expired() && (brain.carrying == World::kBlockAir)) {
                    brain.current_task = Brain::kTaskWandering;
                // If we're gathering food
                } else if (brain.current_task == Brain::kTaskGatheringFood) {
                    // Make sure our destination hasn't expired
                    if (!brain.sensed_food.Expired()) {
                        // If we have a path already, move there
                        if (!brain.next_position.empty()) {
                            newXY = brain.next_position.back();
                            brain.next_position.pop_back();
                        } else {
                            // Generate a new path
                            // FIXME: arbitrary search distance around FindNearestBlockOfType isn't great
                            World::PosXY nearest_underground = brain.world->FindNearestBlockOfType(brain.sensed_food.position, World::kBlockUnderground, 10);
                            std::vector<World::PosXY> temp_route = brain.world->FindPath(brain.current_position, nearest_underground);
                            brain.next_position.empty();
                            for(std::vector<World::PosXY>::reverse_iterator i = temp_route.rbegin(); i != temp_route.rend(); ++i)
                            {
                                brain.next_position.push_back(*i);
                            }
                        }
                    } else {
                        // Go back to sensing/searching
                        brain.current_task = Brain::kTaskSearchingFood;
                    }
                    this->NextPosition(brain.current_position, brain.current_destination.position, newXY);
                    std::cout << "Path info gathering: " << brain.current_position.x << ", " << brain.current_position.y << "New: " << newXY.x << ", " << newXY.y << " Block: " << brain.world->GetBlockAtPos(newXY) << std::endl;

                } else if ((brain.current_task == Brain::kTaskDeliveringFood) && (brain.carrying != World::kBlockAir)) {
                    this->NextPosition(brain.current_position, brain.current_destination.position, newXY);
                    std::cout << "Path info delivering: " << brain.current_position.x << ", " << brain.current_position.y << "New: " << newXY.x << ", " << newXY.y << " Block: " << brain.world->GetBlockAtPos(newXY) << std::endl;
                }
            }
        }
        if ((newXY.x != 0) && (newXY.y != 0)) {
            World::BlockTypes block = brain.world->GetBlockAtPos(newXY);
            if (block == World::BlockTypes::kBlockUnderground) {
                brain.current_position = newXY;
            } else {
                std::cout << "Block: " << block;
                std::cout << "Path obstructed!" << brain.current_position.x << ", " << brain.current_position.y << "New: " << newXY.x << ", " << newXY.y << " Block: " << brain.world->GetBlockAtPos(newXY) << std::endl;
                //Fixme: This is a panic fix to move to an open block 1 block away if it exists.
                //       This should be handled by better pathfinding instead.
                //brain.current_position = brain.world->FindNearestBlockOfType(brain.current_position, World::kBlockUnderground, 3);
            }
        } else {
            std::cout << "Path invalid! " << brain.current_position.x << ", " << brain.current_position.y << " New: " << newXY.x << ", " << newXY.y << " Block: " << brain.world->GetBlockAtPos(newXY) << std::endl;
            //brain.current_position = brain.world->FindNearestBlockOfType(brain.current_position, World::kBlockUnderground, 3);

        }
    }
}

void move_neuron::NextPosition(World::PosXY origin, World::PosXY destination, World::PosXY& next) {
    std::cout << "Next position origin: " << origin.x << ", " << origin.y << std::endl;
    std::cout << "Next position destination: " << destination.x << ", " << destination.y << std::endl;
    double distance = sqrt(pow(destination.x - origin.x, 2) + pow(destination.y - origin.y, 2));
    double directionX = (destination.x - origin.x) / distance;
    double directionY = (destination.y - origin.y) / distance;
    next.x = round(origin.x + directionX);
    next.y = round(origin.y + directionY);
    if ((next.x < 0) || (next.y < 0)){
        std::cout << "STOP HERE WTF IS GOING ON!" << std::endl;
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
                if ((this->brain.current_destination.position.x == 0) && (this->brain.current_destination.position.y == 0)) {
                    this->brain.current_task = Brain::kTaskSearchingFood;
                }

                break;
            case Brain::kTaskDeliveringFood:
                // std::cout << "Delivering" << std::endl;
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
        int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
        int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

        for (int i = 0; i < 8; i++) {
            int x = this->brain.current_position.x + dx[i];
            int y = this->brain.current_position.y + dy[i];
            this->brain.adjacent_blocks[i].block = this->brain.world->GetBlockAtPos(World::PosXY{x,y});
            this->brain.adjacent_blocks[i].position = World::PosXY{x,y};
        }
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
            this->brain.current_destination.SetExpired();
            this->brain.sensed_food.SetExpired();
            std::cout << "Picked up food: " << this->brain.current_destination.position.x << ", " << this->brain.current_destination.position.y << std::endl;
        }
    }
    if ((threshold > this->threshold) && (this->brain.carrying == World::kBlockFood)) {
        if (this->brain.current_destination.Expired()) {
            //FIXME: Arbitrarily searching 20 blocks around the stockpile, should be dynamic
            this->brain.dropoff_position = this->brain.world->FindNearestBlockOfType(this->brain.food_stockpile, World::kBlockUnderground, 20);
            this->brain.current_destination.position = brain.dropoff_position;
            std::cout << "Delivering to: " << this->brain.dropoff_position.x << ", " << this->brain.dropoff_position.y << std::endl;
            this->brain.current_destination.StartTimer(10000);
        }
        if (this->brain.world->OneBlockAway(this->brain.current_position, this->brain.dropoff_position)) {
            if (this->brain.world->GetBlockAtPos(this->brain.dropoff_position) == World::kBlockUnderground) {
                this->brain.world->SetBlockAtPos(this->brain.dropoff_position, World::kBlockStockpiledFood);
                World::Tile *tile = this->brain.world->PosToTile(this->brain.dropoff_position.x,
                                                                 this->brain.dropoff_position.y);
                tile->RegenerateTexture();
                this->brain.carrying = World::kBlockAir;
                this->brain.current_destination.SetExpired();
                //this->brain.current_task = Brain::kTaskWandering;
                std::cout << "Dropped off food" << std::endl;
            }
        }
    }
}