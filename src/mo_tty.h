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
