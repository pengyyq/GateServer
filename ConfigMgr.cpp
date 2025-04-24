#include "ConfigMgr.h"
#include <boost/filesystem.hpp>
#include<iostream>
#include"boost/property_tree/ini_parser.hpp"
#include"boost/property_tree/ptree.hpp"
ConfigMgr::ConfigMgr() {
	//获取工作目录
	boost::filesystem::path current_path = boost::filesystem::current_path();
	//构建config.ini路径
	boost::filesystem::path config_path = current_path / "config.ini";
	std::cout<<"config_path:"<<config_path<<std::endl;

	//使用boost.propertytree来读取ini文件
	boost::property_tree::ptree pt;
	boost::property_tree::read_ini(config_path.string(), pt);


	//遍历ini文件中所有的session
	for (const auto& session_pair : pt) {
		//获取section名
		const std::string& section_name = session_pair.first;
		//获取section的key-value对
		const boost::property_tree::ptree& section_tree = session_pair.second;

		//对于每个section遍历所有的key-value对
		std::map < std::string, std::string>section_config;
		for (const auto& key_value_pair : section_tree) {
			const std::string& key = key_value_pair.first;
			const std::string& value = key_value_pair.second.get_value<std::string>();
			section_config[key] = value;
		}

		SectionInfo sectionInfo;
		sectionInfo._section_datas = section_config;
		//将section信息加入到config_map中
		_config_map[section_name] = sectionInfo;

		// 输出所有的section和key-value对  
		for (const auto& section_entry : _config_map) {
			const std::string& section_name = section_entry.first;
			SectionInfo section_config = section_entry.second;
			std::cout << "[" << section_name << "]" << std::endl;
			for (const auto& key_value_pair : section_config._section_datas) {
				std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
			}
		}

	}

}