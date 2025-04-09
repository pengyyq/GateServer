#include "LogicSystem.h"
#include"HttpConnection.h"
LogicSystem::LogicSystem() {
	//ʵ�־����·�ɺͻص�����
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

//�ذ�������,����·�ɺͻص�����Ϊ����
void LogicSystem::RegGet(std::string url ,HttpHandler handler) {
	m_GetHandlers.insert(make_pair(url, handler));

}


