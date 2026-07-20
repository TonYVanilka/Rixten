#pragma once

#include <cstddef>
#include <iostream>

enum LogLevel {
    FATAL,
    ERROR,
    WARN,
    INFO,
    DEBUG
};

namespace Logger {

    inline LogLevel logLevel = LogLevel::DEBUG;

    inline const char* LogTypeToString(LogLevel type) {
        switch (type) {
            case LogLevel::FATAL: return "\033[31m\033[1m[FATAL]\033[0m"; // bold red
            case LogLevel::ERROR: return "\033[31m[ERROR]\033[0m"; // red
            case LogLevel::WARN: return "\033[33m[WARN]\033[0m"; // yellow
            case LogLevel::INFO: return "\033[36m[INFO]\033[0m"; // cyan
            case LogLevel::DEBUG: return "\033[34m[DEBUG]\033[0m"; // blue
            default:
                return "UNKNOWN";
        }
    }

    template <typename... Args>
    inline void Log(LogLevel lvl, Args&& ... args) {
        
        if (lvl > logLevel) return;

        std::cout << LogTypeToString(lvl) << ' ';
        (std::cout << ... << args) << std::endl;
    }

};

#ifdef DISABLE_LOGGING
    #define LOG_DEBUG(...) ((void)0)
    #define LOG_INFO(...) ((void)0)
    #define LOG_WARN(...) ((void)0)
    #define LOG_ERROR(...) ((void)0)
    #define LOG_FATAL(...) ((void)0)
#else 
    #define LOG_DEBUG(...) Logger::Log(LogLevel::DEBUG, __VA_ARGS__);
    #define LOG_INFO(...) Logger::Log(LogLevel::INFO, __VA_ARGS__);
    #define LOG_WARN(...) Logger::Log(LogLevel::WARN, __VA_ARGS__);
    #define LOG_ERROR(...) Logger::Log(LogLevel::ERROR, __VA_ARGS__);
    #define LOG_FATAL(...) Logger::Log(LogLevel::FATAL, __VA_ARGS__);
#endif  // DISABLE_LOGGING