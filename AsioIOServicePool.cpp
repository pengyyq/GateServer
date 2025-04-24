#include "AsioIOServicePool.h"
#include<iostream>
using namespace std;
AsioIOServicePool::AsioIOServicePool(size_t size) :_ioServices(size),_works(size) ,_threads(size){
	/*为每个 io_context 对象创建一个 executor_work_guard，
		并将其封装在一个 unique_ptr<Work> 中，存储在 _works 容器中，
		以确保 io_context 在异步任务处理完毕之前保持活跃状态。*/
	for (size_t i = 0; i < _ioServices.size(); i++) {
		_works[i] = unique_ptr<Work>(new Work(make_work_guard(_ioServices[i].get_executor())));
	}
	//遍历每一个线程对象,在内部启动
	for (size_t i = 0; i < _ioServices.size(); i++) {
		_threads.emplace_back([this, i] {
			_ioServices[i].run();
			});
	}

	
}

void AsioIOServicePool::Stop() {
	for (auto& work : _works) {
		auto executor = work->get_executor();
		auto& ioContext = boost::asio::query(executor, boost::asio::execution::context);
		ioContext.stop();
		work.reset();
	}
	for (auto& thread : _threads) {
		thread.join();
	}

}


boost::asio::io_context& AsioIOServicePool::GetIOService() {
	auto& service = _ioServices[_nextIOService++];
		if (_nextIOService == _ioServices.size()) {
			_nextIOService = 0;
		}
		return service;

}