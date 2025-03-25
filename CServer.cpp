#include "CServer.h"

#include<iostream>
CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):m_ioc(ioc),m_acceptor(ioc,tcp::endpoint(tcp::v4(),port)),m_socket(ioc)
{
	
}

//启动服务器,监听连接
void CServer::Start()
{
	auto self = shared_from_this();
	m_acceptor.async_accept(m_socket, [self](beast::error_code ec) {
		try {
			//出错
			if (ec) {
				//出错则重连
				self->Start();
				return;
			}
			//创建新连接并创建HttpConnect类管理连接
			//处理的是已经建立的连接
			std::make_shared<HttpConnection>(std::move(self->m_socket))->Start();
			//继续接收新连接
			self->Start();
		}
		catch (std::exception& exp) {
			std::cerr << "Exception: " << exp.what() << std::endl;
		}
		});
}
