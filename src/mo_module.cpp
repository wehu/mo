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

#include <stdlib.h>
#include "mo_module.h"
#include "mo_logger.h"
#include "mo_utils.h"
#include "mo.h"
#include <utility>
#include <sys/stat.h>

using namespace std;

namespace Mo {

SCM Module::prefix;
export_map_t Module::exports;
char * Module::current_module_name = NULL;

static bool fexist(string filename){
  struct stat buf;
  if(stat(filename.c_str(), &buf)) return false;
  return true;
}

void Module::LoadFile(void * fn){
  string filename = (char *)fn;
  Main::RegisterSCMFunctions();
  //TODO add path searching
  if(fexist(filename)){
  } else {
    filename += ".mo";
    if(fexist(filename)){
    } else if(fexist(string("./mo_modules/") + filename)) {
      filename = string("./mo_modules/") + filename;
    } else if(fexist(string("/usr/local/lib/mo_modules/") + filename)){
      filename = string("/usr/local/lib/mo_modules/") + filename;
    } else if(fexist(string("/usr/lib/mo_modules/") + filename)){
      filename = string("/usr/lib/mo_modules/") + filename;
    } else if(fexist(string("/lib/mo_modules/") + filename)){
      filename = string("/lib/mo_modules/") + filename;
    }
  }
  scm_c_primitive_load(filename.c_str());
}

SCM Module::Require(SCM fn, SCM p){
  CheckArgType(fn, scm_string_p, "require-mo", 1);
  CheckArgType(p, scm_symbol_p, "require-mo", 2);
  scm_gc_unprotect_object(prefix);
  prefix = scm_symbol_to_string(p);
  scm_gc_protect_object(prefix);
  current_module_name = scm_to_locale_string(fn);
  if(exports.find(string(current_module_name))==exports.end()){
    exports.insert(make_pair(string(current_module_name), map<string, SCM>()));
    scm_c_define_module(current_module_name,
                      LoadFile,
                      current_module_name);
    scm_c_use_module(current_module_name);
  }
  if(exports.find(string(current_module_name))!=exports.end()){
    map<string, SCM> exps = exports[string(current_module_name)];
    map<string, SCM>::iterator it;
    for(it=exps.begin();it!=exps.end();it++){
      char * exp = scm_to_locale_string(scm_string_append(scm_list_3(
               prefix, 
               scm_from_locale_string("/"), 
               scm_from_locale_string((it->first.c_str())))));
      scm_c_define(exp, scm_variable_ref(it->second));
    }
  }
  scm_gc_unprotect_object(prefix);
  prefix = scm_string_to_symbol(scm_from_locale_string("global"));
  scm_gc_protect_object(prefix);
  current_module_name = NULL;
  return fn;
}

SCM Module::Export(SCM name){
  SCM var = scm_lookup(name);
  if(current_module_name != NULL){
    if(exports.find(string(current_module_name))!=exports.end()){
      exports[current_module_name].insert(make_pair(
                            string(scm_to_locale_string(scm_symbol_to_string(name))),
                            var));
      scm_gc_protect_object(var);
    }
  }
  return name;
}

void Module::RegisterSCMFunctions(){
  scm_c_define_gsubr("require-mo", 2, 0, 0, (void *)Require);
  scm_c_define_gsubr("export-mo", 1, 0, 0, (void *)Export);
}

void Module::Init(){

  prefix = scm_string_to_symbol(scm_from_locale_string("global"));
  scm_gc_protect_object(prefix);

}

}
