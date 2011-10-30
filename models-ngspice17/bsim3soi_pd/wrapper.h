extern "C" {
  #include "b3soipddef.h"
}

#define info	 B3SOIPDinfo
#define INSTANCE B3SOIPDinstance
#define MODEL	 B3SOIPDmodel

#define SPICE_LETTER "M"
#define DEVICE_TYPE "ngspice_bsim3soipd|ngspice_bsimpd2p23"
#define MIN_NET_NODES 4
#define MAX_NET_NODES 7
#define INTERNAL_NODES 21
#define MODEL_TYPE "nmos57|pmos57"
#define UNCONNECTED_NODES uFLOAT

static std::string port_names[] = {"d", "g", "s", "bg", "p", "body", "temp"};
static std::string state_names[] = {};
