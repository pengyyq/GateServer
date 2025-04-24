#pragma once
#include <map>
#include <string>
#include "SectionInfo.h" 
#include"const.h"  // 
class ConfigMgr
{
public:
    ~ConfigMgr() {
        _config_map.clear();
    }
    SectionInfo operator[](const std::string& section) {
        if (_config_map.find(section) == _config_map.end()) {
            return SectionInfo();
        }
        return _config_map[section];
    }
    static ConfigMgr& Inst() {
        static ConfigMgr instance;
        return instance;
    }

    ConfigMgr& operator=(const ConfigMgr& src) {
        if (&src == this) {
            return *this;
        }

        this->_config_map = src._config_map;
    };

    ConfigMgr(const ConfigMgr& src) {
        this->_config_map = src._config_map;
    }

private:
    ConfigMgr();
    // 存储session名称和session键值数据
    std::map<std::string, SectionInfo> _config_map;
};