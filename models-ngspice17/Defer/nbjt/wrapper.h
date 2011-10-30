extern "C" {
  #include "nbjtdefs.h"
  extern SPICEdev NBJTinfo;
}

#define info	 NBJTinfo
#define INSTANCE NBJTinstance
#define MODEL	 NBJTmodel

#define SPICE_LETTER "Q"
#define DEVICE_TYPE "ngspice_nbjt"
#define MIN_NET_NODES 3
#define MAX_NET_NODES 3
#define INTERNAL_NODES 0
#define MODEL_TYPE "NBJT"

char* state_names[] = {""};
