#ifndef __MO_LOGGER_H__
#define __MO_LOGGER_H__

#include "libguile.h"
#include "uv.h"

namespace Mo {

class Logger{
  enum VERBOSE_LEVEL {
    
  };
  private:
  static SCM soc_loc_func;
  static void SCMErr(SCM);
  static void SCMInfo(SCM);
  static void SCMDebug(SCM);
  static void RegSocLocFunc(SCM); 
  public:
  static void Err(const char *, ...);
  static void Info(const char *, ...);
  static void Debug(const char *, ...);

  static void ShowSourceLocation(SCM);

  static void RegisterSCMFunctions();
  static void Init();
};

}
#endif
