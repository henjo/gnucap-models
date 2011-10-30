extern "C" {
  #include "b3soidef.h"
}

#define info	 B3SOIinfo
#define INSTANCE B3SOIinstance
#define MODEL	 B3SOImodel

#define SPICE_LETTER "M"
#define DEVICE_TYPE "ngspice_bsim3soi|ngspice_bsimsoi3p0"
#define MIN_NET_NODES 4
#define MAX_NET_NODES 7
#define INTERNAL_NODES 21
#define MODEL_TYPE "nmos58|pmos58"
#define UNCONNECTED_NODES uFLOAT

static std::string port_names[] = {"d", "g", "s", "bg", "p", "body", "temp"};
static std::string state_names[] = {};
