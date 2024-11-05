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

using namespace std;


class Asr {
public:
    Asr();
    ~Asr();

    void init();
    void start();
    void sendAudio();

    Asr (const Asr&) = delete;
    Asr& operator= (const Asr&) = delete;

// private:
//     shared_ptr<list<AVPacket*> > m_av_packet;
//     int m_deley_sec;
//     int m_total_deley_ms;
//     AVFormatContext *m_output_format_context;
};