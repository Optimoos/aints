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

aints::aints(World& world) {
    this->world = &world;
}

aints::~aints() {

}

void aints::tick() {
    this->fn.tick(World::PosXY{this->locX,this->locY}, 1.0f, this->world);
    if (this->fn.food_location.x != 0) {
        this->desired_destination = this->fn.food_location;
    }
    this->mn.tick(1.0f, &this->locX, &this->locY, this->world, this->desired_destination);
    this->updateLocation(this->getX(), this->getY());
}
