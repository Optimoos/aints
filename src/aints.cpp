//
// Created by Mark on 2022-07-18.
//

#include "aints.h"

int aints::setId(int newId)
{
  this->id= newId;
  return this->id;
}

int aints::getId() const { return this->id; }

bool aints::updateLocation(int64_t x, int64_t y)
{
  this->brain.current_position.x= x;
  this->brain.current_position.y= y;
  if ((this->brain.current_position.x == x) &&
      (this->brain.current_position.y == y))
  {
    return true;
  }
  else
    return false;
}

int64_t aints::getX() const { return this->brain.current_position.x; }

int64_t aints::getY() const { return this->brain.current_position.y; }

PosXY aints::GetPosition() const { return this->brain.current_position; }

aints::aints(std::shared_ptr<World> world) : world(world)
{

  std::cout << "Ant constructed" << std::endl;
}

aints::~aints() { std::cout << "Ant deconstructed" << std::endl; }

void aints::tick()
{
  //    this->fn.tick(PosXY{this->brain.current_position.x,this->brain.current_position.y},
  //    1.0f, this->world); if (this->fn.food_location.x != 0) {
  //        this->brain.path_to_target = this->fn.food_location;
  //    }
  //    this->mn.tick(1.0f, this->brain);
  //    this->updateLocation(this->getX(), this->getY());
  this->tn.tick();
  if (this->brain.invalid_state)
  {
    this->dan.tick(1.0f);
    this->brain.invalid_state= false;
  }
}
