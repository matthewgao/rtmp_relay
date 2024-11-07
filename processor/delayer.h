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
#include "replacer.h"

using namespace std;


class Delayer {
public:
    Delayer(int deley_sec, AVFormatContext *output_format_context);
    ~Delayer();

    void pushVideoFrame(AVPacket *pkt);
    void pushAudioFrame(AVPacket *pkt);
    void replaceAudioPacket(int64_t start, int64_t end);

    int getDelaySec(){
        return this->m_deley_sec;
    }

    void setReplacer(Replacer* r) {
        m_replacer = r;
    }

    Delayer (const Delayer&) = delete;
    Delayer& operator= (const Delayer&) = delete;

private:
    void flush();

    list<AVPacket*> m_av_packet;
    int m_deley_sec;
    int m_total_deley_ms;
    AVFormatContext *m_output_format_context;
    Replacer *m_replacer;
    std::mutex m_mutex;
};