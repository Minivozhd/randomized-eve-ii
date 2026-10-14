#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>

class PE2Logger {
  public:
    static std::shared_ptr<PE2Logger> getInstance(const std::string &filename) {
        if (std::filesystem::exists(filename)) {
            std::filesystem::remove(filename);
        }
        static std::shared_ptr<PE2Logger> instance{new PE2Logger{filename}};
        return instance;
    }

    template <typename... Args> void log(Args... args) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::ostringstream stream;
        logHelper(stream, args...);
        stream << std::endl;
        logfile << stream.str();
        logfile.flush();
    }

    ~PE2Logger() {
        logfile.close();
    }

  private:
    explicit PE2Logger(const std::string &filename) : logfile(filename, std::ios_base::app) {
        if (!logfile.is_open()) {
            throw std::runtime_error("PE2Logger: Unable to open log file");
        }
    }

    template <typename T, typename... Args> void logHelper(std::ostringstream &stream, T t, Args... args) {
        stream << t;
        stream.flush();
        logHelper(stream, args...);
    }

    void logHelper(std::ostringstream &stream) {
        stream << "";
    }

    std::ofstream logfile;
    std::mutex log_mutex;
};
