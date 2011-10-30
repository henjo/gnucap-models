extern "C" {
  #include "cswdefs.h"
}

#define info	 CSWinfo
#define INSTANCE CSWinstance
#define MODEL	 CSWmodel

#define SPICE_LETTER "W"
#define DEVICE_TYPE "ngspice_csw"
#define MIN_NET_NODES 3
#define MAX_NET_NODES 3
#define INTERNAL_NODES 0
#define MODEL_TYPE "csw"

static std::string port_names[] = {"p", "n", "in"};
static std::string state_names[] = {};
