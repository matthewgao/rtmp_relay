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
    // Delayer();
    Delayer(int deley_sec, AVFormatContext *output_format_context);
    ~Delayer();

    void PushVideoFrame(AVPacket *pkt);
    void PushAudioFrame(AVPacket *pkt);

    int GetDelaySec(){
        return this->m_deley_sec;
    }

    //mutex is not copyable so Delayer must not copyable either
    Delayer (const Delayer&) = delete;
    Delayer& operator= (const Delayer&) = delete;

private:
    void Flush();

    shared_ptr<list<AVPacket*> > m_av_packet;
    int m_deley_sec;
    int m_total_deley_ms;
    AVFormatContext *m_output_format_context;
};