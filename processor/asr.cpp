#include "asr.h"

#define OUTPUT_CH_LAYOUT	AV_CH_LAYOUT_MONO
#define OUTPUT_SAMPLE_RATE	16000
#define OUTPUT_SAMPLE_FMT	AV_SAMPLE_FMT_S16

void onTranscriptionStarted(AlibabaNls::NlsEvent* cbEvent, void* cbParam) {
	printf("onTranscriptionStarted code: %d task id: %s\n", cbEvent->getStatusCode(), cbEvent->getTaskId());
}

void onTaskFailed(AlibabaNls::NlsEvent* cbEvent, void* cbParam) {
	printf("onTaskFailed code: %d task id: %s, restart the task\n", cbEvent->getStatusCode(), cbEvent->getTaskId());
    Asr* asr = (Asr*)cbParam;
    asr->resetAsrTimeBase();
    asr->init();
    asr->start();
}

void onChannelClosed(AlibabaNls::NlsEvent* cbEvent, void* cbParam) {
	printf("onChannelClosed: %d task id: %s, restart the task\n", cbEvent->getStatusCode(), cbEvent->getTaskId());
    Asr* asr = (Asr*)cbParam;
    asr->resetAsrTimeBase();
    asr->init();
    asr->start();
}

void onTranscriptionResultChanged(AlibabaNls::NlsEvent* cbEvent, void* cbParam) {
	printf("onTranscriptionResultChanged code: %d result: %s, wordlist: %ld\n", cbEvent->getStatusCode(), cbEvent->getResult(), 
        cbEvent->getSentenceWordsList().size());
}

void onTranscriptionCompleted(AlibabaNls::NlsEvent* cbEvent, void* cbParam) {
	printf("onTranscriptionCompleted code: %d result: %s  wordlist: %ld\n", cbEvent->getStatusCode(), cbEvent->getResult(),
        cbEvent->getSentenceWordsList().size());
}

void onSentenceBegin(AlibabaNls::NlsEvent* cbEvent, void* cbParam) {
	// printf("onSentenceBegin code: %d task id: %s\n", cbEvent->getStatusCode(), cbEvent->getTaskId());
    printf("-------------------------------------------------\n");
}

void onSentenceEnd(AlibabaNls::NlsEvent* cbEvent, void* cbParam) {
	// printf("onSentenceEnd code: %d, wordlistSize: %ld\n", cbEvent->getStatusCode(), cbEvent->getSentenceWordsList().size());

    Asr* asr = (Asr*)cbParam;
    
    auto list = cbEvent->getSentenceWordsList();
    // for (auto &item : list) {
    //     printf(" %s |", item.text.c_str());
    //     if (asr->hitDict(item.text)) {
    //         printf("\nmute word: %s\n", item.text.c_str());
    //         int64_t base = asr->getTimeBase();
    //         int64_t start_pts = base + item.startTime;
    //         int64_t end_pts = base + item.endTime;
    //         asr->getDelayer()->replaceAudioPacket(start_pts, end_pts);
    //         asr->sendHitWord(item.text, item.startTime, item.endTime);
    //     }
    // }

    string sentence = cbEvent->getResult();
    auto result = asr->hitDict(sentence, list);
    for(auto& item : *result) {
        int64_t base = asr->getTimeBase();
        int64_t start_pts = base + item.m_start;
        int64_t end_pts = base + item.m_end;
        asr->getDelayer()->replaceAudioPacket(start_pts, end_pts);
        asr->sendHitWord(item.m_text, item.m_start, item.m_end);
    }

    asr->sendSentence(cbEvent->getResult(), cbEvent->getSentenceBeginTime(), 
        cbEvent->getSentenceBeginTime() + cbEvent->getSentenceTime(), list);
    printf("\nALL_RESULT: %s, from %ld to %ld\n", cbEvent->getResult(), cbEvent->getSentenceBeginTime(), cbEvent->getSentenceTime());
}

Asr::Asr(string akId, string akSecret, string m_appkey):m_akId(akId),m_akSecret(akSecret),m_appkey(m_appkey) {
	pcmResampleInit();
    m_first_audio_pts = 0;
    m_max_sentence_silence_ms = 200; //200ms
    m_biz_client = nullptr;
}

Asr::~Asr() {
    AlibabaNls::NlsClient::releaseInstance();
    delete m_request;
    pcmResampleFree();
}

void
Asr::init() {
    AlibabaNls::NlsClient::getInstance()->setSyncCallTimeout(10*1000);
    AlibabaNls::NlsClient::getInstance()->startWorkThread(3);
    AlibabaNls::NlsClient::getInstance()->setLogConfig(
            NULL, AlibabaNls::LogLevel::LogInfo, 100, 10);
}

void 
Asr::pcmResampleInit() {
	PcmParams pp;
	pp.channel_layout = AV_CH_LAYOUT_MONO;
	pp.sample_rate = 16000;
	pp.sample_fmt = AV_SAMPLE_FMT_S16;

	m_pr_ctx.swr_ctx = swr_alloc();
	m_pr_ctx.src = pp;
	m_pr_ctx.dst = pp;
	m_pr_ctx.data = NULL;
	m_pr_ctx.data_len = 0;
	m_pr_ctx.max_nb_samples = 0;
}

void
Asr::pcmResampleSetParams(const PcmParams *pcm_src, const PcmParams *pcm_dst) {
	av_opt_set_int(m_pr_ctx.swr_ctx, "in_channel_layout", pcm_src->channel_layout, 0);
	av_opt_set_int(m_pr_ctx.swr_ctx, "in_sample_rate", pcm_src->sample_rate, 0);
	av_opt_set_sample_fmt(m_pr_ctx.swr_ctx, "in_sample_fmt", (AVSampleFormat)pcm_src->sample_fmt, 0);

	av_opt_set_int(m_pr_ctx.swr_ctx, "out_channel_layout", pcm_dst->channel_layout, 0);
	av_opt_set_int(m_pr_ctx.swr_ctx, "out_sample_rate", pcm_dst->sample_rate, 0);
	av_opt_set_sample_fmt(m_pr_ctx.swr_ctx, "out_sample_fmt", (AVSampleFormat)pcm_dst->sample_fmt, 0);

	swr_init(m_pr_ctx.swr_ctx);
}

void 
Asr::pcmResampleFree() {
	swr_free(&m_pr_ctx.swr_ctx);
	av_freep(&m_pr_ctx.data);
}

int 
Asr::pcmResample(const AVFrame *frame) {
	int nb_samples = av_rescale_rnd(swr_get_delay(m_pr_ctx.swr_ctx, m_pr_ctx.src.sample_rate) + frame->nb_samples,
        m_pr_ctx.dst.sample_rate, m_pr_ctx.src.sample_rate, AV_ROUND_UP);

	int nb_channels = av_get_channel_layout_nb_channels(m_pr_ctx.dst.channel_layout);
	if (nb_samples > m_pr_ctx.max_nb_samples) {
		m_pr_ctx.max_nb_samples = nb_samples;
		av_freep(&m_pr_ctx.data);
		av_samples_alloc(&m_pr_ctx.data, NULL, nb_channels, nb_samples, (AVSampleFormat)m_pr_ctx.dst.sample_fmt, 1);
	}

	int ret = swr_convert(m_pr_ctx.swr_ctx, &m_pr_ctx.data, nb_samples,
			(const uint8_t **) frame->data, frame->nb_samples);
	if (ret < 0) {
		return ret;
	}

	m_pr_ctx.data_len = av_samples_get_buffer_size(NULL,
			nb_channels, ret, (AVSampleFormat)m_pr_ctx.dst.sample_fmt, 1);

	return 0;
}

int 
Asr::checkToken() {
    std::time_t curTime = std::time(0);
    if (m_token.empty()) {
        if (m_expireTime - curTime < 10) {
            std::cout << "the token will be expired, please generate new token" << std::endl;
            int ret = generateToken();
            if (ret < 0) {
                std::cout << "generate token failed" << std::endl;
                return -1;
            } else {
                if (m_token.empty() || m_expireTime < 0) {
                    std::cout << "generate empty token" << std::endl;
                    return -2;
                }
            }
        }
    }
    return 0;
}

int 
Asr::generateToken() {
    AlibabaNlsCommon::NlsToken nlsTokenRequest;
    nlsTokenRequest.setAccessKeyId(m_akId);
    nlsTokenRequest.setKeySecret(m_akSecret);
    int retCode = nlsTokenRequest.applyNlsToken();

    if (retCode < 0) {
        std::cout << "Failed error code: " << retCode
                << "  error msg: " << nlsTokenRequest.getErrorMsg() << std::endl;
        return retCode;
    }

    m_token = nlsTokenRequest.getToken();
    m_expireTime = nlsTokenRequest.getExpireTime();

    return 0;
}

int
Asr::start() {
    generateToken();

    m_request =
        AlibabaNls::NlsClient::getInstance()->createTranscriberRequest("cpp", true);
    if (m_request == NULL) {
        std::cout << "SpeechTranscriberRequest failed." << std::endl;
        return -1;
    }
    /*
    * 2. 设置用于接收结果的回调
    */
    m_request->setOnTranscriptionStarted(onTranscriptionStarted, this);
    // 设置识别结果变化回调函数
    m_request->setOnTranscriptionResultChanged(onTranscriptionResultChanged, this);
    // 设置语音转写结束回调函数
    m_request->setOnTranscriptionCompleted(onTranscriptionCompleted, this);
    // 设置一句话开始回调函数
    m_request->setOnSentenceBegin(onSentenceBegin, this);
    // 设置一句话结束回调函数
    m_request->setOnSentenceEnd(onSentenceEnd, this);
    // 设置异常识别回调函数
    m_request->setOnTaskFailed(onTaskFailed, this);
    // 设置识别通道关闭回调函数
    m_request->setOnChannelClosed(onChannelClosed, this);
    //   if (g_on_message_flag) {
    //     // 设置所有服务端返回信息回调函数
    //     m_request->setOnMessage(onMessage, this);
    //     // 开启所有服务端返回信息回调函数, 其他回调(除了OnBinaryDataRecved)失效
    //     m_request->setEnableOnMessage(true);
    //   }
    // 是否开启重连机制
    m_request->setEnableContinued(true);

    m_request->setFormat("pcm");
    m_request->setAppKey(m_appkey.c_str());
    m_request->setToken(m_token.c_str());
    m_request->setUrl("wss://nls-gateway.cn-shanghai.aliyuncs.com/ws/v1");

    // 设置音频数据采样率, 可选参数，目前支持16000, 8000. 默认是16000
    m_request->setSampleRate(16000);
    // 设置是否返回中间识别结果, 可选参数. 默认false
    m_request->setIntermediateResult(false);
    // 设置是否在后处理中添加标点, 可选参数. 默认false
    m_request->setPunctuationPrediction(false);
    // 设置是否在后处理中执行数字转写, 可选参数. 默认false
    m_request->setInverseTextNormalization(false);
    m_request->setEnableWords(true);
    m_request->setSemanticSentenceDetection(false);
    m_request->setMaxSentenceSilence(m_max_sentence_silence_ms);

    int ret = m_request->start();
    if (ret < 0) {
        //seems like sdk bug, ret < 0 is not failed actucally 
        printf("start asr failed, ret=%d\n", ret);
        // return ret;
    }

    return 0;
}

int
Asr::createAudioDecoder(AVFormatContext* input_format_context) {
    const AVCodec* temp_decoder = NULL;
    m_audio_stream_index = av_find_best_stream(input_format_context, AVMEDIA_TYPE_AUDIO, -1, -1, &temp_decoder, 0);
    if (m_audio_stream_index < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not find an audio stream, m_audio_stream_index=%d\n", m_audio_stream_index);
        return -1;
    }

    m_audio_decoder = const_cast<AVCodec *>(temp_decoder);
    m_audio_decoder_ctx = avcodec_alloc_context3(m_audio_decoder);
    if (!m_audio_decoder_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Could not allocate a decoding context\n");
        return -1;
    }

    avcodec_parameters_to_context(m_audio_decoder_ctx, input_format_context->streams[m_audio_stream_index]->codecpar);

    if (avcodec_open2(m_audio_decoder_ctx, m_audio_decoder, NULL) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not open m_audio_decoder_ctx\n");
        avcodec_free_context(&m_audio_decoder_ctx);
        return -1;
    }

    if (m_audio_decoder_ctx == NULL) {
        av_log(NULL, AV_LOG_ERROR, "Could not open m_audio_decoder_ctx, is NULL\n");
        return -1;
    }
    
    PcmParams pcm_src;
    pcm_src.channel_layout = m_audio_decoder_ctx->channel_layout;
    pcm_src.sample_rate = m_audio_decoder_ctx->sample_rate;
    pcm_src.sample_fmt = m_audio_decoder_ctx->sample_fmt;
    PcmParams pcm_dst;
    pcm_dst.channel_layout = OUTPUT_CH_LAYOUT;
    pcm_dst.sample_rate = OUTPUT_SAMPLE_RATE;
    pcm_dst.sample_fmt = OUTPUT_SAMPLE_FMT;
    pcmResampleSetParams(&pcm_src, &pcm_dst);

    return 0;
}

int 
Asr::sendAudio(AVPacket *pkt) {
    checkToken();

    while(true) {
        int ret = avcodec_send_packet(m_audio_decoder_ctx, pkt);
        if (ret >= 0) {
            break;
        }

        if (ret == AVERROR(EAGAIN)) {
            av_log(NULL, AV_LOG_WARNING, "avcodec_send_packet EAGAIN\n");
            continue;
        } else if (ret == AVERROR_EOF) {
            av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet AVERROR_EOF\n");
            return -1;
        } else {
            av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet unknown error\n");
            return -1;
        }
    }

    AVFrame *frame = av_frame_alloc();
    while(true) {
        int ret = avcodec_receive_frame(m_audio_decoder_ctx, frame);
        if (ret >= 0) {
            ret = pcmResample(frame);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "pcmResample failed\n");
                break;
            }

            int sent_byte = m_request->sendAudio(m_pr_ctx.data, m_pr_ctx.data_len, ENCODER_OPUS);
            if (m_first_audio_pts == 0 && sent_byte > 0) {
                av_log(NULL, AV_LOG_INFO, "set first pts to %ld\n", pkt->pts);
                m_first_audio_pts = pkt->pts;
            }

            break;
        }

        if (ret == AVERROR(EAGAIN)) {
            avcodec_send_packet(m_audio_decoder_ctx, pkt);
            av_log(NULL, AV_LOG_WARNING, "avcodec_receive_frame EAGAIN\n");
            continue;
            // break;
        } else if (ret == AVERROR_EOF) {
            av_log(NULL, AV_LOG_ERROR, "avcodec_receive_frame AVERROR_EOF\n");
            av_frame_unref(frame);
            av_frame_free(&frame);
            return -1;
        } else {
            // Error occurred
            av_log(NULL, AV_LOG_ERROR, "avcodec_receive_frame unknown error\n");
            av_frame_unref(frame);
            av_frame_free(&frame);
            return -1;
        }
    }

    av_frame_unref(frame);
    av_frame_free(&frame);
    return 0;
}


int 
Asr::createBlackListDict(string file) {
    m_dict.init(file);
    return 0;
}

std::shared_ptr<std::list<AudioSegement> > 
Asr::hitDict(string& sentence, std::list<AlibabaNls::WordInfomation>& word_list)
{
    std::shared_ptr<std::list<AudioSegement> > out = std::make_shared<std::list<AudioSegement> >();

    for(auto& bword: m_dict.getDict()){
        size_t pos = 0;
        int64_t end_pos = 0;
        while ((pos = sentence.find(bword.first, end_pos)) != std::string::npos) {
            std::cout << "Substring found at position: " << pos << std::endl;
            end_pos = pos + bword.first.length();

            int64_t start = 0;
            int64_t end = 0;

            int64_t cnt = 0;
            bool found = false;
            for(auto& item: word_list) {
                
                if (cnt <= pos) {
                    start = item.startTime;
                    cnt += item.text.length();
                    continue;
                }

                if (cnt >= end_pos) {
                    end = item.endTime;
                    cnt += item.text.length();
                    found = true;
                    break;
                }

                cnt += item.text.length();
            }

            if  (found) {
                printf("mute_word: %s, from %ld to %ld\n",bword.first.c_str(), start, end);
                out->push_back(AudioSegement(start, end, bword.first));
            }
        }
    }

    return out;
}

void
Asr::sendHitWord(const string& word, int32_t start, int32_t end) {
    if (m_biz_client == nullptr) {
        return;
    }

    m_biz_client->sendHitWords(word, start, end);
}

void
Asr::sendSentence(const string& sentence, int32_t start, int32_t end, std::list<AlibabaNls::WordInfomation>& word_list) {
    if (m_biz_client == nullptr) {
        return;
    }

    m_biz_client->sendSentence(sentence, start, end, word_list);
}