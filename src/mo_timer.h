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

#ifndef __MO_TIMER_H__
#define __MO_TIMER_H__

#include <libguile.h>
#include "uv.h"

#include "mo_object.h"

// timer

namespace Mo {

class Timer : public Object {

  public:
  Timer(uint64_t);
  virtual ~Timer();

  static void RegisterSCMFunctions();
  static void Init();

  private:
  uint64_t repeat;

  static uv_timer_t * GetHandle(Timer *);
  static Timer * GetTimer(uv_timer_t *);

  static SCM On(SCM id, SCM e, SCM cb);
  static void OnTimeOut(uv_timer_t*, int);

  static Timer * Start(SCM cb, SCM delay);
  static void Stop(Timer*);
  static void Close(Timer*);

  static SCM SetTimeout(SCM cb, SCM delay);
  static SCM SetInterval(SCM cb, SCM delay);
  static SCM ClearTimer(SCM id);

};

}

#endif
