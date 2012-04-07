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

#include <stdlib.h>
#include "mo.h"
#include "mo_logger.h"
#include "mo_tick.h"
#include "mo_timer.h"
#include "mo_event.h"
#include "mo_process.h"
#include "mo_pipe.h"
#include "mo_tcp.h"
#include "mo_fs.h"
#include "mo_tty.h"
#include "mo_module.h"

namespace Mo {

SCM Main::Run(){
  uv_run(uv_default_loop());
  return SCM_BOOL_T;
}

void Main::RegisterSCMFunctions(){
  scm_c_define_gsubr("run-mo", 0, 0, 0, (void *)Run);

  Logger::RegisterSCMFunctions();
  Event::RegisterSCMFunctions();

  Tick::RegisterSCMFunctions();
  Timer::RegisterSCMFunctions();
  Process::RegisterSCMFunctions();
  Pipe::RegisterSCMFunctions();
  TCP::RegisterSCMFunctions();
  FS::RegisterSCMFunctions();
  TTY::RegisterSCMFunctions();

  Module::RegisterSCMFunctions();

}

void Main::Init(){

  init_mo_object_type();

  // logger and event should be initialized first
  Logger::Init();
  Event::Init();

  uv_loop_t * loop = uv_loop_new();
  assert(loop!=NULL);

  Tick::Init();
  Timer::Init();
  Process::Init();
  Pipe::Init();
  TCP::Init();
  FS::Init();
  TTY::Init();

  Module::Init();

}

}
