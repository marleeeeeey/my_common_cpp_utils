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

enum class JsonLoadOpions
{
    None,
    RemoveComments,
};

inline nlohmann::json LoadJsonFromFile(
    const std::filesystem::path& jsonFilePath, JsonLoadOpions options = JsonLoadOpions::None)
{
    std::ifstream inputFile(jsonFilePath);
    if (!inputFile.is_open())
        throw std::runtime_error("[LoadJsonFromFile] Error opening file for reading: " + jsonFilePath.string());

    try
    {
        if (options == JsonLoadOpions::None)
        {
            std::stringstream buffer;
            buffer << inputFile.rdbuf();
            return nlohmann::json::parse(buffer);
        }

        // This type of loading may produce errors like this:
        // Error: [json.exception.parse_error.101] parse error at line 69, column 0: syntax error while parsing value -
        // invalid string: control character U+000A (LF) must be escaped to \u000A or \n; last read: '"https:<U+000A>'
        std::stringstream buffer;
        std::string line;
        while (std::getline(inputFile, line))
        {
            if (options == JsonLoadOpions::RemoveComments)
            {
                // Remove one-line comments
                std::size_t commentPos = line.find("//");
                if (commentPos != std::string::npos)
                {
                    line.erase(commentPos);
                }
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