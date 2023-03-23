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
    this->brain.current_position.x = x;
    this->brain.current_position.y = y;
    if ((this->brain.current_position.x == x) && (this->brain.current_position.y == y))
    {
        return true;
    } else
        return false;
}

int64_t aints::getX() {
    return this->brain.current_position.x;
}

int64_t aints::getY() {
    return this->brain.current_position.y;
}

aints::aints(World& world) {
    this->world = &world;
    brain.world = &world;
}

aints::~aints() {

}

void aints::tick() {
    this->fn.tick(World::PosXY{this->brain.current_position.x,this->brain.current_position.y}, 1.0f, this->world);
    if (this->fn.food_location.x != 0) {
        this->brain.next_position = this->fn.food_location;
    }
    this->mn.tick(1.0f, this->brain);
    this->updateLocation(this->getX(), this->getY());
    this->tn.tick();
}
