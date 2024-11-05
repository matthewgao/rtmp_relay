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


class Delayer {
public:
    Delayer(int deley_sec, AVFormatContext *output_format_context);
    ~Delayer();

    void pushVideoFrame(AVPacket *pkt);
    void pushAudioFrame(AVPacket *pkt);

    int getDelaySec(){
        return this->m_deley_sec;
    }

    Delayer (const Delayer&) = delete;
    Delayer& operator= (const Delayer&) = delete;

private:
    void flush();

    shared_ptr<list<AVPacket*> > m_av_packet;
    int m_deley_sec;
    int m_total_deley_ms;
    AVFormatContext *m_output_format_context;
};