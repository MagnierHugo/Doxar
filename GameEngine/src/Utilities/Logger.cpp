#include <Utilities/Logger.hpp>

#include <vector>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

std::shared_ptr<spdlog::logger> Logger::sLogger;

void Logger::Init()
{
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("mnemen.log", true);

    consoleSink->set_pattern("%^[%c] [%l] %v%$");
    fileSink->set_pattern("[%c] [%l] %v");

    sLogger = std::make_shared<spdlog::logger>("MNEMEN", spdlog::sinks_init_list{ consoleSink, fileSink });
    spdlog::register_logger(sLogger);
    sLogger->set_level(spdlog::level::trace);
    sLogger->flush_on(spdlog::level::trace);
}