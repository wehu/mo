#ifndef __MO_MODULE_H__
#define __MO_MODULE_H__

#include <libguile.h>
#include "uv.h"
#include <string>
#include <map>

// Mo
namespace Mo {

typedef std::map<std::string, std::map<std::string, SCM> > export_map_t;

class Module {
  private:
  static SCM prefix;
  static export_map_t exports;
  static char * current_module_name;

  static SCM Require(SCM, SCM);
  static SCM Export(SCM);
  static void LoadFile(void * fn);

  public:

  static void RegisterSCMFunctions();

  static void Init();
};

}
#endif
