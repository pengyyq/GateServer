#include "MysqlMgr.h"


MysqlMgr::~MysqlMgr() {

}

int MysqlMgr::RegUser(const std::string& name, const std::string& email, const std::string& pwd)
{
    return _dao.RegUser(name, email, pwd);
}

MysqlMgr::MysqlMgr() {

}

bool MysqlMgr::CheckEmail(const std::string& name, const std::string& email) {
    return _dao.CheckEmail(name, email);
}

bool MysqlMgr::UpdataPwd(const std::string& name, const std::string& pwd) {
    return _dao.UpdataPwd(name, pwd);
}

bool MysqlMgr::CheckPwd(const std::string &name, const std::string& pwd, UserInfo& userInfo){
    return _dao.CheckPwd(name,pwd, userInfo);
}

