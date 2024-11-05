#include "replacer.h"
#include <chrono>

Replacer::Replacer(){

}

Replacer::~Replacer() {

}

int
Replacer::Init(AVFormatContext* input_format_context, const AVCodecParameters *par) {
    int ret = CreateAudioDecoder(input_format_context, par);
    if (ret < 0) {
        return ret;
    }

    ret = CreateAudioEncoder();
    if (ret < 0) {
        return ret;
    }
}

int
Replacer::CreateAudioDecoder(AVFormatContext* input_format_context, const AVCodecParameters *par) {
    // Find the audio stream
    // AVCodec *m_audio_decoder = NULL;
    const AVCodec* temp_decoder = NULL;

    int audio_stream_index = av_find_best_stream(input_format_context, AVMEDIA_TYPE_AUDIO, -1, -1, &temp_decoder, 0);
    if (audio_stream_index < 0) {
        fprintf(stderr, "Could not find an audio stream\n");
        return -1;
    }
    m_audio_decoder = const_cast<AVCodec *>(temp_decoder);

    // Create a codec context for the m_audio_decoder
    m_audio_decoder_ctx = avcodec_alloc_context3(m_audio_decoder);
    if (!m_audio_decoder_ctx) {
        fprintf(stderr, "Could not allocate a decoding context\n");
        return -1;
    }

    // avcodec_parameters_to_context(m_audio_decoder_ctx, input_format_ctx->streams[audio_stream_index]->codecpar);
    avcodec_parameters_to_context(m_audio_decoder_ctx, par);

    if (avcodec_open2(m_audio_decoder_ctx, m_audio_decoder, NULL) < 0) {
        fprintf(stderr, "Could not open m_audio_decoder\n");
        avcodec_free_context(&m_audio_decoder_ctx);
        return -1;
    }

    if (m_audio_decoder_ctx == NULL) {
        /* code */
        return -1;
    }
    
    return 0;
}

int
Replacer::CreateAudioEncoder() {
    m_audio_encoder = avcodec_find_encoder(m_audio_decoder_ctx->codec_id);

    av_log(NULL, AV_LOG_ERROR, "codec id is%d\n", m_audio_decoder_ctx->codec_id);
    if (!m_audio_encoder) {
        fprintf(stderr, "Necessary encoder not found\n");
        return -1;
    }

    m_audio_encoder_ctx = avcodec_alloc_context3(m_audio_encoder);
    if (!m_audio_encoder_ctx) {
        fprintf(stderr, "Could not allocate an encoding context\n");
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
Replacer::ReplaceAudioToMute(AVPacket *packet) {
    AVFrame *frame = av_frame_alloc();
    // AVFrame frame;
    // AVFrame *filtered_frame = av_frame_alloc();

    // AVPacket *out_packet;
    // out_packet = (AVPacket*)av_mallocz(sizeof(AVPacket));
    // av_init_packet(out_packet);
    int64_t orig = packet->pts;
    int64_t dts = packet->dts;
    int64_t duration = packet->duration;
    
    while(true) {
        int ret = avcodec_send_packet(m_audio_decoder_ctx, packet);
        if (ret >= 0) {
            break;
        }

        if (ret == AVERROR(EAGAIN)) {
            printf("avcodec_send_packet EAGAIN\n");
            continue;
        } else if (ret == AVERROR_EOF) {
            // End of file reached
            printf("avcodec_send_packet AVERROR_EOF\n");
            av_frame_unref(frame);
            return packet;
        } else {
            // Error occurred
            av_frame_unref(frame);
            return packet;
        }
    }

    while(true) {
        int ret = avcodec_receive_frame(m_audio_decoder_ctx, frame);
        if (ret >= 0) {
            int data_size = av_get_bytes_per_sample(m_audio_decoder_ctx->sample_fmt);
            for (int i = 0; i < frame->nb_samples; i++)
                for (int ch = 0; ch < m_audio_decoder_ctx->ch_layout.nb_channels; ch++){
                    // fwrite(frame->data[ch] + data_size*i, 1, data_size, outfile);
                    memset(frame->data[ch]+ data_size*i, 0, data_size);
            }

            break;
        }

        if (ret == AVERROR(EAGAIN)) {
            // avcodec_send_packet(m_audio_decoder_ctx, packet);
            // printf("avcodec_receive_frame EAGAIN\n");
            // continue;
            break;
        } else if (ret == AVERROR_EOF) {
            // End of file reached
            av_frame_unref(frame);
            return packet;
        } else {
            // Error occurred
            printf("avcodec_receive_frame unkown\n");
            av_frame_unref(frame);
            return packet;
        }
    }
    
    while(true) {
        int ret = avcodec_send_frame(m_audio_encoder_ctx, frame);
        if (ret >= 0) {
            break;
        }

        if (ret == AVERROR(EAGAIN)) {
            printf("avcodec_send_frame EAGAIN\n");
            continue;
        } else if (ret == AVERROR_EOF) {
            // End of file reached
            av_frame_unref(frame);
            return packet;
        } else {
            // Error occurred
            printf("avcodec_send_frame unkown\n");
            av_frame_unref(frame);
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
            // avcodec_send_frame(m_audio_encoder_ctx, frame);
            // printf("avcodec_receive_packet EAGAIN\n");
            // continue;
            break;
        } else if (ret == AVERROR_EOF) {
            // End of file reached
            av_frame_unref(frame);
            return packet;
        } else {
            // Error occurred
            printf("avcodec_receive_packet unkown\n");
            av_frame_unref(frame);
            return packet;
        }
    }
    out_packet->pts = orig;
    out_packet->dts = dts;
    out_packet->stream_index = 1;
    // av_packet_rescale_ts(out_packet, m_audio_decoder_ctx->time_base, m_audio_encoder_ctx->time_base);
    out_packet->duration = duration;
    printf("mute audio pts orig  %d packet %d\n", orig, out_packet->pts);
    printf("mute audio %d dur orig  %d\n", duration,out_packet->duration);
    printf("mute audio size orig  %d\n", out_packet->size);
    av_frame_unref(frame);
    av_packet_unref(packet);
    av_packet_free(&packet);
    return out_packet;
}