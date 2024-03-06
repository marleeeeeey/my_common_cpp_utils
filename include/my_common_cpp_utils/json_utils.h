#pragma once
#include <nlohmann/json.hpp>
#include <optional>
#include <sstream>

namespace utils
{

// Function to access data at a specified path within JSON.
// Path is a string with keys separated by dots. Example: "a.b.c".
inline std::optional<nlohmann::json> getElementByPath(const nlohmann::json& jsonData, std::string_view path)
{
    nlohmann::json currentNode = jsonData;
    std::istringstream ss(path.data());
    std::string segment;

    while (std::getline(ss, segment, '.'))
    {
        if (currentNode.is_object() && currentNode.find(segment) != currentNode.end())
            currentNode = currentNode[segment];
        else
            return std::nullopt;
    }

    return currentNode;
}

} // namespace utils