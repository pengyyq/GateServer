#pragma once
#include <grpcpp/grpcpp.h>
#include"message.grpc.pb.h"
#include"const.h"
#include"Singleton.h"
#include"memory"
using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class RPConPool {
public:
    RPConPool(size_t poolsize,std::string host,std::string port) :b_stop_(false),poolsize_(poolsize),
        host_(host),port_(port) {
        for (size_t i = 0; i < poolsize_; i++) {
            std::shared_ptr<Channel> channel = grpc::CreateChannel(host+":"+port, grpc::InsecureChannelCredentials());
            //unique_ptr不支持拷贝构造
            connections_.push(VarifyService::NewStub(channel));
        }
    }
    ~RPConPool() {
        std::lock_guard<std::mutex> lock(mutex_);
        Close();
        while (!connections_.empty()) {
            connections_.pop();
        }
        
    }

    void Close() {
        b_stop_ = true;
        cv_.notify_all();
    }

    std::unique_ptr<VarifyService::Stub> GetConnection() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] {
            if (b_stop_) {
                return true;
            }
            return!connections_.empty();//返回false则挂起，释放锁
            });

        if (b_stop_) {
            return nullptr;
        }
        //
        auto context = std::move(connections_.front());
        connections_.pop();
        return context;
    }

    //当连接池中没有可用连接时，将使用完的连接放回连接池，并通知等待线程
    void returnConnection(std::unique_ptr<VarifyService::Stub> context) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (b_stop_) {
            return;
        }
        connections_.push(std::move(context));
        cv_.notify_one();
    }

private:
    std::atomic<bool>b_stop_;
    size_t poolsize_;
    std::string host_;
    std::string port_;
    std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

class VerifyGrpcClient :public Singleton<VerifyGrpcClient>
{
    friend class Singleton<VerifyGrpcClient>;
public:

    GetVarifyRsp GetVarifyCode(std::string email) {
        ClientContext context;
        GetVarifyRsp reply;
        GetVarifyReq request;
        request.set_email(email);
        auto stub = pool_->GetConnection();
        Status status = stub->GetVarifyCode(&context, request, &reply);

        if (status.ok()) {
            pool_->returnConnection(std::move(stub));
            return reply;
        }
        else {
            pool_->returnConnection(std::move(stub));
            reply.set_error(ErrorCodes::RPCFailed);
            return reply;
        }
    }

private:
    VerifyGrpcClient();
       
    
    std::unique_ptr<RPConPool> pool_;
    std::unique_ptr<VarifyService::Stub> stub_;
};
