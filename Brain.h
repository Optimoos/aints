//
// Created by Mark on 2023-03-22.
//

#ifndef AIANTS_BRAIN_H
#define AIANTS_BRAIN_H

#include "world.h"

class Brain {

public:
    World::PosXY next_position{0,0};
    World::PosXY current_position{0,0};
    World* world;

    Brain();

};


#endif //AIANTS_BRAIN_H
