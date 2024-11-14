extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/log.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
}
#include "processor/relayer.h"

const char * BANNER = R"(
██████╗ ████████╗███╗   ███╗██████╗     ██████╗ ███████╗██╗      █████╗ ██╗   ██╗
██╔══██╗╚══██╔══╝████╗ ████║██╔══██╗    ██╔══██╗██╔════╝██║     ██╔══██╗╚██╗ ██╔╝
██████╔╝   ██║   ██╔████╔██║██████╔╝    ██████╔╝█████╗  ██║     ███████║ ╚████╔╝ 
██╔══██╗   ██║   ██║╚██╔╝██║██╔═══╝     ██╔══██╗██╔══╝  ██║     ██╔══██║  ╚██╔╝  
██║  ██║   ██║   ██║ ╚═╝ ██║██║         ██║  ██║███████╗███████╗██║  ██║   ██║   
╚═╝  ╚═╝   ╚═╝   ╚═╝     ╚═╝╚═╝         ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝   ╚═╝   
)";

std::string g_appkey = "";
std::string g_akId = "";
std::string g_akSecret = "";
std::string g_token = "";
std::string g_in_url = "";
std::string g_out_url = "";
std::string g_dict_file = "";
int g_delay = 15;
int g_sentence_gap = 200; //200ms
bool g_enable_grpc = false;
std::string g_grpc_host = "";

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
        } else if (!strcmp(argv[index], "--dict")) {
            index++;
            if (invalied_argv(index, argc)) return 1;
            g_dict_file = argv[index];
        } else if (!strcmp(argv[index], "--delay")) {
            index++;
            if (invalied_argv(index, argc)) return 1;
            g_delay = atoi(argv[index]);
        } else if (!strcmp(argv[index], "--maxSentenceGap")) {
            index++;
            if (invalied_argv(index, argc)) return 1;
            g_sentence_gap = atoi(argv[index]);
        } else if (!strcmp(argv[index], "--grpc")) {
            g_enable_grpc = true;
        } else if (!strcmp(argv[index], "--grpcHost")) {
            index++;
            if (invalied_argv(index, argc)) return 1;
            g_grpc_host = argv[index];
        } 
        index++;
    }

    if (g_in_url.empty() || g_out_url.empty()) {
        return 1;
    }

    return 0;
}

void my_log_callback(void* ptr, int level, const char* fmt, va_list vl) {
    if (level > av_log_get_level()) {
        return; // 如果当前级别大于设置的级别，返回
    }

    // 获取当前时间戳
    int64_t currentTime = av_gettime(); // 获取当前时间（微秒）
    // 转换为秒
    double seconds = currentTime / 1000000.0;

    // 输出时间戳和日志信息
    fprintf(stderr, "[%f] ", seconds); // 打印时间戳
    vfprintf(stderr, fmt, vl); // 打印日志信息
}

int main(int argc, char *argv[]) {
    cout<< BANNER <<endl;
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
        << "  --dict dictionary.txt\n"
        << "  --delay 10\n"
        << "  --maxSentenceGap 200, >200\n"
        << "  --grpc\n"
        << "  --grpcHost 127.0.0.1:1234\n"
        << "eg:\n"
        << "  ./rtmp_relay --in xxxxxx --out xxxxxx\n"
        << std::endl;
        return -1;
    }

    av_log(NULL, AV_LOG_INFO, "from [%s] to [%s]\n", g_in_url.c_str(), g_out_url.c_str());
    av_log(NULL, AV_LOG_INFO, "dict [%s]\n", g_dict_file.c_str());

    avformat_network_init();

    // 设置自定义的日志回调
    // av_log_set_callback(my_log_callback);

    // 设置日志级别（可根据需求调整）
    av_log_set_level(AV_LOG_INFO);


    Relayer relayer(g_in_url.c_str(), g_out_url.c_str());
    relayer.setKey(g_akId, g_akSecret, g_appkey);
    relayer.setDictFile(g_dict_file);
    relayer.setDelaySec(g_delay);
    relayer.setMaxSentenceSilence(g_sentence_gap);
    if(g_enable_grpc) {
        relayer.enableGrpc();
        relayer.setGrpcHost(g_grpc_host);
    }
    int ret = relayer.init();
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "relayer init fail %d\n", ret);
        return -1;
    }
    
    relayer.startProcess();

    return 0;
}
