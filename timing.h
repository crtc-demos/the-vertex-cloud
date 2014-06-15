#ifndef TIMING_H
#define TIMING_H 1

#include <stdint.h>

typedef struct {
  void (*preinit_assets) (void);
  void (*init_effect) (void *params);
  void (*prepare_frame) (uint32_t time_offset, void *params, int iparam);
  void (*display_effect) (uint32_t time_offset, void *params, int iparam,
			  int pass);
  void (*gl_effect) (uint32_t time_offset, void *params, int iparam);
  void (*uninit_effect) (void *params);
  void (*finalize) (void *params);
} effect_methods;

typedef struct {
  uint64_t start_time;
  uint64_t end_time;
  effect_methods *methods;
  void *params;
  int iparam;
  int finalize;
} do_thing_at;

extern float audio_amplitude (int delay);

#define UNUSED __attribute__ ((unused))

#endif
