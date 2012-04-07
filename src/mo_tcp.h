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

#ifndef __MO_TCP_H__
#define __MO_TCP_H__

#include "mo_stream.h"

namespace Mo {

class TCP : public Stream {
  private:

  static uv_tcp_t * GetHandle(TCP * t);
  static TCP * GetTCP(uv_tcp_t * handle);

  static SCM Bind(SCM, SCM, SCM);
  static SCM Bind6(SCM, SCM, SCM);
  static SCM Connect(SCM, SCM, SCM, SCM);
  static SCM Connect6(SCM, SCM, SCM, SCM);

  static SCM SetNoDelay(SCM, SCM);
  static SCM KeepAlive(SCM, SCM, SCM);
 
  public:
  TCP();
  virtual ~TCP();
  static SCM New();
  static void RegisterSCMFunctions();
  static void Init();
};

}

#endif
