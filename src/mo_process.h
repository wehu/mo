/* Copyright (C) 2009-2012 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Written by Wei Hu <huwei04@hotmail.com>, 2012.  */

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
