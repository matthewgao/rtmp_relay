extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/log.h>
#include <libavutil/opt.h>
}
#include "processor/relayer.h"

std::string g_appkey = "";
std::string g_akId = "";
std::string g_akSecret = "";
std::string g_token = "";
std::string g_in_url = "";
std::string g_out_url = "";

int 
invalied_argv(int index, int argc) {
    if (index >= argc) {
        std::cout << "invalid params..." << std::endl;
        return 1;
    }

    return 0;
}

int 
parse_argv(int argc, char *argv[]) {
    int index = 1;
    while (index < argc) {
        if (!strcmp(argv[index], "--appkey")) {
            index++;
            if (invalied_argv(index, argc)) return 1;
            g_appkey = argv[index];
        } else if (!strcmp(argv[index], "--akId")) {
            index++;
            if (invalied_argv(index, argc)) return 1;
            g_akId = argv[index];
        } else if (!strcmp(argv[index], "--akSecret")) {
            index++;
            if (invalied_argv(index, argc)) return 1;
            g_akSecret = argv[index];
        } else if (!strcmp(argv[index], "--token")) {
            index++;
            if (invalied_argv(index, argc)) return 1;
            g_token = argv[index];
        } else if (!strcmp(argv[index], "--in")) {
            index++;
            if (invalied_argv(index, argc)) return 1;
            g_in_url = argv[index];
        } else if (!strcmp(argv[index], "--out")) {
            index++;
            if (invalied_argv(index, argc)) return 1;
            g_out_url = argv[index];
        }
        index++;
    }

    if (g_in_url.empty() || g_out_url.empty()) {
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (parse_argv(argc, argv) != 0) {
        av_log(NULL, AV_LOG_ERROR, "wrong params\n");
        std::cout
        << "params is not valid.\n"
        << "Usage:\n"
        << "  --appkey <appkey>\n"
        << "  --akId <AccessKey ID>\n"
        << "  --akSecret <AccessKey Secret>\n"
        << "  --token <Token>\n"
        << "  --in <rtmp addr>\n"
        << "  --out <rtmp addr>\n"
        << "eg:\n"
        << "  ./rtmp_relay --in xxxxxx --out xxxxxx\n"
        << std::endl;
        return -1;
    }

    av_log(NULL, AV_LOG_INFO, "from [%s] to [%s]\n", g_in_url.c_str(), g_out_url.c_str());

    avformat_network_init();

    Relayer relayer(g_in_url.c_str(), g_out_url.c_str());
    int ret = relayer.init();
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "relayer init fail %d\n", ret);
    }
    
    relayer.startProcess();

    return 0;
}
