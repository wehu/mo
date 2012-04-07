#include <stdlib.h>
#include "mo_fs.h"
#include "mo_logger.h"
#include "mo_utils.h"
#include <assert.h>
#include <string>
#include <fcntl.h>

using namespace std;

namespace Mo {

static Object * fs_g = new Object();

void FS::OnFS(uv_fs_t * req){
  assert(req!=NULL);
  SCM args = scm_list_2(scm_from_int(req->result), scm_from_int(req->errorno));
  string e;
  switch(req->fs_type){
    case UV_FS_OPEN:
      e = "open";
      break;
    case UV_FS_CLOSE:
      e = "close";
      break;
    default:
      e = "undefined";
      break;
  };
  e += (uint64_t)req;
  fs_g->Run(e, args);
  uv_fs_req_cleanup(req);
}

SCM FS::Open(SCM fn, SCM flags, SCM mode, SCM cb){
  CheckArgType(fn, scm_string_p, "fs-open", 1);
  CheckArgType(flags, scm_integer_p, "fs-open", 2);
  CheckArgType(mode, scm_integer_p, "fs-open", 3);
  CheckArgType(cb, scm_procedure_p, "fs-open", 4);
  uv_fs_t * req = (uv_fs_t *)malloc(sizeof(uv_fs_t));
  assert(req!=NULL);
  int r = uv_fs_open(uv_default_loop(),
             req,
             scm_to_locale_string(fn),
             scm_to_int(flags),
             scm_to_int(mode),
             OnFS);
  if(r) Logger::Err("uv_fs_open failed! : %d", r);
  string e = "open";
  e += (uint64_t)req;
  fs_g->On(e, cb);
  //uv_unref(uv_default_loop());
  return fn;
}

SCM FS::Close(SCM fd, SCM cb){
  CheckArgType(fd, scm_integer_p, "fs-close", 1);
  CheckArgType(cb, scm_procedure_p, "fs-close", 2);
  uv_fs_t * req = (uv_fs_t *)malloc(sizeof(uv_fs_t));
  assert(req!=NULL);
  int r = uv_fs_close(uv_default_loop(),
             req,
             scm_to_uint64(fd),
             OnFS);
  if(r) Logger::Err("uv_fs_close failed! : %d", r);
  string e = "close";
  e += (uint64_t)req;
  fs_g->On(e, cb);
  //uv_unref(uv_default_loop());
  return fd;
}

void FS::RegisterSCMFunctions(){
  scm_c_define_gsubr("fs-open", 4, 0, 0, (void *)Open);
  scm_c_define_gsubr("fs-close", 2, 0, 0, (void *)Close);
}

void FS::Init(){
}

}
