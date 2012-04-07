#ifndef __MO_H__
#define __MO_H__

#include <libguile.h>
#include "uv.h"

// Mo
namespace Mo {

class Main {
  public:
  static SCM Run();

  static void RegisterSCMFunctions();

  static void Init();
};

}
#endif
