#pragma once
extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <iostream>

using namespace std;

class Replacer {
public:
    Replacer();
    virtual ~Replacer();
    int init(AVFormatContext* input_format_context);
    AVPacket* replaceAudioToMute(AVPacket *pkt);
    int createAudioDecoder(AVFormatContext* input_format_context);
    int createAudioEncoder();

    int getAudioIndex() const {
        return m_audio_stream_index;
    }

    Replacer (const Replacer&) = delete;
    Replacer& operator= (const Replacer&) = delete;

private:
    AVCodec *m_audio_decoder;
    AVCodecContext *m_audio_decoder_ctx;
    const AVCodec *m_audio_encoder;
    AVCodecContext *m_audio_encoder_ctx;
    int m_audio_stream_index;
    AVRational m_time_base;
};