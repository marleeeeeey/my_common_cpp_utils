#pragma once
#include <utils/Logger.h>

namespace utils
{

template <typename T>
std::string toString( const T& value )
{
    std::ostringstream ss;
    ss << value;
    return ss.str();
}

template <typename T>
T fromString( std::string_view string )
{
    std::istringstream ss( string.data() );
    T value;
    ss >> value;
    return value;
}

template <typename T>
T fromChar( char c )
{
    std::istringstream ss( std::string( 1, c ) );
    T value;
    ss >> value;

    if ( ss.fail() )
        throw std::runtime_error( MY_FMT( "Failed to convert char `{}` to type `{}`", c, typeid( T ).name() ) );

    return value;
}

// trim from start (in place)
inline void ltrim( std::string& s )
{
    s.erase( s.begin(), std::ranges::find_if( s, []( int ch ) { return !std::isspace( ch ); } ) );
}

// trim from end (in place)
inline void rtrim( std::string& s )
{
    s.erase( std::find_if( s.rbegin(), s.rend(), []( int ch ) { return !std::isspace( ch ); } ).base(), s.end() );
}

// trim from both ends (in place)
inline void trim( std::string& s )
{
    ltrim( s );
    rtrim( s );
}

// trim from start (copying)
inline std::string ltrimCopy( std::string s )
{
    ltrim( s );
    return s;
}

// trim from end (copying)
inline std::string rtrimCopy( std::string s )
{
    rtrim( s );
    return s;
}

// trim from both ends (copying)
inline std::string trimCopy( std::string s )
{
    trim( s );
    return s;
}

} // namespace utils