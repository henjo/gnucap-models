extern "C" {
  #include "b3soidddef.h"
}

#define info	 B3SOIDDinfo
#define INSTANCE B3SOIDDinstance
#define MODEL	 B3SOIDDmodel

#define SPICE_LETTER "M"
#define DEVICE_TYPE "ngspice_bsim3soidd|ngspice_bsimdd2p1"
#define MIN_NET_NODES 4
#define MAX_NET_NODES 7
#define INTERNAL_NODES 46
#define MODEL_TYPE "nmos56|pmos56"
#define UNCONNECTED_NODES uFLOAT

static std::string port_names[] = {"d", "g", "s", "bg", "body", "temp", "p"};
static std::string state_names[] = {};
