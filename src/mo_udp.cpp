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

#include "mo_udp.h"
#include <stdlib.h>
#include "mo_utils.h"
#include "mo_pipe.h"
#include <assert.h>
#include "mo_logger.h"

namespace Mo {

SCM UDP::New(){
  UDP * p = new UDP();
  assert(p!=NULL);
  p->type = MO_UDP;
  return p->smob;
}

uv_udp_t * UDP::GetHandle(UDP * t){
  return &(Object::GetHandle(t)->udp);
}

UDP * UDP::GetUDP(uv_udp_t * handle){
  return (UDP *)Object::GetObject((mo_any_handle *)handle);
}

SCM UDP::Bind(SCM id, SCM ip, SCM port){
  //CheckArgType(id, scm_integer_p, "udp-bind", 1);
  assert_object_type(id);
  CheckArgType(ip, scm_string_p, "udp-bind", 2);
  CheckArgType(port, scm_integer_p, "udp-bind", 3);
  UDP * u = (UDP *)get_object(id);
  assert(u!=NULL);
  char * i = scm_to_locale_string(ip);
  int p = scm_to_int(port);
  uv_udp_t * h = GetHandle(u);
  if(h){
    struct sockaddr_in addr;
    addr = uv_ip4_addr(i, p);
    int r = uv_udp_bind(h, addr, 0);
    if(r) Logger::Err("uv_udp_bind failed! : %d", r);
  };
  return id; 
}

SCM UDP::Bind6(SCM id, SCM ip, SCM port, SCM flags){
  //CheckArgType(id, scm_integer_p, "udp-bind6", 1);
  assert_object_type(id);
  CheckArgType(ip, scm_string_p, "udp-bind6", 2);
  CheckArgType(port, scm_integer_p, "udp-bind6", 3);
  CheckArgType(flags, scm_integer_p, "udp-bind6", 4);
  UDP * u = (UDP *)get_object(id);
  assert(u!=NULL);
  char * i = scm_to_locale_string(ip);
  int p = scm_to_int(port);
  int f = scm_to_uint(flags);
  uv_udp_t * h = GetHandle(u);
  if(h){
    struct sockaddr_in6 addr;
    addr = uv_ip6_addr(i, p);
    int r = uv_udp_bind6(h, addr, f);
    if(r) Logger::Err("uv_udp_bind6 failed! : %d", r);
  };
  return id;
}

SCM UDP::SetMembership(SCM id, SCM addr, SCM interface, SCM membership){
  //CheckArgType(id, scm_integer_p, "udp-set-membership", 1);
  assert_object_type(id);
  CheckArgType(addr, scm_string_p, "udp-set-membership", 2);
  CheckArgType(interface, scm_string_p, "udp-set-membership", 3);
  CheckArgType(membership, scm_integer_p, "udp-set-membership", 4);
  UDP * u = (UDP *)get_object(id);
  assert(u!=NULL);
  char * a = scm_to_locale_string(addr);
  char * i = scm_to_locale_string(interface);
  int m = scm_to_uint(membership);
  uv_udp_t * h = GetHandle(u);
  if(h){
    int r = uv_udp_set_membership(h, a, i, (uv_membership)m);
    if(r) Logger::Err("uv_udp_set_membership failed! : %d", r);
  }
  return id;
}

SCM UDP::SetMulticastLoop(SCM id, SCM on){
  //CheckArgType(id, scm_integer_p, "udp-set-multicast-loop", 1);
  assert_object_type(id);
  CheckArgType(on, scm_integer_p, "udp-set-multicast-loop", 2);
  UDP * u = (UDP *)get_object(id);
  assert(u!=NULL);
  int o = scm_to_uint(on);
  uv_udp_t * h = GetHandle(u);
  if(h){
    int r = uv_udp_set_multicast_loop(h, o);
    if(r) Logger::Err("uv_udp_set_multicast_loop failed! : %d", r);
  }
  return id;
}

SCM UDP::SetMulticastTTL(SCM id, SCM ttl){
  //CheckArgType(id, scm_integer_p, "udp-set-multicast-ttl", 1);
  assert_object_type(id);
  CheckArgType(ttl, scm_integer_p, "udp-set-multicast-ttl", 2);
  UDP * u = (UDP *)get_object(id);
  assert(u!=NULL);
  int t = scm_to_uint(ttl);
  uv_udp_t * h = GetHandle(u);
  if(h){
    int r = uv_udp_set_multicast_ttl(h, t);
    if(r) Logger::Err("uv_udp_set_multicast_ttl failed! : %d", r);
  }
  return id;
}

SCM UDP::SetBroadcast(SCM id, SCM on){
  //CheckArgType(id, scm_integer_p, "udp-set-broadcast", 1);
  assert_object_type(id);
  CheckArgType(on, scm_integer_p, "udp-set-broadcast", 2);
  UDP * u = (UDP *)get_object(id);
  assert(u!=NULL);
  int o = scm_to_uint(on);
  uv_udp_t * h = GetHandle(u);
  if(h){
    int r = uv_udp_set_broadcast(h, o);
    if(r) Logger::Err("uv_udp_set_broadcast failed! : %d", r);
  }
  return id;
}

SCM UDP::SetTTL(SCM id, SCM ttl){
  //CheckArgType(id, scm_integer_p, "udp-set-ttl", 1);
  assert_object_type(id);
  CheckArgType(ttl, scm_integer_p, "udp-set-ttl", 2);
  UDP * u = (UDP *)get_object(id);
  assert(u!=NULL);
  int t = scm_to_uint(ttl);
  uv_udp_t * h = GetHandle(u);
  if(h){
    int r = uv_udp_set_ttl(h, t);
    if(r) Logger::Err("uv_udp_set_ttl failed! : %d", r);
  }
  return id;
}

SCM UDP::Send(SCM id, SCM data, SCM ip, SCM port, SCM cb){
  
}

SCM UDP::Send6(SCM id, SCM data, SCM ip, SCM port, SCM cb){
}

SCM UDP::Receive(SCM id){
}

SCM UDP::StopReceive(SCM id){
}

UDP::UDP(){
  int r = uv_udp_init(uv_default_loop(), (uv_udp_t*)&handle);
  if(r) Logger::Err("uv_udp_init failed! : %d", r);
};

UDP::~UDP(){
}

void UDP::RegisterSCMFunctions(){
  scm_c_define_gsubr("udp-new", 0, 0, 0, (void *)New);
  scm_c_define_gsubr("udp-on", 3, 0, 0, (void *)On);
  scm_c_define_gsubr("udp-resume", 1, 0, 0, (void *)Resume);
  scm_c_define_gsubr("udp-pause", 1, 0, 0, (void *)Pause);
  scm_c_define_gsubr("udp-write", 2, 0, 0, (void *)Write);
  scm_c_define_gsubr("udp-destroy", 1, 0, 0, (void *)Destroy);
  scm_c_define_gsubr("udp-shutdown", 1, 0, 0, (void *)ShutDown);
  scm_c_define_gsubr("udp-readable?", 1, 0, 0, (void *)IsReadable);
  scm_c_define_gsubr("udp-writable?", 1, 0, 0, (void *)IsWritable);

  scm_c_define_gsubr("udp-listen", 3, 0, 0, (void *)Listen);
  scm_c_define_gsubr("udp-accept", 2, 0, 0, (void *)Accept);

  scm_c_define_gsubr("udp-bind", 3, 0, 0, (void *)Bind);
  scm_c_define_gsubr("udp-bind6", 4, 0, 0, (void *)Bind6);

}

void UDP::Init(){
}

}
