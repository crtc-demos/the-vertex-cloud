#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
/*#include <GL/glut.h>*/

#include <kos.h>
#include <pthread.h>
#include <kmg/kmg.h>
#include <png/png.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define FLOATTYPE float
#include "transform.h"

#include "convex.h"

#define NUMPOINTS 2000
#if 0
#define debug_printf(...) printf (__VA_ARGS__)
#else
#define debug_printf(...) do { } while (0)
#endif

static FLOATTYPE points[NUMPOINTS][3];
static FLOATTYPE velocity[NUMPOINTS][3];
static FLOATTYPE accn[NUMPOINTS][3];

struct halfedge_info;

typedef struct vertex_info
{
  vec3 vertex;
  struct halfedge_info *halfedge;
  struct vertex_info *next;
} vertex_info;

typedef struct face_info
{
  struct halfedge_info *halfedge;
  vertex_info *face_vertices;
  int visited_stamp;
  struct face_info *next;
} face_info;

typedef struct halfedge_info
{
  face_info *face;
  vertex_info *vertex;
  
  struct halfedge_info *opposite;
  struct halfedge_info *prev;
  struct halfedge_info *next;
} halfedge_info;

typedef struct faceref_list
{
  face_info *face;
  struct faceref_list *next;
} faceref_list;

typedef struct hfedgeref_list
{
  halfedge_info *halfedge;
  struct hfedgeref_list *next;
} hfedgeref_list;

#define USE_POOLS
#undef DEBUG_POOLS

#ifdef USE_POOLS
typedef struct mem_pool {
  char *buffer;
  unsigned int used;
  unsigned int capacity;
  struct mem_pool *next;
} mem_pool;

static mem_pool pools[2] = {
  { .buffer = NULL, .used = 0, .capacity = 0, .next = NULL },
  { .buffer = NULL, .used = 0, .capacity = 0, .next = NULL }
};

static mem_pool *current_pool = NULL;

#define POOL_PART_SIZE	(64 * 1024)

static void
pool_clear (mem_pool *pool)
{
  if (pool->buffer == NULL)
    {
      /* Make an initial pool.  */
      pool->buffer = malloc (POOL_PART_SIZE);
      pool->used = 0;
      pool->capacity = POOL_PART_SIZE;
      pool->next = NULL;
    }
  else
    {
      mem_pool *iter;
  
      for (iter = pool; iter; iter = iter->next)
	iter->used = 0;
    }

  current_pool = pool;
}

static void *
pool_alloc (unsigned int amt)
{
retry:
  if (current_pool->used + amt > current_pool->capacity)
    {
      mem_pool *newpart;
      unsigned int newsize = (amt > POOL_PART_SIZE) ? amt : POOL_PART_SIZE;
      
      if (current_pool->next != NULL)
        {
	  current_pool = current_pool->next;
          goto retry;
	}
      
      newpart = malloc (sizeof (mem_pool));

#ifdef DEBUG_POOLS
      printf ("Pool allocator: allocating new part (size %u)\n", newsize);
#endif
      
      current_pool->next = newpart;
      
      newpart->buffer = malloc (newsize);
      newpart->used = amt;
      newpart->capacity = newsize;
      newpart->next = NULL;
      
      current_pool = newpart;
      
      return &newpart->buffer[0];
    }
  else
    {
      char *block = &current_pool->buffer[current_pool->used];

      current_pool->used += amt;
      
      return block;
    }
}

#define MALLOC(X) pool_alloc (X)
#define FREE(X)

#else  /* !USE_POOLS */

#define MALLOC(X) malloc(X)
#define FREE(X) free(X)

#endif

vertex_info *
add_vertex (vec3 vec, halfedge_info *halfedge, vertex_info *prev)
{
  vertex_info *newvtx = MALLOC (sizeof (vertex_info));
  
  assert (newvtx);
  
  memcpy (newvtx->vertex, vec, sizeof (FLOATTYPE) * 3);

  newvtx->halfedge = halfedge;
  
  newvtx->next = prev;
  
  return newvtx;
}

face_info *
add_face (halfedge_info *halfedge, face_info *prev)
{
  face_info *newface = MALLOC (sizeof (face_info));
  
  assert (newface);
  
  assert (halfedge != NULL);
  
  newface->face_vertices = NULL;
  newface->visited_stamp = 0;
  newface->halfedge = halfedge;

  newface->next = prev;
  
  return newface;
}

void
push_face (face_info *face, faceref_list **stack)
{
  faceref_list *newfr = MALLOC (sizeof (faceref_list));
  
  assert (newfr);
  
  newfr->face = face;
  newfr->next = *stack;
  
  *stack = newfr;
}

face_info *
pop_face (faceref_list **stack)
{
  face_info *popped;
  faceref_list *next;
  
  assert (*stack != NULL);
  
  next = (*stack)->next;
  popped = (*stack)->face;

  FREE (*stack);
  *stack = next;

  return popped;
}

halfedge_info *
add_halfedge (face_info *face, vertex_info *vertex)
{
  halfedge_info *newhalfedge = MALLOC (sizeof (halfedge_info));
  
  assert (newhalfedge);
  
  newhalfedge->face = face;
  newhalfedge->vertex = vertex;

  newhalfedge->opposite = NULL;
  newhalfedge->next = NULL;
  newhalfedge->prev = NULL;
  
  return newhalfedge;
}

void
push_halfedge (halfedge_info *halfedge, hfedgeref_list **list)
{
  hfedgeref_list *newhe = MALLOC (sizeof (hfedgeref_list));
  
  assert (newhe);
  
  newhe->halfedge = halfedge;
  newhe->next = *list;
  
  *list = newhe;
}

halfedge_info *
pop_halfedge (hfedgeref_list **list)
{
  halfedge_info *halfedge;
  hfedgeref_list *next;
  
  assert (*list != NULL);
  
  next = (*list)->next;
  halfedge = (*list)->halfedge;
  
  FREE (*list);
  *list = next;
  
  return halfedge;
}

static void
edge_loop (halfedge_info **hearr, int count, face_info *face)
{
  int i;
  
  for (i = 0; i < count; i++)
    {
      halfedge_info *halfedge = hearr[i];
      halfedge_info *nextedge = hearr[(i + 1 == count) ? 0 : i + 1];
      halfedge_info *prevedge = hearr[(i == 0) ? count - 1 : i - 1];
      
      halfedge->face = face;
      halfedge->next = nextedge;
      halfedge->prev = prevedge;
    }
}

vertex_info *
vertex_list (FLOATTYPE points[][3], int numpoints)
{
  int i;
  vertex_info *vlist = NULL;
  
  for (i = 0; i < numpoints; i++)
    {
      vlist = add_vertex (points[i], NULL, vlist);
      //printf ("ALLOCATED: %p\n", vlist);
    }
  
  return vlist;
}

/***
  Create a tetrahedron using half-edge data structure.

             D
             .
	    /|\
	   / | \
    F4 -> /  |  \ <- F3
	 /  F 1  \ 
	/    |    \
       /   _,+._   \
      /_,-'  C  '-._\
   A *---------------* B
           ^ F2
***/

face_info *
initial_simplex (vertex_info *va, vertex_info *vb, vertex_info *vc,
		 vertex_info *vd)
{
  halfedge_info *he_ab = add_halfedge (NULL, va);
  halfedge_info *he_bd = add_halfedge (NULL, vb);
  halfedge_info *he_da = add_halfedge (NULL, vd);
  
  halfedge_info *he_ac = add_halfedge (NULL, va);
  halfedge_info *he_cb = add_halfedge (NULL, vc);
  halfedge_info *he_ba = add_halfedge (NULL, vb);
  
  halfedge_info *he_db = add_halfedge (NULL, vd);
  halfedge_info *he_bc = add_halfedge (NULL, vb);
  halfedge_info *he_cd = add_halfedge (NULL, vc);
  
  halfedge_info *he_dc = add_halfedge (NULL, vd);
  halfedge_info *he_ca = add_halfedge (NULL, vc);
  halfedge_info *he_ad = add_halfedge (NULL, va);
  
  face_info *f1 = add_face (he_ab, NULL);
  face_info *f2 = add_face (he_ac, f1);
  face_info *f3 = add_face (he_db, f2);
  face_info *f4 = add_face (he_dc, f3);
  
  halfedge_info *loop[3];
  
  /* Set up opposites of edges.  */
  he_ab->opposite = he_ba;
  he_bd->opposite = he_db;
  he_da->opposite = he_ad;
  
  he_ac->opposite = he_ca;
  he_cb->opposite = he_bc;
  he_ba->opposite = he_ab;
  
  he_db->opposite = he_bd;
  he_bc->opposite = he_cb;
  he_cd->opposite = he_dc;
  
  he_dc->opposite = he_cd;
  he_ca->opposite = he_ac;
  he_ad->opposite = he_da;
  
  /* Set up halfedges in loops.  */
  loop[0] = he_ab;
  loop[1] = he_bd;
  loop[2] = he_da;
  edge_loop (loop, 3, f1);
  
  loop[0] = he_ac;
  loop[1] = he_cb;
  loop[2] = he_ba;
  edge_loop (loop, 3, f2);
  
  loop[0] = he_db;
  loop[1] = he_bc;
  loop[2] = he_cd;
  edge_loop (loop, 3, f3);
  
  loop[0] = he_dc;
  loop[1] = he_ca;
  loop[2] = he_ad;
  edge_loop (loop, 3, f4);
  
  /* Set some (arbitrary) half-edges for vertices.  */
  /*va->halfedge = he_ab;
  vb->halfedge = he_bc;
  vc->halfedge = he_cd;
  vd->halfedge = he_da;*/
  
  return f4;
}

face_info *
locate_simplex_points (vertex_info *vlist)
{
  FLOATTYPE lo[3], hi[3];
  vertex_info *loptr[3], *hiptr[3];
  vertex_info *vptr;
  int i, extremities;
  FLOATTYPE max_dist = 0.0;
  int furthest_extremity;
  vertex_info *sa, *sb, *sc, *sd;
  plane base;
  
  for (i = 0; i < 3; i++)
    {
      lo[i] = hi[i] = vlist->vertex[i];
      loptr[i] = hiptr[i] = vlist;
    }
  
  for (vptr = vlist->next; vptr != NULL; vptr = vptr->next)
    for (i = 0; i < 3; i++)
      {
	if (vptr->vertex[i] < lo[i])
	  {
	    lo[i] = vptr->vertex[i];
	    loptr[i] = vptr;
	  }

	if (vptr->vertex[i] > hi[i])
	  {
	    hi[i] = vptr->vertex[i];
	    hiptr[i] = vptr;
	  }
      }
  
  if (hi[0] - lo[0] > hi[1] - lo[1]
      && hi[0] - lo[0] > hi[2] - lo[2])
    extremities = 0;
  else if (hi[1] - lo[1] > hi[0] - lo[0]
	   && hi[1] - lo[1] > hi[2] - lo[2])
    extremities = 1;
  else
    extremities = 2;
  
  debug_printf ("extremities: %d\n", extremities);
  
  furthest_extremity = -1;

  sa = loptr[extremities];
  sb = hiptr[extremities];

  for (i = 0; i < 6; i++)
    {
      FLOATTYPE dist
        = vec3_distance_to_line (i < 3 ? loptr[i]->vertex
				       : hiptr[i - 3]->vertex, sa->vertex,
				 sb->vertex);

      debug_printf ("dist: %f\n", dist);

      dist = fabs (dist);

      if (furthest_extremity == -1 || dist > max_dist)
        {
	  furthest_extremity = i;
	  max_dist = dist;
	}
    }
  
  sc = (furthest_extremity < 3) ? loptr[furthest_extremity]
				: hiptr[furthest_extremity - 3];
  
  debug_printf ("furthest (%d) = %f\n", furthest_extremity, max_dist);

  plane_from_triangle (&base, sa->vertex, sb->vertex, sc->vertex);
  
  debug_printf ("plane [%f,%f,%f]:%f\n", base.normal[0], base.normal[1],
	  base.normal[2], base.offset);
  
  max_dist = 0.0;
  sd = NULL;
  
  for (vptr = vlist; vptr != NULL; vptr = vptr->next)
    {
      FLOATTYPE pt_to_plane = vec3_distance_to_plane (vptr->vertex, &base);
      
      if (fabs (pt_to_plane) > fabs (max_dist))
        {
          max_dist = pt_to_plane;
	  sd = vptr;
	}
    }

  assert (sd != NULL);
  
  if (max_dist < 0.0)
    {
      debug_printf ("initial simplex the right way round\n");
      return initial_simplex (sa, sb, sc, sd);
    }
  else
    {
      debug_printf ("initial simplex inside-out\n");
      return initial_simplex (sb, sa, sc, sd);
    }
}

void
protect_face_vertices (face_info *flist)
{
  face_info *fptr;

  /* Don't count the new polygons' vertices in their point sets (avoiding
     floating-point precision issues with distance-to-plane function).  */
  for (fptr = flist; fptr != NULL; fptr = fptr->next)
    {
      halfedge_info *he = fptr->halfedge;
      halfedge_info *first_he = he;

      if (he != NULL)
	do
          {
	    /*printf ("MARKING VERTEX WITH HALFEDGE: %p (face=%p)\n", he->vertex,
		    fptr->halfedge ? fptr : NULL);*/
	    he->vertex->halfedge = he;
	    he = he->prev;
	  }
	while (he != first_he);
    }
}

void
unprotect_face_vertices (face_info *flist)
{
  face_info *fptr;

  /* Clear vertex halfedge pointers.  */
  for (fptr = flist; fptr != NULL; fptr = fptr->next)
    {
      halfedge_info *he = fptr->halfedge;
      halfedge_info *first_he = he;

      do
        {
	  he->vertex->halfedge = NULL;
	  he = he->prev;
	}
      while (he != first_he);
    }
}

/* Assign points from the faces in FRLIST to the faces in FLIST on a first-come,
   first-served basis.  */

void
assign_points (face_info *flist, faceref_list *frlist, bool delete_unused)
{
  vertex_info *vptr;
  face_info *fptr;
  
  protect_face_vertices (flist);
  
  for (fptr = flist; fptr != NULL; fptr = fptr->next)
    {
      int i = 0;
      vec3 tri[3];
      halfedge_info *he = fptr->halfedge;
      plane faceplane;
      faceref_list *frptr;

      for (i = 0; i < 3; i++)
	{
	  memcpy (tri[i], he->vertex->vertex, sizeof (FLOATTYPE) * 3);
	  he = he->prev;
	}

      assert (fptr->face_vertices == NULL);

      plane_from_triangle (&faceplane, tri[0], tri[1], tri[2]);

      for (frptr = frlist; frptr != NULL; frptr = frptr->next)
        {
          vertex_info *vlist_out = NULL;

	  for (vptr = frptr->face->face_vertices; vptr != NULL;)
	    {
	      FLOATTYPE dist = vec3_distance_to_plane (vptr->vertex,
						       &faceplane);
	      vertex_info *next = vptr->next;

	      if (dist > 0.0 && vptr->halfedge == NULL)
		{
		  vptr->next = fptr->face_vertices;
		  fptr->face_vertices = vptr;
		  //printf ("IS FACE VERTEX: %p (face=%p)\n", vptr, fptr);
		}
	      else
		{
		  vptr->next = vlist_out;
		  vlist_out = vptr;
		  /*printf ("BEHIND FACE: %p (leaving on face %p)\n", vptr,
			  frptr->face);*/
		}

	      vptr = next;
	    }

          frptr->face->face_vertices = vlist_out;
	}
    }

  if (delete_unused)
    {
      faceref_list *frptr;

      for (frptr = frlist; frptr != NULL; frptr = frptr->next)
	for (vptr = frptr->face->face_vertices; vptr != NULL;)
          {
	    vertex_info *next = vptr->next;
	    if (vptr->halfedge == NULL)
	      {
	        //printf ("FREE UNUSED IN ASSIGN_POINTS: %p\n", vptr);
	        FREE (vptr);
	      }
	    vptr = next;
	  }
    }

  unprotect_face_vertices (flist);
}

void
init_convex_hull (face_info **faces, faceref_list **facestack,
		  vertex_info *vlist)
{
  face_info *fptr;
  face_info tmpface;
  faceref_list tmpflist;

  *faces = locate_simplex_points (vlist);

  tmpface.face_vertices = vlist;
  tmpface.next = NULL;
  tmpflist.face = &tmpface;
  tmpflist.next = NULL;
  assign_points (*faces, &tmpflist, true);

  *facestack = NULL;
  
  for (fptr = *faces; fptr != NULL; fptr = fptr->next)
    if (fptr->face_vertices)
      push_face (fptr, facestack);
}

void
plane_from_face (plane *outplane, const face_info *face)
{
  vec3 pts[3];
  int i;
  halfedge_info *hptr;
  
  for (i = 0, hptr = face->halfedge; i < 3; i++, hptr = hptr->prev)
    memcpy (pts[i], hptr->vertex->vertex, sizeof (FLOATTYPE) * 3);
  
  plane_from_triangle (outplane, pts[0], pts[1], pts[2]);
}

static void
visible_faces_1 (faceref_list **flist, int stamp,
		 hfedgeref_list **horizon_edges, face_info *face,
		 const plane *face_plane, const vertex_info *pt)
{
  halfedge_info *first = face->halfedge, *heptr;
  
  face->visited_stamp = stamp;
  
  push_face (face, flist);

  heptr = first;

  do
    {
      halfedge_info *opposite = heptr->opposite;
      plane opp_plane;
      face_info *opp_face = opposite->face;
      
      plane_from_face (&opp_plane, opp_face);
      
      if (vec3_distance_to_plane (pt->vertex, &opp_plane) < 0.0)
        {
	  debug_printf ("setting horizon edge on: %p\n", heptr);
	  push_halfedge (heptr, horizon_edges);
	}
      else if (opp_face->visited_stamp != stamp)
	visible_faces_1 (flist, stamp, horizon_edges, opp_face, face_plane, pt);

      heptr = heptr->next;
    }
  while (heptr != first);
}

faceref_list *
visible_faces (face_info *face, hfedgeref_list **horizon_edges,
	       const plane *face_plane, const vertex_info *pt)
{
  faceref_list *visible = NULL;
  static int stamp = 1;
  
  assert (*horizon_edges == NULL);
  
  visible_faces_1 (&visible, stamp, horizon_edges, face, face_plane, pt);
  
  stamp++;
  
  return visible;
}

/* Unlink a halfedge, possibly changing a reference to the same halfedge to
   point to one of the previous neighbours.  */

void
unlink_halfedge (halfedge_info *halfedge, halfedge_info **hfref)
{
  debug_printf ("unlinking halfedge %p", halfedge);
  halfedge->prev->next = halfedge->next;
  halfedge->next->prev = halfedge->prev;
  if (halfedge == *hfref)
    {
      if (halfedge == halfedge->next)
        {
	  debug_printf (": no more halfedges");
	  *hfref = NULL;
	}
      else
        {
	  debug_printf (": updating link from face to %p", halfedge->next);
	  *hfref = halfedge->next;
	}
    }
  debug_printf ("\n");
}

face_info *
replace_faces (faceref_list *visible, hfedgeref_list **horizon_edges,
	       vertex_info *pt)
{
  face_info *flist = NULL /*, *fptr*/;
  hfedgeref_list *hfref_ptr;
  
  for (hfref_ptr = *horizon_edges; hfref_ptr != NULL;
       hfref_ptr = hfref_ptr->next)
    {
      halfedge_info *horizon_edge = hfref_ptr->halfedge;
      face_info *newface;
      halfedge_info *horiz_to_pt;
      halfedge_info *pt_to_horiz;

      /*debug_printf ("horizon edge %p: prev=%p, next=%p\n", horizon_edge,
	      horizon_edge->prev, horizon_edge->next);*/

      flist = newface = add_face (horizon_edge, flist);

      horiz_to_pt = add_halfedge (newface, horizon_edge->opposite->vertex);
      pt_to_horiz = add_halfedge (newface, pt);

      unlink_halfedge (horizon_edge, &horizon_edge->face->halfedge);

      horizon_edge->next = horiz_to_pt;
      horizon_edge->prev = pt_to_horiz;

      horiz_to_pt->next = pt_to_horiz;
      horiz_to_pt->prev = horizon_edge;

      pt_to_horiz->next = horizon_edge;
      pt_to_horiz->prev = horiz_to_pt;

      assert (horizon_edge->vertex->halfedge == NULL);

      debug_printf ("setting horizon edge vertex %p to %p\n",
		    horizon_edge->vertex, horizon_edge);
      horizon_edge->vertex->halfedge = horizon_edge;
      horizon_edge->face = newface;
    }
  
  for (hfref_ptr = *horizon_edges; hfref_ptr != NULL;
       hfref_ptr = hfref_ptr->next)
    {
      halfedge_info *horizon_edge = hfref_ptr->halfedge;
      halfedge_info *next_horiz = horizon_edge->opposite->vertex->halfedge;
      debug_printf ("chasing horizon: %p next: %p\n", horizon_edge, next_horiz);
      /* Join up opposite edges.  */
      horizon_edge->next->opposite = next_horiz->prev;
      next_horiz->prev->opposite = horizon_edge->next;
    }
  
  /* Clear vertex halfedge pointers.  */
  while (*horizon_edges)
    {
      halfedge_info *horizon_edge = pop_halfedge (horizon_edges);
      debug_printf ("clearing horizon vertex halfedge %p\n",
		    horizon_edge->vertex);
      horizon_edge->vertex->halfedge = NULL;
    }
  
  debug_printf ("visible: %p\n", visible);
  
  /* Free all the halfedges for the face now, but leave the face itself to be
     freed later because we still have references to it elsewhere (e.g. in the
     work stack of faces to be processed).  */
  for (; visible != NULL; visible = visible->next)
    {
      face_info *face = visible->face;
      halfedge_info *halfedge = face->halfedge;
      halfedge_info *first = halfedge;
      
      /* We either have a NULL halfedge or a circular list of them: that's why
         this construct looks a bit weird.  */
      if (halfedge)
	do
          {
	    halfedge_info *next = halfedge->next;
	    assert (halfedge->face == face);
	    FREE (halfedge);
	    halfedge = next;
	  }
	while (halfedge != first);

      debug_printf ("mark face deleted: %p\n", face);
      face->halfedge = NULL;
    }
  
  return flist;
}

void cleanup (face_info **faces);

void
convex_hull_step (face_info **faces, faceref_list **facestack, bool all)
{
  face_info *fptr;

restart:
  if (*facestack != NULL)
    {
      face_info *face = pop_face (facestack);
      plane faceplane;
      vertex_info *most_distant = NULL, *vptr;
      FLOATTYPE distance = 0.0;
      faceref_list *frlist, *frptr;
      hfedgeref_list *horizon_edges = NULL;
      face_info *new_faces = NULL;
      
      /* Ignore deleted faces.  */
      if (face->halfedge == NULL)
        goto restart;
      
      plane_from_face (&faceplane, face);

      for (vptr = face->face_vertices; vptr != NULL; vptr = vptr->next)
        {
	  FLOATTYPE thispt = vec3_distance_to_plane (vptr->vertex, &faceplane);

	  assert (thispt > 0.0);

	  if (thispt > distance)
	    {
	      distance = thispt;
	      most_distant = vptr;
	    }
	}

      assert (most_distant && distance > 0.0);
      
      frlist = visible_faces (face, &horizon_edges, &faceplane, most_distant);
      for (frptr = frlist; frptr != NULL; frptr = frptr->next)
        debug_printf ("visible face: %p\n", frptr->face);

      new_faces = replace_faces (frlist, &horizon_edges, most_distant);

      assign_points (new_faces, frlist, false);

      /* We don't need the list of new face refs any more.  */
      while (frlist)
        {
	  faceref_list *next = frlist->next;
	  FREE (frlist);
	  frlist = next;
	}

      /* Attach new faces to polyhedron (append list in-place).  */
      for (fptr = new_faces; fptr != NULL; fptr = fptr->next)
        {
	  /* Push new faces onto stack.  */
	  if (fptr->face_vertices)
	    push_face (fptr, facestack);

	  if (fptr->next == NULL)
	    {
	      fptr->next = *faces;
	      break;
	    }
	}

      assert (fptr != NULL);

      *faces = new_faces;
      
      if (all)
        goto restart;
    }
  else
    cleanup (faces);
}

void
cleanup (face_info **faces)
{
  face_info *livefaces = NULL, *fptr;
  
  /* Mark the live vertices by setting their halfedge pointer to something
     reasonable.  */
  protect_face_vertices (*faces);
  
  for (fptr = *faces; fptr != NULL;)
    {
      face_info *next = fptr->next;
      
      if (fptr->halfedge != NULL)
        {
	  fptr->next = livefaces;
	  livefaces = fptr;
	}
      else
        {
	  vertex_info *vptr;
	  
	  /* Delete any face_vertices we might have left on a deleted face.  */
	  for (vptr = fptr->face_vertices; vptr != NULL;)
	    {
	      vertex_info *next = vptr->next;
	      if (vptr->halfedge == NULL)
	        {
		  debug_printf ("deleting vertex %p\n", vptr);
		  //printf ("DELETE IN CLEANUP: %p (face=%p)\n", vptr, fptr);
		  FREE (vptr);
		}
	      /*else
	        printf ("HALFEDGE NOT NULL: %p (face=%p)\n", vptr, fptr);*/
	      vptr = next;
	    }

          debug_printf ("deleting face %p\n", fptr);
	  //printf ("DELETE FACE IN CLEANUP: %p\n", fptr);
	  FREE (fptr);
	}
    
      fptr = next;
    }
  
  *faces = livefaces;
}

/* Our vertices started out in a list, but by the time the convex hull has been
   computed aren't in any sensible order.  Go through each of the vertices
   relinking them into a single list.  */

vertex_info *
get_vertex_list (face_info *faces)
{
  vertex_info *vlist = NULL, *vnext;
  face_info *fptr;
  int vcount = 0;
  
  /* First zero all the vertices' next pointers.  This makes each vertex a
     singleton list.  */
  
  for (fptr = faces; fptr != NULL; fptr = fptr->next)
    {
      halfedge_info *hptr = fptr->halfedge;
      halfedge_info *first = hptr;
      
      do
        {
	  hptr->vertex->next = NULL;
	  hptr = hptr->next;
	}
      while (hptr != first);
    }
  
  /* Now go through the vertices again.  If a vertex has a next pointer, it is
     in the list already.  If it doesn't, then add it to VLIST (a circular
     list).  */

  for (fptr = faces; fptr != NULL; fptr = fptr->next)
    {
      halfedge_info *hptr = fptr->halfedge;
      halfedge_info *first = hptr;
      
      //printf ("SCAN FACE: %p\n", fptr);
      
      do
        {
	  vertex_info *vtx = hptr->vertex;
	  
	  //printf ("CHECK VERTEX IN LIST: %p\n", vtx);
	  
	  if (vlist == NULL)
	    vlist = vtx->next = vtx;
	  else if (vtx->next == NULL)
	    {
	      vtx->next = vlist->next;
	      vlist->next = vtx;
	      //printf ("ADD VERTEX TO LIST: %p (face=%p)\n", vtx, fptr);
	      vcount++;
	    }

	  hptr = hptr->next;
	}
      while (hptr != first);
    }
  
  /* Now untie the circular list to make it linear.  */
  
  vnext = vlist->next;
  vlist->next = NULL;
  vlist = vnext;
  
  //printf ("vertices in final polyhedron: %d\n", vcount);
  
  return vlist;
}

static void
free_vertex_list (vertex_info *vlist)
{
  while (vlist)
    {
      vertex_info *next = vlist->next;
      //printf ("DELETE WITH CONVEX HULL: %p\n", vlist);
      FREE (vlist);
      vlist = next;
    }
}

void
delete_convex_hull (face_info **faces)
{
#ifndef USE_POOLS
  face_info *fptr;
  
  free_vertex_list (get_vertex_list (*faces));
  
  for (fptr = *faces; fptr != NULL;)
    {
      face_info *next = fptr->next;
      halfedge_info *hptr = fptr->halfedge;
      halfedge_info *first = hptr;
      
      assert (fptr->face_vertices == NULL);
      
      do
        {
	  halfedge_info *next = hptr->next;
	  FREE (hptr);
	  hptr = next;
	}
      while (hptr != first);

      //printf ("DELETE CONVEX HULL FACE: %p\n", fptr);
      FREE (fptr);

      fptr = next;
    }

#endif
  *faces = NULL;
}

static face_info *faces[2] = { NULL, NULL };
static faceref_list *facestack;
static volatile int drawing_face = 0;
static volatile int ask_quit = 0;

void
setup_points ()
{
  int i;
  
  for (i = 0; i < NUMPOINTS; i++)
    {
      do
        {
	  points[i][0] = drand48 () * 3.0 - 1.5;
	  points[i][1] = drand48 () * 3.0 - 1.5;
	  points[i][2] = drand48 () * 3.0 - 1.5;
	  velocity[i][0] = drand48 () * 3.0 - 1.5;
	  velocity[i][1] = drand48 () * 3.0 - 1.5;
	  velocity[i][2] = drand48 () * 3.0 - 1.5;
	  accn[i][0] = accn[i][1] = accn[i][2] = 0.0;
	}
      while (vec3_length (points[i]) > 1.8);
    }
}

static pthread_mutex_t move_points_mutex = PTHREAD_MUTEX_INITIALIZER;
static volatile unsigned int points_mode = 0;

static unsigned int
move_points (unsigned int numpoints)
{
  unsigned int i;
  const float t = 0.01;
  
  for (i = 0; i < numpoints; i++)
    {
      points[i][0] += velocity[i][0] * t;
      points[i][1] += velocity[i][1] * t;
      points[i][2] += velocity[i][2] * t;
      velocity[i][0] += -points[i][0] * t;
      velocity[i][1] += -points[i][1] * t;
      velocity[i][2] += -points[i][2] * t;
    }
  
  return numpoints;
}

static unsigned int
move_points_twist (void)
{
  int i, j;
  unsigned int count = 0;
  static float offs = 0.3;
  
  for (j = -3; j <= 3; j++)
    {
      float y = sin (j * M_PI / 3.2);
      float amt = 2.0 * cos (j * M_PI / 3.2);
      float flop = (j & 1) ? -1.0 : 1.0;

      if (j == 0)
        amt = -amt;

      for (i = 0; i < 7; i++)
	{
	  float x = amt * cos (flop * (offs + (float) i * 2.0 * M_PI / 7.0));
	  float z = amt * sin (flop * (offs + (float) i * 2.0 * M_PI / 7.0));
	  points[count][0] = x;// + drand48() / 100.0;
	  points[count][1] = y;// + drand48() / 100.0;
	  points[count][2] = z;// + drand48() / 100.0;
	  count++;
	}
    }
  
  offs += 0.01;
  
  return count;
}

static kos_img_t envmap_txr;
static pvr_ptr_t envmap_texaddr;
static GLuint envmap_binding;

/* We use these later, so make it global.  */
static GLfloat mview[16];
static GLfloat rotpart[16];

static void
draw_convex_hull (face_info *fptr, int draw_type, float rotational)
{
  vec3 eyepos = { 0.0, 0.0, -10.0 };
  vec3 v_eyedir = { 0.0, 0.0, -1.0 };
  
  glPushMatrix ();
  glRotatef (rotational, 0.0, 1.0, 0.0);
  glRotatef (rotational / 3.5, 0.0, 0.0, 1.0);
  glGetFloatv (GL_MODELVIEW_MATRIX, mview);
  
  mat44_extract_rotation (rotpart, mview);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  
  rotational += 1;
  
  if (draw_type == 0)
    glDisable (GL_TEXTURE_2D);
  else if (draw_type == 1)
    {
      glEnable (GL_TEXTURE_2D);
      glBindTexture (GL_TEXTURE_2D, envmap_binding);
      /*PVR_TXRFMT_VQ_ENABLE | PVR_TXRFMT_TWIDDLED | PVR_TXRFMT_ARGB4444*/
      glKosTex2D (PVR_TXRFMT_RGB565 | PVR_TXRFMT_TWIDDLED,
		  1024, 1024, envmap_texaddr);
    }
  
  glBegin (GL_TRIANGLES);
  for (; fptr != NULL; fptr = fptr->next)
    {
      halfedge_info *first = fptr->halfedge, *hptr;
      vec3 pts[3];
      vec3 pt_rot, norm_rot;
      int i;
      
      if (fptr->halfedge == NULL)
        continue;

      hptr = first;
      for (i = 0; i < 3; i++)
        {
	  //printf ("copying %p to %p\n", hptr->vertex->vertex, pts[i]);
	  memcpy (pts[i], hptr->vertex->vertex, sizeof (FLOATTYPE) * 3);
	  hptr = hptr->prev;
	}
      vec3_sub_vec3 (pts[2], pts[2], pts[0]);
      vec3_sub_vec3 (pts[1], pts[1], pts[0]);
      vec3_cross_vec3 (pts[0], pts[1], pts[2]);
      vec3_normalize (pts[0], pts[0]);
      //glNormal3fv (pts[0]);
      if (draw_type == 0)
	glColor3f (0.5+0.5*pts[0][0], 0.5+0.5*pts[0][1], 0.5+0.5*pts[0][2]);
      else
	glColor3f (1.0, 1.0, 1.0);

      transp_mat44_mul_vec3 (norm_rot, rotpart, pts[0]);
      //printf ("length of norm_rot:%f\n", vec3_length (norm_rot));

      hptr = first;
      do
        {
	  vec3 incident, reflect, tmp;
	  float norm_dot_incident;
	  
	  transp_mat44_mul_vec3 (pt_rot, mview, hptr->vertex->vertex);
	  
	  if (draw_type == 1)
	    {
	      vec3_sub_vec3 (incident, pt_rot, eyepos);
	      vec3_normalize (incident, incident);
	      norm_dot_incident = vec3_dot_vec3 (norm_rot, incident);
	      vec3_scale (tmp, norm_rot, 2 * norm_dot_incident);
	      vec3_add_vec3 (reflect, incident, tmp);

	      vec3_add_vec3 (reflect, reflect, v_eyedir);
	      vec3_normalize (reflect, reflect);

	      glTexCoord2f (0.5+0.5*reflect[0], 0.5-0.5*reflect[1]);
	    }
	  glVertex3fv (pt_rot);
          hptr = hptr->prev;
	}
      while (hptr != first);
    }
  glEnd ();
  
  glPopMatrix ();
}

typedef struct fragment_info
{
  vec3 pts[3];
  vec3 vel;
  struct fragment_info *next;
} fragment_info;

static mem_pool smash_pool =
  { .buffer = NULL, .used = 0, .capacity = 0, .next = NULL };

fragment_info *
smash_object (face_info *fptr)
{
  fragment_info *frags = NULL;
  mem_pool *save_pool = current_pool;

  pool_clear (&smash_pool);

  for (; fptr != NULL; fptr = fptr->next)
    {
      halfedge_info *hptr = fptr->halfedge;
      fragment_info *newfrag;
      int i;
      
      newfrag = MALLOC (sizeof (fragment_info));
      newfrag->next = frags;
      frags = newfrag;
      
      for (i = 0; i < 3; i++)
        {
	  transp_mat44_mul_vec3 (newfrag->pts[i], mview, hptr->vertex->vertex);
	  hptr = hptr->prev;
	}
      /* Just any old vertex.  */
      transp_mat44_mul_vec3 (newfrag->vel, rotpart, hptr->vertex->vertex);
      vec3_scale (newfrag->vel, newfrag->vel, 0.04);
      newfrag->vel[1] += 0.1;
    }
  
  current_pool = save_pool;
  
  return frags;
}

static void
draw_smashed (fragment_info *frags)
{
  vec3 eyepos = { 0.0, 0.0, -10.0 };
  vec3 v_eyedir = { 0.0, 0.0, -1.0 };
  int i;

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  
  glEnable (GL_TEXTURE_2D);
  glBindTexture (GL_TEXTURE_2D, envmap_binding);
  glKosTex2D (PVR_TXRFMT_RGB565 | PVR_TXRFMT_TWIDDLED,
	      1024, 1024, envmap_texaddr);

  glBegin (GL_TRIANGLES);
  for (; frags != NULL; frags = frags->next)
    {
      vec3 pts[3], incident, reflect, tmp;
      FLOATTYPE norm_dot_incident;
      
      memcpy (pts, frags->pts, sizeof (FLOATTYPE) * 9);
      
      vec3_sub_vec3 (pts[2], pts[2], pts[0]);
      vec3_sub_vec3 (pts[1], pts[1], pts[0]);
      vec3_cross_vec3 (pts[0], pts[1], pts[2]);
      vec3_normalize (pts[0], pts[0]);

      glColor3f (1.0, 1.0, 1.0);
      
      for (i = 0; i < 3; i++)
        {
	  vec3_sub_vec3 (incident, frags->pts[i], eyepos);
	  vec3_normalize (incident, incident);
	  norm_dot_incident = vec3_dot_vec3 (pts[0], incident);
	  vec3_scale (tmp, pts[0], 2 * norm_dot_incident);
	  vec3_add_vec3 (reflect, incident, tmp);

	  vec3_add_vec3 (reflect, reflect, v_eyedir);
	  vec3_normalize (reflect, reflect);

	  glTexCoord2f (0.5+0.5*reflect[0], 0.5-0.5*reflect[1]);
	  glVertex3fv (frags->pts[i]);
	  
	  frags->pts[i][0] += frags->vel[0];
	  frags->pts[i][1] += frags->vel[1];
	  frags->pts[i][2] += frags->vel[2];
	}

      frags->vel[1] -= 0.007;
    }
  glEnd ();
}

static pthread_mutex_t recalc_face_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t ask_quit_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *
recalculate_convex_hull_thread (void *args UNUSED)
{
  int quit = 0;
  int num_points;
  int points_mode_now;
  
  while (!quit)
    {
      int recalc_face = 1 - drawing_face;

      pthread_mutex_lock (&move_points_mutex);
      points_mode_now = points_mode;
      pthread_mutex_unlock (&move_points_mutex);
      
      switch (points_mode_now)
        {
	case 0:
	  num_points = move_points (NUMPOINTS);
	  break;
	case 1:
	  num_points = move_points_twist ();
	  break;
	}

      if (faces[recalc_face])
        {
	  delete_convex_hull (&faces[recalc_face]);
	  pthread_mutex_lock (&ask_quit_mutex);
	  if (ask_quit)
	    quit = 1;
	  pthread_mutex_unlock (&ask_quit_mutex);
	  continue;
	}

#ifdef USE_POOLS
      pool_clear (&pools[recalc_face]);
#endif
      init_convex_hull (&faces[recalc_face], &facestack,
			vertex_list (points, num_points));
      convex_hull_step (&faces[recalc_face], &facestack, true);
      
      pthread_mutex_lock (&recalc_face_mutex);
      drawing_face = recalc_face;
      pthread_mutex_unlock (&recalc_face_mutex);
    }

  if (faces[drawing_face])
    delete_convex_hull (&faces[drawing_face]);

  return NULL;
}

static void
preinit_convex_hull_assets (void)
{
  static int initialised = 0;
  
  if (!initialised)
    {
      setup_points ();

      kmg_to_img ("/rd/spheremap.kmg", &envmap_txr);
      envmap_texaddr = pvr_mem_malloc (envmap_txr.byte_count);
      pvr_txr_load_kimg (&envmap_txr, envmap_texaddr, KOS_IMG_FMT_RGB565);
      kos_img_free (&envmap_txr, 0);
      
      initialised = 1;
    }
}

static void
finalize_convex_hull (void *params UNUSED)
{
}

static matrix_t projection __attribute__((aligned(32)));

static matrix_t screen_mat __attribute__((aligned(32))) =
  {
    { 320.0,  0,     0, 0 },
    { 0,     -240.0, 0, 0 },
    { 0,      0,     1, 0 },
    { 320.0,  240.0, 0, 1 }
  };

#define SUNS 50

static vec3 sunpoints[SUNS];
static vec3 sunvel[SUNS];

static void
init_convex_hull_effect (void *params)
{
  convex_hull_data *cdata = params;
  int i, rc;
  
  //glKosInit ();

  glGenTextures (1, &envmap_binding);
  
  vid_border_color (0, 0, 0);
  pvr_set_bg_color (0.0, 0.0, 0.0);
  
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_CULL_FACE);
  glEnable (GL_TEXTURE_2D);
  glShadeModel (GL_FLAT);
  glClearDepth (1.0f);
  glDepthFunc (GL_LEQUAL);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (30.0,			/* Field of view in degrees.  */
		  640.0 / 480.0,	/* Aspect ratio.  */
		  1.0,			/* Z near.  */
		  50.0);		/* Z far.  */
  glGetFloatv (GL_PROJECTION_MATRIX, (GLfloat *) &projection);

  mat_load (&screen_mat);
  mat_apply (&projection);
  mat_store (&projection);
  
  glKosMatrixDirty ();

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  gluLookAt (0.0, 0.0, -10.0,		/* Eye position.  */
	     0.0, 0.0,   0.0,		/* Centre.  */
	     0.0, 1.0,   0.0);		/* Up.  */

  ask_quit = 0;
  drawing_face = 0;
  faces[0] = faces[1] = NULL;

  cdata->sun_txr = pvr_mem_malloc (256 * 256 * 2);
  png_to_texture ("/rd/sun.png", cdata->sun_txr, PNG_MASK_ALPHA);

  rc = pthread_create (&cdata->conv_hull_thread, NULL,
		       recalculate_convex_hull_thread, NULL);
  if (rc == 0)
    printf ("Created convex hull worker thread\n");
  else
    printf ("Couldn't create thread! (%d)\n", rc);
  
  for (i = 0; i < SUNS; i++)
    {
      float x, y, z, radius;

      do
        {
	  x = drand48 () * 8.0 - 4.0;
	  y = drand48 () * 8.0 - 4.0;
	  z = drand48 () * 8.0 - 4.0;
	  radius = sqrt (x * x + y * y + z * z);
	}
      while (radius < 2.0 || radius > 4.0);

      sunpoints[i][0] = x;
      sunpoints[i][1] = y;
      sunpoints[i][2] = z;
      sunvel[i][0] = x * 0.04;
      sunvel[i][1] = y * 0.04 + 0.1;
      sunvel[i][2] = z * 0.04;
    }

  cdata->fragmented = NULL;
}

static void
display_convex_hull_effect (uint32_t time_offset, void *params,
			    int iparam)
{
  convex_hull_data *cdata = params;
  pvr_poly_cxt_t cxt;
  pvr_poly_hdr_t poly;
  pvr_vertex_t vert;
  int i;
  const float sundown = 29000.0f;

  pthread_mutex_lock (&move_points_mutex);
  points_mode = iparam >> 1;
  pthread_mutex_unlock (&move_points_mutex);

  if (time_offset > 44500 && iparam == 1 && cdata->fragmented == NULL)
    {
      pthread_mutex_lock (&recalc_face_mutex);
      cdata->fragmented = smash_object (faces[drawing_face]);
      pthread_mutex_unlock (&recalc_face_mutex);
      for (i = 0; i < SUNS; i++)
        transp_mat44_mul_vec3 (sunpoints[i], mview, sunpoints[i]);
    }
    
  if (!cdata->fragmented)
    {
      pthread_mutex_lock (&recalc_face_mutex);
      if (faces[drawing_face])
	draw_convex_hull (faces[drawing_face], iparam & 1, time_offset / 20.0);
      pthread_mutex_unlock (&recalc_face_mutex);
    }
  else
    draw_smashed (cdata->fragmented);

  glKosFinishList ();

  if (iparam == 1 && time_offset > sundown)
    {
      pvr_poly_cxt_txr (&cxt, PVR_LIST_TR_POLY,
			PVR_TXRFMT_ARGB1555 | PVR_TXRFMT_TWIDDLED, 256, 256,
			cdata->sun_txr, PVR_FILTER_BILINEAR);

      cxt.blend.src = PVR_BLEND_SRCALPHA;
      cxt.blend.dst = PVR_BLEND_INVSRCALPHA;

      pvr_poly_compile (&poly, &cxt);

      mat_load (&projection);

      for (i = 0; i < SUNS; i++)
	{
	  vec3 cent, higher;
	  float size;
	  float drop;

          if (!cdata->fragmented)
	    transp_mat44_mul_vec3 (cent, mview, sunpoints[i]);
	  else
	    memcpy (cent, sunpoints[i], sizeof (FLOATTYPE) * 3);

	  higher[0] = cent[0];
	  higher[1] = cent[1] - 0.5;
	  higher[2] = cent[2];
	  
	  drop = (time_offset - sundown) / 100.0 - (5.0 + i);
	  
	  if (drop > 0)
	    drop = 0;
	  
	  if (cdata->fragmented)
	    {
	      sunpoints[i][0] += sunvel[i][0];
	      sunpoints[i][1] += sunvel[i][1];
	      sunpoints[i][2] += sunvel[i][2];
	      sunvel[i][1] -= 0.007;
	    }
	  
	  cent[1] -= drop;
	  higher[1] -= drop;
	  
	  mat_trans_single (cent[0], cent[1], cent[2]);
	  mat_trans_single (higher[0], higher[1], higher[2]);

	 // printf ("%f %f\n", cent[0], cent[1]);

	  size = higher[1] - cent[1];

	  pvr_prim (&poly, sizeof (poly));

	  vert.flags = PVR_CMD_VERTEX;
	  vert.x = cent[0] - size;
	  vert.y = cent[1] + size;
	  vert.z = cent[2];
	  vert.u = 0.0f;
	  vert.v = 1.0f;
	  vert.argb = 0x80ffffff;
	  vert.oargb = 0x0;
	  pvr_prim (&vert, sizeof (vert));

	  vert.x = cent[0] - size;
	  vert.y = cent[1] - size;
	  vert.u = 0.0f;
	  vert.v = 0.0f;
	  pvr_prim (&vert, sizeof (vert));

	  vert.x = cent[0] + size;
	  vert.y = cent[1] + size;
	  vert.u = 1.0f;
	  vert.v = 1.0f;
	  pvr_prim (&vert, sizeof (vert));

	  vert.flags = PVR_CMD_VERTEX_EOL;
	  vert.x = cent[0] + size;
	  vert.y = cent[1] - size;
	  vert.u = 1.0f;
	  vert.v = 0.0f;
	  pvr_prim (&vert, sizeof (vert));
	}

      glKosMatrixDirty ();

      //pvr_list_finish ();
      glKosFinishList ();
   }
}

static void
uninit_convex_hull_effect (void *params)
{
  convex_hull_data *cdata = params;
  int rc;
  void *status;

  pthread_mutex_lock (&ask_quit_mutex);
  ask_quit = 1;
  pthread_mutex_unlock (&ask_quit_mutex);
  
  rc = pthread_join (cdata->conv_hull_thread, &status);
  printf ("Joined with convex hull thread (%d)\n", rc);
  
  pvr_mem_free (cdata->sun_txr);
}

convex_hull_data convex_hull_0;

effect_methods convex_hull_methods =
{
  .preinit_assets = &preinit_convex_hull_assets,
  .init_effect = &init_convex_hull_effect,
  .prepare_frame = NULL,
  .display_effect = NULL,
  .gl_effect = &display_convex_hull_effect,
  .uninit_effect = &uninit_convex_hull_effect,
  .finalize = &finalize_convex_hull
};

#if 0
int
main (int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
  int cable_type, quit = 0, rc;
  pthread_t conv_hull_thread;
  void *status;

  cable_type = vid_check_cable ();
  
  if (cable_type == CT_VGA)
    vid_init (DM_640x480_VGA, PM_RGB565);
  else
    vid_init (DM_640x480_PAL_IL, PM_RGB565);

  init_pvr ();
  
  while (!quit)
    {
      MAPLE_FOREACH_BEGIN (MAPLE_FUNC_CONTROLLER, cont_state_t, st)
        if (st->buttons & CONT_START)
	  quit = 1;
      MAPLE_FOREACH_END ()
    }
  
  
  glKosShutdown ();
  
  pvr_shutdown ();
  
  vid_shutdown ();

  return 0;
}
#endif
