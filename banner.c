#include <kos.h>
#include <kmg/kmg.h>
#include <png/png.h>

#include "banner.h"

banner_data banner_0 =
{
  .image = "/rd/banner1.png"
};

banner_data banner_1 =
{
  .image = "/rd/banner2.png"
};

banner_data banner_2 =
{
  .image = "/rd/banner3.png"
};

static void
draw_box (banner_data *data)
{
  pvr_poly_cxt_t cxt;
  pvr_poly_hdr_t poly;
  pvr_vertex_t vert;

  pvr_poly_cxt_txr (&cxt, PVR_LIST_OP_POLY,
		    PVR_TXRFMT_RGB565 | PVR_TXRFMT_TWIDDLED, 1024, 512,
		    data->texture, PVR_FILTER_BILINEAR);
  //pvr_poly_cxt_col (&cxt, PVR_LIST_OP_POLY);
  pvr_poly_compile (&poly, &cxt);
  
  pvr_prim (&poly, sizeof (poly));
  
  vert.flags = PVR_CMD_VERTEX;
  vert.x = 0.0f;
  vert.y = 480.0f;
  vert.z = 0.001f;
  vert.u = 0.0f;
  vert.v = 480.0f / 512.0f;
  vert.argb = 0xffffffff;
  vert.oargb = 0;
  pvr_prim (&vert, sizeof (vert));
  
  vert.x = 0.0f;
  vert.y = 0.0f;
  vert.u = 0.0f;
  vert.v = 0.0f;
  pvr_prim (&vert, sizeof (vert));
  
  vert.x = 640.0f;
  vert.y = 480.0f;
  vert.u = 640.0f / 1024.f;
  vert.v = 480.0f / 512.0f;
  pvr_prim (&vert, sizeof (vert));
  
  vert.flags = PVR_CMD_VERTEX_EOL;
  vert.x = 640.0f;
  vert.y = 0.0f;
  vert.u = 640.0f / 1024.f;
  vert.v = 0.0f;
  pvr_prim (&vert, sizeof (vert));
}

static void
init_effect (void *params)
{
  banner_data *data = params;
  
  //printf ("pvr_mem_malloc: ");
  data->texture = pvr_mem_malloc (1024 * 512 * 2);
  //printf ("returned %p\n", data->texture);
  png_to_texture (data->image, data->texture, PNG_NO_ALPHA);
}

static void
uninit_effect (void *params)
{
  banner_data *data = params;
  
  //printf ("pvr_mem_free: %p\n", data->texture);
  pvr_mem_free (data->texture);
}

static void
render_effect (uint32_t time_offset UNUSED, void *params, int iparam UNUSED,
	       int pass)
{
  banner_data *data = params;

  if (pass != PVR_LIST_OP_POLY)
    return;
  
  draw_box (data);
}

effect_methods banner_methods = {
  .preinit_assets = NULL,
  .init_effect = &init_effect,
  .prepare_frame = NULL,
  .display_effect = &render_effect,
  .uninit_effect = &uninit_effect,
  .finalize = NULL
};
