#include "delayer.h"
#include <chrono>

Delayer::Delayer(int deley_sec, AVFormatContext *output_format_context):m_deley_sec(deley_sec),m_output_format_context(output_format_context){
    m_total_deley_ms = 0;
    this->m_av_packet = make_shared<list<AVPacket*> >();
}

Delayer::~Delayer() {

}

void 
Delayer::PushVideoFrame(AVPacket *pkt) {
    this->m_av_packet->push_back(pkt);
    m_total_deley_ms += pkt->duration;
    Flush();
}

void 
Delayer::PushAudioFrame(AVPacket *pkt) {
    this->m_av_packet->push_back(pkt);
    m_total_deley_ms += pkt->duration;
    Flush();
}

void 
Delayer::Flush() {
    if (m_total_deley_ms > m_deley_sec*1000*2) {
        while(true) {
            // printf("flush m_total_deley_ms=%d\n", m_total_deley_ms);
            AVPacket *p = this->m_av_packet->front();
            if (m_total_deley_ms - p->duration > m_deley_sec*1000*2) {
                this->m_av_packet->pop_front();
                m_total_deley_ms -= p->duration;
                av_interleaved_write_frame(m_output_format_context, p);
                av_packet_unref(p);
                // av_freep(p);
                av_packet_free(&p);
            } else {
                break;
            }
        }
    }
}