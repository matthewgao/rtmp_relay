#include "replacer.h"
#include <chrono>


Replacer::Replacer(){
    m_audio_decoder = nullptr;
    m_audio_decoder_ctx = nullptr;
    m_audio_encoder = nullptr;
    m_audio_encoder_ctx = nullptr;
    m_audio_stream_index = 1;
}

Replacer::~Replacer() {

}

int
Replacer::init(AVFormatContext* input_format_context) {
    int ret = createAudioDecoder(input_format_context);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "create audio decoder failed, ret=%d\n", ret);
        return ret;
    }

    ret = createAudioEncoder();
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "create audio encoder failed, ret=%d\n", ret);
        return ret;
    }

    return 0;
}

int
Replacer::createAudioDecoder(AVFormatContext* input_format_context) {
    const AVCodec* temp_decoder = NULL;
    m_audio_stream_index = av_find_best_stream(input_format_context, AVMEDIA_TYPE_AUDIO, -1, -1, &temp_decoder, 0);
    if (m_audio_stream_index < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not find an audio stream, m_audio_stream_index=%d\n", m_audio_stream_index);
        return -1;
    }

    av_log(NULL, AV_LOG_ERROR, "audio stream, m_audio_stream_index=%d\n", m_audio_stream_index);

    m_audio_decoder = const_cast<AVCodec *>(temp_decoder);

    // Create a codec context for the m_audio_decoder
    m_audio_decoder_ctx = avcodec_alloc_context3(m_audio_decoder);
    if (!m_audio_decoder_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Could not allocate a decoding context\n");
        return -1;
    }

    avcodec_parameters_to_context(m_audio_decoder_ctx, input_format_context->streams[m_audio_stream_index]->codecpar);
    m_time_base =  input_format_context->streams[m_audio_stream_index]->time_base;

    if (avcodec_open2(m_audio_decoder_ctx, m_audio_decoder, NULL) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not open m_audio_decoder_ctx\n");
        avcodec_free_context(&m_audio_decoder_ctx);
        return -1;
    }

    if (m_audio_decoder_ctx == NULL) {
        av_log(NULL, AV_LOG_ERROR, "Could not open m_audio_decoder_ctx, is NULL\n");
        return -1;
    }
    
    return 0;
}

int
Replacer::createAudioEncoder() {
    m_audio_encoder = avcodec_find_encoder(m_audio_decoder_ctx->codec_id);

    if (!m_audio_encoder) {
        av_log(NULL, AV_LOG_ERROR, "encoder not found\n");
        return -1;
    }

    m_audio_encoder_ctx = avcodec_alloc_context3(m_audio_encoder);
    if (!m_audio_encoder_ctx) {
        av_log(NULL, AV_LOG_ERROR, "could not allocate an encoding context\n");
        return -1;
    }

    m_audio_encoder_ctx->channels = m_audio_decoder_ctx->channels;
    m_audio_encoder_ctx->channel_layout = av_get_default_channel_layout(m_audio_decoder_ctx->channels);
    m_audio_encoder_ctx->sample_rate = m_audio_decoder_ctx->sample_rate;
    m_audio_encoder_ctx->sample_fmt = *m_audio_decoder->sample_fmts; // use the first supported format
    m_audio_encoder_ctx->bit_rate = m_audio_decoder_ctx->bit_rate;

    avcodec_open2(m_audio_encoder_ctx, m_audio_encoder, NULL);

    return 0;
}

AVPacket * 
Replacer::replaceAudioToMute(AVPacket *packet) {
    
    int64_t orig_pts = packet->pts;
    int64_t orig_dts = packet->dts;
    int64_t orig_duration = packet->duration;
    
    while(true) {
        int ret = avcodec_send_packet(m_audio_decoder_ctx, packet);
        if (ret >= 0) {
            break;
        }

        if (ret == AVERROR(EAGAIN)) {
            av_log(NULL, AV_LOG_WARNING, "avcodec_send_packet EAGAIN\n");
            continue;
        } else if (ret == AVERROR_EOF) {
            av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet AVERROR_EOF\n");
            return packet;
        } else {
            av_log(NULL, AV_LOG_ERROR, "avcodec_send_packet unknown error\n");
            return packet;
        }
    }

    AVFrame *frame = av_frame_alloc();
    while(true) {
        int ret = avcodec_receive_frame(m_audio_decoder_ctx, frame);
        if (ret >= 0) {
            int data_size = av_get_bytes_per_sample(m_audio_decoder_ctx->sample_fmt);

            // static int64_t pcmlen = 0;
            // uint8_t *pcmbuf = frame->data[0];
            // double gainFactor = pow(10, -6/20.0);
            // double totalPeriodW = 2*M_PI/(44100*1/1000);

            

            // char x[data_size*2] = {0};
            // for (int s = 0; s <data_size; s++) {
            //     pcmlen %= 220500;
            //     double val = 100 * sin(pcmlen++ * totalPeriodW);
            //     x[2*s] = x[2*s+1] = (short)(val * gainFactor);
            //     av_log(NULL, AV_LOG_ERROR, "totalPeriodW %f %f %d\n", totalPeriodW, val, (short)(val * gainFactor));
            // }

            // for (int i = 0; i < frame->nb_samples; i++) {
            //     for (int ch = 0; ch < m_audio_decoder_ctx->ch_layout.nb_channels; ch++) {
            //         memcpy(frame->data[ch]+ data_size*i, x, data_size);
            //     }
            // }


            for (int i = 0; i < frame->nb_samples; i++) {
                for (int ch = 0; ch < m_audio_decoder_ctx->ch_layout.nb_channels; ch++) {
                    memset(frame->data[ch]+ data_size*i, 0, data_size);
                }
            }



            // for (int i = 0; i < frame->nb_samples; i++) {
            //     for (int ch = 0; ch < m_audio_decoder_ctx->ch_layout.nb_channels; ch++) {
            //         for (int s = 0; s <data_size; s++) {
            //         pcmlen %= 220500;
            //         double val = 32767 * sin(pcmlen++ * totalPeriodW);
            //         memset(frame->data[ch]+ i*data_size +s, (short)(val * gainFactor), 2);
            //         }
            //     }
            // }

            // static int64_t pcmlen = 0;
            // int ch = 0;
            // uint8_t *pcmbuf = frame->data[0];
            // double gainFactor = pow(10, -6/20.0);
            // double totalPeriodW = 2*M_PI/(44100*1/1000);
            // memset(frame->data[ch], 0, data_size);
            // for (int i=0; i<frame->nb_samples; i++) {
            //     pcmlen %= 220500;
            //     double val = 32767 * sin(pcmlen++ * totalPeriodW);
            //     av_log(NULL, AV_LOG_WARNING, "val %d\n", (short)(val * gainFactor));
            //     pcmbuf[2*i] = pcmbuf[2*i+1] = (short)(val * gainFactor);
            // }

            break;
        }

        if (ret == AVERROR(EAGAIN)) {
            avcodec_send_packet(m_audio_decoder_ctx, packet);
            av_log(NULL, AV_LOG_WARNING, "avcodec_receive_frame EAGAIN\n");
            continue;
            // break;
        } else if (ret == AVERROR_EOF) {
            av_log(NULL, AV_LOG_ERROR, "avcodec_receive_frame AVERROR_EOF\n");
            av_frame_unref(frame);
            av_frame_free(&frame);
            return packet;
        } else {
            // Error occurred
            av_log(NULL, AV_LOG_ERROR, "avcodec_receive_frame unknown error\n");
            av_frame_unref(frame);
            av_frame_free(&frame);
            return packet;
        }
    }
    
    while(true) {
        int ret = avcodec_send_frame(m_audio_encoder_ctx, frame);
        if (ret >= 0) {
            break;
        }

        if (ret == AVERROR(EAGAIN)) {
            av_log(NULL, AV_LOG_WARNING, "avcodec_send_frame EAGAIN\n");
            continue;
        } else if (ret == AVERROR_EOF) {
            // End of file reached
            av_log(NULL, AV_LOG_ERROR, "avcodec_send_frame AVERROR_EOF\n");
            av_frame_unref(frame);
            av_frame_free(&frame);
            return packet;
        } else {
            // Error occurred
            av_log(NULL, AV_LOG_ERROR, "avcodec_send_frame unknown error\n");
            av_frame_unref(frame);
            av_frame_free(&frame);
            return packet;
        }
    }

    AVPacket *out_packet = av_packet_alloc();
    av_init_packet(out_packet);
    while(true) {
        int ret = avcodec_receive_packet(m_audio_encoder_ctx, out_packet);
        if (ret >= 0) {
            break;
        }

        if (ret == AVERROR(EAGAIN)) {
            avcodec_send_frame(m_audio_encoder_ctx, frame);
            av_log(NULL, AV_LOG_WARNING, "avcodec_receive_packet EAGAIN\n");
            continue;
        } else if (ret == AVERROR_EOF) {
            // End of file reached
            av_log(NULL, AV_LOG_ERROR, "avcodec_receive_packet AVERROR_EOF\n");
            av_frame_unref(frame);
            av_frame_free(&frame);
            av_packet_unref(out_packet);
            av_packet_free(&out_packet);
            return packet;
        } else {
            // Error occurred
            printf("avcodec_receive_packet unkown\n");
            av_log(NULL, AV_LOG_ERROR, "avcodec_receive_packet unknown error\n");
            av_frame_unref(frame);
            av_frame_free(&frame);
            av_packet_unref(out_packet);
            av_packet_free(&out_packet);
            return packet;
        }
    }

    out_packet->pts = orig_pts;
    out_packet->dts = orig_dts;
    out_packet->stream_index = m_audio_stream_index;
    // av_packet_rescale_ts(out_packet, m_audio_decoder_ctx->time_base, m_audio_encoder_ctx->time_base);
    out_packet->duration = orig_duration;
    //int64_t ms = av_rescale_q(out_packet->pts, m_time_base, (AVRational){1,1000});
    //printf("mute audio pts orig_pts  %d packet %d %d\n", out_packet->duration, out_packet->pts, ms);
    // printf("mute audio %d dur orig  %d\n", orig_duration, out_packet->duration);
    // av_log(NULL, AV_LOG_INFO, "mute audio size orig  %d\n", out_packet->size);
    av_frame_unref(frame);
    av_frame_free(&frame);
    av_packet_unref(packet);
    av_packet_free(&packet);
    return out_packet;
}