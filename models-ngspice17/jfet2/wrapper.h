extern "C" {
  #include "jfet2defs.h"
}

#define info	 JFET2info
#define INSTANCE JFET2instance
#define MODEL	 JFET2model

#define SPICE_LETTER "J"
#define DEVICE_TYPE "ngspice_jfet2"
#define MIN_NET_NODES 3
#define MAX_NET_NODES 3
#define INTERNAL_NODES 2
#define MODEL_TYPE "njf2|pjf2"

static std::string port_names[] = {"d", "g", "s"};
static std::string state_names[] = {};
