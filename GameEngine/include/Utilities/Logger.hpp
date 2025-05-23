#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "Common.hpp"


/// @brief A Logger class for logging messages at various levels of severity.
/// 
/// This class provides static methods to log messages using the spdlog library. 
/// It supports different log levels, including trace, info, warn, error, and critical. 
/// Additionally, the class allows for conditional debug logging in debug builds.
class Logger
{
public:
    /// @brief Initializes the logger.
    /// 
    /// This method sets up the logger instance and prepares it for use.
    static void Init();

    /// @brief Retrieves the logger instance.
    /// 
    /// This method returns a shared pointer to the logger instance, which can be used to log messages.
    /// @return A shared pointer to the logger instance.
    static std::shared_ptr<spdlog::logger> GetLogger() { return sLogger; }

private:
    /// @brief The shared pointer to the logger instance.
    static std::shared_ptr<spdlog::logger> sLogger;
};


template<typename T>
concept _IsPointer = std::is_pointer<T>::value && !(std::is_same<T, char*>::value || std::is_same<T, const char*>::value);

/**
 * \brief Helper function to automatically cast pointers to void*
 */
template<typename T>
    requires(_IsPointer<T>)
constexpr const void* log_pointer_cast(T ptr) {
    return static_cast<void*>(ptr);
}

/**
 * \brief Helper function to automatically cast pointers to void*
 */
template<typename T>
    requires(!_IsPointer<T>)
constexpr const T log_pointer_cast(T& value) {
    return std::forward<T>(value);
}

/**
 * \brief Helper function to automatically cast pointers to void*
 */
template<typename T>
    requires(!_IsPointer<T>)
constexpr const T log_pointer_cast(T&& value) {
    return std::forward<T>(value);
}

#define PROCESS_ARG(arg) log_pointer_cast<decltype(arg)>(arg)


/// @brief Macro for logging trace-level messages.
/// 
/// Logs a message with the "trace" log level using the Logger class.
/// @param ... The message to log.
#define LOG_TRACE(message, ...)    ::Logger::GetLogger()->trace(message __VA_OPT__(, ARGS_PROCESSING(PROCESS_ARG, __VA_ARGS__)))

/// @brief Macro for logging info-level messages.
/// 
/// Logs a message with the "info" log level using the Logger class.
/// @param ... The message to log.
#define LOG_INFO(message, ...)     ::Logger::GetLogger()->info(message __VA_OPT__(, ARGS_PROCESSING(PROCESS_ARG, __VA_ARGS__)))

/// @brief Macro for logging warn-level messages.
/// 
/// Logs a message with the "warn" log level using the Logger class.
/// @param ... The message to log.
#define LOG_WARN(message, ...)     ::Logger::GetLogger()->warn(message __VA_OPT__(, ARGS_PROCESSING(PROCESS_ARG, __VA_ARGS__)))

/// @brief Macro for logging error-level messages.
/// 
/// Logs a message with the "error" log level using the Logger class.
/// @param ... The message to log.
#define LOG_ERROR(message, ...)    ::Logger::GetLogger()->error(message __VA_OPT__(, ARGS_PROCESSING(PROCESS_ARG, __VA_ARGS__)))

/// @brief Macro for logging critical-level messages.
/// 
/// Logs a message with the "critical" log level using the Logger class.
/// @param ... The message to log.
#define LOG_CRITICAL(message, ...) ::Logger::GetLogger()->critical(message __VA_OPT__(, ARGS_PROCESSING(PROCESS_ARG, __VA_ARGS__)))

/// @brief Macro for logging debug-level messages (enabled in debug builds only).
/// 
/// Logs a message with the "debug" log level using the Logger class. This is only available
/// in debug builds, and the macro will be empty in release builds.
/// @param ... The message to log.
#ifdef DEBUG
#define LOG_DEBUG(message, ...)    ::Logger::GetLogger()->debug(message __VA_OPT__(, ARGS_PROCESSING(PROCESS_ARG, __VA_ARGS__)))
#else
#define LOG_DEBUG(...)
#endif