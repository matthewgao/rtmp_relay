#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/log.h>
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libavutil/time.h>
#include <math.h>

#define SAMPLE_RATE 44100
#define BEEP_DURATION 1 // beep持续时间，单位为秒
#define BEEP_FREQUENCY 1000 // beep音频频率
#define BEEP_AMPLITUDE 0.5 // beep音频幅度
#define AAC_FRAME_SIZE 1024 // AAC编码的帧大小

// 生成指定数量的 beep 音频样本
static void generate_beep(int16_t *samples, int sample_count) {
    for (int i = 0; i < sample_count; i++) {
        // 生成正弦波
        double t = (double)i / SAMPLE_RATE;
        samples[i] = (int16_t)(sin(2 * M_PI * BEEP_FREQUENCY * t) * 32767 * BEEP_AMPLITUDE); // 16位音频
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input rtmp> <output rtmp>\n", argv[0]);
        return -1;
    }

    const char* input_url = argv[1];
    const char* output_url = argv[2];

    // av_register_all();
    avformat_network_init();

    AVFormatContext *input_format_context = NULL;
    AVFormatContext *output_format_context = NULL;

    // 打开输入流
    if (avformat_open_input(&input_format_context, input_url, NULL, NULL) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not open input stream\n");
        return -1;
    }

    // 查找流信息
    if (avformat_find_stream_info(input_format_context, NULL) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not find stream information\n");
        avformat_close_input(&input_format_context);
        return -1;
    }

    // 创建输出流
    avformat_alloc_output_context2(&output_format_context, NULL, "flv", output_url);
    if (!output_format_context) {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
        avformat_close_input(&input_format_context);
        return -1;
    }

    // 复制视频流
    for (int i = 0; i < input_format_context->nb_streams; i++) {
        AVStream *in_stream = input_format_context->streams[i];
        AVStream *out_stream = avformat_new_stream(output_format_context, NULL);
        if (!out_stream) {
            av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
            avformat_free_context(output_format_context);
            avformat_close_input(&input_format_context);
            return -1;
        }

        // 复制参数
        if (avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar) < 0) {
            av_log(NULL, AV_LOG_ERROR, "Failed to copy codec parameters\n");
            avformat_free_context(output_format_context);
            avformat_close_input(&input_format_context);
            return -1;
        }
    }

    // 打开输出流
    if (!(output_format_context->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&output_format_context->pb, output_url, AVIO_FLAG_WRITE) < 0) {
            av_log(NULL, AV_LOG_ERROR, "Could not open output URL\n");
            avformat_free_context(output_format_context);
            avformat_close_input(&input_format_context);
            return -1;
        }
    }

    avformat_write_header(output_format_context, NULL);

    AVPacket pkt;
    int audio_stream_index = -1;

    // 找到音频流索引
    for (int i = 0; i < input_format_context->nb_streams; i++) {
        if (input_format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            break;
        }
    }

    // 音频编码器
    AVCodecContext *codec_context = NULL;
    AVCodec *codec = NULL;

    // 获取音频编码器
    codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    if (!codec) {
        av_log(NULL, AV_LOG_ERROR, "Codec not found\n");
        avformat_free_context(output_format_context);
        avformat_close_input(&input_format_context);
        return -1;
    }

    // 创建编码上下文
    codec_context = avcodec_alloc_context3(codec);
    if (!codec_context) {
        av_log(NULL, AV_LOG_ERROR, "Could not allocate codec context\n");
        avformat_free_context(output_format_context);
        avformat_close_input(&input_format_context);
        return -1;
    }

    codec_context->sample_rate = SAMPLE_RATE;
    codec_context->channel_layout = AV_CH_LAYOUT_STEREO; // 设置为立体声
    codec_context->channels = 2;
    codec_context->sample_fmt = codec->sample_fmts[0]; // 获取默认音频格式
    codec_context->bit_rate = 128000;

    // 打开编码器
    if (avcodec_open2(codec_context, codec, NULL) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not open codec\n");
        avcodec_free_context(&codec_context);
        avformat_free_context(output_format_context);
        avformat_close_input(&input_format_context);
        return -1;
    }

    // 计算 beep 的样本数量
    int beep_samples = SAMPLE_RATE * BEEP_DURATION;
    int16_t *beep_buffer = (int16_t *)av_malloc(beep_samples * sizeof(int16_t));
    if (!beep_buffer) {
        av_log(NULL, AV_LOG_ERROR, "Could not allocate beep buffer\n");
        avcodec_free_context(&codec_context);
        avformat_free_context(output_format_context);
        avformat_close_input(&input_format_context);
        return -1;
    }
    generate_beep(beep_buffer, beep_samples); // 生成 beep

    // 为编码后的包分配内存
    AVPacket beep_pkt;

    int audio_frame_count = 0;
    int64_t last_beep_time = 0; // 用于跟踪上次 beep 的时间
    while (1) {
        if (av_read_frame(input_format_context, &pkt) < 0) break;

        // 检查流类型
        if (pkt.stream_index == audio_stream_index) {
            audio_frame_count++;
            int64_t current_time = av_gettime(); // 获取当前时间
            if (current_time - last_beep_time >= 2000000) { // 每2秒插入一个 beep
                // 创建一个新的音频帧
                AVFrame *beep_frame = av_frame_alloc();
                if (!beep_frame) {
                    av_log(NULL, AV_LOG_ERROR, "Could not allocate beep frame\n");
                    av_packet_unref(&pkt);
                    break;
                }

                beep_frame->nb_samples = AAC_FRAME_SIZE; // 确保与AAC帧大小匹配
                beep_frame->format = AV_SAMPLE_FMT_S16; // PCM格式
                beep_frame->channels = codec_context->channels;

                // 分配音频帧的缓冲区
                if (av_frame_get_buffer(beep_frame, 0) < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Could not allocate audio data buffers\n");
                    av_frame_free(&beep_frame);
                    av_packet_unref(&pkt);
                    break;
                }

                // 将 beep 数据复制到帧
                for (int i = 0; i < AAC_FRAME_SIZE; i++) {
                    int index = i % beep_samples; // 循环使用 beep 数据
                    beep_frame->data[0][i * 2] = beep_buffer[index]; // 左声道
                    beep_frame->data[0][i * 2 + 1] = beep_buffer[index]; // 右声道
                }

                // 将帧发送到编码器
                if (avcodec_send_frame(codec_context, beep_frame) < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Error sending frame to encoder\n");
                    av_frame_free(&beep_frame);
                    av_packet_unref(&pkt);
                    break;
                }

                // 从编码器接收编码包
                av_init_packet(&beep_pkt);
                beep_pkt.data = NULL;
                beep_pkt.size = 0;

                if (avcodec_receive_packet(codec_context, &beep_pkt) == 0 && beep_pkt.size > 0) {
                    beep_pkt.stream_index = audio_stream_index;
                    av_interleaved_write_frame(output_format_context, &beep_pkt);
                    last_beep_time = current_time; // 更新上次 beep 时间
                }

                av_frame_free(&beep_frame);
            } else {
                // 直接转发音频包
                av_interleaved_write_frame(output_format_context, &pkt);
            }

            av_packet_unref(&pkt);
        } else {
            av_interleaved_write_frame(output_format_context, &pkt);
        }
    }

    av_write_trailer(output_format_context);

    // 释放资源
    avcodec_free_context(&codec_context);
    av_free(beep_buffer);
    avio_closep(&output_format_context->pb);
    avformat_free_context(output_format_context);
    avformat_close_input(&input_format_context);
    avformat_free_context(input_format_context);

    return 0;
}
