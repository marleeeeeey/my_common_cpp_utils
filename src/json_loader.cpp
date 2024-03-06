#include "json_loader.h"
#include <fstream>
#include <my_common_cpp_utils/logger.h>
#include <stdexcept>

namespace utils
{

void JsonLoader::loadFromFile(const std::filesystem::path& jsonFilePath)
{
    if (isLoaded_)
        throw std::runtime_error("JSON data is already loaded. Use saveToSameFile() to save changes");

    std::ifstream inputFile(jsonFilePath);
    if (!inputFile.is_open())
        throw std::logic_error(MY_FMT("Error opening file for reading: {}", jsonFilePath.string()));

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

        root_ = nlohmann::json::parse(buffer);
        isLoaded_ = true;
    }
    catch (const std::exception& e)
    {
        throw std::logic_error(
            MY_FMT("Error loading JSON from file. File: {}. Error: {}", jsonFilePath.string(), e.what()));
    }
}

void JsonLoader::saveToSameFile() const
{
    if (!lastFilename_.has_value())
        throw std::logic_error("No filename specified for saving JSON");

    return saveToFile(lastFilename_.value());
}

void JsonLoader::saveToFile(const std::string& filename) const
{
    std::ofstream output_file(filename);
    if (!output_file.is_open())
        throw std::logic_error(MY_FMT("Error opening file for writing: {}", filename));

    output_file << root_.dump(2);
    output_file.close();

    MY_LOG_FMT(info, "JSON data has been written to {}", filename);
}

void JsonLoader::loadFromString(const std::string& jsonString)
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