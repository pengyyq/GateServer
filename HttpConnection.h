#pragma once
#include"const.h"
#include"LogicSystem.h"
class HttpConnection:public std::enable_shared_from_this<HttpConnection>
{
	friend class LogicSystem;
public:
	HttpConnection(tcp::socket socket);
	void Start();

	//��ʱ���
	void CheckDeadline();
	//
	void WriteResponse();

	//��������
	void HandleReq();

private:
	tcp::socket m_socket;//用于读写的套接字
	beast::flat_buffer m_buffer{ 8192 };//用于读写的缓冲区
	http::request<http::dynamic_body> m_request;//请求对象，用于解析请求数据
	http::response<http::dynamic_body>m_response;//响应对象，回应客户端

	//实现get参数解析
	std::string m_get_url;//GET请求的url
	std::unordered_map<std::string, std::string>m_get_param;//GET请求的参数

	//实现post参数解析
	std::string m_post_url;//POST请求的url
	std::unordered_map<std::string, std::string>m_post_param;//POST请求的参数

	//超时定时器
	net::steady_timer m_deadline{
		m_socket.get_executor(),
		std::chrono::seconds(30)
	};

};

