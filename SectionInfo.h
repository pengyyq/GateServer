#ifndef SECTIONINFO_H
#define SECTIONINFO_H

#include "const.h"
#include <map>
#include <string>

/**
 * @brief SectionInfo �ṹ�壬���ڴ洢�͹����ֵ�����ݡ�
 */
struct SectionInfo {
    /**
     * @brief Ĭ�Ϲ��캯����
     */
    SectionInfo() {}

    /**
     * @brief ������������� _section_datas ���ݡ�
     */
    ~SectionInfo() {
        _section_datas.clear();
    }

    /**
     * @brief �������캯������� _section_datas ���ݡ�
     * @param src Դ SectionInfo ����
     */
    SectionInfo(const SectionInfo& src) {
        _section_datas = src._section_datas;
    }

    /**
     * @brief ��ֵ��������أ���� _section_datas ���ݡ�
     * @param src Դ SectionInfo ����
     * @return ��ǰ��������á�
     */
    SectionInfo& operator=(const SectionInfo& src) {
        if (&src == this) {
            return *this;
        }

        this->_section_datas = src._section_datas;
        return *this; // ���ص�ǰ���������
    }

    /**
     * @brief �洢��ֵ�����ݵ� map ������
     */
    std::map<std::string, std::string> _section_datas;

    /**
     * @brief ���� [] �������ͨ������ȡֵ��
     * @param key ��ֵ��
     * @return ��������ڣ����ض�Ӧ��ֵ�����򷵻ؿ��ַ�����
     */
    std::string operator[](const std::string& key) {
        if (_section_datas.find(key) == _section_datas.end()) {
            return ""; // ��������ʱ���ؿ��ַ���
        }
        return _section_datas[key]; // ���ؼ���Ӧ��ֵ
    }
};

#endif // SECTIONINFO_H