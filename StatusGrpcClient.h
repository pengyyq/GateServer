#pragma once
#include"const.h"
#include"Singleton.h"
#include"ConfigMgr.h"
#include <grpcpp/impl/codegen/client_callback.h>
#include "message.pb.h"
#include "message.grpc.pb.h"
#include <grpcpp/security/credentials.h>
#include <grpcpp/create_channel.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::StatusService;

//����һ��StatusPool
class StatusConPool {

public:
	StatusConPool(size_t poolSize, std::string host, std::string port) : poolSize_(poolSize), host_(host), port_(port), b_stop_(false) {
		for (size_t i = 0; i < poolSize_; i++){
			std::shared_ptr<Channel>channel = grpc::CreateChannel(host_ + ":" + port_, grpc::InsecureChannelCredentials());
			connections_.push(StatusService::NewStub(channel));

		}
	}

	~StatusConPool() {
		std::lock_guard<std::mutex>lock(mutex_);
		Close();
		while (!connections_.empty()) {
            connections_.pop();
		}
	}

	void Close() {
		b_stop_ = true;
		cond_.notify_all();
	}

	std::unique_ptr<StatusService::Stub>getConnection() {
		std::unique_lock<std::mutex>lock(mutex_);
		cond_.wait(lock, [this] {
			if (b_stop_) {
				return true;
			}
			return !connections_.empty();

			});
		if (b_stop_) {
			return nullptr;
		}
		auto context =  std::move(connections_.front());
        connections_.pop();
		return context;

	}

	void returnConnection(std::unique_ptr<StatusService::Stub>context) {
		std::unique_lock<std::mutex>lock(mutex_);
		if (b_stop_) {
			return ;
		}
		connections_.push(std::move(context));
		cond_.notify_one();
	}
		
private:
	std::atomic<bool>b_stop_;
	std::mutex mutex_;
	std::queue<std::unique_ptr<StatusService::Stub>>connections_;
	std::condition_variable cond_;
	std::string host_;
	std::string port_;
	size_t poolSize_;
	

};

class StatusGrpcClient :public Singleton<StatusGrpcClient> {
public:
	friend class Singleton<StatusGrpcClient>;
	~StatusGrpcClient() = default;
	GetChatServerRsp GetChatServer(int uid);

private:
	StatusGrpcClient();
	std::unique_ptr<StatusConPool> pool_;

};
