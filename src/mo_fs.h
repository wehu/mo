#ifndef __MO_FS_H__
#define __MO_FS_H__

#include "libguile.h"
#include "uv.h"
#include "mo_object.h"

namespace Mo {

class FS{
  private:
  static void OnFS(uv_fs_t * req);
  static SCM Open(SCM fn, SCM flags, SCM mode, SCM cb);
  static SCM Close(SCM fd, SCM cb);
  public:
  FS();
  virtual ~FS();
  static void RegisterSCMFunctions();
  static void Init();
};

}

#endif
