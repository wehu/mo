#include <stdlib.h>
#include "mo.h"
#include "mo_logger.h"

using namespace Mo;

static void inner_main(void *data, int argc, char **argv){

  Main::RegisterSCMFunctions();
  Main::Init();

  if (argc == 1)
    //TODO need async debug mode.
    scm_shell(argc, argv);
  else if (argc == 2){
    scm_c_primitive_load(argv[1]);
    Main::Run();
  } else {
    Logger::Err("Unexpected arguments!");
    exit(-1);
  }

  uv_err_t last_err = uv_last_error(uv_default_loop());
  if(last_err.code != UV_OK && last_err.code!= UV_EOF){
    Logger::Err(uv_strerror(last_err));    
  }
}

int main(int argc, char **argv){
  scm_boot_guile(argc, argv, inner_main, 0);
  return 0;
}
