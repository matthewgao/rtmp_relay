#pragma once
extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
#include "libavutil/opt.h"
#include "libswresample/swresample.h"
#include <libavcodec/avcodec.h>
}

#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "nlsClient.h"
#include "nlsEvent.h"
#include "nlsToken.h"
#include "speechTranscriberRequest.h"

using namespace std;

typedef struct {
	int channel_layout;
	int sample_rate;
	int sample_fmt;
} PcmParams;

typedef struct {
	SwrContext *swr_ctx;
	PcmParams src;
	PcmParams dst;
	uint8_t *data;
	int data_len;
	int max_nb_samples;
} PcmResampleContext;

void 
pcmresample_init(PcmResampleContext *pc);

void 
pcmresample_setparams(PcmResampleContext *pc, const PcmParams *src, const PcmParams *dst);

void 
pcmresample_free(PcmResampleContext *pc);

int 
pcmresample_resample(PcmResampleContext *pc, const AVFrame *frame);

class Asr {
public:
    Asr(string akId, string akSecret, string m_appkey);
    ~Asr();

    void init();
    int start();

    int checkToken();
    int generateToken();
    int sendAudio(AVPacket *pkt);
    int createAudioDecoder(AVFormatContext* input_format_context);

    Asr (const Asr&) = delete;
    Asr& operator= (const Asr&) = delete;

private:
    string m_appkey;
    string m_token;
    string m_akId;
    string m_akSecret;
    long m_expireTime;
    AlibabaNls::SpeechTranscriberRequest *m_request;
    PcmResampleContext m_pr_ctx;
    AVCodec *m_audio_decoder;
    AVCodecContext *m_audio_decoder_ctx;
    int m_audio_stream_index;
};