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

    nlohmann::json& Root() { return root_; }
    const nlohmann::json& Root() const { return root_; }

    void LoadFromString(const std::string& jsonString);
    void LoadFromFile(const std::filesystem::path& jsonFilePath);

    void SaveToSameFile() const;
    void SaveToFile(const std::string& filename) const;

    bool IsLoaded() const { return isLoaded_; }
};

} // namespace utils