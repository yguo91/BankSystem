#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <string>
#include <map>

class ConfigManager {
private:
    static ConfigManager* instance;
    std::map<std::string, std::string> settings;
    ConfigManager() {}
public:
    static ConfigManager* getInstance();
    std::string getConfig(const std::string& key);
    void setConfig(const std::string& key, const std::string& value);
};

#endif // CONFIGMANAGER_H
