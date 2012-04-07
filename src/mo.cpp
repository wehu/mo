/* Copyright (C) 2009-2012 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Written by Wei Hu <huwei04@hotmail.com>, 2012.  */

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
