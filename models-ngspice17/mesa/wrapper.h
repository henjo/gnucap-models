extern "C" {
  #include "mesadefs.h"
}

#define info	 MESAinfo
#define INSTANCE MESAinstance
#define MODEL	 MESAmodel

#define SPICE_LETTER "Z"
#define DEVICE_TYPE "ngspice_mesa"
#define MIN_NET_NODES 3
#define MAX_NET_NODES 3
#define INTERNAL_NODES 5
#define MODEL_TYPE "nmf2|pmf2|nmf3|pmf3|nmf4|pmf4"

static std::string port_names[] = {"d", "g", "s"};
static std::string state_names[] = {};
