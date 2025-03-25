#include "LogicSystem.h"
#include"HttpConnection.h"
LogicSystem::LogicSystem() {
	RegGet("/test", [](std::shared_ptr<HttpConnection> connection) {
		beast::ostream(connection->m_response.body())<<"receive get_test req"
	});

}

void LogicSystem::RegGet(std::string url ,HttpHandler handler) {
	m_GetHandlers[url] = handler;
}


