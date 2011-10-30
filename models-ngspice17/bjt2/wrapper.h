extern "C" {
  #include "bjt2defs.h"
}

#define info	 BJT2info
#define INSTANCE BJT2instance
#define MODEL	 BJT2model

#define SPICE_LETTER "Q"
#define DEVICE_TYPE "ngspice_bjt2"
#define MIN_NET_NODES 3
#define MAX_NET_NODES 4
#define INTERNAL_NODES 3
#define MODEL_TYPE "npn|pnp|npn2|pnp2"
#define UNCONNECTED_NODES uGROUND

static std::string port_names[] = {"c", "b", "e", "s"};
static std::string state_names[] = {};

/* There are 4 internal nodes listed, but the last one
 * is mapped to another internally.
 * So, only 3 of them matter externally in the sense of
 * needing a node allocated.
 */
