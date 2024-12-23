/*
 * Copyright 2009-2017 Alibaba Cloud All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ALIBABANLS_COMMON_FILETRANS_H_
#define ALIBABANLS_COMMON_FILETRANS_H_

#include <string>

#include "nlsGlobal.h"

namespace AlibabaNlsCommon {

enum FileTransEvent { TaskUnknown = 0, TaskFailed, TaskCompleted };

struct resultResponse {
  std::string taskId;
  FileTransEvent event;
  unsigned int statusCode;
  std::string errorMsg;
  std::string result;
};

struct resultRequest {
  void *client;
  std::string taskId;
  std::string domain;
  std::string serverVersion;
  struct resultResponse *response;
};

typedef void (*FileTransCallbackMethod)(void *, void *);

class NLS_SDK_CLIENT_EXPORT FileTrans {
 public:
  virtual ~FileTrans();
  FileTrans();

  /**
   * @brief 调用文件转写.
   * @note 调用之前, 请先设置请求参数.
   * @param sync 是否同步
   * @return 成功则返回0; 失败返回负值.
   */
  int applyFileTrans(bool sync = true);

  /**
   * @brief 获取当前请求的事件.
   * @return 返回当前请求的事件, 详见FileTransEvent.
   */
  int getEvent();

  /**
   * @brief 获取错误信息.
   * @return 成功则返回错误信息; 失败返回NULL.
   */
  const char *getErrorMsg();

  /**
   * @brief 获取结果.
   * @note
   * @return 成功则返回json格式字符串; 失败返回NULL.
   */
  const char *getResult();

  /**
   * @brief 获取当前TaskId.
   * @return 成功则返回错误信息; 失败返回NULL.
   */
  const char *getTaskId();

  struct resultRequest getRequestParams();

  /**
   * @brief 设置阿里云账号的KeySecret/stsKeySecret
   * @param KeySecret Secret字符串
   * @return void
   */
  void setKeySecret(const std::string &KeySecret);

  /**
   * @brief 设置阿里云账号的KeyId/stsKeyId
   * @param KeyId Id字符串
   * @return void
   */
  void setAccessKeyId(const std::string &accessKeyId);

  /**
   * @brief 设置阿里云账号的临时访问凭证stsToken
   * @param stsToken字符串
   * @return void
   */
  void setStsToken(const std::string &stsToken);

  /**
   * @brief 设置APPKEY
   * @param appKey
   * @return void
   */
  void setAppKey(const std::string &appKey);

  /**
   * @brief 设置音频文件连接地址
   * @param fileLinkUrl 音频文件Url地址
   * @return void
   */
  void setFileLinkUrl(const std::string &fileLinkUrl);

  /**
   * @brief 设置地域ID
   * @param regionId 服务地区
   * @return void
   */
  void setRegionId(const std::string &regionId);

  /**
   * @brief 设置功能
   * @param action 功能
   * @return void
   */
  void setAction(const std::string &action);

  /**
   * @brief 设置域名
   * @param domain 域名字符串
   * @return void
   */
  void setDomain(const std::string &domain);

  /**
   * @brief 设置API版本
   * @param serverVersion API版本字符串
   * @return void
   */
  void setServerVersion(const std::string &serverVersion);

  /**
   * @brief 输入参数
   * @param customJsonString json字符串
   *
   * @return void
   */
  void setCustomParam(const std::string &customJsonString);

  /**
   * @brief 设置输出文本的编码格式
   * @param textFormat 编码格式 UTF-8 or GBK
   * @return void
   */
  void setOutputFormat(const std::string &textFormat);

  void setEventListener(FileTransCallbackMethod event, void *param = NULL);

  int applyResultRequest(struct resultRequest param);

 private:
#if defined(__ANDROID__) || defined(__linux__)
  int codeConvert(char *from_charset, char *to_charset, char *inbuf,
                  size_t inlen, char *outbuf, size_t outlen);
#endif

  /**
   * @brief 字符串UTF8转GBK
   * @param strUTF8 UTF8格式字符串
   * @return 转换成GBK后字符串
   */
  std::string utf8ToGbk(const std::string &strUTF8);

  int paramCheck();

 private:
  std::string accessKeySecret_;
  std::string accessKeyId_;
  std::string stsToken_;

  std::string domain_;
  std::string serverVersion_;

  std::string regionId_;
  std::string endpointName_;
  std::string product_;
  std::string action_;
  std::string appKey_;
  std::string fileLink_;
  std::string serverResourcePath_;
  std::string outputFormat_;

  std::string customParam_;

  FileTransCallbackMethod eventHandler_;
  void *paramHandler_;

  struct resultRequest requestParams_;
  struct resultResponse resultResponse_;
};

}  // namespace AlibabaNlsCommon

#endif  // ALIBABANLS_COMMON_FILETRANS_H_
