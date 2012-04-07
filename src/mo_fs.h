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

#ifndef __MO_FS_H__
#define __MO_FS_H__

#include "libguile.h"
#include "uv.h"
#include "mo_object.h"

namespace Mo {

class FS{
  private:
  static void OnFS(uv_fs_t * req);
  static SCM Open(SCM fn, SCM flags, SCM mode, SCM cb);
  static SCM Close(SCM fd, SCM cb);
  public:
  FS();
  virtual ~FS();
  static void RegisterSCMFunctions();
  static void Init();
};

}

#endif
