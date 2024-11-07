#pragma once
extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <iostream>

using namespace std;

class Relayer {
public:
    Relayer(string in_url, string out_url);
    ~Relayer();

    int init();
    void startProcess();
    void setKey(string akId, string akSecret, string appkey);

    Relayer (const Relayer&) = delete;
    Relayer& operator= (const Relayer&) = delete;

private:
    string m_in_url;
    string m_out_url;
    AVFormatContext *m_input_format_context = NULL;
    AVFormatContext *m_output_format_context = NULL;
    string m_appkey;
    string m_akId;
    string m_akSecret;
    int m_audio_stream_index;
    int m_video_stream_index;
};