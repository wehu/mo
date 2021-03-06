/*
* Copyright 2012 The MO Authors. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#include "mo_logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include "mo_utils.h"

namespace Mo {

using namespace std;

SCM Logger::soc_loc_func = NULL;

void Logger::Err(const char * format, ...){
  va_list ap;
  va_start(ap, format);
  printf("ERROR: ");
  vprintf(format, ap);
  printf("\n");
  va_end(ap);
}

void Logger::SCMErr(SCM s){
  CheckArgType(s, scm_string_p, "err", 1);
  Err(scm_to_locale_string(s));
}

void Logger::Info(const char * format, ...){
  va_list ap;
  va_start(ap, format);
  printf("INFO: ");
  vprintf(format, ap);
  printf("\n");
  va_end(ap);
}

void Logger::SCMInfo(SCM s){
  CheckArgType(s, scm_string_p, "info", 1);
  Info(scm_to_locale_string(s));
}

void Logger::Debug(const char * format, ...){
  va_list ap;
  va_start(ap, format);
  printf("DEBUG: ");
  vprintf(format, ap);
  printf("\n");
  va_end(ap);
}

void Logger::SCMDebug(SCM s){
  CheckArgType(s, scm_string_p, "debug", 1);
  Debug(scm_to_locale_string(s));
}

void Logger::ShowSourceLocation(SCM obj){
  if(soc_loc_func)
    scm_call_1(soc_loc_func, obj);
}

void Logger::RegSocLocFunc(SCM cb){
  soc_loc_func = cb;
}

void Logger::RegisterSCMFunctions(){
  //scm_c_define_gsubr("err", 1, 0, 0, (void *)SCMErr);
  //scm_c_define_gsubr("info", 1, 0, 0, (void *)SCMInfo);
  //scm_c_define_gsubr("debug", 1, 0, 0, (void *)SCMDebug);
  scm_c_define_gsubr("register-source-location-function", 1, 0, 0, (void *)RegSocLocFunc);
  scm_c_eval_string("(read-enable 'positions)");
  scm_c_eval_string("(register-source-location-function \
                       (lambda (obj) \
                         (display \"  @\") \
                         (let ((loc (if (syntax-source obj) \
                                       (syntax-source obj) \
                                       (if (procedure-source obj) \
                                         (procedure-source obj) \
                                         (current-source-location))))) \
                           (if (list? loc) \
                             (map \
                               (lambda (p) \
                                 (display (car p)) \
                                 (display \": \") \
                                 (display (cdr p)) \
                                 (display \"; \")) \
                             loc))) \
                         (display \"\\n\")))");
}

void Logger::Init(){
}

}
