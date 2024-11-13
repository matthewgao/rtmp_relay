#include "relayer.h"
#include "delayer.h"
#include "replacer.h"
#include "asr.h"
#include <unistd.h>

Relayer::Relayer(const string& in_url, const string& out_url):m_in_url(in_url), m_out_url(out_url) {

}

Relayer::~Relayer() {
    av_write_trailer(m_output_format_context);
    avio_closep(&m_output_format_context->pb);
    avformat_free_context(m_output_format_context);
    avformat_close_input(&m_input_format_context);
    avformat_free_context(m_input_format_context);
}

int
Relayer::init() {
    // 打开输入流
    if (avformat_open_input(&m_input_format_context, m_in_url.c_str(), NULL, NULL) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not open input stream\n");
        return -1;
    }

    // 查找流信息
    if (avformat_find_stream_info(m_input_format_context, NULL) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not find stream information\n");
        return -1;
    }

    // 创建输出流
    avformat_alloc_output_context2(&m_output_format_context, NULL, "flv", m_out_url.c_str());
    if (!m_output_format_context) {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
        return -1;
    }

    // 复制视频流
    for (unsigned int i = 0; i < m_input_format_context->nb_streams; i++) {
        AVStream *in_stream = m_input_format_context->streams[i];
        AVStream *out_stream = avformat_new_stream(m_output_format_context, NULL);
        if (!out_stream) {
            av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
            return -1;
        }

        // 复制参数
        if (avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar) < 0) {
            av_log(NULL, AV_LOG_ERROR, "Failed to copy codec parameters\n");
            return -1;
        }
    }

    m_audio_stream_index = av_find_best_stream(m_input_format_context, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (m_audio_stream_index < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not find an audio stream, m_audio_stream_index=%d\n", m_audio_stream_index);
        return -1;
    }

    m_video_stream_index = av_find_best_stream(m_input_format_context, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (m_video_stream_index < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not find an video stream, m_video_stream_index=%d\n", m_video_stream_index);
        return -1;
    }

    av_log(NULL, AV_LOG_INFO, "m_video_stream_index=%d, m_audio_stream_index=%d\n", m_video_stream_index, m_audio_stream_index);

    // 打开输出流
    if (!(m_output_format_context->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&m_output_format_context->pb, m_out_url.c_str(), AVIO_FLAG_WRITE) < 0) {
            av_log(NULL, AV_LOG_ERROR, "Could not open output URL\n");
            return -1;
        }
    }

    m_replacer = make_shared<Replacer>();
    int ret = m_replacer->init(m_input_format_context);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not open output URL %d\n", ret);
        return -1;
    }

    av_log(NULL, AV_LOG_INFO, "delay %d\n", m_delay_sec);
    m_delayer = make_shared<Delayer>(m_delay_sec, m_output_format_context);
    m_delayer->setReplacer(m_replacer);
    m_delayer->setOutputUrl(m_out_url);

    m_asr = make_shared<Asr>(m_akId, m_akSecret, m_appkey);
    m_asr->init();
    m_asr->setMaxSentenceSilence(m_max_sentence_silence_ms);
    m_asr->setDelayer(m_delayer);
    m_asr->createAudioDecoder(m_input_format_context);
    m_asr->createBlackListDict(m_dict_filename);
    ret = m_asr->start(); 
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not open asr %d\n", ret);
        return -1;
    }

    return 0;
}

void 
Relayer::startProcess() {
    // int ret = avformat_write_header(m_output_format_context, NULL);
    // if (ret < 0) {
    //     av_log(NULL, AV_LOG_ERROR, "fail to writer format header %d\n", ret);
    //     return;
    // }

    int audio_frame_count = 0;
    av_log(NULL, AV_LOG_ERROR, "start to process\n");
    while (1) {
        AVPacket *pkt = av_packet_alloc();
        if (av_read_frame(m_input_format_context, pkt) < 0) {
            av_packet_free(&pkt);
            break;
        }

        // 检查流类型
        if (pkt->stream_index == 0) { // 假设视频流在索引 0
            // av_log(NULL, AV_LOG_INFO, "video size %d, duration %lld pts %lld\n", pkt->size, pkt->duration, pkt->pts);
            m_delayer->pushVideoFrame(pkt);
        } else if (pkt->stream_index == 1) { // 假设音频流在索引 1
            audio_frame_count++;

            // av_log(NULL, AV_LOG_INFO, "size %d, duration %lld pts %lld\n", pkt->size, pkt->duration, pkt->pts);
            m_asr->sendAudio(pkt);
            m_delayer->pushAudioFrame(pkt);
            //do not send too fast to asr, it will cause "Too many audio data in evbuffer"
            if(audio_frame_count < 200) {
                // av_log(NULL, AV_LOG_INFO, "sleep 10ms\n");
                usleep(10000);
            }
            // if ((audio_frame_count/100) % 2 == 1){
            //     AVPacket* tmp = replacer->replaceAudioToMute(pkt);
            //     m_delayer->pushAudioFrame(tmp);
            // } else {
            //     m_delayer->pushAudioFrame(pkt);
            // }

        } else {
            // av_packet_unref(pkt);
            av_packet_free(&pkt);
        }
    }
}

void 
Relayer::setKey(const string& akId, const string& akSecret, const string& appkey) {
    m_akId = akId;
    m_akSecret = akSecret;
    m_appkey = appkey;
}

void Relayer::setDictFile(const string& filename) {
    m_dict_filename = filename;
}