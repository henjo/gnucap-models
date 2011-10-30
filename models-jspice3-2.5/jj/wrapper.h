extern "C" {
  #include "jjdefs.h"
}

#define info	 JJinfo
#define INSTANCE JJinstance
#define MODEL	 JJmodel

#define SPICE_LETTER "B"
#define DEVICE_TYPE "jspice_jj"
#define MIN_NET_NODES 2
#define MAX_NET_NODES 3
#define INTERNAL_NODES 1
#define MODEL_TYPE "jj"
#define UNCONNECTED_NODES uFLOAT

static std::string port_names[] = {"a", "c", "p"};
static std::string state_names[] = {};
double JJdphi = 2.07e-16;
