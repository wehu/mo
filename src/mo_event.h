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
