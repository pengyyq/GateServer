#ifndef SECTIONINFO_H
#define SECTIONINFO_H

#include "const.h"
#include <map>
#include <string>

/**
 * @brief SectionInfo 结构体，用于存储和管理键值对数据。
 */
struct SectionInfo {
    /**
     * @brief 默认构造函数。
     */
    SectionInfo() {}

    /**
     * @brief 析构函数，清空 _section_datas 数据。
     */
    ~SectionInfo() {
        _section_datas.clear();
    }

    /**
     * @brief 拷贝构造函数，深拷贝 _section_datas 数据。
     * @param src 源 SectionInfo 对象。
     */
    SectionInfo(const SectionInfo& src) {
        _section_datas = src._section_datas;
    }

    /**
     * @brief 赋值运算符重载，深拷贝 _section_datas 数据。
     * @param src 源 SectionInfo 对象。
     * @return 当前对象的引用。
     */
    SectionInfo& operator=(const SectionInfo& src) {
        if (&src == this) {
            return *this;
        }

        this->_section_datas = src._section_datas;
        return *this; // 返回当前对象的引用
    }

    /**
     * @brief 存储键值对数据的 map 容器。
     */
    std::map<std::string, std::string> _section_datas;

    /**
     * @brief 重载 [] 运算符，通过键获取值。
     * @param key 键值。
     * @return 如果键存在，返回对应的值；否则返回空字符串。
     */
    std::string operator[](const std::string& key) {
        if (_section_datas.find(key) == _section_datas.end()) {
            return ""; // 键不存在时返回空字符串
        }
        return _section_datas[key]; // 返回键对应的值
    }
};

#endif // SECTIONINFO_H