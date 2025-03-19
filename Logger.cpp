#include "Logger.h"

Logger* Logger::instance = nullptr;

Logger* Logger::getInstance() {
    if (!instance)
        instance = new Logger();
    return instance;
}

void Logger::log(const std::string& message) {
    logs.push_back(message);
    std::cout << "[LOG]: " << message << std::endl;
}
