#ifndef AIANTS_BLOCK_H
#define AIANTS_BLOCK_H

#include <array>
#include <cstdint>
#include <memory>
#include <utility>

// FIXME: move this internal to Block, or potentially its own class
const enum BlockTypes
{
  kBlockAir,
  kBlockDirt,
  kBlockGrass,
  kBlockFood,
  kBlockStone,
  kBlockWater,
  kBlockSand,
  kBlockUnderground,
  kBlockStockpiledFood,
  kBlockInvalid
};

typedef struct BlockColor
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} BlockColor;

// Uses small buffer optimized type erasure proposed by Klaus Iglberger
// Although this requires more work while standing up functions, it is expected
// that longer term the types of blocks will be more variable and type erasure
// will make that simpler versus Visitor/variant.
template< size_t Capacity = 32U, size_t Alignment = alignof(void*) >
class Block
{
 public:
  //  template< typename BlockT, typename DrawStrategy >
  template< typename BlockT, typename Strategy>
  Block( BlockT block)
  {
    using Model = OwningModel<BlockT, Strategy>;

    static_assert( sizeof(Model) <= Capacity, "Given type is too large" );
    static_assert( alignof(Model) <= Alignment, "Given type is misaligned" );

    std::construct_at( static_cast<Model*>(pimpl())
                          , std::move(block), std::move(drawer) );
    // or:
    // auto* ptr =
    //    const_cast<void*>(static_cast<void const volatile*>(pimpl()));
    // ::new (ptr) Model( std::move(Block), std::move(drawer) );
  }
  // Constructors/operators begin

  Block( Block const& other )
  {
    other.pimpl()->clone( pimpl() );
  }

  Block& operator=( Block const& other )
  {
    // Copy-and-Swap Idiom
    Block copy( other );
    buffer_.swap( copy.buffer_ );
    return *this;
  }

  Block( Block&& other ) noexcept
  {
    other.pimpl()->move( pimpl() );
  }

  Block& operator=( Block&& other ) noexcept
  {
    // Copy-and-Swap Idiom
    Block copy( std::move(other) );
    buffer_.swap( copy.buffer_ );
    return *this;
  }

  ~Block()
  {
    std::destroy_at( pimpl() );
    // or: pimpl()->~Concept();
  }
  // Constructors/operators end

 private:
  // Functions begin, start with friends to call functions on specific Block

  friend void draw( Block const& block )
  {
    block.pimpl()->operate();
  }

  // Functions continue, concept provides function definitions

  struct Concept  // The External Polymorphism design pattern
  {
    virtual ~Concept() = default;
    virtual void operate() const = 0;
    virtual void clone( Concept* memory ) const = 0;  // The Prototype design pattern
    virtual void move( Concept* memory ) = 0;
  };

  // Functions continue, Model concept template links public to private functions

  template< typename BlockT, typename Strategy >
  struct OwningModel : public Concept
  {
    OwningModel( BlockT block, Strategy strategy )
        : block_( std::move(block) ),
          strategy_( std::move(strategy) )
    {}

    void operate() const override
    {
      strategy_( block_ );
    }

    void clone( Concept* memory ) const override
    {
      std::construct_at( static_cast<OwningModel*>(memory), *this );

      // or:
      // auto* ptr = const_cast<void*>(static_cast<void const volatile*>(memory));
      // ::new (ptr) OwningModel( *this );
    }

    void move( Concept* memory ) override
    {
      std::construct_at( static_cast<OwningModel*>(memory), std::move(*this) );

      // or:
      // auto* ptr = const_cast<void*>(static_cast<void const volatile*>(memory));
      // ::new (ptr) OwningModel( std::move(*this) );
    }

    BlockT block_;
    Strategy strategy_;
  };

  Concept* pimpl()  // The Bridge design pattern
  {
    return reinterpret_cast<Concept*>( buffer_.data() );
  }

  Concept const* pimpl() const
  {
    return reinterpret_cast<Concept const*>( buffer_.data() );
  }

  alignas(Alignment) std::array<std::byte,Capacity> buffer_;
};


class AirBlock
{
 public:
  explicit AirBlock( double attribute )
      : attribute_( attribute )
  {}

  double attribute() const { return attribute_; }
  BlockColor color() const { return color_; }

 private:
  double attribute_;
  const BlockColor color_{102, 191, 255, 255};
};



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
