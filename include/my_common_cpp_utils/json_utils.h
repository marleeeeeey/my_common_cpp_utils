#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
#include <optional>
#include <sstream>
#include <stdexcept>

namespace utils
{

// Function to access data at a specified path within JSON.
// Path is a string with keys separated by dots. Example: "a.b.c".
inline std::optional<nlohmann::json> GetElementByPath(const nlohmann::json& jsonData, std::string_view path)
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

inline nlohmann::json LoadJsonFromFile(const std::filesystem::path& jsonFilePath)
{
    std::ifstream inputFile(jsonFilePath);
    if (!inputFile.is_open())
        throw std::runtime_error("[LoadJsonFromFile] Error opening file for reading: " + jsonFilePath.string());

    try
    {
        std::stringstream buffer;
        std::string line;
        while (std::getline(inputFile, line))
        {
            // Remove one-line comments
            std::size_t commentPos = line.find("//");
            if (commentPos != std::string::npos)
            {
                line.erase(commentPos);
            }
            buffer << line << "\n";
        }

        return nlohmann::json::parse(buffer);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error(
            "[LoadJsonFromFile] Error reading file: " + jsonFilePath.string() + ". Error: " + e.what());
    }
}

} // namespace utils