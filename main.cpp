extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/log.h>
#include <libavutil/opt.h>
}
#include "processor/relayer.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input rtmp> <output rtmp>\n", argv[0]);
        return -1;
    }

    const char* input_url = argv[1];
    const char* output_url = argv[2];

    av_log(NULL, AV_LOG_INFO, "from [%s] to [%s]\n", input_url, output_url);

    avformat_network_init();

    Relayer relayer(input_url, output_url);
    int ret = relayer.init();
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "relayer init fail %d\n", ret);
    }
    
    relayer.startProcess();

    return 0;
}
