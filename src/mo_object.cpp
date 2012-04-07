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
#include "mo_object.h"
#include "mo_tick.h"
#include <assert.h>

using namespace std;

namespace Mo {

struct mo_object_data {
  Object * obj;
};

struct mo_object_smob {
  struct mo_object_data *data;
};

static scm_t_bits object_tag;

static bool initialized = false;

Object::Object(){
  handle.obj = this;

  with_smob = initialized;

  if(with_smob) {
    struct mo_object_smob * o = (struct mo_object_smob *)scm_gc_malloc(sizeof(struct mo_object_smob), "mo_object");
    assert(o!=NULL);
    o->data = NULL;
    SCM_NEWSMOB(smob, object_tag, o);
    o->data = (struct mo_object_data *)scm_gc_malloc(sizeof(struct mo_object_data), "mo_data");
    assert(o->data!=NULL);
    o->data->obj = this;
    scm_gc_protect_object(smob);
  }
}

void Object::OnClose(uv_handle_t * handle){
  assert(handle!=NULL);
  Object * obj = GetObject((mo_any_handle *)handle);
  assert(obj!=NULL);
  obj->Run("close", SCM_EOL);
  delete obj;
}

mo_any_handle * Object::GetHandle(Object * obj){
  assert(obj!=NULL);
  return &(obj->handle.uv_fields);
}

Object * Object::GetObject(mo_any_handle * handle){
  assert(handle!=NULL);
  return ((mo_handle_t*)handle)->obj;
}

Object::~Object(){
  event_map_t::iterator it;
  for(it=events.begin();it!=events.end();it++){
    vector<SCM> q = it->second;
    vector<SCM>::iterator it2;
    for(it2=q.begin(); it2!=q.end(); it2++)
      scm_gc_unprotect_object(*it2);
  }
  //abandon_object(smob);
  if(with_smob) {
    struct mo_object_smob * o = (struct mo_object_smob *)SCM_SMOB_DATA(smob);
    assert(o!=NULL);
    assert(o->data!=NULL);
    o->data->obj = NULL;

    scm_gc_unprotect_object(smob);
  }
}

void Object::On(string e, SCM cb){
  if(events.find(e) == events.end())
    events.insert(make_pair(e, vector<SCM>()));
  scm_gc_protect_object(cb);
  events[e].push_back(cb);
}

void Object::Remove(string e){
  if(events.find(e) != events.end()){
    vector<SCM> q = events[e];
    vector<SCM>::iterator it;
    for(it=q.begin(); it!=q.end(); it++)
      scm_gc_unprotect_object(*it);
    events.erase(events.find(e));
  }
}

void Object::Notify(string e, SCM args){
  if(events.find(e) != events.end()){
    vector<SCM> q = events[e];
    vector<SCM>::iterator it;
    for(it=q.begin(); it!=q.end(); it++)
      Tick::New((*it), args);
  }
}

void Object::Run(string e, SCM args){
  if(events.find(e) != events.end()){
    vector<SCM> q = events[e];
    vector<SCM>::iterator it;
    for(it=q.begin(); it!=q.end(); it++)
      scm_apply_0((*it), args);
  }
}

static SCM mark_object(SCM id){
  return SCM_BOOL_F;
}

static size_t free_object(SCM id){
  struct mo_object_smob * o = (struct mo_object_smob *)SCM_SMOB_DATA(id);
  if(o->data) scm_gc_free(o->data, sizeof(struct mo_object_data), "mo_data");
  scm_gc_free(o, sizeof(struct mo_object_smob), "mo_object");
  return 0;
}

void init_mo_object_type(){
  object_tag = scm_make_smob_type("mo_object", sizeof(struct mo_object_smob));
  scm_set_smob_mark(object_tag, mark_object);
  scm_set_smob_free(object_tag, free_object);
  initialized = true;
}

Object * get_object(SCM id){
  struct mo_object_smob * o = (struct mo_object_smob *)SCM_SMOB_DATA(id);
  assert(o!=NULL);
  return o->data->obj;
}

void assert_object_type(SCM id){
  scm_assert_smob_type(object_tag, id);
}

}
