#include "ConfigManager.h"

ConfigManager* ConfigManager::instance = nullptr;

ConfigManager* ConfigManager::getInstance() {
    if (!instance)
        instance = new ConfigManager();
    return instance;
}

std::string ConfigManager::getConfig(const std::string& key) {
    return settings[key];
}

void ConfigManager::setConfig(const std::string& key, const std::string& value) {
    settings[key] = value;
}
