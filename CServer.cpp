#include "CServer.h"

#include<iostream>
CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):m_ioc(ioc),m_acceptor(ioc,tcp::endpoint(tcp::v4(),port)),m_socket(ioc)
{
	
}

//����������,��������
void CServer::Start()
{
	auto self = shared_from_this();
	m_acceptor.async_accept(m_socket, [self](beast::error_code ec) {
		try {
			//����
			if (ec) {
				//����������
				self->Start();
				return;
			}
			//���������Ӳ�����HttpConnect���������
			//��������Ѿ�����������
			std::make_shared<HttpConnection>(std::move(self->m_socket))->Start();
			//��������������
			self->Start();
		}
		catch (std::exception& exp) {
			std::cerr << "Exception: " << exp.what() << std::endl;
		}
		});
}
