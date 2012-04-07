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
#include "mo_timer.h"
#include "mo_logger.h"
#include "mo_utils.h"
#include <string>
#include <assert.h>

// timer

using namespace std;

namespace Mo {

Timer::Timer(uint64_t delay){
  repeat = delay;
  int r = uv_timer_init(uv_default_loop(), (uv_timer_t *)&handle);
  if(r) Logger::Err("uv_timer_init failed! : %d", r);
}

Timer::~Timer(){
}

uv_timer_t *Timer::GetHandle(Timer * timer){
  return &(Object::GetHandle(timer)->timer);
}

Timer * Timer::GetTimer(uv_timer_t * handle){
  return (Timer *)Object::GetObject((mo_any_handle *)handle);
}

Timer * Timer::Start(SCM cb, SCM delay){
  Timer * timer = new Timer(scm_to_uint64(delay));
  assert(timer!=NULL);
  timer->Object::On("timeout", cb);
  int r = uv_timer_start(GetHandle(timer), OnTimeOut, scm_to_int64(delay), timer->repeat);
  if(r) Logger::Err("uv_timer_start failed! : %d", r);
  return timer;
}

void Timer::Stop(Timer * timer){
  assert(timer!=NULL);
  uv_timer_t * h = GetHandle(timer);
  int r = uv_timer_stop(GetHandle(timer));
  if(r) Logger::Err("uv_timer_stop failed! : %d", r);
  uv_unref(uv_default_loop());
  //Close(timer);
  delete timer;
}

void Timer::Close(Timer * timer){
  assert(timer!=NULL);
  uv_handle_t * h = (uv_handle_t *)GetHandle(timer);
  if(uv_is_active(h)){
    uv_close(h, OnClose);
  }
}

void Timer::OnTimeOut(uv_timer_t* handle, int status){
  Timer * timer = GetTimer(handle);
  assert(timer!=NULL);
  timer->Object::Run("timeout", SCM_EOL);
  if(timer->repeat > 0){
    uv_timer_set_repeat(handle, timer->repeat);
    int r = uv_timer_again(handle);
    if(r) Logger::Err("uv_timer_again failed! : %d", r);
  } else
    Stop(timer);
}

SCM Timer::SetTimeout(SCM cb, SCM delay){
  CheckArgType(cb, scm_procedure_p, "set-timeout", 1);
  CheckArgType(delay, scm_integer_p, "set-timeout", 2);
  Timer * timer = Start(cb, delay);
  assert(timer!=NULL);
  timer->repeat = 0;
  return timer->smob;
}

SCM Timer::SetInterval(SCM cb, SCM delay){
  CheckArgType(cb, scm_procedure_p, "set-interval", 1);
  CheckArgType(delay, scm_integer_p, "set-interval", 2);
  Timer * timer = Start(cb, delay);
  assert(timer!=NULL);
  return timer->smob;
}

SCM Timer::ClearTimer(SCM id){
  //CheckArgType(id, scm_integer_p, "clear-interval", 1);
  assert_object_type(id);
  Timer * timer = (Timer*)get_object(id);
  assert(timer!=NULL);
  timer->repeat = 0;
  timer->Object::Remove("timeout");
  return id;
}

SCM Timer::On(SCM id, SCM e, SCM cb){
  //CheckArgType(id, scm_integer_p, "timer-on", 1);
  assert_object_type(id);
  CheckArgType(e, scm_string_p, "timer-on", 2);
  CheckArgType(cb, scm_procedure_p, "timer-on", 3);
  Timer * timer = (Timer *)get_object(id);
  assert(timer!=NULL);
  string ev = scm_to_locale_string(e);
  timer->Object::On(ev, cb);
  return id; 
}

void Timer::RegisterSCMFunctions(){

  scm_c_define_gsubr("set-timeout", 2, 0, 0, (void *)SetTimeout);
  scm_c_define_gsubr("set-interval", 2, 0, 0, (void *)SetInterval);
  scm_c_define_gsubr("clear-timer", 1, 0, 0, (void *)ClearTimer);

  scm_c_define_gsubr("timer-on", 3, 0, 0, (void *)On);
}

void Timer::Init(){
}

}
