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
