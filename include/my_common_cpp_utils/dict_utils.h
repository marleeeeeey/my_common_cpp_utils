#pragma once
#include <vector>

namespace utils
{

template <typename Map>
auto GetKeys(const Map& map) -> std::vector<typename Map::key_type>
{
    std::vector<typename Map::key_type> keys;
    for (const auto& pair : map)
    {
        keys.push_back(pair.first);
    }
    return keys;
}

} // namespace utils