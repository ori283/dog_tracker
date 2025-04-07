#include "../include/configLoader.h"
#include <iostream>
#include <sstream>

ConfigLoader::ConfigLoader(const std::string& configFile)
    : m_configFile(configFile) {}

bool ConfigLoader::loadConfig() {
    std::ifstream file(m_configFile);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << m_configFile << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            // Remove any whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            m_config[key] = value;
        }
    }

    // Verify required fields
    if (m_config.find("IP") == m_config.end() ||
        m_config.find("USERNAME") == m_config.end() ||
        m_config.find("PASSWORD") == m_config.end() ||
        m_config.find("PORT") == m_config.end() ||
        m_config.find("STREAM_PATH") == m_config.end()) {
        std::cerr << "Missing required configuration fields" << std::endl;
        return false;
    }

    return true;
}

std::string ConfigLoader::getValue(const std::string& key) const {
    auto it = m_config.find(key);
    if (it != m_config.end()) {
        return it->second;
    }
    return "";
}

std::string ConfigLoader::getCameraUrl() const {
    std::stringstream ss;
    ss << "rtsp://" << getValue("USERNAME") << ":" << getValue("PASSWORD")
       << "@" << getValue("IP") << ":" << getValue("PORT") << "/" << getValue("STREAM_PATH");
    return ss.str();
} 