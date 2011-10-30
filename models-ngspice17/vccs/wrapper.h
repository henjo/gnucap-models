extern "C" {
  #include "vccsdefs.h"
}

#define info	 VCCSinfo
#define INSTANCE VCCSinstance
#define MODEL	 VCCSmodel

#define SPICE_LETTER "G"
#define DEVICE_TYPE "ngspice_vccs"
#define MIN_NET_NODES 4
#define MAX_NET_NODES 4
#define INTERNAL_NODES 0
#define MODEL_TYPE ""

static std::string port_names[] = {"sink", "src", "ps", "ns"};
static std::string state_names[] = {};
