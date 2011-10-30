extern "C" {
  #define NODE NodE
  #include "cpldefs.h"
  #undef NodE
}

#define info	 CPLinfo
#define INSTANCE CPLinstance
#define MODEL	 CPLmodel

#define SPICE_LETTER "P"
#define DEVICE_TYPE "ngspice_cpl"
#define MIN_NET_NODES 2
#define MAX_NET_NODES 2
#define INTERNAL_NODES 0
#define MODEL_TYPE "cpl"

static std::string port_names[] = {"p1", "p2"};
static std::string state_names[] = {};
