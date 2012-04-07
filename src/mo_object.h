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

#ifndef __MO_OBJECT_H__
#define __MO_OBJECT_H__

#include <libguile.h>
#include "uv.h"
#include <queue>
#include <string>
#include <map>

// event

namespace Mo {

typedef std::map <std::string, std::vector<SCM> > event_map_t;

union mo_any_handle {
  uv_tcp_t tcp;
  uv_pipe_t pipe;
  uv_process_t process;
  uv_stream_t stream;
  uv_prepare_t prepare;
  uv_check_t check;
  uv_idle_t idle;
  uv_async_t async;
  uv_timer_t timer;
  uv_getaddrinfo_t getaddrinfo;
  uv_fs_event_t fs_event;
  uv_tty_t tty;
};

class Object {
  struct mo_handle_t {
    mo_any_handle uv_fields;
    Object * obj;
  };
  protected:
  event_map_t events;
  mo_handle_t handle;

  static mo_any_handle * GetHandle(Object *);
  static Object * GetObject(mo_any_handle *);

  static void OnClose(uv_handle_t *);

  bool with_smob;

  public:
  SCM smob;

  Object();
  virtual ~Object();
  void On(std::string e, SCM cb);
  void Remove(std::string e);
  void Notify(std::string e, SCM args);
  void Run(std::string e, SCM args);
};

void init_mo_object_type();
Object * get_object(SCM id);
void assert_object_type(SCM id);

}
#endif
