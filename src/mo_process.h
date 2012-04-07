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

#ifndef __MO_PROCESS_H__
#define __MO_PROCESS_H__

#include <libguile.h>
#include "uv.h"
#include "mo_object.h"
#include <assert.h>

// process

namespace Mo {

class Process : public Object {
  public:
  uv_process_options_t opts;
  Process();
  virtual ~Process();
  private:
  static uv_process_t * GetHandle(Process*);
  static Process * GetProcess(uv_process_t *);

  static void OnExit(uv_process_t * p, int exit_status, int term_signal);

  static SCM On(SCM id, SCM e, SCM cb);

  static SCM New();
  static SCM SetFile(SCM id, SCM file);
  static SCM SetArgs(SCM id, SCM args);
  static SCM SetCwd(SCM id, SCM cwd);
  static SCM SetEnv(SCM id, SCM env);
  static SCM SetStdIn(SCM id, SCM s);
  static SCM SetStdOut(SCM id, SCM s);
  static SCM SetStdErr(SCM id, SCM s);
  static SCM SetOnExit(SCM id, SCM cb);

  static SCM GetFile(SCM id);
  static SCM GetArgs(SCM id);
  static SCM GetCwd(SCM id);
  static SCM GetEnv(SCM id);
  static SCM GetStdIn(SCM id);
  static SCM GetStdOut(SCM id);
  static SCM GetStdErr(SCM id);

  static SCM GetPid(SCM id);
  
  static SCM Run(SCM id);
  static SCM Kill(SCM id, SCM signum);

  public:
  static void RegisterSCMFunctions();

  static void Init();
};

}

#endif
