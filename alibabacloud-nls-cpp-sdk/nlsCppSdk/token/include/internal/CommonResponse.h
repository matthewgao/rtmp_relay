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

#ifndef ALIBABANLS_COMMON_COMMONRESPONSE_H_
#define ALIBABANLS_COMMON_COMMONRESPONSE_H_

#include <string>

namespace AlibabaNlsCommon {

class CommonResponse {
 public:
  CommonResponse();
  explicit CommonResponse(const std::string &payload);
  ~CommonResponse();

  std::string payload() const;

 private:
  std::string payload_;
};

}  // namespace AlibabaNlsCommon
#endif  // !ALIBABANLS_COMMON_COMMONRESPONSE_H_
