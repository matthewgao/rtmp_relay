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
    int Init(AVFormatContext* input_format_context, const AVCodecParameters *par);
    AVPacket* ReplaceAudioToMute(AVPacket *pkt);
    int CreateAudioDecoder(AVFormatContext* input_format_context, const AVCodecParameters *par);
    int CreateAudioEncoder();

    //mutex is not copyable so Replacer must not copyable either
    Replacer (const Replacer&) = delete;
    Replacer& operator= (const Replacer&) = delete;

private:
    AVCodec *m_audio_decoder = nullptr;
    AVCodecContext *m_audio_decoder_ctx = nullptr;
    const AVCodec *m_audio_encoder = nullptr;
    AVCodecContext *m_audio_encoder_ctx = nullptr;
};