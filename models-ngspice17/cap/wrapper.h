extern "C" {
  #include "capdefs.h"
}

#define info	 CAPinfo
#define INSTANCE CAPinstance
#define MODEL	 CAPmodel

#define SPICE_LETTER "C"
#define DEVICE_TYPE "ngspice_cap"
#define MIN_NET_NODES 2
#define MAX_NET_NODES 2
#define INTERNAL_NODES 0
#define MODEL_TYPE "c"

static std::string port_names[] = {"p", "n"};
static std::string state_names[] = {"qcap", "ccap"};
