extern "C" {
  #include "hsm1def.h"
}

#define info	 HSM1info
#define INSTANCE HSM1instance
#define MODEL	 HSM1model

#define SPICE_LETTER "M"
#define DEVICE_TYPE "ngspice_hisim"
#define MIN_NET_NODES 4
#define MAX_NET_NODES 4
#define INTERNAL_NODES 2
#define MODEL_TYPE "nmos64|pmos64"

static std::string port_names[] = {"d", "g", "s", "b"};
static std::string state_names[] = {};
