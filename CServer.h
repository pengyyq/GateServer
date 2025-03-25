#pragma once
#include"const.h"
#include"HttpConnection.h"

class CServer:public std::enable_shared_from_this<CServer>
{
public:
	CServer(boost::asio::io_context& ioc, unsigned short& port);
	void Start();
	

private:
	tcp::acceptor m_acceptor;
	net::io_context& m_ioc;
	tcp::socket m_socket;

};

