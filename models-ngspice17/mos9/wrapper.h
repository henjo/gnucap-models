extern "C" {
  #include "mos9defs.h"
}

#define info	 MOS9info
#define INSTANCE MOS9instance
#define MODEL	 MOS9model

#define SPICE_LETTER "M"
#define DEVICE_TYPE "ngspice_mos9"
#define MIN_NET_NODES 4
#define MAX_NET_NODES 4
#define INTERNAL_NODES 2
#define MODEL_TYPE "nmos9|pmos9"

static std::string port_names[] = {"d", "g", "s", "b"};
static std::string state_names[] = {};
