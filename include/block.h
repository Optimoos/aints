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

class GrassBlock
{
 public:
  explicit GrassBlock( double attribute = 0.0)
      : attribute_( attribute )
  {}

  // Copy constructor
  GrassBlock(const GrassBlock& other)
      : attribute_(other.attribute_) {}

  // Copy assignment operator
  GrassBlock& operator=(const GrassBlock& other) {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    return *this;
  }

  // Move constructor
  GrassBlock(GrassBlock&& other) noexcept
      : attribute_(other.attribute_) {
    other.attribute_ = 0.0;
  }

  // Move assignment operator
  GrassBlock& operator=(GrassBlock&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    other.attribute_ = 0.0;
    return *this;
  }

  // Destructor
  ~GrassBlock() {

  }

  double attribute() const { return attribute_; }
  BlockColor GetBlockColor() const { return color_; }

 private:
  double attribute_;
  const BlockColor color_{0, 228, 48, 255};
};

class FoodBlock
{
 public:
  explicit FoodBlock( double attribute = 0.0)
      : attribute_( attribute )
  {}

  // Copy constructor
  FoodBlock(const FoodBlock& other)
      : attribute_(other.attribute_) {}

  // Copy assignment operator
  FoodBlock& operator=(const FoodBlock& other) {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    return *this;
  }

  // Move constructor
  FoodBlock(FoodBlock&& other) noexcept
      : attribute_(other.attribute_) {
    other.attribute_ = 0.0;
  }

  // Move assignment operator
  FoodBlock& operator=(FoodBlock&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    other.attribute_ = 0.0;
    return *this;
  }

  // Destructor
  ~FoodBlock() {

  }

  double attribute() const { return attribute_; }
  BlockColor GetBlockColor() const { return color_; }

 private:
  double attribute_;
  const BlockColor color_{0, 117, 44, 255};
};

class WaterBlock
{
 public:
  explicit WaterBlock( double attribute = 0.0)
      : attribute_( attribute )
  {}

  // Copy constructor
  WaterBlock(const WaterBlock& other)
      : attribute_(other.attribute_) {}

  // Copy assignment operator
  WaterBlock& operator=(const WaterBlock& other) {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    return *this;
  }

  // Move constructor
  WaterBlock(WaterBlock&& other) noexcept
      : attribute_(other.attribute_) {
    other.attribute_ = 0.0;
  }

  // Move assignment operator
  WaterBlock& operator=(WaterBlock&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    other.attribute_ = 0.0;
    return *this;
  }

  // Destructor
  ~WaterBlock() {

  }

  double attribute() const { return attribute_; }
  BlockColor GetBlockColor() const { return color_; }

 private:
  double attribute_;
  const BlockColor color_{0, 121, 241, 255};
};

class SandBlock
{
 public:
  explicit SandBlock( double attribute = 0.0)
      : attribute_( attribute )
  {}

  // Copy constructor
  SandBlock(const SandBlock& other)
      : attribute_(other.attribute_) {}

  // Copy assignment operator
  SandBlock& operator=(const SandBlock& other) {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    return *this;
  }

  // Move constructor
  SandBlock(SandBlock&& other) noexcept
      : attribute_(other.attribute_) {
    other.attribute_ = 0.0;
  }

  // Move assignment operator
  SandBlock& operator=(SandBlock&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    other.attribute_ = 0.0;
    return *this;
  }

  // Destructor
  ~SandBlock() {

  }

  double attribute() const { return attribute_; }
  BlockColor GetBlockColor() const { return color_; }

 private:
  double attribute_;
  const BlockColor color_{211, 176, 131, 255};
};

class StockpiledFoodBlock
{
 public:
  explicit StockpiledFoodBlock( double attribute = 0.0)
      : attribute_( attribute )
  {}

  // Copy constructor
  StockpiledFoodBlock(const StockpiledFoodBlock& other)
      : attribute_(other.attribute_) {}

  // Copy assignment operator
  StockpiledFoodBlock& operator=(const StockpiledFoodBlock& other) {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    return *this;
  }

  // Move constructor
  StockpiledFoodBlock(StockpiledFoodBlock&& other) noexcept
      : attribute_(other.attribute_) {
    other.attribute_ = 0.0;
  }

  // Move assignment operator
  StockpiledFoodBlock& operator=(StockpiledFoodBlock&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    other.attribute_ = 0.0;
    return *this;
  }

  // Destructor
  ~StockpiledFoodBlock() {

  }

  double attribute() const { return attribute_; }
  BlockColor GetBlockColor() const { return color_; }

 private:
  double attribute_;
  const BlockColor color_{0, 117, 44, 255};
};

class InvalidBlock
{
 public:
  explicit InvalidBlock( double attribute = 0.0)
      : attribute_( attribute )
  {}

  // Copy constructor
  InvalidBlock(const InvalidBlock& other)
      : attribute_(other.attribute_) {}

  // Copy assignment operator
  InvalidBlock& operator=(const InvalidBlock& other) {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    return *this;
  }

  // Move constructor
  InvalidBlock(InvalidBlock&& other) noexcept
      : attribute_(other.attribute_) {
    other.attribute_ = 0.0;
  }

  // Move assignment operator
  InvalidBlock& operator=(InvalidBlock&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    attribute_ = other.attribute_;
    other.attribute_ = 0.0;
    return *this;
  }

  // Destructor
  ~InvalidBlock() {

  }

  double attribute() const { return attribute_; }
  BlockColor GetBlockColor() const { return color_; }

 private:
  double attribute_;
  const BlockColor color_{255, 0, 255, 255};
};

using Block = std::variant<AirBlock, DirtBlock, StoneBlock, UndergroundBlock, GrassBlock, FoodBlock, WaterBlock, SandBlock, StockpiledFoodBlock, InvalidBlock>;

//class GetColor
//{
// public:
//  void operator()( AirBlock const& b ) const
//  {
//    return b.GetBlockColor();
//  }
//  void operator()( DirtBlock const& b ) const
//  {
//    return b.GetBlockColor();
//  }
//  void operator()( StoneBlock const& b ) const
//  {
//    return b.GetBlockColor();
//  }
//  void operator()( UndergroundBlock const& b ) const
//  {
//    return b.GetBlockColor();
//  }
//};

#endif  // AIANTS_BLOCK_H
