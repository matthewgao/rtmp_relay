#include "delayer.h"

Delayer::Delayer(int deley_sec, AVFormatContext *output_format_context):m_deley_sec(deley_sec),m_output_format_context(output_format_context){
    m_total_deley_ms = 0;
    // this->m_av_packet = make_shared<list<AVPacket*> >();
}

Delayer::~Delayer() {
    for (auto &p : m_av_packet) {
        av_packet_free(&p);
    }
}

void 
Delayer::pushVideoFrame(AVPacket *pkt) {
    std::lock_guard<std::mutex> guard(m_mutex);
    m_av_packet.push_back(pkt);
    m_total_deley_ms += pkt->duration;
    flush();
}

void 
Delayer::pushAudioFrame(AVPacket *pkt) {
    std::lock_guard<std::mutex> guard(m_mutex);
    m_av_packet.push_back(pkt);
    m_total_deley_ms += pkt->duration;
    flush();
}

void 
Delayer::flush() {
    if (m_total_deley_ms > m_deley_sec*1000*2) {
        while(true) {
            // printf("flush m_total_deley_ms=%d, size=%d\n", m_total_deley_ms, m_av_packet->size());
            AVPacket *p = m_av_packet.front();
            if (m_total_deley_ms - p->duration > m_deley_sec*1000*2) {
                m_av_packet.pop_front();
                m_total_deley_ms -= p->duration;
                av_interleaved_write_frame(m_output_format_context, p);
                // av_packet_unref(p);
                av_packet_free(&p);
            } else {
                break;
            }
        }
    }
}

void 
Delayer::replaceAudioPacket(int64_t start, int64_t end) {
    std::lock_guard<std::mutex> guard(m_mutex);
    av_log(NULL, AV_LOG_ERROR, "mute the packet from pts %ld to %ld\n", start, end);
    for (auto &p : m_av_packet) {
        if (p->stream_index != m_replacer->getAudioIndex()) {
            continue;
        }

        if (p->pts >= start && p->pts <= end) {
            AVPacket* out = m_replacer->replaceAudioToMute(p);
            p = out;
        }
    }
}