#pragma once

#include <string>
#include <memory>
#include <grpcpp/grpcpp.h>
#include <service.grpc.pb.h>

using namespace std;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using relay::api::StreamInvokeService;
using relay::api::HitWordRequest;
using relay::api::HitWordResponse;

class BizServer {
  public:
    BizServer(string ip, string port);
    virtual ~BizServer() {};

  private:
    std::unique_ptr<StreamInvokeService::Stub> m_stub;
};