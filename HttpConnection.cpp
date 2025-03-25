#include "HttpConnection.h"
#include <iostream>
//socket无法拷贝，所以不能用默认构造，要用std::move()移动构造
HttpConnection::HttpConnection(tcp::socket socket):m_socket(std::move(socket))
{

}

void HttpConnection::Start()
{
	auto self = shared_from_this();
	http::async_read(m_socket, m_buffer, m_request, [self](const boost::system::error_code ec,
		std::size_t bytes_transferred) {
			try {
				if (ec) {
					std::cout<<"Error: "<<ec.message()<<std::endl;
					return;
				}
				//无需做粘包处理
				//处理读到的数据
				boost::ignore_unused(bytes_transferred);
				self->HandleReq();
				self->CheckDeadline();
			}
			catch (const std::exception& exc) {
				std::cerr << "Exception: " << exc.what() << "\n";
			}
		});
}

void HttpConnection::CheckDeadline()
{
	auto self = shared_from_this();
	m_deadline.async_wait([self](const boost::system::error_code &ec) {
		if (!ec) {
			self->m_socket.close();
		}
	});
}

void HttpConnection::WriteResponse()
{
	auto self = shared_from_this();
	m_response.content_length(m_response.body().size());
	http::async_write(m_socket, m_response, [self](beast::error_code ec, 
			std::size_t bytes_transferred) {
			self->m_socket.shutdown(tcp::socket::shutdown_send,ec);
			self->m_deadline.cancel();
			});
}

void HttpConnection::HandleReq()
{
	//设置版本
	m_response.version(m_request.version());
	m_response.keep_alive(false);
	if (m_request.method() == http::verb::get) {
		//处理get请求
		bool success = LogicSystem::Getinstance()->HandleGet(m_request.target(),shared_from_this());
		if (!success) {
			m_response.result(http::status::not_found);//404
			m_response.set(http::field::content_type, "text/plain");
			beast::ostream(m_response.body()) << "url not found\r\n";
			WriteResponse();
			return;
		}
		m_response.result(http::status::ok);//404
		m_response.set(http::field::server,"GateServer");
		WriteResponse();
		return;
	}
}


