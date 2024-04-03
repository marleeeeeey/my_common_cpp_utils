#include <json_loader.h>
#include <my_cpp_utils/config.h>
#include <my_cpp_utils/logger.h>

namespace utils
{

namespace
{
JsonLoader& GetSingletoneInstance()
{
    static JsonLoader jsonLoader;
    return jsonLoader;
}
} // namespace

void Config::InitInstanceFromString(const std::string& jsonSourceAsString)
{
    GetSingletoneInstance().LoadFromString(jsonSourceAsString);
};

void Config::InitInstanceFromFile(const std::filesystem::path& jsonFilePath)
{
    GetSingletoneInstance().LoadFromFile(jsonFilePath);
};

nlohmann::json& Config::GetInstance()
{
    JsonLoader& jsonLoader = GetSingletoneInstance();
    if (!jsonLoader.IsLoaded())
        throw std::runtime_error("Config is not initialized. Use initInstanceFromFile() or initInstanceFromString()");

    return jsonLoader.Root();
};

void Config::Save()
{
    GetSingletoneInstance().SaveToSameFile();
};
} // namespace utils