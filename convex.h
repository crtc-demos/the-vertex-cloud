#ifndef CONVEX_H
#define CONVEX_H 1

#include "timing.h"

typedef struct
{
  pthread_t conv_hull_thread;
  pvr_ptr_t sun_txr;
} convex_hull_data;

extern effect_methods convex_hull_methods;
extern convex_hull_data convex_hull_0;

#endif
