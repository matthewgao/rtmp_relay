#include "relayer.h"
#include "delayer.h"
#include "replacer.h"
#include "asr.h"

Relayer::Relayer(string in_url, string out_url):m_in_url(in_url), m_out_url(out_url) {

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
    for (int i = 0; i < m_input_format_context->nb_streams; i++) {
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

    // 打开输出流
    if (!(m_output_format_context->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&m_output_format_context->pb, m_out_url.c_str(), AVIO_FLAG_WRITE) < 0) {
            av_log(NULL, AV_LOG_ERROR, "Could not open output URL\n");
            return -1;
        }
    }
}

void 
Relayer::startProcess() {
    int ret = avformat_write_header(m_output_format_context, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "fail to writer format header %d\n", ret);
        return;
    }

    int audio_frame_count = 0;
    Delayer *delayer = new Delayer(2, m_output_format_context);
    Replacer *replacer = new Replacer();
    ret = replacer->init(m_input_format_context);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not open output URL %d\n", ret);
        return;
    }

    Asr *asr = new Asr(m_akId, m_akSecret, m_appkey);
    asr->init();
    asr->createAudioDecoder(m_input_format_context);
    ret = asr->start(); 
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not open asr %d\n", ret);
        return;
    }

    while (1) {
        AVPacket *pkt = av_packet_alloc();
        if (av_read_frame(m_input_format_context, pkt) < 0) {
            av_packet_free(&pkt);
            break;
        }

        // 检查流类型
        if (pkt->stream_index == 0) { // 假设视频流在索引 0
            // av_log(NULL, AV_LOG_INFO, "size %d, duration %lld pts %lld\n", pkt->size, pkt->duration, pkt->pts);
            delayer->pushVideoFrame(pkt);
        } else if (pkt->stream_index == 1) { // 假设音频流在索引 1
            audio_frame_count++;
            // if (audio_frame_count % AUDIO_DISCARD_INTERVAL != 0) {
            //     for (size_t i = 1; i < pkt->size; i++)
            //     {
            //         pkt->data[i] = 0;
            //     }
                // av_log(NULL, AV_LOG_INFO, "size %d, duration %lld pts %lld\n", pkt->size, pkt->duration, pkt->pts);
                // av_interleaved_write_frame(m_output_format_context, pkt);

                // av_log(NULL, AV_LOG_INFO, "drop %d\n", audio_frame_count);
                
            // }
            // av_packet_unref(&pkt);
            asr->sendAudio(pkt);
            if ((audio_frame_count/100) % 2 == 1){
                AVPacket* tmp = replacer->replaceAudioToMute(pkt);
                delayer->pushAudioFrame(tmp);
            } else {
                delayer->pushAudioFrame(pkt);
            }

        } else {
            // av_packet_unref(pkt);
            av_packet_free(&pkt);
        }
    }
}

void 
Relayer::setKey(string akId, string akSecret, string appkey) {
    m_akId = akId;
    m_akSecret = akSecret;
    m_appkey = appkey;
}