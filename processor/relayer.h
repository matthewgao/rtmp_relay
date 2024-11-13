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

class Delayer;
class Asr;
class Replacer;

class Relayer {
public:
    Relayer(const string& in_url, const string& out_url);
    virtual ~Relayer();

    int init();
    void startProcess();
    void setKey(const string& akId, const string& akSecret, const string& appkey);
    void setDictFile(const string& filename);
    void setDelaySec(int delay) {
        m_delay_sec = delay;
    }

    void setMaxSentenceSilence(int gap_ms) {
        m_max_sentence_silence_ms = gap_ms;
    }

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
    string m_dict_filename;
    shared_ptr<Delayer> m_delayer = nullptr;
    shared_ptr<Asr> m_asr = nullptr;
    shared_ptr<Replacer> m_replacer = nullptr;
    int m_delay_sec;
    int m_max_sentence_silence_ms;
};