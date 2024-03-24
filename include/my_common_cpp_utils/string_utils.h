#pragma once
#include <my_common_cpp_utils/logger.h>

namespace utils
{

template <typename T>
std::string ToString(const T& value)
{
    std::ostringstream ss;
    ss << value;
    return ss.str();
}

template <typename T>
T FromString(std::string_view string)
{
    std::istringstream ss(string.data());
    T value;
    ss >> value;
    return value;
}

template <typename T>
T FromChar(char c)
{
    std::istringstream ss(std::string(1, c));
    T value;
    ss >> value;

    if (ss.fail())
        throw std::runtime_error(MY_FMT("Failed to convert char `{}` to type `{}`", c, typeid(T).name()));

    return value;
}

// trim from start (in place)
inline void TrimLeft(std::string& s)
{
    s.erase(s.begin(), std::ranges::find_if(s, [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
inline void TrimRight(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
}

// trim from both ends (in place)
inline void Trim(std::string& s)
{
    TrimLeft(s);
    TrimRight(s);
}

// trim from start (copying)
inline std::string TrimLeftCopy(std::string s)
{
    TrimLeft(s);
    return s;
}

// trim from end (copying)
inline std::string TrimRightCopy(std::string s)
{
    TrimRight(s);
    return s;
}

// trim from both ends (copying)
inline std::string TrimCopy(std::string s)
{
    Trim(s);
    return s;
}

inline std::string JoinStrings(const std::vector<std::string>& strings, const std::string& separator)
{
    std::string result;
    for (size_t i = 0; i < strings.size(); ++i)
    {
        if (i > 0)
        {
            result += separator;
        }
        result += strings[i];
    }
    return result;
}

template <typename T>
    requires requires(T a) {
        a.begin();
        a.end();
        ToString(*a.begin());
    }
std::string JoinStrings(const T& container, const std::string& separator = ", ")
{
    std::string result;
    for (auto it = container.begin(); it != container.end(); ++it)
    {
        if (it != container.begin())
        {
            result += separator;
        }
        result += ToString(*it);
    }
    return result;
}

} // namespace utils