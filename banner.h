#ifndef BANNER_H
#define BANNER_H 1

#include "timing.h"

typedef struct
{
  const char *image;
  pvr_ptr_t texture;
} banner_data;

extern effect_methods banner_methods;
extern banner_data banner_0;
extern banner_data banner_1;
extern banner_data banner_2;

#endif
