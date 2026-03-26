#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <ctime>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};


class Logger {
    std::ofstream log_file_;
    std::mutex mutex_;

    std::string get_current_time() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        char buffer[64];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));
        return std::string(buffer);
    }

    std::string level_to_string(LogLevel level) {
        switch (level)
        {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
        }
    }

public:
    Logger(const std::string filename) {
        log_file_.open(filename, std::ios::app);
        if (!log_file_.is_open()) {
            throw std::runtime_error("Could not open log file!");
        }
    }

    ~Logger() {
        if (log_file_.is_open()) {
            log_file_.close();
        }
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void log(LogLevel level, const std::string& message) {

        // format message: [2026-03-25 17:15:22] [INFO] This is a message
        std::string log_entry = "[" + get_current_time() + "] [" + level_to_string(level) + "] " + message + "\n";
        log(log_entry);
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << message;

        log_file_ << message;
        log_file_.flush();
    }
};

#endif