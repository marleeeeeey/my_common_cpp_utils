#include <fstream>
#include <iostream>
#include <my_common_cpp_utils/JsonLoader.h>
#include <my_common_cpp_utils/Logger.h>

namespace utils
{

JsonLoader::JsonLoader() = default;

json& JsonLoader::root()
{
    return root_;
}

const json& JsonLoader::root() const
{
    return root_;
}

bool JsonLoader::loadFromFile(std::string_view filename)
{
    std::ifstream inputFile(filename.data());
    if (!inputFile.is_open())
    {
        MY_LOG_FMT(warn, "Error opening file for reading: {}", filename);
        return false;
    }

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
    }
    catch (const std::exception& e)
    {
        MY_LOG_FMT(warn, "Error loading JSON from file. File: {}. Error: {}", filename, e.what());
        return false;
    }

    return true;
}

bool JsonLoader::saveToSameFile() const
{
    if (!lastFilename_.has_value())
    {
        MY_LOG(warn, "No filename specified for saving JSON");
        return false;
    }

    return saveToFile(lastFilename_.value());
}

bool JsonLoader::saveToFile(const std::string& filename) const
{
    try
    {
        std::ofstream output_file(filename);
        if (!output_file.is_open())
        {
            MY_LOG_FMT(warn, "Error opening file for writing: {}", filename);
            return false;
        }

        output_file << root_.dump(2);
        output_file.close();

        MY_LOG_FMT(info, "JSON data has been written to {}", filename);
        return true;
    }
    catch (const std::exception& e)
    {
        MY_LOG_FMT(warn, "Error saving JSON to file. File: {}. Error: {}", filename, e.what());
        return false;
    }
}

bool JsonLoader::loadFromString(std::string_view jsonString)
{
    try
    {
        root_ = json::parse(jsonString);
        return true;
    }
    catch (const std::exception& e)
    {
        MY_LOG_FMT(warn, "Error parsing JSON from string: {}", e.what());
        return false;
    }
}

std::optional<json> getElementByPath(const json& jsonData, std::string_view path)
{
    json currentNode = jsonData;
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