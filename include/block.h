#ifndef AIANTS_BLOCK_H
#define AIANTS_BLOCK_H

#include <array>
#include <cstdint>
#include <memory>
#include <utility>
#include <variant>
#include <vector>

typedef struct BlockColor
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;

} BlockColor;

class AirBlock
{
 public:
  explicit AirBlock( double attribute = 0.0)
      : attribute_( attribute )
  {}

  // Copy constructor
  AirBlock(const AirBlock& other)
      : attribute_(other.attribute_) {}

  // Copy assignment operator
  AirBlock& operator=(const AirBlock& other) {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    return *this;
  }

  // Move constructor
  AirBlock(AirBlock&& other) noexcept
      : attribute_(other.attribute_) {
    other.attribute_ = 0.0;
  }

  // Move assignment operator
  AirBlock& operator=(AirBlock&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    other.attribute_ = 0.0;
    return *this;
  }

  // Destructor
  ~AirBlock() {

  }

  double attribute() const { return attribute_; }
  BlockColor GetBlockColor() const { return color_; }

 private:
  double attribute_;
  const BlockColor color_{102, 191, 255, 255};
};

class DirtBlock
{
 public:
  explicit DirtBlock( double attribute = 0.0)
      : attribute_( attribute )
  {}

  // Copy constructor
  DirtBlock(const DirtBlock& other)
      : attribute_(other.attribute_) {}

  // Copy assignment operator
  DirtBlock& operator=(const DirtBlock& other) {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    return *this;
  }

  // Move constructor
  DirtBlock(DirtBlock&& other) noexcept
      : attribute_(other.attribute_) {
    other.attribute_ = 0.0;
  }

  // Move assignment operator
  DirtBlock& operator=(DirtBlock&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    other.attribute_ = 0.0;
    return *this;
  }

  // Destructor
  ~DirtBlock() {

  }

  double attribute() const { return attribute_; }
  BlockColor GetBlockColor() const { return color_; }

 private:
  double attribute_;
  const BlockColor color_{127, 106, 79, 255};
};

class StoneBlock
{
 public:
  explicit StoneBlock( double attribute = 0.0)
      : attribute_( attribute )
  {}

  // Copy constructor
  StoneBlock(const StoneBlock& other)
      : attribute_(other.attribute_) {}

  // Copy assignment operator
  StoneBlock& operator=(const StoneBlock& other) {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    return *this;
  }

  // Move constructor
  StoneBlock(StoneBlock&& other) noexcept
      : attribute_(other.attribute_) {
    other.attribute_ = 0.0;
  }

  // Move assignment operator
  StoneBlock& operator=(StoneBlock&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    other.attribute_ = 0.0;
    return *this;
  }

  // Destructor
  ~StoneBlock() {

  }

  double attribute() const { return attribute_; }
  BlockColor GetBlockColor() const { return color_; }

 private:
  double attribute_;
  const BlockColor color_{80, 80, 80, 255};
};

class UndergroundBlock
{
 public:
  explicit UndergroundBlock( double attribute = 0.0)
      : attribute_( attribute )
  {}

  // Copy constructor
  UndergroundBlock(const UndergroundBlock& other)
      : attribute_(other.attribute_) {}

  // Copy assignment operator
  UndergroundBlock& operator=(const UndergroundBlock& other) {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    return *this;
  }

  // Move constructor
  UndergroundBlock(UndergroundBlock&& other) noexcept
      : attribute_(other.attribute_) {
    other.attribute_ = 0.0;
  }

  // Move assignment operator
  UndergroundBlock& operator=(UndergroundBlock&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    other.attribute_ = 0.0;
    return *this;
  }

  // Destructor
  ~UndergroundBlock() {

  }

  double attribute() const { return attribute_; }
  BlockColor GetBlockColor() const { return color_; }

 private:
  double attribute_;
  const BlockColor color_{76, 63, 47, 255};
};

using Block = std::variant<AirBlock, DirtBlock, StoneBlock, UndergroundBlock>;

class GetColor
{
 public:
  void operator()( AirBlock const& b ) const
  {
    return b.GetBlockColor();
  }
  void operator()( DirtBlock const& b ) const
  {
    return b.GetBlockColor();
  }
  void operator()( StoneBlock const& b ) const
  {
    return b.GetBlockColor();
  }
  void operator()( UndergroundBlock const& b ) const
  {
    return b.GetBlockColor();
  }
};

//void GetAllBlockColors( Blocks const& blocks )
//{
//  for( auto const& block : blocks )
//  {
//    std::visit( GetColor{}, block );
//  }
//}

//class Block
//{
// public:
//  class IEntity;
// private:
//
//};

// Interface for entities interacting with blocks
//class Block::IEntity
//{
// public:
// private:
//
//};


#endif  // AIANTS_BLOCK_H
