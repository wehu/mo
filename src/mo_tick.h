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

#ifndef __MO_TICK_H__
#define __MO_TICK_H__

#include <libguile.h>
#include "uv.h"
#include <queue>
#include <string>
#include <map>

// tick

namespace Mo {

class Tick {

  private:
  static std::queue<std::pair<SCM, SCM> > queue;

  static uv_prepare_t prepare_watcher;
  static uv_check_t   check_watcher;
  static uv_idle_t    spinner;

  static void RunOne();

  static void Prepare(uv_prepare_t* handle, int status);
  static void Check(uv_check_t* handle, int status);
  static void Idle(uv_idle_t* handle, int status);

  public:
  static SCM New(SCM cb, SCM args);
  static SCM New(SCM cb);

  static void RegisterSCMFunctions();
  static void Init();
};

}

#endif
