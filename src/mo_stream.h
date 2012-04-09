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

#ifndef __STREAM_H__
#define __STREAM_H__

#include "uv.h"
#include "libguile.h"
#include <string>
#include <map>
#include "mo_object.h"

namespace Mo {

typedef enum {
  MO_PIPE = 0,
  MO_TCP,
  MO_UDP
} MO_STREAM_T;

class Stream : public Object {
  
  public:

  Stream();
  virtual ~Stream();

  static uv_stream_t * GetHandle(Stream * s);
  static Stream * GetStream(uv_stream_t * handle);

  MO_STREAM_T type;

  static uv_buf_t OnAlloc(uv_handle_t * handle, size_t suggested_size);
  static void OnRead(uv_stream_t* s, ssize_t nread, uv_buf_t buf);
  static void OnWrite(uv_write_t* req, int status);
  static void OnConnection(uv_stream_t * server, int status);
  static void OnConnect(uv_connect_t *req, int status);
  static void OnShutDown(uv_shutdown_t *req, int status);

  static SCM On(SCM, SCM, SCM);
  static SCM Pause(SCM);
  static SCM Resume(SCM);
  static SCM Write(SCM, SCM);
  static SCM Destroy(SCM);
  static SCM ShutDown(SCM);
  //static SCM Pipe(SCM);

  static SCM IsReadable(SCM);
  static SCM IsWritable(SCM);

  static SCM Listen(SCM, SCM);
  static SCM Accept(SCM, SCM);

  static void RegisterSCMFunctions();

  static void Init();
};

}

#endif
