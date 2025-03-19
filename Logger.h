#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>
#include <iostream>

class Logger {
private:
    static Logger* instance;
    std::vector<std::string> logs;
    Logger() {}
public:
    static Logger* getInstance();
    void log(const std::string& message);
};

#endif // LOGGER_H
