extern "C" {
  #include "b3soifddef.h"
}

#define info	 B3SOIFDinfo
#define INSTANCE B3SOIFDinstance
#define MODEL	 B3SOIFDmodel

#define SPICE_LETTER "M"
#define DEVICE_TYPE "ngspice_bsim3soifd|ngspice_bsimfd2p1"
#define MIN_NET_NODES 4
#define MAX_NET_NODES 7
#define INTERNAL_NODES 46
#define MODEL_TYPE "nmos55|pmos55"
#define UNCONNECTED_NODES uFLOAT

static std::string port_names[] = {"d", "g", "s", "bg", "body", "temp", "p"};
static std::string state_names[] = {};
