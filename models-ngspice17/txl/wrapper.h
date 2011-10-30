extern "C" {
  #define NODE NodE
  #include "txldefs.h"
  #undef NodE
}

#define info	 TXLinfo
#define INSTANCE TXLinstance
#define MODEL	 TXLmodel

#define SPICE_LETTER "Y"
#define DEVICE_TYPE "ngspice_txl"
#define MIN_NET_NODES 2
#define MAX_NET_NODES 2
#define INTERNAL_NODES 0
#define MODEL_TYPE "txl"

static std::string port_names[] = {"y1", "y2"};
static std::string state_names[] = {};
