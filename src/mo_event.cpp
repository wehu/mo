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
#include <utility>
#include "mo_event.h"
#include "mo_utils.h"
#include "mo_logger.h"

using namespace std;

namespace Mo {

Object * events_g = new Object();

SCM Event::On(SCM e, SCM cb){
  CheckArgType(e, scm_string_p, "event-on", 1);
  CheckArgType(cb, scm_procedure_p, "event-on", 2);
  string ev = scm_to_locale_string(e);
  events_g->On(ev, cb);
  return e;
}

SCM Event::Remove(SCM e){
  CheckArgType(e, scm_string_p, "event-remove", 1);
  string ev = scm_to_locale_string(e);
  events_g->Remove(ev);
  return e;
}

SCM Event::Notify(SCM e, SCM args){
  CheckArgType(e, scm_string_p, "event-on", 1);
  CheckArgType(args, scm_list_p, "event-on", 2);
  string ev = scm_to_locale_string(e);
  events_g->Notify(ev, args);
  return e;
}

SCM Event::Run(SCM e, SCM args){
  CheckArgType(e, scm_string_p, "event-run", 1);
  CheckArgType(args, scm_list_p, "event-run", 2);
  string ev = scm_to_locale_string(e);
  events_g->Run(ev, args);
  return e;
}

void Event::RegisterSCMFunctions(){
  scm_c_define_gsubr("event-on", 2, 0, 0, (void *)On);
  scm_c_define_gsubr("event-remove", 1, 0, 0, (void *)Remove);
  scm_c_define_gsubr("event-notify", 2, 0, 0, (void *)Notify);
  scm_c_define_gsubr("event-run", 2, 0, 0, (void *)Run);
}

void Event::Init(){

}

}
