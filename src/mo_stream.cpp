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
#include "mo_stream.h"
#include <utility>
#include "mo_utils.h"
#include <assert.h>
#include "mo_logger.h"
#include "mo_tcp.h"
#include "mo_pipe.h"

using namespace std;

namespace Mo {

uv_stream_t * Stream::GetHandle(Stream * s){
  return &(Object::GetHandle(s)->stream);
}

Stream * Stream::GetStream(uv_stream_t * handle){
  return (Stream *)Object::GetObject((mo_any_handle *)handle);
}

SCM Stream::On(SCM id, SCM e, SCM cb){
  //CheckArgType(id, scm_integer_p, "stream-on", 1);
  assert_object_type(id);
  CheckArgType(e, scm_string_p, "stream-on", 2);
  CheckArgType(cb, scm_procedure_p, "steam-on", 3);
  Stream * s = (Stream*)get_object(id);
  assert(s!=NULL);
  string ev = scm_to_locale_string(e);
  s->Object::On(ev, cb);
  return id;
}

uv_buf_t Stream::OnAlloc(uv_handle_t * handle, size_t suggested_size){
  Stream * s = GetStream((uv_stream_t *)handle);
  assert(s!=NULL);
  s->Object::Run("alloc", scm_list_1(scm_from_uint64(suggested_size)));
  uv_buf_t buf;
  buf.base = (char *)malloc(suggested_size);
  assert(buf.base!=NULL);
  buf.len = suggested_size;
  return buf;
}

void Stream::OnConnection(uv_stream_t * server, int status){
  Stream * s = GetStream(server);
  assert(s!=NULL);
  SCM c;
  switch(s->type){
    case MO_TCP: c = TCP::New(); break;
    default: c = Pipe::New();
  }
  assert(c!=NULL);
  uv_stream_t * client = GetHandle((Stream *)get_object(c));
  assert(client!=NULL);
  int r = uv_accept(server, client);
  if(r) Logger::Err("uv_accept failed! : %d", r);
  s->Object::Run("connection", scm_list_1(c));
}

void Stream::OnConnect(uv_connect_t *req, int status){
  assert(req!=NULL);
  Stream * s = GetStream((uv_stream_t *)(req->handle));
  assert(s!=NULL);
  s->Object::Run("connect", scm_list_1(s->smob));
  free(req);
}

void Stream::OnRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf){
  uv_err_t err = uv_last_error(uv_default_loop());
  Stream * s = GetStream((uv_stream_t *)handle);
  assert(s!=NULL);
  if (nread > 0) {
    s->Object::Run("read", scm_list_1(Buf2List(buf, nread)));
  } else if (nread < 0) {
    if (err.code == UV_EOF) {
      s->Object::Run("readend", SCM_EOL);
      uv_close((uv_handle_t*)handle, OnClose);
    }
  };
  free(buf.base);
}

void Stream::OnWrite(uv_write_t* req, int status){
  assert(req!=NULL);
  Stream * s = GetStream((uv_stream_t *)(req->handle));
  assert(s!=NULL);
  s->Object::Run("write", scm_list_1(s->smob));
  free(req);
}

void Stream::OnShutDown(uv_shutdown_t* req, int status){
  assert(req!=NULL);
  Stream * s = GetStream((uv_stream_t *)(req->handle));
  assert(s!=NULL);
  s->Object::Run("shutdown", scm_list_1(s->smob));
  free(req);
}

SCM Stream::Resume(SCM id){
  //CheckArgType(id, scm_integer_p, "stream-on", 1);
  assert_object_type(id);
  Stream *s = (Stream*)get_object(id);
  assert(s!=NULL);
  uv_stream_t * h = GetHandle(s);
  if(h && uv_is_readable(h)){
    int r = uv_read_start(h, 
                  OnAlloc,
                  OnRead);
    if(r) Logger::Err("uv_read_start failed! : %d", r);
  };
  return id;
}

SCM Stream::Pause(SCM id){
  //CheckArgType(id, scm_integer_p, "stream-pause", 1);
  assert_object_type(id);
  Stream *s = (Stream*)get_object(id);
  assert(s!=NULL);
  uv_stream_t * h = GetHandle(s);
  //if(!uv_is_closing((uv_handle_t *)s)){
    if(h && uv_is_readable(h)){
      int r = uv_read_stop(h);
      if(r) Logger::Err("uv_read_stop failed! : %d", r);
    };
  //};
  return id;
}

SCM Stream::Write(SCM id, SCM data){
  //CheckArgType(id, scm_integer_p, "stream-write", 1);
  assert_object_type(id);
  CheckArgType(data, scm_list_p, "stream-write", 2);
  Stream *s = (Stream*)get_object(id);
  assert(s!=NULL);
  uv_stream_t * h = GetHandle(s);
  if(h && uv_is_writable(h)) {
    uv_write_t * req = (uv_write_t *)malloc(sizeof(uv_write_t));
    assert(req!=NULL);
    uv_buf_t buf = List2Buf(data);
    int r = uv_write(req, h, &buf, 1, OnWrite);
    if(r) Logger::Err("uv_write failed! : %d", r);
  };
  return id;
}

SCM Stream::Destroy(SCM id){
  //CheckArgType(id, scm_integer_p, "stream-pause", 1);
  assert_object_type(id);
  Stream *s = (Stream*)get_object(id);
  assert(s!=NULL);
  uv_stream_t * h = GetHandle(s);
  if(h)
    uv_close((uv_handle_t*)h, OnClose);
  return id;
}

SCM Stream::ShutDown(SCM id){
  //CheckArgType(id, scm_integer_p, "stream-pause", 1);
  assert_object_type(id);
  Stream *s = (Stream*)get_object(id);
  assert(s!=NULL);
  uv_stream_t * h = GetHandle(s);
  if(h){
    uv_shutdown_t * req = (uv_shutdown_t *)malloc(sizeof(uv_shutdown_t));;
    int r = uv_shutdown(req, h, OnShutDown);
    if(r) Logger::Err("uv_shutdown failed! : %d", r);
  }
  return id;
}

//  SCM Pipe();
//};

SCM Stream::IsReadable(SCM id){
  //CheckArgType(id, scm_integer_p, "stream-readable?", 1);
  assert_object_type(id);
  Stream *s = (Stream*)get_object(id);
  assert(s!=NULL);
  uv_stream_t * h = GetHandle(s);
  if(h)
    if(uv_is_readable(h))
      return SCM_BOOL_T;
    else
      return SCM_BOOL_F;
  else
    return SCM_BOOL_F;
}

SCM Stream::IsWritable(SCM id){
  //CheckArgType(id, scm_integer_p, "stream-writable?", 1);
  assert_object_type(id);
  Stream *s = (Stream*)get_object(id);
  assert(s!=NULL);
  uv_stream_t * h = GetHandle(s);
  if(h)
    if(uv_is_writable(h))
      return SCM_BOOL_T;
    else
      return SCM_BOOL_F;
  else
    return SCM_BOOL_F;
}

SCM Stream::Listen(SCM id, SCM backlog, SCM cb){
  //CheckArgType(id, scm_integer_p, "stream-listen", 1);
  assert_object_type(id);
  CheckArgType(backlog, scm_integer_p, "stream-listen", 2);
  CheckArgType(cb, scm_procedure_p, "stream-listen", 3);
  Stream *s = (Stream*)get_object(id);
  assert(s!=NULL);
  int bl = scm_to_int(backlog);
  uv_stream_t * h = GetHandle(s);
  if(h){
    int r = uv_listen(h, bl, OnConnection);
    if(r) Logger::Err("uv_listen failed! : %d", r);
  }
  return id;
}

SCM Stream::Accept(SCM server, SCM client){
  //CheckArgType(server, scm_integer_p, "stream-accept", 1);
  assert_object_type(server);
  //CheckArgType(client, scm_integer_p, "stream-accept", 2);
  assert_object_type(client);
  Stream *s = (Stream*)get_object(server);
  assert(s!=NULL);
  uv_stream_t * sh = GetHandle(s);
  Stream *c = (Stream*)get_object(client);
  assert(c!=NULL);
  uv_stream_t * ch = GetHandle(c);
  if(sh && ch){
    int r = uv_accept(sh, sh);
    if(r) Logger::Err("uv_accept failed! : %d", r);
  }
  return server;
}

Stream::Stream(){
};

Stream::~Stream(){
}

void Stream::RegisterSCMFunctions(){
  scm_c_define_gsubr("stream-on", 3, 0, 0, (void *)On);
  scm_c_define_gsubr("stream-resume", 1, 0, 0, (void *)Resume);
  scm_c_define_gsubr("stream-pause", 1, 0, 0, (void *)Pause);
  scm_c_define_gsubr("stream-write", 2, 0, 0, (void *)Write);
  scm_c_define_gsubr("stream-destroy", 1, 0, 0, (void *)Destroy);
  scm_c_define_gsubr("stream-shutdown", 1, 0, 0, (void *)ShutDown);
  scm_c_define_gsubr("stream-readable?", 1, 0, 0, (void *)IsReadable);
  scm_c_define_gsubr("stream-writable?", 1, 0, 0, (void *)IsWritable);
}

void Stream::Init(){
}

}
