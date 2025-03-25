#pragma once
#include"const.h"
class HttpConnection;
typedef std::function<void(std::shared_ptr<HttpConnection>)>HttpHandler;

class LogicSystem :public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	bool HandleGet(std::string, std::shared_ptr<HttpConnection>);
	void RegGet(std::string, HttpHandler handler);
	void RegPost(std::string, HttpHandler handler);
	bool HandlePost(std::string, std::shared_ptr<HttpConnection>);
private:
	LogicSystem();
	//post和get的回调函数map,key为url,value为回调函数
	std::map<std::string,HttpHandler> m_GetHandlers;
	std::map<std::string, HttpHandler> m_PostHandlers;
};


