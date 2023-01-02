//
// Created by Mark on 2022-07-18.
//

#include "aints.h"
#include <entt/entt.hpp>
#include <iostream>


int aints::setId(int newId)
{
    this->id = newId;
    return this->id;
}

int aints::getId()
{
    return this->id;
}

bool aints::updateLocation(int64_t x, int64_t y)
{
    this->locX = x;
    this->locY = y;
    if ((this->locX == x) && (this->locY == y))
    {
        return true;
    } else
        return false;

};
int64_t aints::getX()
{
    return this->locX;
};
int64_t aints::getY()
{
    return this->locY;
};


aints::aints()
{

};

aints::~aints()
{

};

void aints::move() {
    int64_t newX = this->getX() + ((std::rand() % 3) -1);
    int64_t newY = this->getY() + ((std::rand() % 3) -1);
    this->updateLocation(newX, newY);
}

void aints::tick() {
    this->move();
}