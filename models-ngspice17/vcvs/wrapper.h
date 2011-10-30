extern "C" {
  #include "vcvsdefs.h"
}

#define info	 VCVSinfo
#define INSTANCE VCVSinstance
#define MODEL	 VCVSmodel

#define SPICE_LETTER "E"
#define DEVICE_TYPE "ngspice_vcvs"
#define MIN_NET_NODES 4
#define MAX_NET_NODES 4
#define INTERNAL_NODES 1
#define MODEL_TYPE ""

static std::string port_names[] = {"p", "n", "ps", "ns"};
static std::string state_names[] = {};
