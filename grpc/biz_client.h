#pragma once

#include <string>
#include <memory>
#include <grpcpp/grpcpp.h>
#include "service.grpc.pb.h"
#include "nlsEvent.h"

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
using relay::api::SentenceRequest;
using relay::api::SentenceResponse;
using relay::api::WordList;

class BizClient {
  public:
    BizClient(const string& host);
    virtual ~BizClient() {};

    void sendHitWords(const string& words, int32_t start, int32_t end, const string& sentence);
    void sendSentence(const string& sentence, int32_t start, int32_t end, std::list<AlibabaNls::WordInfomation>& word_list);

  private:
    std::unique_ptr<StreamInvokeService::Stub> m_stub;
};