#pragma once
#include<vector>
#include<boost/asio.hpp>
#include"Singleton.h"
//实现一个线程池，管理多个线程，每个线程都有自己的io_service对象，可以同时处理多个连接
class AsioIOServicePool :public Singleton<AsioIOServicePool>
{
	friend Singleton<AsioIOServicePool>;
public:
	using IOService = boost::asio::io_context;
	using Work = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
	using WorkPtr = std::unique_ptr<Work>;

	~AsioIOServicePool() {
		Stop();
		std::cout<<"AsioIOServicePool is destroyed"<<std::endl;
	}
	//禁用拷贝构造函数和赋值运算符
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;
	//返回一个io_context对象，用来处理连接
	boost::asio::io_context& GetIOService();

	void Stop();

private:
	AsioIOServicePool(std::size_t size = 2);
	std::vector<IOService>_ioServices;
	std::vector<WorkPtr>_works;
	std::vector<std::thread> _threads;
	std::size_t _nextIOService = 0;
};


