extern "C" {
  #include "vbicdefs.h"
}

#define info	 VBICinfo
#define INSTANCE VBICinstance
#define MODEL	 VBICmodel

#define SPICE_LETTER "Q"
#define DEVICE_TYPE "ngspice_vbic"
#define MIN_NET_NODES 3
#define MAX_NET_NODES 4
#define INTERNAL_NODES 7
#define MODEL_TYPE "npn|pnp|npn4|pnp4"
#define UNCONNECTED_NODES uGROUND

static std::string port_names[] = {"c", "b", "e", "s"};
static std::string state_names[] = {};
