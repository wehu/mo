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

#ifndef __MO_TIMER_H__
#define __MO_TIMER_H__

#include <libguile.h>
#include "uv.h"

#include "mo_object.h"

// timer

namespace Mo {

class Timer : public Object {

  public:
  Timer(uint64_t);
  virtual ~Timer();

  static void RegisterSCMFunctions();
  static void Init();

  private:
  uint64_t repeat;

  static uv_timer_t * GetHandle(Timer *);
  static Timer * GetTimer(uv_timer_t *);

  static SCM On(SCM id, SCM e, SCM cb);
  static void OnTimeOut(uv_timer_t*, int);

  static Timer * Start(SCM cb, SCM delay);
  static void Stop(Timer*);
  static void Close(Timer*);

  static SCM SetTimeout(SCM cb, SCM delay);
  static SCM SetInterval(SCM cb, SCM delay);
  static SCM ClearTimer(SCM id);

};

}

#endif
