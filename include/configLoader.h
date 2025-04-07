#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <string>
#include <map>
#include <fstream>

class ConfigLoader {
public:
    ConfigLoader(const std::string& configFile = "../configuration/config.txt");
    bool loadConfig();
    std::string getValue(const std::string& key) const;
    std::string getCameraUrl() const;

private:
    std::string m_configFile;
    std::map<std::string, std::string> m_config;
};

#endif // CONFIG_LOADER_H 