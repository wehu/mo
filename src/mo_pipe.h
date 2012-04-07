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

#ifndef __MO_PIPE_H__
#define __MO_PIPE_H__

#include "mo_stream.h"

namespace Mo {

class Pipe : public Stream {
  private:

  static SCM Open(SCM id, SCM fd);
  static SCM Bind(SCM id, SCM fn);
  static SCM Connect(SCM id, SCM fn, SCM cb);

  static void OnRead2(uv_pipe_t* handle, ssize_t nread, uv_buf_t buf, uv_handle_type pending);
  static SCM Resume2(SCM id);
  static SCM Write2(SCM id, SCM data, SCM s_id);

  int ipc;
 
  public:
  Pipe(int ipc=0);
  virtual ~Pipe();
  static SCM IsIPC(SCM);
  static SCM New();
  static SCM NewIPC();
  static uv_pipe_t * GetHandle(Pipe*);
  static Pipe * GetPipe(uv_pipe_t *);
  static void RegisterSCMFunctions();
  static void Init();
};

}

#endif
