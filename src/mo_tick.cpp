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
#include <utility>
#include "mo_tick.h"
#include "mo_logger.h"
#include "mo_utils.h"

using namespace std;

namespace Mo {

// tick

queue<pair<SCM, SCM> > Tick::queue;

uv_prepare_t Tick::prepare_watcher;
uv_check_t   Tick::check_watcher;
uv_idle_t    Tick::spinner;

void Tick::RunOne(){
  if (!queue.empty()) {
    SCM cb = queue.front().first;
    SCM args = queue.front().second;
    scm_apply_0(cb, args);
    queue.pop();
    scm_gc_unprotect_object(cb);
    scm_gc_unprotect_object(args);
  }
  if(uv_is_active((uv_handle_t *) &spinner) && queue.empty()){
    int r = uv_idle_stop(&spinner);
    if(r) Logger::Err("uv_idle_stop failed! : %d", r);
    uv_unref(uv_default_loop());
  }
}

void Tick::Prepare(uv_prepare_t* handle, int status){
  RunOne();
}

void Tick::Check(uv_check_t* handle, int status){
  RunOne();
}

void Tick::Idle(uv_idle_t* handle, int status){
  RunOne();
}

SCM Tick::New(SCM cb, SCM args){
  CheckArgType(cb, scm_procedure_p, "next-tick", 1);
  CheckArgType(args, scm_list_p, "next-tick", 2);
  scm_gc_protect_object(cb);
  scm_gc_protect_object(args);
  queue.push(make_pair(cb, args));
  if(!uv_is_active((uv_handle_t *) &spinner)){
    int r = uv_idle_start(&spinner, Idle);
    if(r) Logger::Err("uv_idle_start failed! : %d", r);
    uv_ref(uv_default_loop());
  }
  return cb;
}

SCM Tick::New(SCM cb){
  return New(cb, SCM_EOL);
}

void Tick::RegisterSCMFunctions(){
  SCM (*f)(SCM) = New;
  scm_c_define_gsubr("next-tick", 1, 0, 0, (void *)f);
}

void Tick::Init(){

  int r=0; 
  r = uv_prepare_init(uv_default_loop(), &prepare_watcher);
  if(r) Logger::Err("uv_prepare_init failed! : %d", r);
  r = uv_prepare_start(&prepare_watcher, Prepare);
  if(r) Logger::Err("uv_prepare_start failed! : %d", r);
  uv_unref(uv_default_loop());

  r = uv_check_init(uv_default_loop(), &check_watcher);
  if(r) Logger::Err("uv_check_init failed! : %d", r);
  r = uv_check_start(&check_watcher, Check);
  if(r) Logger::Err("uv_check_start failed! : %d", r);
  uv_unref(uv_default_loop());

  r = uv_idle_init(uv_default_loop(), &spinner);
  if(r) Logger::Err("uv_idle_init failed! : %d", r);
  uv_unref(uv_default_loop());

}

}
