/*
* Copyright 2012 The MO Authors. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#ifndef __MO_UDP_H__
#define __MO_UDP_H__

#include "mo_stream.h"

namespace Mo {

class UDP : public Stream {
  private:

  static uv_udp_t * GetHandle(UDP * t);
  static UDP * GetUDP(uv_udp_t * handle);

  static void OnSend(uv_udp_send_t* req, int status);
  static void OnRecv(uv_udp_t *handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags);

  static SCM Bind(SCM, SCM, SCM);
  static SCM Bind6(SCM, SCM, SCM, SCM);

  static SCM SetMembership(SCM, SCM, SCM, SCM);
  static SCM SetMulticastLoop(SCM, SCM);
  static SCM SetMulticastTTL(SCM, SCM);
  static SCM SetBroadcast(SCM, SCM);
  static SCM SetTTL(SCM, SCM);

  static SCM Send(SCM, SCM, SCM, SCM);
  static SCM Send6(SCM, SCM, SCM, SCM);

  static SCM Receive(SCM);
  static SCM StopReceive(SCM);
 
  public:
  UDP();
  virtual ~UDP();
  static SCM New();
  static void RegisterSCMFunctions();
  static void Init();
};

}

#endif
