#include "AsioIOServicePool.h"
#include<iostream>
using namespace std;
AsioIOServicePool::AsioIOServicePool(size_t size) :_ioServices(size),_works(size) ,_threads(size){
	/*Ϊÿ�� io_context ���󴴽�һ�� executor_work_guard��
		�������װ��һ�� unique_ptr<Work> �У��洢�� _works �����У�
		��ȷ�� io_context ���첽���������֮ǰ���ֻ�Ծ״̬��*/
	for (size_t i = 0; i < _ioServices.size(); i++) {
		_works[i] = unique_ptr<Work>(new Work(make_work_guard(_ioServices[i].get_executor())));
	}
	//����ÿһ���̶߳���,���ڲ�����
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