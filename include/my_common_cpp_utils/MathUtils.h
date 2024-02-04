#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <unordered_set>
#include <algorithm>

namespace utils
{

template <typename T>
bool isEqual( T lhs, T rhs, T eps = 0.0001f )
{
    return std::fabs( lhs - rhs ) < eps;
}

inline bool randomBool()
{
    return glm::linearRand( 0, 1 ) == 1;
}

// Returns 1 if val > 0, -1 if val < 0, and 0 if val == 0.
template <typename T>
T sign( T val )
{
    return ( T( 0 ) < val ) - ( val < T( 0 ) );
}

#ifdef SFML_AVAILABLE

template <typename T>
T getArea( const sf::Vector2<T>& size )
{
    return size.x * size.y;
}

template <typename T>
sf::Vector2<T> normalize( const sf::Vector2<T>& v )
{
    float length = std::sqrt( v.x * v.x + v.y * v.y );
    if ( length != 0 )
        return sf::Vector2<T>( v.x / length, v.y / length );
    return v;
}

#endif // SFML_AVAILABLE

// value = 85, step = 10 => 80
template <typename T>
T roundStep( T value, T step )
{
    return step * glm::round( value / step );
}

template <typename T>
class NoRepeatableRandoms
{
public:
    NoRepeatableRandoms() = default;

    explicit NoRepeatableRandoms( size_t capacity, T min, T max ) : capacity_( capacity ), min_( min ), max_( max )
    {
        initValues();
    }

    bool empty() const { return values_.empty(); }

    T pull()
    {
        if ( values_.empty() )
            initValues();

        auto it = values_.begin();
        auto number = *it;
        values_.erase( it );
        return number;
    }

    T pullOrThrow()
    {
        if ( values_.empty() )
            throw std::logic_error( "NoRepeatableRandoms::pullOrThrow() - no more numbers" );

        return pull();
    }

    void sort()
    {
        std::vector<T> sortedValues( values_.begin(), values_.end() );
        std::sort( sortedValues.begin(), sortedValues.end() );
        values_ = std::unordered_set<T>( sortedValues.begin(), sortedValues.end() );
    }
private:
    void initValues()
    {
        while ( values_.size() != capacity_ )
            values_.insert( glm::linearRand<T>( min_, max_ ) );
    }

    size_t capacity_;
    std::unordered_set<T> values_;
    T min_;
    T max_;
};

} // namespace utils