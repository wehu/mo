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
