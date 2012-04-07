#ifndef __MO_UTILS_H__
#define __MO_UTILS_H__

#include "uv.h"
#include "libguile.h"

namespace Mo {

void CheckType(SCM, SCM(*)(SCM), const char*);

void CheckArgType(SCM, SCM(*)(SCM), const char*, int);

SCM Buf2List(uv_buf_t, size_t);

uv_buf_t List2Buf(SCM);

}

#endif
