extern "C" {
  #include "hfetdefs.h"
}

#define info	 HFETAinfo
#define INSTANCE HFETAinstance
#define MODEL	 HFETAmodel

#define SPICE_LETTER "Z"
#define DEVICE_TYPE "ngspice_hfet"
#define MIN_NET_NODES 3
#define MAX_NET_NODES 3
#define INTERNAL_NODES 5
#define MODEL_TYPE "nhfet|phfet|nhfet5|phfet5"

static std::string port_names[] = {"d", "g", "s"};
static std::string state_names[] = {};

#undef M
