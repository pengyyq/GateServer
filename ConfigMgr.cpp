#include "ConfigMgr.h"
#include <boost/filesystem.hpp>
#include<iostream>
#include"boost/property_tree/ini_parser.hpp"
#include"boost/property_tree/ptree.hpp"
ConfigMgr::ConfigMgr() {
	//��ȡ����Ŀ¼
	boost::filesystem::path current_path = boost::filesystem::current_path();
	//����config.ini·��
	boost::filesystem::path config_path = current_path / "config.ini";
	std::cout<<"config_path:"<<config_path<<std::endl;

	//ʹ��boost.propertytree����ȡini�ļ�
	boost::property_tree::ptree pt;
	boost::property_tree::read_ini(config_path.string(), pt);


	//����ini�ļ������е�session
	for (const auto& session_pair : pt) {
		//��ȡsection��
		const std::string& section_name = session_pair.first;
		//��ȡsection��key-value��
		const boost::property_tree::ptree& section_tree = session_pair.second;

		//����ÿ��section�������е�key-value��
		std::map < std::string, std::string>section_config;
		for (const auto& key_value_pair : section_tree) {
			const std::string& key = key_value_pair.first;
			const std::string& value = key_value_pair.second.get_value<std::string>();
			section_config[key] = value;
		}

		SectionInfo sectionInfo;
		sectionInfo._section_datas = section_config;
		//��section��Ϣ���뵽config_map��
		_config_map[section_name] = sectionInfo;

		// ������е�section��key-value��  
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