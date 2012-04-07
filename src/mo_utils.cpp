#include "mo_logger.h"
#include "mo_utils.h"
#include <string>

using namespace std;

namespace Mo {

void CheckType(SCM obj, SCM(*f)(SCM), const char * msg){
  if(!scm_to_bool(f(obj))) {
    Logger::Err(msg);
    Logger::ShowSourceLocation(obj);
    exit(-1);
  }
}

void CheckArgType(SCM obj, SCM(*f)(SCM), const char * fn, int pos){
  string msg = fn;
  bool check = true;
  char t[256];
  sprintf(t, "%d", pos);
  msg += " : argument ";
  msg += t;
  msg += " should be";
  if(f == scm_integer_p)
    msg += " an integer";
  else if (f == scm_string_p)
    msg += " a string";
  else if (f == scm_procedure_p)
    msg += " a lambda closure";
  else if (f == scm_list_p)
    msg += " a list";
  else if (f == scm_keyword_p)
    msg += " a keyword";
  else if (f == scm_symbol_p)
    msg += " a symbol";
  else
    check = false;
  msg += "!";
  if(check)
    CheckType(obj, f, msg.c_str());
}

SCM Buf2List(uv_buf_t buf, size_t n){
  SCM l = SCM_EOL;
  for(size_t i=0; i<n; i++){
    l = scm_append(scm_list_2(l, scm_list_1(scm_from_int8(*(buf.base+i)))));
  }
  return l;
}

uv_buf_t List2Buf(SCM l){
  uv_buf_t buf;
  buf.len = scm_to_uint64(scm_length(l));
  buf.base = (char *)malloc(buf.len);
  for(size_t i=0; i<buf.len; i++){
    *(buf.base+i) = (char)scm_to_int8(scm_list_ref(l, scm_from_uint(i)));
  };
  return buf;
}

}