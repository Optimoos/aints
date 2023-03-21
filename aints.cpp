//
// Created by Mark on 2022-07-18.
//

#include "aints.h"
#include <entt/entt.hpp>
#include <iostream>

int aints::setId(int newId) {
    this->id = newId;
    return this->id;
}

int aints::getId() {
    return this->id;
}

bool aints::updateLocation(int64_t x, int64_t y) {
    this->locX = x;
    this->locY = y;
    if ((this->locX == x) && (this->locY == y))
    {
        return true;
    } else
        return false;
}

int64_t aints::getX() {
    return this->locX;
}

int64_t aints::getY() {
    return this->locY;
}

aints::aints(World* world) {
    this->world = world;
}

aints::~aints() {

}

void move_neuron::tick(float tick_threshold, int64_t* x, int64_t* y, World* world) {
    if (tick_threshold > this->threshold) {
        int64_t newX = *x + ((std::rand() % 3) -1);
        int64_t newY = *y + ((std::rand() % 3) -1);
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

void aints::tick() {
    this->mn.tick(1.0f, &this->locX, &this->locY, this->world);
    this->updateLocation(this->getX(), this->getY());
}

move_neuron::move_neuron() {
    this->threshold = 0.7f;
}
