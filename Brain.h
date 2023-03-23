//
// Created by Mark on 2023-03-22.
//

#ifndef AIANTS_BRAIN_H
#define AIANTS_BRAIN_H

#include <chrono>
#include "world.h"


class Brain {

public:
    class LocationInformation {
    public:
        World::PosXY position{0,0};

        void StartTimer(int64_t time_until_expiry) {this->expired = false; this->expiry = time_until_expiry; this->expiry_clock = std::chrono::steady_clock::now();};
        bool Expired(){this->expiry_delta += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - this->expiry_clock).count();
            if (this->expiry_delta > this->expiry) { this->expiry_delta = 0; this->expired = true;};
            this->expiry_clock = std::chrono::steady_clock::now();
            return this->expired;
        };
    private:
        bool expired{true};
        int64_t expiry{5000};
        int64_t expiry_delta{0};
        std::chrono::time_point<std::chrono::steady_clock> expiry_clock{std::chrono::steady_clock::now()};

    };

    enum TaskType {
        kTaskWandering,
        kTaskSearchingFood,
        kTaskGatheringFood,
        kTaskDeliveringFood

    };



    TaskType current_task{kTaskWandering};
    World::BlockTypes carrying{World::kBlockAir};
    World::PosXY next_position{0,0};
    World::PosXY current_position{0,0};
    LocationInformation current_destination;
    World* world;

};


#endif //AIANTS_BRAIN_H
