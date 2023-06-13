#include <iostream>
#include <fstream> 
#include "Config.h"


/**
 * @brief Get a singleton instance of Config
 * 
 * @return Config* 
 */
Config* Config::GetInstance()
{
    static Config* instance;
    if (!instance)
    {
        instance = new Config;
    }

    return instance;
}

/**
 * @brief Construct a new Config:: Config object
 * 
 */
Config::Config()
{
    std::string configPath("./config.txt");
    std::ifstream ifs(configPath.c_str(), std::ifstream::in);

    if (ifs.is_open())
    {
        std::string line;

        while (std::getline(ifs, line))
        {
            if (line.find(CFG_PREFIX) == 0)
            {
                int pos = line.find('=');
                std::string key(line.substr(0, pos));
                std::string value(line.substr(pos + 1));
                this->config.insert(std::pair<std::string, std::string>(key, value));
            }
        }

        ifs.close();
        
    }
    else
    {
        std::cout << "Cannot open config file" << std::endl;
        exit(1);
    }
}

/**
 * @brief Destroy the Config:: Config object
 * 
 */
Config::~Config()
{
    this->config.clear();
}


std::string Config::GetConfigParam(std::string key)
{
    std::map<std::string, std::string>::iterator it = this->config.find(key);

    if (it != this->config.end())
    {
        return it->second;
    }
    else
    {
        return "";
    }
}