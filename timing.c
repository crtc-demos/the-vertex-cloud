/* Timing/main demo harness.  */

#include <math.h>
#include <stdlib.h>
#include <alloca.h>
#include <stdint.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

#include <kos.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <png/png.h>
#include <kmg/kmg.h>
#include <modplug/modplug.h>

#include "convex.h"
#include "banner.h"

#define PLAY_AUDIO
#undef HOLD
#undef DEBUG

#undef SKIP_TO_TIME
//#define SKIP_TO_TIME 70000

#ifdef SKIP_TO_TIME
uint64_t offset_time = 0;
#else
#define offset_time 0
#endif

extern uint8 romdisk[];

KOS_INIT_FLAGS (INIT_DEFAULT);
KOS_INIT_ROMDISK (romdisk);

static do_thing_at sequence[] = {
#if 1
  //{      0,  20000, &convex_hull_methods, &convex_hull_0, 2, 0 },
  {      0,  15000, &banner_methods, &banner_0, 0, 0 },
  {  15000,  35000, &convex_hull_methods, &convex_hull_0, 0, 0 },
  {  35000,  48000, &banner_methods, &banner_1, 0, 0 },
  {  48000,  95000, &convex_hull_methods, &convex_hull_0, 1, 1 },
  {  95000, 109000, &banner_methods, &banner_2, 0, 0 },
#else

  { 0, 300000, &wave_methods, NULL, 0, 0 },
  { 0, 300000, &building_methods, NULL, 0, 0 },
#endif
};

/* Other random bits...  */

  /*{ 20000, 22500, &torus_methods, &torus1, 0 },
  { 21000, 23500, &torus_methods, &torus2, 0 },
  { 22000, 24500, &torus_methods, &torus1, 1 },
  { 25000, 26000, &torus_methods, &torus2, 0 } */
  /*{     0,  10000, &wobble_tube_methods, NULL, 0 },*/
  /*{ 0, 600000, &fire_methods, NULL, 0 },
  { 0, 600000, &bumpy_cube_methods, NULL, 0 }*/
  /*{ 0, 22000, &foggy_tube_methods, NULL, 0 }*/
  /*{0, 300000, &zoomy_duck_methods, NULL, 0 }*/

#define ARRAY_SIZE(X) (sizeof (X) / sizeof (X[0]))

#define MAX_ACTIVE 20

static unsigned int current_millis;

static ModPlugFile *mpfile;

static uint8 *pcm_buffer;

static void *
modplug_callback (snd_stream_hnd_t hnd UNUSED, int smp_req, int *smp_recv)
{
  int bytes, fetch;
  
  fetch = (smp_req >= SND_STREAM_BUFFER_MAX) ? SND_STREAM_BUFFER_MAX : smp_req;
  bytes = ModPlug_Read (mpfile, pcm_buffer, fetch);
  *smp_recv = bytes;
  return pcm_buffer;
}

static volatile int terminate_sound = 0;
static pthread_mutex_t terminate_sound_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *
modplug_fill_thread (void *args)
{
  snd_stream_hnd_t sndstream_hnd = *(snd_stream_hnd_t *) args;
  int quit = 0;

  while (!quit)
    {
      snd_stream_poll (sndstream_hnd);
      pthread_mutex_lock (&terminate_sound_mutex);
      quit = terminate_sound;
      pthread_mutex_unlock (&terminate_sound_mutex);
      //thd_sleep (50);
    }
  
  return NULL;
}

static void
init_pvr (void)
{
  pvr_init_params_t params = {
    { PVR_BINSIZE_32,	/* Opaque polygons.  */
      PVR_BINSIZE_0,	/* Opaque modifiers.  */
      PVR_BINSIZE_8,	/* Translucent polygons.  */
      PVR_BINSIZE_0,	/* Translucent modifiers.  */
      PVR_BINSIZE_8 },	/* Punch-thrus.  */
    512 * 1024,		/* Vertex buffer size 512K.  */
    0,			/* No DMA.  */
    0			/* No FSAA.  */
  };
  
  pvr_init (&params);
}

int
main (int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
  int cable_type;
  int quit = 0, rc;
  uint64_t start_time;
  unsigned int i;
  unsigned int next_effect;
  do_thing_at *active_effects[MAX_ACTIVE];
  unsigned int num_active_effects;
  const unsigned int num_effects = ARRAY_SIZE (sequence);
  int sndstream_hnd;
  FILE *f;
  struct stat statbuf;
  void *moddata;
  //uint64_t before_sending, after_sending;
  pthread_t sound_thread_hnd;
  void *status;
  ModPlug_Settings mp_settings;

  cable_type = vid_check_cable ();
  if (cable_type == CT_VGA)
    vid_init (DM_640x480_VGA, PM_RGB565);
  else
    vid_init (DM_640x480_NTSC_IL, PM_RGB565);

  init_pvr ();

#ifdef USE_DMA
  pvr_set_vertbuf (PVR_LIST_OP_POLY, dmabuffers[0], 1024 * 1024);
  pvr_set_vertbuf (PVR_LIST_TR_POLY, dmabuffers[1], 1024 * 1024);
#endif

  glKosInit ();
  
  glViewport (0, 0, 640, 480);
  
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_CULL_FACE);
  glShadeModel (GL_SMOOTH);
  glClearDepth (1.0f);
  glDepthFunc (GL_LEQUAL);
    
  pvr_set_bg_color (0.0, 0.0, 0.0);
  
  for (i = 0; i < ARRAY_SIZE (sequence); i++)
    {
      if (sequence[i].methods->preinit_assets)
        sequence[i].methods->preinit_assets ();
    }
  
  num_active_effects = 0;
  next_effect = 0;
  
  start_time = timer_ms_gettime64 ();

#ifdef SKIP_TO_TIME
  offset_time = SKIP_TO_TIME;
#endif
  
  rc = stat ("/rd/lamb_-_setting_sun.xm", &statbuf);
  if (rc != 0)
    {
      printf ("couldn't stat file\n");
      exit (1);
    }
  moddata = malloc (statbuf.st_size);
  
  printf ("Reading tracker module (%d bytes)\n", (int) statbuf.st_size);
  f = fopen ("/rd/lamb_-_setting_sun.xm", "r");
  fread (moddata, 1, statbuf.st_size, f);
  fclose (f);
  
  ModPlug_GetSettings (&mp_settings);
  mp_settings.mChannels = 2;
  mp_settings.mBits = 16;
  mp_settings.mFrequency = 44100;
  mp_settings.mResamplingMode = MODPLUG_RESAMPLE_SPLINE;
  mp_settings.mReverbDepth = 0;
  mp_settings.mReverbDelay = 0;
  mp_settings.mBassAmount = 0;
  mp_settings.mSurroundDepth = 0;
  mp_settings.mLoopCount = 0;
  ModPlug_SetSettings (&mp_settings);
  
  mpfile = ModPlug_Load (moddata, statbuf.st_size);
  
  printf ("Loaded module: %p\n", mpfile);
  
  snd_stream_init ();
  sndstream_hnd = snd_stream_alloc (modplug_callback, SND_STREAM_BUFFER_MAX);
  
  pcm_buffer = malloc (SND_STREAM_BUFFER_MAX * 4);
  
  rc = pthread_create (&sound_thread_hnd, NULL, &modplug_fill_thread,
		       &sndstream_hnd);
  if (rc != 0)
    {
      printf ("Couldn't start sound thread (%d)\n", rc);
      exit (1);
    }
  
  snd_stream_start (sndstream_hnd, 44100, 1);

#ifdef SKIP_TO_TIME
  //ModPlug_Seek (mpfile, SKIP_TO_TIME);
#endif
    
  while (!quit)
    {
      uint64_t current_time;
      unsigned int i, j;

      /* Normal running, not manual camera control (hold) mode.  */
      MAPLE_FOREACH_BEGIN (MAPLE_FUNC_CONTROLLER, cont_state_t, st)
        if (st->buttons & CONT_START)
	  quit = 1;
      MAPLE_FOREACH_END ()

      current_time = offset_time + timer_ms_gettime64 () - start_time;

      /* For audio...  */
      current_millis = current_time;

#ifdef DEBUG
      printf ("terminate old effects\n");
#endif

      /* Terminate old effects.  */
      for (i = 0; i < num_active_effects; i++)
        {
	  if (current_time >= active_effects[i]->end_time)
	    {
	      /*printf ("uninit effect %d (iparam=%d)\n", i,
		      active_effects[i]->iparam);*/

	      if (active_effects[i]->methods->uninit_effect)
	        {
	          active_effects[i]->methods->uninit_effect (
		    active_effects[i]->params);
		}
	      /* If we're not going to use this one any more, release any
	         global resources (etc.).  */
	      if (active_effects[i]->finalize
		  && active_effects[i]->methods->finalize)
		{
		  active_effects[i]->methods->finalize (
		    active_effects[i]->params);
		}
	      active_effects[i] = NULL;
	    }
	}

      /* And remove from active list.  */
      for (i = 0, j = 0; j < num_active_effects;)
        {
	  active_effects[i] = active_effects[j];

	  if (active_effects[i] == NULL)
	    j++;
	  else
	    {
	      i++;
	      j++;
	    }
	}

      num_active_effects = i;

#ifdef DEBUG
      printf ("start new effects\n");
#endif

      while (next_effect < num_effects
	     && current_time >= sequence[next_effect].start_time)
	{
	  /* Only start the effect if it's before its ending time.  */
	  if (current_time < sequence[next_effect].end_time)
	    {
	      active_effects[num_active_effects] = &sequence[next_effect];

	      /*printf ("init effect %d (%p, iparam=%d)\n", next_effect,
		      sequence[next_effect].methods->init_effect,
		      sequence[next_effect].iparam);*/

	      if (sequence[next_effect].methods->init_effect)
		{
		  sequence[next_effect].methods->init_effect (
		    sequence[next_effect].params);
		}

	      num_active_effects++;
	    }
	  else if (sequence[next_effect].finalize)
	    {
	      /* We must have skipped over an effect: finalize it now.  */
	      if (sequence[next_effect].methods->finalize)
	        {
	          sequence[next_effect].methods->finalize (
		    sequence[next_effect].params);
		}
	    }

	  next_effect++;
	}

      if (next_effect == num_effects && num_active_effects == 0)
        quit = 1;

      /* Do things we need to do before starting to send stuff to the PVR.  */

#ifdef DEBUG
      printf ("prepare frame (active effects=%d)\n", num_active_effects);
#endif
      
      for (i = 0; i < num_active_effects; i++)
        {
	  if (active_effects[i]->methods->prepare_frame)
	    {
#ifdef DEBUG
	      printf ("prepare frame: %p\n",
		      active_effects[i]->methods->prepare_frame);
#endif
	      active_effects[i]->methods->prepare_frame (
		current_time - active_effects[i]->start_time,
		active_effects[i]->params, active_effects[i]->iparam);
	    }
	}

#ifdef DEBUG
      printf ("fixing matrices\n");
#endif

#ifdef DEBUG
      printf ("begin frame\n");
#endif

      if (num_active_effects == 1 && active_effects[0]->methods->gl_effect)
        {
	  glKosBeginFrame ();
	  
	  active_effects[0]->methods->gl_effect (
	    current_time - active_effects[0]->start_time,
	    active_effects[0]->params, active_effects[0]->iparam);
	  
	  glKosFinishFrame ();
	}
      else
        {
	  pvr_wait_ready ();
      
	  pvr_scene_begin ();

	  //before_sending = timer_ms_gettime64 ();
	  pvr_list_begin (PVR_LIST_OP_POLY);
	  glKosMatrixDirty ();

	  for (i = 0; i < num_active_effects; i++)
            {
	      if (active_effects[i]->methods->display_effect)
		active_effects[i]->methods->display_effect (
		  current_time - active_effects[i]->start_time,
		  active_effects[i]->params, active_effects[i]->iparam,
		  PVR_LIST_OP_POLY);
	    }

	  pvr_list_finish ();
	  pvr_list_begin (PVR_LIST_TR_POLY);
	  glKosMatrixDirty ();

	  for (i = 0; i < num_active_effects; i++)
            {
	      if (active_effects[i]->methods->display_effect)
		active_effects[i]->methods->display_effect (
		  current_time - active_effects[i]->start_time,
		  active_effects[i]->params, active_effects[i]->iparam,
		  PVR_LIST_TR_POLY);
	    }

	  pvr_list_finish ();

	  pvr_list_begin (PVR_LIST_PT_POLY);
	  glKosMatrixDirty ();

	  for (i = 0; i < num_active_effects; i++)
            {
	      if (active_effects[i]->methods->display_effect)
		active_effects[i]->methods->display_effect (
		  current_time - active_effects[i]->start_time,
		  active_effects[i]->params, active_effects[i]->iparam,
		  PVR_LIST_PT_POLY);
	    }

	  pvr_list_finish ();

	  //after_sending = timer_ms_gettime64 ();

	  pvr_scene_finish ();
	}

/*
      printf ("transform %d / render %d\n", (int)
        (timer_ms_gettime64 () - after_sending),
	(int) (after_sending - before_sending));
*/
      
#ifdef DEBUG
      printf ("finished frame\n");
#endif
    }

  for (i = 0; i < num_active_effects; i++)
    if (active_effects[i]->methods->uninit_effect)
      {
	active_effects[i]->methods->uninit_effect (
	  active_effects[i]->params);
      }

  pthread_mutex_lock (&terminate_sound_mutex);
  terminate_sound = 1;
  pthread_mutex_unlock (&terminate_sound_mutex);

  rc = pthread_join (sound_thread_hnd, &status);
  printf ("Terminated sound buffer filling thread (%d)\n", rc);

  snd_stream_destroy (sndstream_hnd);
  snd_stream_shutdown ();

  glKosShutdown ();
  pvr_shutdown ();
  vid_shutdown ();
  
  return 0;
}
