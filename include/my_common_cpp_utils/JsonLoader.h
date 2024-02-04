#pragma once
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

namespace utils
{

class JsonLoader
{
    json root_;
    std::optional<std::string> lastFilename_;
public:
    JsonLoader();
    JsonLoader(const JsonLoader&) = delete;
    JsonLoader& operator=(const JsonLoader&) = delete;

    json& root();
    const json& root() const;

    bool loadFromString(std::string_view jsonString);
    bool loadFromFile(std::string_view filename);

    bool saveToSameFile() const;
    bool saveToFile(const std::string& filename) const;
};

// Function to access data at a specified path within JSON
std::optional<json> getElementByPath(const json& jsonData, std::string_view path);

} // namespace utils