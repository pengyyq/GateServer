#include "LogicSystem.h"
#include"HttpConnection.h"
LogicSystem::LogicSystem() {
	//实现具体的路由和回调函数
	RegGet("/test", [](std::shared_ptr<HttpConnection> connection) {
		beast::ostream(connection->m_response.body()) << "receive get_test req";
	});

}

bool LogicSystem::HandleGet(std::string url, std::shared_ptr<HttpConnection>con)
{
	if (m_GetHandlers.find(url) == m_GetHandlers.end()) {
		return false;
	}
	m_GetHandlers[url](con);
	return true;
}

//回包处理函数,接收路由和回调函数为参数
void LogicSystem::RegGet(std::string url ,HttpHandler handler) {
	m_GetHandlers.insert(make_pair(url, handler));

}


