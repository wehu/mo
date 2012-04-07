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

#include <stdlib.h>
#include "mo.h"
#include "mo_logger.h"

using namespace Mo;

static void inner_main(void *data, int argc, char **argv){

  Main::RegisterSCMFunctions();
  Main::Init();

  if (argc == 1)
    //TODO need async debug mode.
    scm_shell(argc, argv);
  else if (argc == 2){
    scm_c_primitive_load(argv[1]);
    Main::Run();
  } else {
    Logger::Err("Unexpected arguments!");
    exit(-1);
  }

  uv_err_t last_err = uv_last_error(uv_default_loop());
  if(last_err.code != UV_OK && last_err.code!= UV_EOF){
    Logger::Err(uv_strerror(last_err));    
  }
}

int main(int argc, char **argv){
  scm_boot_guile(argc, argv, inner_main, 0);
  return 0;
}
