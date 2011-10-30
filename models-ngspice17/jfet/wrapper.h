extern "C" {
  #include "jfetdefs.h"
}

#define info	 JFETinfo
#define INSTANCE JFETinstance
#define MODEL	 JFETmodel

#define SPICE_LETTER "J"
#define DEVICE_TYPE "ngspice_jfet"
#define MIN_NET_NODES 3
#define MAX_NET_NODES 3
#define INTERNAL_NODES 2
#define MODEL_TYPE "njf|pjf|njf1|pjf1"

static std::string port_names[] = {"d", "g", "s"};
static std::string state_names[] = {};
