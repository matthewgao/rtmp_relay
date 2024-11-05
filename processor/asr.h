#pragma once
extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
}

#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "nlsClient.h"
#include "nlsEvent.h"
#include "nlsToken.h"
#include "speechRecognizerRequest.h"

using namespace std;


class Asr {
public:
    Asr();
    ~Asr();

    void init();
    void start();
    void sendAudio();
    int checkToken();
    int generateToken();

    Asr (const Asr&) = delete;
    Asr& operator= (const Asr&) = delete;

private:
    string m_appkey;
    string m_token;
    string m_akId;
    string m_akSecret;
    long m_expireTime;
};