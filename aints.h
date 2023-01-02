//
// Created by Mark on 2022-07-18.
//

#ifndef AIANTS_AINTS_H
#define AIANTS_AINTS_H

#include <cstdint>
#include <vector>

class aints
{
public:
    int setId(int newId);
    int getId();
    bool updateLocation(int64_t x, int64_t y);
    int64_t getX();
    int64_t getY();
    //std::vector<*neuron> neurons;

    aints();
    ~aints();

    void move();
    void tick();

private:
    int id = 0;
    int64_t locX = 0;
    int64_t locY = 0;
};

#endif //AIANTS_AINTS_H
