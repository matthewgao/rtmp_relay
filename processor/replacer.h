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
    ~Replacer();
    int Init(AVFormatContext* input_format_context);
    AVPacket* ReplaceAudioToMute(AVPacket *pkt);
    int CreateAudioDecoder(AVFormatContext* input_format_context);
    int CreateAudioEncoder();

    //mutex is not copyable so Replacer must not copyable either
    Replacer (const Replacer&) = delete;
    Replacer& operator= (const Replacer&) = delete;

private:
    AVCodec *m_audio_decoder;
    AVCodecContext *m_audio_decoder_ctx;
    const AVCodec *m_audio_encoder;
    AVCodecContext *m_audio_encoder_ctx;
    int m_audio_stream_index;
};