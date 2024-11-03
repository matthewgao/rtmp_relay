#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/log.h>
#include <libavutil/opt.h>

#define AUDIO_DISCARD_INTERVAL 100

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
        return -1;
    }

    // 创建输出流
    avformat_alloc_output_context2(&output_format_context, NULL, "flv", output_url);
    if (!output_format_context) {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
        return -1;
    }

    // 复制视频流
    for (int i = 0; i < input_format_context->nb_streams; i++) {
        AVStream *in_stream = input_format_context->streams[i];
        AVStream *out_stream = avformat_new_stream(output_format_context, NULL);
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
    if (!(output_format_context->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&output_format_context->pb, output_url, AVIO_FLAG_WRITE) < 0) {
            av_log(NULL, AV_LOG_ERROR, "Could not open output URL\n");
            return -1;
        }
    }

    avformat_write_header(output_format_context, NULL);

    AVPacket pkt;
    int audio_frame_count = 0;

    while (1) {
        if (av_read_frame(input_format_context, &pkt) < 0) break;

        // 检查流类型
        if (pkt.stream_index == 0) { // 假设视频流在索引 0
            av_interleaved_write_frame(output_format_context, &pkt);
        } else if (pkt.stream_index == 1) { // 假设音频流在索引 1
            audio_frame_count++;
            if (audio_frame_count % AUDIO_DISCARD_INTERVAL != 0) {
                for (size_t i = 1; i < pkt.size; i++)
                {
                    pkt.data[i] = 0;
                }

                av_interleaved_write_frame(output_format_context, &pkt);


                
                printf("drop %d\n", audio_frame_count);
            }
            av_packet_unref(&pkt);
        } else {
            av_packet_unref(&pkt);
        }
    }

    av_write_trailer(output_format_context);

    avio_closep(&output_format_context->pb);
    avformat_free_context(output_format_context);
    avformat_close_input(&input_format_context);
    avformat_free_context(input_format_context);

    return 0;
}
