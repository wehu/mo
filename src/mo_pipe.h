#ifndef __MO_PIPE_H__
#define __MO_PIPE_H__

#include "mo_stream.h"

namespace Mo {

class Pipe : public Stream {
  private:

  static SCM Open(SCM id, SCM fd);
  static SCM Bind(SCM id, SCM fn);
  static SCM Connect(SCM id, SCM fn, SCM cb);

  static void OnRead2(uv_pipe_t* handle, ssize_t nread, uv_buf_t buf, uv_handle_type pending);
  static SCM Resume2(SCM id);
  static SCM Write2(SCM id, SCM data, SCM s_id);

  int ipc;
 
  public:
  Pipe(int ipc=0);
  virtual ~Pipe();
  static SCM IsIPC(SCM);
  static SCM New();
  static SCM NewIPC();
  static uv_pipe_t * GetHandle(Pipe*);
  static Pipe * GetPipe(uv_pipe_t *);
  static void RegisterSCMFunctions();
  static void Init();
};

}

#endif
