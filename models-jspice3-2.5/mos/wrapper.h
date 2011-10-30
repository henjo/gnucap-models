extern "C" {
  #include "mosdefs.h"
}

#define info	 MOSinfo
#define INSTANCE MOSinstance
#define MODEL	 MOSmodel

#define SPICE_LETTER "M"
#define DEVICE_TYPE "jspice_mos"
#define MIN_NET_NODES 4
#define MAX_NET_NODES 4
#define INTERNAL_NODES 2
#define MODEL_TYPE "nmos|pmos|nmos1|pmos1|nmos2|pmos2|nmos3|pmos3|nmos6|pmos6|nmos8|pmos8"

static std::string port_names[] = {"d", "g", "s", "b"};
static std::string state_names[] = {};
