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
