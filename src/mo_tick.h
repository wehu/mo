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
  static std::queue<std::pair<SCM, SCM> > queue;

  static uv_prepare_t prepare_watcher;
  static uv_check_t   check_watcher;
  static uv_idle_t    spinner;

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
