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
