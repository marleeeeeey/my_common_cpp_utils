#include <json_loader.h>
#include <my_common_cpp_utils/config.h>
#include <my_common_cpp_utils/logger.h>

namespace utils
{

namespace
{
JsonLoader& getSingletoneInstance()
{
    static JsonLoader jsonLoader;
    return jsonLoader;
}
} // namespace

void Config::initInstanceFromString(std::string& jsonSourceAsString)
{
    getSingletoneInstance().loadFromString(jsonSourceAsString);
};

void Config::initInstanceFromFile(std::filesystem::path& jsonFilePath)
{
    getSingletoneInstance().loadFromFile(jsonFilePath);
};

nlohmann::json& Config::getInstance()
{
    JsonLoader& jsonLoader = getSingletoneInstance();
    if (!jsonLoader.isLoaded())
        throw std::runtime_error("Config is not initialized. Use initInstanceFromFile() or initInstanceFromString()");

    return jsonLoader.root();
};

void Config::save()
{
    getSingletoneInstance().saveToSameFile();
};
} // namespace utils