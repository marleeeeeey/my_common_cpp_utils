#include "my_common_cpp_utils/Config.h"
#include "my_common_cpp_utils/JsonLoader.h"
#include "my_common_cpp_utils/Logger.h"
#include <memory>

namespace utils
{

std::shared_ptr<JsonLoader>& Config::getInstance( Mode mode, std::string_view jsonSourceAsString )
{
    static std::shared_ptr<JsonLoader> jsonLoader;

    // Return already loaded config
    if ( jsonLoader && mode != Mode::ForceReload )
        return jsonLoader;

    if ( !jsonSourceAsString.empty() )
    {
        jsonLoader = std::make_shared<JsonLoader>();
        if ( !jsonLoader->loadFromString( jsonSourceAsString.data() ) )
            throw std::runtime_error( "Failed to load config from string" );
        MY_LOG_FMT( info, "Config file loaded from string: `{}`", jsonSourceAsString );
    }
    else
    {
        jsonLoader = std::make_shared<JsonLoader>();
        std::filesystem::path fullPath = std::filesystem::absolute( "config\\config.json" );
        if ( !jsonLoader->loadFromFile( fullPath.string() ) )
            throw std::runtime_error( "Failed to load config file" );
        MY_LOG_FMT( info, "Config file loaded: {}", fullPath.string() );
    }

    return jsonLoader;
}

} // namespace utils