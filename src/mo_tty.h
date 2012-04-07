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
