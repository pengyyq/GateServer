#pragma once
#include"const.h"
#include"LogicSystem.h"
class HttpConnection:public std::enable_shared_from_this<HttpConnection>
{
public:
	HttpConnection(tcp::socket socket);
	void Start();

	//超时检测
	void CheckDeadline();
	//
	void WriteResponse();

	//处理请求
	void HandleReq();

private:
	tcp::socket m_socket;
	beast::flat_buffer m_buffer{ 8192 };
	http::request<http::dynamic_body> m_request;
	http::response<http::dynamic_body>m_response;

	net::steady_timer m_deadline{
		m_socket.get_executor(),
		std::chrono::seconds(30)
	};


};

