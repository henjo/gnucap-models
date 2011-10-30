extern "C" {
  #include "soi3defs.h"
}

#define info	 SOI3info
#define INSTANCE SOI3instance
#define MODEL	 SOI3model

#define SPICE_LETTER "M"
#define DEVICE_TYPE "ngspice_soi3"
#define MIN_NET_NODES 4
#define MAX_NET_NODES 6
#define INTERNAL_NODES 7
#define MODEL_TYPE "nmos62|pmos62"
#define UNCONNECTED_NODES uFLOAT

static std::string port_names[] = {"d", "g", "s", "bg", "body", "temp"};
static std::string state_names[] = {};
