/*
* Copyright 2012 The MO Authors. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

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
