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
#include "mo_process.h"
#include "mo_pipe.h"
#include "mo_utils.h"
#include <string>
#include <assert.h>
#include "mo_logger.h"

// process

using namespace std;

namespace Mo {

Process::Process(){
  memset(&opts, 0, sizeof(uv_process_options_t));
  opts.exit_cb = OnExit;
}

Process::~Process(){
}

uv_process_t * Process::GetHandle(Process * p){
  return &(Object::GetHandle(p)->process);
}

Process * Process::GetProcess(uv_process_t * handle){
  return (Process *)Object::GetObject((mo_any_handle *)handle);
}

void Process::OnExit(uv_process_t * handle, int exit_status, int term_signal) {
  Process * p = GetProcess(handle);
  p->Object::Run("exit", scm_list_2(scm_from_int(exit_status), scm_from_int(term_signal)));
  uv_close((uv_handle_t *)handle, OnClose);
}

SCM Process::New(){
  Process * p = new Process();
  assert(p!=NULL);
  return p->smob;
}

SCM Process::SetFile(SCM id, SCM file){
  //CheckArgType(id, scm_integer_p, "process-set-file", 1);
  assert_object_type(id);
  CheckArgType(file, scm_string_p, "process-set-file", 2); 
  Process * p = (Process *)get_object(id);
  assert(p!=NULL);
  p->opts.file = scm_to_locale_string(file);
  return id;
}

SCM Process::SetArgs(SCM id, SCM args){
  //CheckArgType(id, scm_integer_p, "process-set-args", 1);
  assert_object_type(id);
  CheckArgType(args, scm_list_p, "process-set-args", 2);
  Process * p = (Process *)get_object(id);
  assert(p!=NULL);
  int len = scm_to_int(scm_length(args));
  p->opts.args = (char **)malloc(sizeof(char *) * (len+1));
  assert(p->opts.args!=NULL);
  for(int i=0; i<len; i++){
    p->opts.args[i] = scm_to_locale_string(scm_list_ref(args, scm_from_int(i)));
  }
  p->opts.args[len] = NULL;
  return id;
}

SCM Process::SetCwd(SCM id, SCM cwd){
  //CheckArgType(id, scm_integer_p, "process-set-cwd", 1);
  assert_object_type(id);
  CheckArgType(cwd, scm_string_p, "process-set-cwd", 2);
  Process * p = (Process *)get_object(id);
  assert(p!=NULL);
  p->opts.cwd = scm_to_locale_string(cwd);
  return id;
}

SCM Process::SetEnv(SCM id, SCM env){
  //CheckArgType(id, scm_integer_p, "process-set-env", 1);
  assert_object_type(id);
  CheckArgType(env, scm_list_p, "process-set-env", 2);
  Process * p = (Process *)get_object(id);
  assert(p!=NULL);
  int len = scm_to_int(scm_length(env));
  p->opts.env = (char **)malloc(sizeof(char *) * (len+1));
  assert(p->opts.env!=NULL);
  for(int i=0; i<len; i++){
    p->opts.env[i] = scm_to_locale_string(scm_list_ref(env, scm_from_int(i)));
  }
  p->opts.env[len] = NULL;
  return id;
}

SCM Process::SetStdIn(SCM p_id, SCM s_id){
  //CheckArgType(p_id, scm_integer_p, "process-set-stdin", 1);
  assert_object_type(p_id);
  //CheckArgType(s_id, scm_integer_p, "process-set-stdin", 2);
  assert_object_type(s_id);
  Process * p = (Process *)get_object(p_id);
  assert(p!=NULL);
  Pipe * s = (Pipe *)get_object(s_id);
  assert(s!=NULL);
  p->opts.stdin_stream = Pipe::GetHandle(s);
  return p_id;
}

SCM Process::SetStdOut(SCM p_id, SCM s_id){
  //CheckArgType(p_id, scm_integer_p, "process-set-stdout", 1);
  assert_object_type(p_id);
  //CheckArgType(s_id, scm_integer_p, "process-set-stdout", 2);
  assert_object_type(s_id);
  Process * p = (Process *)get_object(p_id);
  assert(p!=NULL);
  Pipe * s = (Pipe *)get_object(s_id);
  assert(s!=NULL);
  p->opts.stdout_stream = Pipe::GetHandle(s);
  return p_id;
}

SCM Process::SetStdErr(SCM p_id, SCM s_id){
  //CheckArgType(p_id, scm_integer_p, "process-set-stderr", 1);
  assert_object_type(p_id);
  //CheckArgType(s_id, scm_integer_p, "process-set-stderr", 2);
  assert_object_type(s_id);
  Process * p = (Process *)get_object(p_id);
  assert(p!=NULL);
  Pipe * s = (Pipe *)get_object(s_id);
  assert(s!=NULL);
  p->opts.stderr_stream = Pipe::GetHandle(s);
  return p_id;
}

SCM Process::SetOnExit(SCM id, SCM cb){
  //CheckArgType(id, scm_integer_p, "process-on-exit", 1);
  assert_object_type(id);
  CheckArgType(cb, scm_procedure_p,"process-on-exit", 2);
  Process * p = (Process *)get_object(id);
  assert(p!=NULL);
  p->Object::On("exit", cb);
  return id;
}

SCM Process::Run(SCM id){
  //CheckArgType(id, scm_integer_p, "process-run", 1);
  assert_object_type(id);
  Process * p = (Process *)get_object(id);
  assert(p!=NULL);
  int r = uv_spawn(uv_default_loop(), GetHandle(p), p->opts);
  if(r) Logger::Err("uv_spawn failed! : %d", r);
  return id;
}

SCM Process::On(SCM id, SCM e, SCM cb){
  //CheckArgType(id, scm_integer_p, "process-on", 1);
  assert_object_type(id);
  CheckArgType(e, scm_string_p, "process-on", 2);
  CheckArgType(cb, scm_procedure_p, "process-on", 3);
  Process * p = (Process *)get_object(id);
  assert(p!=NULL);
  string ev = scm_to_locale_string(e);
  p->Object::On(ev, cb);
  return id;
}

SCM Process::GetFile(SCM id){
  //CheckArgType(id, scm_integer_p, "process-on", 1);
  assert_object_type(id);
  Process * p = (Process *)get_object(id);
  return scm_from_locale_string(p->opts.file);
}

SCM Process::GetArgs(SCM id){
  //CheckArgType(id, scm_integer_p, "process-on", 1);
  assert_object_type(id);
  Process * p = (Process *)get_object(id);
  SCM args = SCM_EOL;
  if(p->opts.args != NULL){
    for(size_t i=0; p->opts.args[i] != NULL; i++){
      args = scm_append(scm_list_2(args, scm_list_1(scm_from_locale_string(p->opts.args[i]))));
    }
  }
  return args;
}

SCM Process::GetCwd(SCM id){
  //CheckArgType(id, scm_integer_p, "process-on", 1);
  assert_object_type(id);
  Process * p = (Process *)get_object(id);
  return scm_from_locale_string(p->opts.cwd);
}

SCM Process::GetEnv(SCM id){
  //CheckArgType(id, scm_integer_p, "process-on", 1);
  assert_object_type(id);
  Process * p = (Process *)get_object(id);
  SCM env = SCM_EOL;
  if(p->opts.env!=NULL){
    for(size_t i=0; p->opts.env[i] != NULL; i++){
      env = scm_append(scm_list_2(env, scm_list_1(scm_from_locale_string(p->opts.env[i]))));
    }
  }
  return env;
}

SCM Process::GetStdIn(SCM id){
  //CheckArgType(id, scm_integer_p, "process-on", 1);
  assert_object_type(id);
  Process * p = (Process *)get_object(id);
  return Pipe::GetPipe(p->opts.stdin_stream)->smob;
}

SCM Process::GetStdOut(SCM id){
  //CheckArgType(id, scm_integer_p, "process-on", 1);
  assert_object_type(id);
  Process * p = (Process *)get_object(id);
  return Pipe::GetPipe(p->opts.stdout_stream)->smob;
}

SCM Process::GetStdErr(SCM id){
  //CheckArgType(id, scm_integer_p, "process-on", 1);
  assert_object_type(id);
  Process * p = (Process *)get_object(id);
  return Pipe::GetPipe(p->opts.stderr_stream)->smob;
}

SCM Process::GetPid(SCM id){
  //CheckArgType(id, scm_integer_p, "process-on", 1);
  assert_object_type(id);
  Process * p = (Process *)get_object(id);
  return scm_from_int(GetHandle(p)->pid);
}

SCM Process::Kill(SCM id, SCM signum){
  //CheckArgType(id, scm_integer_p, "process-on", 1);
  assert_object_type(id);
  CheckArgType(signum, scm_integer_p, "process-kill", 2);
  Process * p = (Process *)get_object(id);
  int r = uv_process_kill(GetHandle(p), scm_to_int(signum));
  if(r) Logger::Err("uv_process_kill failed! : %d", r);
}

void Process::RegisterSCMFunctions(){
  scm_c_define_gsubr("process-new", 0, 0, 0, (void *)New);
  scm_c_define_gsubr("process-set-file", 2, 0, 0, (void *)SetFile);
  scm_c_define_gsubr("process-set-args", 2, 0, 0, (void *)SetArgs);
  scm_c_define_gsubr("process-set-env", 2, 0, 0, (void *)SetEnv);
  scm_c_define_gsubr("process-set-cwd", 2, 0, 0, (void *)SetCwd);
  scm_c_define_gsubr("process-set-stdin", 2, 0, 0, (void *)SetStdIn);
  scm_c_define_gsubr("process-set-stdout", 2, 0, 0, (void *)SetStdOut);
  scm_c_define_gsubr("process-set-stderr", 2, 0, 0, (void *)SetStdErr);
  scm_c_define_gsubr("process-on-exit", 2, 0, 0, (void *)SetOnExit);

  scm_c_define_gsubr("process-get-file", 1, 0, 0, (void *)GetFile);
  scm_c_define_gsubr("process-get-args", 1, 0, 0, (void *)GetArgs);
  scm_c_define_gsubr("process-get-env", 1, 0, 0, (void *)GetEnv);
  scm_c_define_gsubr("process-get-cwd", 1, 0, 0, (void *)GetCwd);
  scm_c_define_gsubr("process-get-stdin", 1, 0, 0, (void *)GetStdIn);
  scm_c_define_gsubr("process-get-stdout", 1, 0, 0, (void *)GetStdOut);
  scm_c_define_gsubr("process-get-stderr", 1, 0, 0, (void *)GetStdErr);

  scm_c_define_gsubr("process-get-pid", 1, 0, 0, (void *)GetPid);

  scm_c_define_gsubr("process-run", 1, 0, 0, (void *)Run);
  scm_c_define_gsubr("process-kill", 2, 0, 0, (void *)Kill);
  
  scm_c_define_gsubr("process-on", 3, 0, 0, (void *)On);
}

void Process::Init(){
}

}
