extern "C" {
  #include "cccsdefs.h"
}

#define info	 CCCSinfo
#define INSTANCE CCCSinstance
#define MODEL	 CCCSmodel

#define SPICE_LETTER "F"
#define DEVICE_TYPE "ngspice_cccs"
#define MIN_NET_NODES 3
#define MAX_NET_NODES 3
#define INTERNAL_NODES 0
#define MODEL_TYPE ""

static std::string port_names[] = {"sink", "src", "in"};
static std::string state_names[] = {};
