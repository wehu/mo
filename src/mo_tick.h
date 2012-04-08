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

#ifndef __MO_TICK_H__
#define __MO_TICK_H__

#include <libguile.h>
#include "uv.h"
#include <queue>
#include <string>
#include <map>

// tick

namespace Mo {

class Tick {

  private:
  // tick queue
  static std::queue<std::pair<SCM, SCM> > queue;

  static uv_prepare_t prepare_watcher;
  static uv_check_t   check_watcher;
  static uv_idle_t    spinner;

  // run one tick
  static void RunOne();

  static void Prepare(uv_prepare_t* handle, int status);
  static void Check(uv_check_t* handle, int status);
  static void Idle(uv_idle_t* handle, int status);

  public:
  static SCM New(SCM cb, SCM args);
  static SCM New(SCM cb);

  static void RegisterSCMFunctions();
  static void Init();
};

}

#endif
