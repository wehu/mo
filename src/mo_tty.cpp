#include "mo_tty.h"
#include <assert.h>
#include "mo_logger.h"
#include "mo_utils.h"

namespace Mo {

TTY::TTY(uv_file fd, int readable){
  int r = uv_tty_init(uv_default_loop(), (uv_tty_t*)&handle, fd, readable);
  if(r) Logger::Err("uv_tty_init failed! : %d", r);
}

TTY::~TTY(){
}

uv_tty_t * TTY::GetHandle(TTY * t){
  return &(Object::GetHandle(t)->tty);
}

TTY * TTY::GetTTY(uv_tty_t * handle){
  return (TTY *)Object::GetObject((mo_any_handle *)handle);
}

SCM TTY::New(SCM fd, SCM readable){
  CheckArgType(fd, scm_integer_p, "tty-new", 1);
  CheckArgType(readable, scm_integer_p, "tty-new", 2);
  TTY * t = new TTY(scm_to_int(fd), scm_to_int(readable));
  assert(t!=NULL);
  return t->smob;
}

SCM TTY::SetMode(SCM id, SCM mode){
  assert_object_type(id);
  CheckArgType(mode, scm_integer_p, "tty-set-mode", 2);
  TTY * t = (TTY *)get_object(id);
  assert(t!=NULL);
  int r = uv_tty_set_mode(GetHandle(t), scm_to_int(mode));
  if(r) Logger::Err("uv_tty_set_mode failed! : %d", r);
  return id;
}

SCM TTY::ResetMode(){
  uv_tty_reset_mode();
  return SCM_BOOL_T;
}

SCM TTY::GetWinSize(SCM id){
  assert_object_type(id);
  TTY * t = (TTY *)get_object(id);
  assert(t!=NULL);
  int width = 0;
  int height = 0;
  int r = uv_tty_get_winsize(GetHandle(t), &width, &height);
  if(r) Logger::Err("uv_tty_get_winsize failed! : %d", r);
  return scm_list_2(scm_from_int(width), scm_from_int(height));
}

void TTY::RegisterSCMFunctions(){
  scm_c_define_gsubr("tty-new", 2, 0, 0, (void *)New);
  scm_c_define_gsubr("tty-set-mode", 2, 0, 0, (void *)SetMode);
  scm_c_define_gsubr("tty-reset-mode", 0, 0, 0, (void *)ResetMode);
  scm_c_define_gsubr("tty-get-winsize", 1, 0, 0, (void *)GetWinSize);
}

void TTY::Init(){
}

}
