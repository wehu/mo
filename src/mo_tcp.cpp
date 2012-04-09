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

#include "mo_tcp.h"
#include <stdlib.h>
#include "mo_utils.h"
#include "mo_pipe.h"
#include <assert.h>
#include "mo_logger.h"

namespace Mo {

SCM TCP::New(){
  TCP * p = new TCP();
  assert(p!=NULL);
  p->type = MO_TCP;
  return p->smob;
}

uv_tcp_t * TCP::GetHandle(TCP * t){
  return &(Object::GetHandle(t)->tcp);
}

TCP * TCP::GetTCP(uv_tcp_t * handle){
  return (TCP *)Object::GetObject((mo_any_handle *)handle);
}

SCM TCP::Bind(SCM id, SCM ip, SCM port){
  //CheckArgType(id, scm_integer_p, "tcp-bind", 1);
  assert_object_type(id);
  CheckArgType(ip, scm_string_p, "tcp-bind", 2);
  CheckArgType(port, scm_integer_p, "tcp-bind", 3);
  TCP * t = (TCP *)get_object(id);
  assert(t!=NULL);
  char * i = scm_to_locale_string(ip);
  int p = scm_to_int(port);
  uv_tcp_t * h = GetHandle(t);
  if(h){
    struct sockaddr_in addr;
    addr = uv_ip4_addr(i, p);
    int r = uv_tcp_bind(h, addr);
    if(r) Logger::Err("uv_tcp_bind failed! : %d", r);
  };
  return id; 
}

SCM TCP::Bind6(SCM id, SCM ip, SCM port){
  //CheckArgType(id, scm_integer_p, "tcp-bind6", 1);
  assert_object_type(id);
  CheckArgType(ip, scm_string_p, "tcp-bind6", 2);
  CheckArgType(port, scm_integer_p, "tcp-bind6", 3);
  TCP * t = (TCP *)get_object(id);
  assert(t!=NULL);
  char * i = scm_to_locale_string(ip);
  int p = scm_to_int(port);
  uv_tcp_t * h = GetHandle(t);
  if(h){
    struct sockaddr_in6 addr;
    addr = uv_ip6_addr(i, p);
    int r = uv_tcp_bind6(h, addr);
    if(r) Logger::Err("uv_tcp_bind6 failed! : %d", r);
  };
  return id;
}

SCM TCP::Connect(SCM id, SCM ip, SCM port, SCM cb){
  //CheckArgType(id, scm_integer_p, "tcp-connect", 1);
  assert_object_type(id);
  CheckArgType(ip, scm_string_p, "tcp-connect", 2);
  CheckArgType(port, scm_integer_p, "tcp-connect", 3);
  CheckArgType(cb, scm_procedure_p, "tcp-connect", 4); 
  TCP * t = (TCP *)get_object(id);
  assert(t!=NULL);
  char * i = scm_to_locale_string(ip);
  int p = scm_to_int(port);
  t->Object::On("connect", cb);
  uv_tcp_t * h = GetHandle(t);
  if(h){
    struct sockaddr_in addr;
    addr = uv_ip4_addr(i, p);
    uv_connect_t * req = (uv_connect_t *)malloc(sizeof(uv_connect_t));
    assert(req!=NULL);
    int r = uv_tcp_connect(req, h, addr, OnConnect);
    if(r) Logger::Err("uv_tcp_connect failed! : %d", r);
  };
  return id;
}

SCM TCP::Connect6(SCM id, SCM ip, SCM port, SCM cb){
  //CheckArgType(id, scm_integer_p, "tcp-connect6", 1);
  assert_object_type(id);
  CheckArgType(ip, scm_string_p, "tcp-connect6", 2);
  CheckArgType(port, scm_integer_p, "tcp-connect6", 3);
  CheckArgType(cb, scm_procedure_p, "tcp-connect6", 4);
  TCP * t = (TCP *)get_object(id);
  assert(t!=NULL);
  char * i = scm_to_locale_string(ip);
  int p = scm_to_int(port);
  t->Object::On("connect", cb);
  uv_tcp_t * h = GetHandle(t);
  if(h){
    struct sockaddr_in6 addr;
    addr = uv_ip6_addr(i, p);
    uv_connect_t * req = (uv_connect_t *)malloc(sizeof(uv_connect_t));
    assert(req!=NULL);
    int r = uv_tcp_connect6(req, h, addr, OnConnect);
    if(r) Logger::Err("uv_tcp_connect6 failed! : %d", r);
  };
  return id;
}

SCM TCP::SetNoDelay(SCM id, SCM enable){
  //CheckArgType(id, scm_integer_p, "tcp-set-nodelay", 1);
  assert_object_type(id);
  CheckArgType(enable, scm_integer_p, "tcp-set-nodelay", 2);
  TCP * t = (TCP *)get_object(id);
  assert(t!=NULL);
  int r = uv_tcp_nodelay(GetHandle(t), scm_to_int(enable));
  if(r) Logger::Err("uv_tcp_nodelay failed! : %d", r);
  return id;
}

SCM TCP::KeepAlive(SCM id, SCM enable, SCM delay){
  //CheckArgType(id, scm_integer_p, "tcp-keep-alive", 1);
  assert_object_type(id);
  CheckArgType(enable, scm_integer_p, "tcp-keep-alive", 2);
  CheckArgType(delay, scm_integer_p, "tcp-keep-alive", 2);
  TCP * t = (TCP *)get_object(id);
  assert(t!=NULL);
  int r = uv_tcp_keepalive(GetHandle(t), scm_to_int(enable), scm_to_int(delay));
  if(r) Logger::Err("uv_tcp_keepalive failed! : %d", r);
  return id;
}

//SCM TCP::GetSockName(SCM id){
//  //CheckArgType(id, scm_integer_p, "tcp-keep-alive", 1);
//  assert_object_type(id);
//  TCP * t = (TCP *)get_object(id);
//  assert(t!=NULL);
//  struct sockaddr* name = (struct sockaddr *)malloc(sizeof(struct sockaddr));
//  int namelen;
//  int r = uv_tcp_getsockname(GetHandle(t), name, &namelen);
//  if(r) Logger::Err("uv_tcp_getsockname failed! : %d", r);
//  SCM 
//  free(name);
//}

TCP::TCP(){
  int r = uv_tcp_init(uv_default_loop(), (uv_tcp_t*)&handle);
  if(r) Logger::Err("uv_tcp_init failed! : %d", r);
};

TCP::~TCP(){
}

void TCP::RegisterSCMFunctions(){
  scm_c_define_gsubr("tcp-new", 0, 0, 0, (void *)New);
  scm_c_define_gsubr("tcp-on", 3, 0, 0, (void *)On);
  scm_c_define_gsubr("tcp-resume", 1, 0, 0, (void *)Resume);
  scm_c_define_gsubr("tcp-pause", 1, 0, 0, (void *)Pause);
  scm_c_define_gsubr("tcp-write", 2, 0, 0, (void *)Write);
  scm_c_define_gsubr("tcp-destroy", 1, 0, 0, (void *)Destroy);
  scm_c_define_gsubr("tcp-shutdown", 1, 0, 0, (void *)ShutDown);
  scm_c_define_gsubr("tcp-readable?", 1, 0, 0, (void *)IsReadable);
  scm_c_define_gsubr("tcp-writable?", 1, 0, 0, (void *)IsWritable);

  scm_c_define_gsubr("tcp-set-nodelay", 2, 0, 0, (void *)SetNoDelay);
  scm_c_define_gsubr("tcp-keep-alive", 3, 0, 0, (void *)KeepAlive);

  scm_c_define_gsubr("tcp-listen", 3, 0, 0, (void *)Listen);
  scm_c_define_gsubr("tcp-accept", 2, 0, 0, (void *)Accept);

  scm_c_define_gsubr("tcp-bind", 3, 0, 0, (void *)Bind);
  scm_c_define_gsubr("tcp-bind6", 3, 0, 0, (void *)Bind6);

  scm_c_define_gsubr("tcp-connect", 4, 0, 0, (void *)Connect);
  scm_c_define_gsubr("tcp-connect6", 4, 0, 0, (void *)Connect6);
}

void TCP::Init(){
}

}
