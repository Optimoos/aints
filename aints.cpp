//
// Created by Mark on 2022-07-18.
//

#include "aints.h"

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

void move_neuron::tick(float tick_threshold, int64_t* x, int64_t* y, World* world, World::PosXY desired) {
    int64_t newX{};
    int64_t newY{};
    if (tick_threshold > this->threshold) {
        if ((desired.x != 0) && (desired.y != 0)) {
            if (*x - desired.x > 0) {
                newX = *x - 1;
            } else {
                newX = *x + 1;
            }
            if (*y - desired.y > 0) {
                newY = *y - 1;
            } else {
                newY = *y + 1;
            }
        } else {
            newX = *x + ((std::rand() % 3) -1);
            newY = *y + ((std::rand() % 3) -1);
        }

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
    this->fn.tick(World::PosXY{this->locX,this->locY}, 1.0f, this->world);
    if (this->fn.food_location.x != 0) {
        this->desired_destination = this->fn.food_location;
    }
    this->mn.tick(1.0f, &this->locX, &this->locY, this->world, this->desired_destination);
    this->updateLocation(this->getX(), this->getY());
}

void detect_food_neuron::tick(World::PosXY origin, float strength, World* world) {
    // Arbitrary adjustment of strength float (expected between 0-1) to give a reasonable search distance
    const uint64_t distance = strength * 100;
    this->food_location = world->FindNearestBlockOfType(origin, World::BlockTypes::kBlockFood, distance);
}

move_neuron::move_neuron() {
    this->threshold = 0.7f;
}

detect_food_neuron::detect_food_neuron() {
    this->threshold = 0.7f;
    this->strength = 0.5f;
}