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

#ifndef __STREAM_H__
#define __STREAM_H__

#include "uv.h"
#include "libguile.h"
#include <string>
#include <map>
#include "mo_object.h"

namespace Mo {

typedef enum {
  MO_PIPE = 0,
  MO_TCP,
  MO_UDP
} MO_STREAM_T;

class Stream : public Object {
  
  public:

  Stream();
  virtual ~Stream();

  static uv_stream_t * GetHandle(Stream * s);
  static Stream * GetStream(uv_stream_t * handle);

  MO_STREAM_T type;

  static uv_buf_t OnAlloc(uv_handle_t * handle, size_t suggested_size);
  static void OnRead(uv_stream_t* s, ssize_t nread, uv_buf_t buf);
  static void OnWrite(uv_write_t* req, int status);
  static void OnConnection(uv_stream_t * server, int status);
  static void OnConnect(uv_connect_t *req, int status);
  static void OnShutDown(uv_shutdown_t *req, int status);

  static SCM On(SCM, SCM, SCM);
  static SCM Pause(SCM);
  static SCM Resume(SCM);
  static SCM Write(SCM, SCM);
  static SCM Destroy(SCM);
  static SCM ShutDown(SCM);
  //static SCM Pipe(SCM);

  static SCM IsReadable(SCM);
  static SCM IsWritable(SCM);

  static SCM Listen(SCM, SCM, SCM);
  static SCM Accept(SCM, SCM);

  static void RegisterSCMFunctions();

  static void Init();
};

}

#endif
