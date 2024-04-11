#pragma once
#include <memory>
#include <my_cpp_utils/json_formatter.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace utils
{

class Logger
{
private:
    inline static std::shared_ptr<spdlog::logger>& GetSingletoneInstance()
    {
        static std::shared_ptr<spdlog::logger> logger;
        return logger;
    }
public:
    inline static void Init(
        const std::filesystem::path& logFilePath, spdlog::level::level_enum level = spdlog::level::level_enum::info)
    {
        auto& logger = GetSingletoneInstance();

        if (logger)
            throw std::runtime_error("Logger is already initialized");

        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::stderr_color_sink_mt>());
        sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(logFilePath.string(), 23, 59));
        logger = std::make_shared<spdlog::logger>("global", begin(sinks), end(sinks));
        logger->set_level(level);

        // Set the pattern
        // [%Y-%m-%d %H:%M:%S.%e] is the timestamp
        // [%l] is the log level
        // [%t] is the thread ID
        // %v is the actual log message
        logger->set_pattern("[%H:%M:%S.%e] [%l] [%t] %v");
    }

    inline static spdlog::logger& GetInstance()
    {
        auto& logger = GetSingletoneInstance();
        if (!logger)
            throw std::runtime_error("Logger is not initialized. Use Logger::init()");

        return *logger;
    }
};

} // namespace utils

#define MY_LOG(severity, fmt_string, ...) \
    utils::Logger::GetInstance().severity(fmt::format(fmt_string __VA_OPT__(, ) __VA_ARGS__))

#define MY_FMT(fmt_string, ...) fmt::format(fmt_string, __VA_ARGS__)