#include "biz_client.h"


BizClient::BizClient(string ip, string port) {
    auto channel = grpc::CreateChannel(ip+":"+port, grpc::InsecureChannelCredentials());

    m_stub = StreamInvokeService::NewStub(channel);
}

void
BizClient::sendHitWords(string& words) {
    HitWordRequest request;
    request.set_word("{user data}");

    // Container for the data we expect from the server.
    HitWordResponse reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = m_stub->hitWords(&context, request, &reply);
    if (!status.ok()) {
        std::cout << status.error_code() << ": " << status.error_message()
                    << std::endl;
        std::cout << "RPC failed";
    }
}