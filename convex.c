#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <GL/glut.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define FLOATTYPE GLfloat
#include "transform.h"

GLfloat light_diffuse[] = {0.9, 0.7, 0.2, 1.0};
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

GLfloat light2_diffuse[] = {0.0, 0.1, 0.4, 1.0};
GLfloat light2_position[] = {-1.0, -1.0, -0.5, 0.0};

GLfloat mat_specular[] = {0.3f, 0.4f, 0.5f, 1.0f};
GLfloat mat_shininess[] = {20.0f};

GLfloat rot;

#define NUMPOINTS 100

GLfloat points[NUMPOINTS][3];

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

vertex_info *
add_vertex (vec3 vec, halfedge_info *halfedge, vertex_info *prev)
{
  vertex_info *newvtx = malloc (sizeof (vertex_info));
  
  memcpy (newvtx->vertex, vec, sizeof (GLfloat) * 3);

  newvtx->halfedge = halfedge;
  
  newvtx->next = prev;
  
  return newvtx;
}

face_info *
add_face (halfedge_info *halfedge, face_info *prev)
{
  face_info *newface = malloc (sizeof (face_info));
  
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
  faceref_list *newfr = malloc (sizeof (faceref_list));
  
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

  free (*stack);
  *stack = next;

  return popped;
}

halfedge_info *
add_halfedge (face_info *face, vertex_info *vertex)
{
  halfedge_info *newhalfedge = malloc (sizeof (halfedge_info));
  
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
  hfedgeref_list *newhe = malloc (sizeof (hfedgeref_list));
  
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
  
  free (*list);
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
vertex_list (GLfloat points[][3], int numpoints)
{
  int i;
  vertex_info *vlist = NULL;
  
  for (i = 0; i < numpoints; i++)
    vlist = add_vertex (points[i], NULL, vlist);
  
  return vlist;
}

/***
  Create a tetrahedron using half-edge data structure.

             D
             .
	    /|\
	   / | \
	  /  |  \
	 /   |   \
	/    |C   \
       /   _,+._   \
      /_,-'     '-._\
   A *---------------* B

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
  GLfloat lo[3], hi[3];
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
  
  furthest_extremity = -1;

  sa = loptr[extremities];
  sb = hiptr[extremities];

  for (i = 0; i < 6; i++)
    {
      FLOATTYPE dist
        = vec3_distance_to_line (i < 3 ? loptr[i]->vertex
				       : hiptr[i - 3]->vertex, sa->vertex,
				 sb->vertex);

      dist = fabs (dist);

      if (furthest_extremity == -1 || dist > max_dist)
        {
	  furthest_extremity = i;
	  max_dist = dist;
	}
    }
  
  sc = (furthest_extremity < 3) ? loptr[furthest_extremity]
				: hiptr[furthest_extremity - 3],
  
  printf ("furthest (%d) = %f\n", furthest_extremity, max_dist);

  plane_from_triangle (&base, sa->vertex, sb->vertex, sc->vertex);
  
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

  return initial_simplex (sa, sb, sc, sd);
}

void
assign_points (face_info *flist, vertex_info *vlist)
{
  vertex_info *vptr;
  face_info *fptr;
  vertex_info *vlist_out;
  
  for (fptr = flist; fptr != NULL; fptr = fptr->next)
    {
      int i;
      vec3 tri[3];
      halfedge_info *he = fptr->halfedge;
      plane faceplane;

      for (i = 0; i < 3; i++)
	{
	  memcpy (tri[i], he->vertex->vertex, sizeof (FLOATTYPE) * 3);
	  he = he->prev;
	}

      plane_from_triangle (&faceplane, tri[0], tri[1], tri[2]);

      vlist_out = NULL;

      for (vptr = vlist; vptr != NULL;)
	{
	  FLOATTYPE dist = vec3_distance_to_plane (vptr->vertex, &faceplane);
	  vertex_info *next = vptr->next;
	  
	  if (dist > 0.0)
	    {
	      vptr->next = fptr->face_vertices;
	      fptr->face_vertices = vptr;
	    }
	  else
	    {
	      vptr->next = vlist_out;
	      vlist_out = vptr;
	    }

	  vptr = next;
	}

      vlist = vlist_out;
    }
  
  /*for (vptr = vlist; vptr != NULL;)
    {
      vertex_info *next = vptr->next;
      free (vptr);
      vptr = next;
    }*/
}

void
plane_from_face (plane *outplane, const face_info *face)
{
  vec3 pts[3];
  int i;
  halfedge_info *hptr;
  
  for (i = 0, hptr = face->halfedge; i < 3; i++, hptr = hptr->prev)
    memcpy (pts[i], hptr->vertex->vertex, sizeof (GLfloat) * 3);
  
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
	  printf ("setting horizon edge on: %p\n", heptr);
	  push_halfedge (heptr, horizon_edges);
	}
      else if (opp_face->visited_stamp != stamp)
	visible_faces_1 (flist, stamp, horizon_edges, opp_face, face_plane, pt);

      heptr = heptr->next;
    }
  while (heptr != first);
}

faceref_list *
visible_faces (face_info *face, int *vis_stamp, hfedgeref_list **horizon_edges,
	       const plane *face_plane, const vertex_info *pt)
{
  faceref_list *visible = NULL;
  static int stamp = 1;
  
  assert (*horizon_edges == NULL);
  
  visible_faces_1 (&visible, stamp, horizon_edges, face, face_plane, pt);
  
  *vis_stamp = stamp;
  
  stamp++;
  
  return visible;
}

face_info *
replace_faces (faceref_list *visible, int stamp, hfedgeref_list **horizon_edges,
	       vertex_info *pt)
{
  face_info *flist = NULL, *fptr;
  hfedgeref_list *hfref_ptr;
  
  for (hfref_ptr = *horizon_edges; hfref_ptr != NULL;
       hfref_ptr = hfref_ptr->next)
    {
      halfedge_info *horizon_edge = hfref_ptr->halfedge;
      face_info *newface;
      halfedge_info *horiz_to_pt;
      halfedge_info *pt_to_horiz;

      /*printf ("horizon edge %p: prev=%p, next=%p\n", horizon_edge,
	      horizon_edge->prev, horizon_edge->next);*/

      flist = newface = add_face (horizon_edge, flist);

      horiz_to_pt = add_halfedge (newface, horizon_edge->opposite->vertex);
      pt_to_horiz = add_halfedge (newface, pt);

      horizon_edge->next = horiz_to_pt;
      horizon_edge->prev = pt_to_horiz;

      horiz_to_pt->next = pt_to_horiz;
      horiz_to_pt->prev = horizon_edge;

      pt_to_horiz->next = horizon_edge;
      pt_to_horiz->prev = horiz_to_pt;

      assert (horizon_edge->vertex->halfedge == NULL);

      horizon_edge->vertex->halfedge = horizon_edge;
      horizon_edge->face = newface;
    }
  
  for (hfref_ptr = *horizon_edges; hfref_ptr != NULL;
       hfref_ptr = hfref_ptr->next)
    {
      halfedge_info *horizon_edge = hfref_ptr->halfedge;
      halfedge_info *next_horiz = horizon_edge->opposite->vertex->halfedge;
      printf ("chasing horizon: %p next: %p\n", horizon_edge, next_horiz);
      /* Join up opposite edges.  */
      horizon_edge->next->opposite = next_horiz->prev;
      next_horiz->prev->opposite = horizon_edge->next;
    }
  
  /* Clear vertex halfedge pointers.  */
  while (*horizon_edges)
    {
      halfedge_info *horizon_edge = pop_halfedge (horizon_edges);
      horizon_edge->vertex->halfedge = NULL;
    }
  
  printf ("visible: %p\n", visible);
  
  while (visible)
    {
      face_info *face = pop_face (&visible);
      halfedge_info *first, *heptr;
      
      first = heptr = face->halfedge;
      
      /*do
        {
	  halfedge_info *next = heptr->next;

	  if (heptr->opposite->opposite != heptr
	      || heptr->next->prev != heptr
	      || heptr->prev->next != heptr)
	    {
	      printf ("detached halfedge: %p\n", heptr);
	      free (heptr);
	    }

	  heptr = next;
	}
      while (heptr != first);*/
      
      printf ("detached face: %p\n", face);
      face->visited_stamp = -1;
    }
  
  return flist;
}

void
idle (void)
{
  static float time;

  time = glutGet(GLUT_ELAPSED_TIME) / 50.0;

  rot = time;
  glutPostRedisplay();
}

static face_info *faces;

void
display (void)
{
  face_info *fptr;
  GLfloat colours[][3] =
    { { 1.0, 1.0, 0.5 },
      { 1.0, 0.5, 1.0 },
      { 1.0, 0.5, 0.5 },
      { 0.5, 1.0, 1.0 },
      { 0.5, 1.0, 0.5 },
      { 0.5, 0.5, 1.0 },
      { 0.5, 0.5, 0.5 } };
  int faceidx = 0;
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glPushMatrix ();
  
  glRotatef (rot * 5, 0.0, 1.0, 0.0);
  glRotatef (rot * 4.85, 1.0, 0.0, 0.0);
  
  glEnable (GL_LIGHTING);
  
  glColor3f (1.0, 1.0, 1.0);
  
  glBegin (GL_TRIANGLES);
  for (fptr = faces; fptr != NULL; fptr = fptr->next)
    {
      halfedge_info *first = fptr->halfedge, *hptr;
      vec3 pts[3];
      int i;
      
      if (fptr->visited_stamp == -1)
        continue;
      
      hptr = first;
      for (i = 0; i < 3; i++)
        {
          memcpy (pts[i], hptr->vertex->vertex, sizeof (GLfloat) * 3);
	  hptr = hptr->prev;
	}
      vec3_sub_vec3 (pts[2], pts[2], pts[0]);
      vec3_sub_vec3 (pts[1], pts[1], pts[0]);
      vec3_cross_vec3 (pts[0], pts[1], pts[2]);
      vec3_normalize (pts[0], pts[0]);
      glNormal3fv (pts[0]);

      hptr = first;
      do
        {
	  glVertex3fv (hptr->vertex->vertex);
          hptr = hptr->prev;
	}
      while (hptr != first);
    }
  glEnd ();
  
  glDisable (GL_LIGHTING);
  
  glBegin (GL_POINTS);
  glPointSize (4.0f);
  for (faceidx = 0, fptr = faces; fptr != NULL; fptr = fptr->next, faceidx++)
    {
      vertex_info *vptr;

      if (fptr->visited_stamp == -1)
        continue;

      glColor3fv (colours[faceidx & 7]);
      
      for (vptr = fptr->face_vertices; vptr != NULL; vptr = vptr->next)
	glVertex3fv (vptr->vertex);
    }
  glEnd ();
  
  glPopMatrix ();
  
/*  base();*/
  glutSwapBuffers();
}

void
gfxinit (void)
{
#ifndef DREAMCAST_KOS
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light2_diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, light2_position);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
#endif
  glEnable(GL_DEPTH_TEST);
#ifndef DREAMCAST_KOS
  glEnable(GL_COLOR_MATERIAL);
#endif
  glEnable(GL_CULL_FACE);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* field of view in degree */ 40.0,
  /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */
  glTranslatef(0.0, 0.0, -3.0);
}

/* glut main function */
int
main (int argc, char **argv)
{
  int i;
  vertex_info *vlist;
  faceref_list *facestack = NULL;
  face_info *fptr;
  
  for (i = 0; i < NUMPOINTS; i++)
    {
      points[i][0] = drand48 () * 3.0 - 1.5;
      points[i][1] = drand48 () * 3.0 - 1.5;
      points[i][2] = drand48 () * 3.0 - 1.5;
    }
  
  vlist = vertex_list (points, NUMPOINTS);
  faces = locate_simplex_points (vlist);
  assign_points (faces, vlist);
  
  for (fptr = faces; fptr != NULL; fptr = fptr->next)
    if (fptr->face_vertices)
      push_face (fptr, &facestack);
  
  while (facestack != NULL)
    {
      face_info *face = pop_face (&facestack);
      plane faceplane;
      vertex_info *most_distant = NULL, *vptr;
      FLOATTYPE distance = 0.0;
      faceref_list *frlist, *frptr;
      int vis_stamp;
      hfedgeref_list *horizon_edges = NULL;
      face_info *new_faces = NULL, *prevface;
      
      /* Ignore deleted faces.  */
      if (face->visited_stamp == -1)
        continue;
      
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
      
      frlist = visible_faces (face, &vis_stamp, &horizon_edges, &faceplane,
			      most_distant);
      for (frptr = frlist; frptr != NULL; frptr = frptr->next)
        printf ("visible face: %p\n", frptr->face);

      new_faces = replace_faces (frlist, vis_stamp, &horizon_edges,
				 most_distant);

      /*for (prevface = NULL, fptr = faces; fptr != NULL;)
        {
	  face_info *next = fptr->next;

	  if (fptr->visited_stamp == -1)
	    {
	      printf ("delete face: %p\n", fptr);
	      free (fptr);

	      if (prevface)
	        prevface->next = next;
	      else
		faces = next;
	    }

	  prevface = fptr;
	  fptr = next;
	}*/

      /* Attach new faces to polyhedron (append list in-place).  */
      for (fptr = new_faces; fptr != NULL; fptr = fptr->next)
	if (fptr->next == NULL)
	  {
	    fptr->next = faces;
	    break;
	  }

      faces = new_faces;
    }
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Convex hull");
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  gfxinit();
  glutMainLoop();

  return 0;
}
