#include "json_loader.h"
#include <fstream>
#include <my_cpp_utils/json_utils.h>
#include <my_cpp_utils/logger.h>
#include <stdexcept>

namespace utils
{

void JsonLoader::LoadFromFile(const std::filesystem::path& jsonFilePath)
{
    if (isLoaded_)
        throw std::runtime_error("JSON data is already loaded. Use saveToSameFile() to save changes");

    root_ = utils::LoadJsonFromFile(jsonFilePath, JsonLoadOpions::RemoveComments);
    isLoaded_ = true;
}

void JsonLoader::SaveToSameFile() const
{
    if (!lastFilename_.has_value())
        throw std::logic_error("No filename specified for saving JSON");

    return SaveToFile(lastFilename_.value());
}

void JsonLoader::SaveToFile(const std::string& filename) const
{
    std::ofstream output_file(filename);
    if (!output_file.is_open())
        throw std::logic_error(MY_FMT("Error opening file for writing: {}", filename));

    output_file << root_.dump(2);
    output_file.close();
}

void JsonLoader::LoadFromString(const std::string& jsonString)
{
    try
    {
        root_ = nlohmann::json::parse(jsonString);
        isLoaded_ = true;
    }
    catch (const std::exception& e)
    {
        throw std::logic_error(MY_FMT("Error parsing JSON from string: {}", e.what()));
    }
}

} // namespace utils