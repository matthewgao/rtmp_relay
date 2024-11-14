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
#include "delayer.h"
#include "../censor/dictionary.h"
#include "../grpc/biz_client.h"

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
    virtual ~Asr();

    void init();
    int start();

    int checkToken();
    int generateToken();
    int sendAudio(AVPacket *pkt);
    int createAudioDecoder(AVFormatContext* input_format_context);
    int createBlackListDict(string file);
    bool hitDict(string& word);

    Asr (const Asr&) = delete;
    Asr& operator= (const Asr&) = delete;

    int64_t getTimeBase() const {
        return m_first_audio_pts;
    }

    void setDelayer(shared_ptr<Delayer> r) {
        m_delayer = r;
    }

    shared_ptr<Delayer> getDelayer() const {
        return m_delayer;
    }

    void setMaxSentenceSilence(int gap_ms) {
        m_max_sentence_silence_ms = gap_ms;
    }

    void resetAsrTimeBase() {
        m_first_audio_pts = 0;
    }

    void setBizClient(shared_ptr<BizClient> client) {
        m_biz_client = client;
    }

    void sendHitWord(const string& word, int32_t start, int32_t end);

private:
    void pcmResampleInit();
    void pcmResampleSetParams(const PcmParams *pcm_src, const PcmParams *pcm_dst);
    void pcmResampleFree();
    int pcmResample(const AVFrame *frame);

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
    int64_t m_first_audio_pts;
    shared_ptr<Delayer> m_delayer;
    Dictionary m_dict;
    int m_max_sentence_silence_ms;
    shared_ptr<BizClient> m_biz_client;
};