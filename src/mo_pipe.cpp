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

#include "mo_pipe.h"
#include <stdlib.h>
#include "mo_utils.h"
#include <assert.h>
#include "mo_logger.h"

namespace Mo {

SCM Pipe::New(){
  Pipe * p = new Pipe();
  assert(p!=NULL);
  p->type = MO_PIPE;
  return p->smob;
}

SCM Pipe::NewIPC(){
  Pipe * p = new Pipe(1);
  assert(p!=NULL);
  p->type = MO_PIPE;
  return p->smob;
}

uv_pipe_t * Pipe::GetHandle(Pipe * p){
  return &(Object::GetHandle(p)->pipe);
}

Pipe * Pipe::GetPipe(uv_pipe_t * handle){
  return (Pipe *)Object::GetObject((mo_any_handle *)handle);
}

Pipe::Pipe(int _ipc){
  ipc = _ipc;
  int r = uv_pipe_init(uv_default_loop(), (uv_pipe_t*)&handle, ipc);
  if(r) Logger::Err("uv_pipe_init failed! : %d", r);
};

SCM Pipe::IsIPC(SCM id){
  assert_object_type(id);
  Pipe * p = (Pipe *)get_object(id);
  assert(p!=NULL);
  if(p->ipc)
    return SCM_BOOL_T;
  else
    return SCM_BOOL_F;
}

Pipe::~Pipe(){
}

SCM Pipe::Bind(SCM id, SCM fn){
  //CheckArgType(id, scm_integer_p, "pipe-bind", 1);
  assert_object_type(id);
  CheckArgType(fn, scm_string_p, "pipe-bind", 2);
  Pipe * p = (Pipe *)get_object(id);
  assert(p!=NULL);
  char * f = scm_to_locale_string(fn);
  uv_pipe_t * h = GetHandle(p);
  if(h){
    int r = uv_pipe_bind(h, f);
    if(r) Logger::Err("uv_pipe_bind failed! : %d", r);
  }
  return id;
}

SCM Pipe::Open(SCM id, SCM fd){
  //CheckArgType(id, scm_integer_p, "pipe-open", 1);
  assert_object_type(id);
  CheckArgType(fd, scm_integer_p, "pipe-open", 2);
  Pipe * p = (Pipe *)get_object(id);
  assert(p!=NULL);
  uv_file fh = scm_to_int(fd);
  uv_pipe_t * h = GetHandle(p);
  if(h){
    uv_pipe_open(h, fh);
  }
  return id;
}

SCM Pipe::Connect(SCM id, SCM fn, SCM cb){
  //CheckArgType(id, scm_integer_p, "pipe-connect", 1);
  assert_object_type(id);
  CheckArgType(fn, scm_string_p, "pipe-connect", 2);
  CheckArgType(cb, scm_procedure_p, "pipe-connect", 3);
  Pipe * p = (Pipe *)get_object(id);
  assert(p!=NULL);
  char * f = scm_to_locale_string(fn);
  p->Object::On("connect", cb);
  uv_pipe_t * h = GetHandle(p);
  if(h){
    uv_connect_t * req = (uv_connect_t *)malloc(sizeof(uv_connect_t));
    assert(req!=NULL);
    uv_pipe_connect(req, h, f, OnConnect);
  }
  return id;
}

void Pipe::OnRead2(uv_pipe_t* handle, ssize_t nread, uv_buf_t buf, uv_handle_type pending){
  uv_err_t err = uv_last_error(uv_default_loop());
  Pipe * p = GetPipe(handle);
  assert(p!=NULL);
  if (nread > 0) {
    p->Object::Run("read2", scm_list_1(Buf2List(buf, nread)));
  } else if (nread < 0) {
    if (err.code == UV_EOF) {
      //uv_close((uv_handle_t*)handle, OnClose);
    }
  };
  free(buf.base);
}

SCM Pipe::Resume2(SCM id){
  //CheckArgType(id, scm_integer_p, "pipe-resume2", 1);
  assert_object_type(id);
  Pipe * p = (Pipe*)get_object(id);
  assert(p!=NULL);
  uv_stream_t * h = (uv_stream_t *)GetHandle(p);
  if(h && uv_is_readable(h) && p->ipc){
    int r = uv_read2_start(h,
                  OnAlloc,
                  OnRead2);
    if(r) Logger::Err("uv_read2_start failed! : %d", r);
  };
  return id;
}

SCM Pipe::Write2(SCM id, SCM data, SCM s_id){
  //CheckArgType(id, scm_integer_p, "pipe-write2", 1);
  assert_object_type(id);
  CheckArgType(data, scm_list_p, "pipe-write2", 2);
  assert_object_type(s_id);
  Pipe *p = (Pipe *)get_object(id);
  assert(p!=NULL);
  uv_stream_t * h = (uv_stream_t *)GetHandle(p);
  if(h && uv_is_writable(h) && p->ipc) {
    Stream * s = (Stream *)get_object(s_id);
    uv_write_t * req = (uv_write_t *)malloc(sizeof(uv_write_t));
    assert(req!=NULL);
    uv_buf_t buf = List2Buf(data);
    int r = uv_write2(req, h, &buf, 1, Stream::GetHandle(s), OnWrite);
    if(r) Logger::Err("uv_write2 failed! : %d", r);
  };
  return id;
}

void Pipe::RegisterSCMFunctions(){
  scm_c_define_gsubr("pipe-new", 0, 0, 0, (void *)New);
  scm_c_define_gsubr("pipe-new-ipc", 0, 0, 0, (void *)NewIPC);

  scm_c_define_gsubr("pipe-ipc?", 1, 0, 0, (void *)IsIPC);

  scm_c_define_gsubr("pipe-on", 3, 0, 0, (void *)On);
  scm_c_define_gsubr("pipe-resume", 1, 0, 0, (void *)Resume);
  scm_c_define_gsubr("pipe-resume2", 1, 0, 0, (void *)Resume2);
  scm_c_define_gsubr("pipe-pause", 1, 0, 0, (void *)Pause);
  scm_c_define_gsubr("pipe-write", 2, 0, 0, (void *)Write);
  scm_c_define_gsubr("pipe-write2", 3, 0, 0, (void *)Write2);
  scm_c_define_gsubr("pipe-destroy", 1, 0, 0, (void *)Destroy);
  scm_c_define_gsubr("pipe-shutdown", 1, 0, 0, (void *)ShutDown);
  scm_c_define_gsubr("pipe-readable?", 1, 0, 0, (void *)IsReadable);
  scm_c_define_gsubr("pipe-writable?", 1, 0, 0, (void *)IsWritable);

  scm_c_define_gsubr("pipe-listen", 3, 0, 0, (void *)Listen);
  scm_c_define_gsubr("pipe-accept", 2, 0, 0, (void *)Accept);

  scm_c_define_gsubr("pipe-bind", 2, 0, 0, (void *)Bind);
  scm_c_define_gsubr("pipe-open", 2, 0, 0, (void *)Open);
  scm_c_define_gsubr("pipe-connect", 3, 0, 0, (void *)Connect);

}

void Pipe::Init(){
}

}
