extern "C" {
  #include "hfet2defs.h"
}

#define info	 HFET2info
#define INSTANCE HFET2instance
#define MODEL	 HFET2model

#define SPICE_LETTER "Z"
#define DEVICE_TYPE "ngspice_hfet2"
#define MIN_NET_NODES 3
#define MAX_NET_NODES 3
#define INTERNAL_NODES 2
#define MODEL_TYPE "nhfet6|phfet6"

static std::string port_names[] = {"d", "g", "s"};
static std::string state_names[] = {};

#undef M
