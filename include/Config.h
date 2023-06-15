#pragma once

#include <string>
#include <map>

#define CFG_PREFIX "CFG_"
#define CFG_MODEL_PATH "CFG_MODEL_PATH"
#define CFG_LABEL_PATH "CFG_LABEL_PATH"
#define CFG_THRESHOLD "CFG_THRESHOLD"
#define CFG_CAMERA_SOURCE "CFG_CAMERA_SOURCE"

class Config
{

public:
    static Config* GetInstance();
    ~Config();

    std::string GetConfigParam(std::string key);

private:
    Config(/* args */);

    std::map<std::string, std::string> config;
};