#include "biz_client.h"


BizClient::BizClient(const string& host) {
    auto channel = grpc::CreateChannel(host, grpc::InsecureChannelCredentials());

    m_stub = StreamInvokeService::NewStub(channel);
}

void
BizClient::sendHitWords(const string& words, int32_t start, int32_t end) {
    HitWordRequest request;
    request.set_word(words);
    request.set_starttime(start);
    request.set_endtime(end);

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

void
BizClient::sendSentence(const string& sentence, int32_t start, int32_t end, std::list<AlibabaNls::WordInfomation>& word_list) {
    SentenceRequest request;
    request.set_sentence(sentence);
    request.set_starttime(start);
    request.set_endtime(end);

    for (auto &item : word_list) {
        WordList *tmp = request.add_wordlist();
        tmp->set_word(item.text);
        tmp->set_starttime(item.startTime);
        tmp->set_endtime(item.endTime);
    }

    // Container for the data we expect from the server.
    SentenceResponse reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = m_stub->sentence(&context, request, &reply);
    if (!status.ok()) {
        std::cout << status.error_code() << ": " << status.error_message()
                    << std::endl;
        std::cout << "RPC failed";
    }
}