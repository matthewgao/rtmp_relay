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
    virtual ~Delayer();

    void pushVideoFrame(AVPacket *pkt);
    void pushAudioFrame(AVPacket *pkt);
    void replaceAudioPacket(int64_t start, int64_t end);

    int getDelaySec() const {
        return this->m_deley_sec;
    }

    void setReplacer(shared_ptr<Replacer> r) {
        m_replacer = r;
    }

    void setOutputUrl(string out) {
        m_out_url = out;
    }

    Delayer (const Delayer&) = delete;
    Delayer& operator= (const Delayer&) = delete;

private:
    void flush();

    list<AVPacket*> m_av_packet;
    int m_deley_sec;
    int m_total_deley_ms;
    AVFormatContext *m_output_format_context;
    shared_ptr<Replacer> m_replacer;
    std::mutex m_mutex;
    bool m_handshake_done;
    string m_out_url;
};