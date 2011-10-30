extern "C" {
  #include "srcdefs.h"
}

#define info	 SRCinfo
#define INSTANCE SRCinstance
#define MODEL	 SRCmodel

#define SPICE_LETTER "B"
#define DEVICE_TYPE "jspice_src"
#define MIN_NET_NODES 2
#define MAX_NET_NODES 2
#define INTERNAL_NODES 0
#define MODEL_TYPE ""

static std::string port_names[] = {"p", "n"};
static std::string state_names[] = {};
