extern "C" {
  #include "mos1defs.h"
}

#define info	 MOS1info
#define INSTANCE MOS1instance
#define MODEL	 MOS1model

#define SPICE_LETTER "M"
#define DEVICE_TYPE "ngspice_mos1"
#define MIN_NET_NODES 4
#define MAX_NET_NODES 4
#define INTERNAL_NODES 2
#define MODEL_TYPE "nmos1|pmos1"

static std::string port_names[] = {"d", "g", "s", "b"};
static std::string state_names[] = {};
