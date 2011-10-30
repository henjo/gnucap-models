extern "C" {
  #include "bjtdefs.h"
  extern SPICEdev BJTinfo;
}

#define info	 BJTinfo
#define INSTANCE BJTinstance
#define MODEL	 BJTmodel

#define SPICE_LETTER "Q"
#define DEVICE_TYPE "ngspice_bjt"
#define MIN_NET_NODES 3
#define MAX_NET_NODES 4
#define INTERNAL_NODES 3
#define MODEL_TYPE "NPN,PNP"

char* state_names[] = {""};
