#ifndef __MO_TTY_H__
#define __MO_TTY_H__

#include "uv.h"
#include "libguile.h"
#include "mo_stream.h"

namespace Mo {

class TTY : public Stream {
  private:
  static SCM New(SCM fd, SCM readable);
  static SCM SetMode(SCM id, SCM mode);
  static SCM ResetMode();
  static SCM GetWinSize(SCM id);

  static uv_tty_t * GetHandle(TTY * t);
  static TTY * GetTTY(uv_tty_t * handle); 
  
  public:
  TTY(uv_file fd, int readable);
  virtual ~TTY();

  static void RegisterSCMFunctions();
  static void Init();
};

}

#endif
