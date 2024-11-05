#include "asr.h"

void OnRecognitionStarted(AlibabaNls::NlsEvent* cbEvent, void* cbParam) {
	printf("connect to nls service status code: %d task id: %s", cbEvent->getStatusCode(), cbEvent->getTaskId());
}

void OnRecognitionTaskFailed(AlibabaNls::NlsEvent* cbEvent, void* cbParam) {
	printf("connect to nls service status code: %d task id: %s", cbEvent->getStatusCode(), cbEvent->getTaskId());
}

void OnRecognitionChannelClosed(AlibabaNls::NlsEvent* cbEvent, void* cbParam) {
	printf("connect to nls service status code: %d task id: %s", cbEvent->getStatusCode(), cbEvent->getTaskId());
}

void OnRecognitionResultChanged(AlibabaNls::NlsEvent* cbEvent, void* cbParam) {
	printf("connect to nls service status code: %d task id: %s", cbEvent->getStatusCode(), cbEvent->getTaskId());
}

void OnRecognitionCompleted(AlibabaNls::NlsEvent* cbEvent, void* cbParam) {
	printf("connect to nls service status code: %d task id: %s", cbEvent->getStatusCode(), cbEvent->getTaskId());
}

Asr::Asr(){

}

Asr::~Asr() {
    AlibabaNls::NlsClient::releaseInstance();
}

void
Asr::sendAudio() {
    
}

void
Asr::init() {
    AlibabaNls::NlsClient::getInstance()->setSyncCallTimeout(10*1000);
    AlibabaNls::NlsClient::getInstance()->startWorkThread(2);
}

int 
Asr::checkToken() {
    std::time_t curTime = std::time(0);
    if (m_token.empty()) {
        if (m_expireTime - curTime < 10) {
            std::cout << "the token will be expired, please generate new token" << std::endl;
            int ret = generateToken();
            if (ret < 0) {
                std::cout << "generate token failed" << std::endl;
                return -1;
            } else {
                if (m_token.empty() || m_expireTime < 0) {
                std::cout << "generate empty token" << std::endl;
                return -2;
                }
            }
        }
    }
}

int 
Asr::generateToken() {
    AlibabaNlsCommon::NlsToken nlsTokenRequest;
    nlsTokenRequest.setAccessKeyId(m_akId);
    nlsTokenRequest.setKeySecret(m_akSecret);
    int retCode = nlsTokenRequest.applyNlsToken();
    /*获取失败原因*/
    if (retCode < 0) {
        std::cout << "Failed error code: " << retCode
                << "  error msg: " << nlsTokenRequest.getErrorMsg() << std::endl;
        return retCode;
    }

    m_token = nlsTokenRequest.getToken();
    m_expireTime = nlsTokenRequest.getExpireTime();

    return 0;
}

void
Asr::start() {
    checkToken();
    // pthread_t tid;
    AlibabaNls::SpeechRecognizerRequest *request =
        AlibabaNls::NlsClient::getInstance()->createRecognizerRequest("cpp", true);
    if (request == NULL) {
        std::cout << "createRecognizerRequest failed." << std::endl;
        // delete cbParam;
        // cbParam = NULL;
        return;
    }
    /*
    * 2. 设置用于接收结果的回调
    */
    // 设置start()成功回调函数
    request->setOnRecognitionStarted(OnRecognitionStarted, this);
    // 设置异常识别回调函数
    request->setOnTaskFailed(OnRecognitionTaskFailed, this);
    // 设置识别通道关闭回调函数
    request->setOnChannelClosed(OnRecognitionChannelClosed, this);
    // 设置中间结果回调函数
    request->setOnRecognitionResultChanged(OnRecognitionResultChanged, this);
    // 设置识别结束回调函数
    request->setOnRecognitionCompleted(OnRecognitionCompleted, this);
    // 设置所有服务端返回信息回调函数
    // request->setOnMessage(onRecognitionMessage, this);
    // request->setEnableOnMessage(true);

    request->setFormat("pcm");
}