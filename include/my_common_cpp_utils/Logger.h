#pragma once
#include "JsonFormatter.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>
#include <type_traits>

namespace utils
{

class Logger
{
public:
    static std::shared_ptr<spdlog::logger>& getInstance()
    {
        static std::shared_ptr<spdlog::logger> combined_logger;

        if ( !combined_logger )
        {
            std::vector<spdlog::sink_ptr> sinks;
            sinks.push_back( std::make_shared<spdlog::sinks::stderr_color_sink_mt>() );
            sinks.push_back( std::make_shared<spdlog::sinks::daily_file_sink_mt>( "game.log", 23, 59 ) );
            combined_logger = std::make_shared<spdlog::logger>( "game", begin( sinks ), end( sinks ) );
        }

        return combined_logger;
    }
};

#define MY_LOG( severity, message ) Logger::getInstance()->severity( message )

#define MY_LOG_FMT( severity, fmt_string, ... ) \
    Logger::getInstance()->severity( fmt::format( fmt_string, __VA_ARGS__ ) )

#define MY_LOG_VAR( severity, var ) MY_LOG_FMT( severity, "{} = {}", #var, ( var ) )

#define MY_FMT( fmt_string, ... ) fmt::format( fmt_string, __VA_ARGS__ )

} // namespace utils