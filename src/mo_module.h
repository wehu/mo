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

#ifndef __MO_MODULE_H__
#define __MO_MODULE_H__

#include <libguile.h>
#include "uv.h"
#include <string>
#include <map>

// Mo
namespace Mo {

typedef std::map<std::string, std::map<std::string, SCM> > export_map_t;

class Module {
  private:
  static SCM prefix;
  static export_map_t exports;
  static char * current_module_name;

  static SCM Require(SCM, SCM);
  static SCM Export(SCM);
  static void LoadFile(void * fn);

  public:

  static void RegisterSCMFunctions();

  static void Init();
};

}
#endif
