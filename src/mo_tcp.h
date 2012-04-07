#ifndef __MO_TCP_H__
#define __MO_TCP_H__

#include "mo_stream.h"

namespace Mo {

class TCP : public Stream {
  private:

  static uv_tcp_t * GetHandle(TCP * t);
  static TCP * GetTCP(uv_tcp_t * handle);

  static SCM Bind(SCM, SCM, SCM);
  static SCM Bind6(SCM, SCM, SCM);
  static SCM Connect(SCM, SCM, SCM, SCM);
  static SCM Connect6(SCM, SCM, SCM, SCM);

  static SCM SetNoDelay(SCM, SCM);
  static SCM KeepAlive(SCM, SCM, SCM);
 
  public:
  TCP();
  virtual ~TCP();
  static SCM New();
  static void RegisterSCMFunctions();
  static void Init();
};

}

#endif
