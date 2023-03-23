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

        void StartTimer(int64_t time_until_expiry) {expiry_delta = time_until_expiry; expiry_counter = std::chrono::steady_clock::now();};
        bool Expired(){expiry_delta -= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - expiry_counter).count();
                        expiry_delta <= 0 ? expired = true : expired = false;
                        return expired;};
    private:
        bool expired{true};
        int64_t expiry_delta{5000};
        std::chrono::time_point<std::chrono::steady_clock> expiry_counter{std::chrono::steady_clock::now()};
    };

    enum TaskType {
        kTaskWandering,
        kTaskSearchingFood,
        kTaskGatheringFood,
        kTaskDeliveringFood

    };



    TaskType current_task{kTaskWandering};
    World::PosXY next_position{0,0};
    World::PosXY current_position{0,0};
    World::PosXY current_destination{0,0};
    World* world;

};


#endif //AIANTS_BRAIN_H
