#ifndef __MO_EVENT_H__
#define __MO_EVENT_H__

#include <libguile.h>
#include "uv.h"
#include <queue>
#include <string>
#include <map>
#include "mo_object.h"

namespace Mo {

// event

class Event : public Object {
  private:

  static SCM On(SCM e, SCM cb);
  static SCM Remove(SCM e);
  static SCM Notify(SCM e, SCM args);
  static SCM Run(SCM e, SCM args);

  public:
  static void RegisterSCMFunctions();
  static void Init();
};

}

#endif
