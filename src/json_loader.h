#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace utils
{

// Class to load and save JSON data. It doesn't save changes automatically.
// Throw exception in case of errors.
class JsonLoader
{
    nlohmann::json root_;
    std::optional<std::string> lastFilename_;
    bool isLoaded_ = false;
public:
    JsonLoader() = default;
    JsonLoader(const JsonLoader&) = delete;
    JsonLoader& operator=(const JsonLoader&) = delete;

    nlohmann::json& root() { return root_; }
    const nlohmann::json& root() const { return root_; }

    void loadFromString(const std::string& jsonString);
    void loadFromFile(const std::filesystem::path& jsonFilePath);

    void saveToSameFile() const;
    void saveToFile(const std::string& filename) const;

    bool isLoaded() const { return isLoaded_; }
};

} // namespace utils