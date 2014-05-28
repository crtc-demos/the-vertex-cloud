#include <stdlib.h>
#include <string.h>
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
  
  newface->halfedge = halfedge;

  prev->next = newface;
  newface->next = NULL;
  
  return newface;
}

/*** Slot in edge in a loop:

    ,-> prev --> edge1 --.  ,-> new --> prev --> edge1 --.
    |                    |  |                            |
    `- edge3 <-- edge2 <-'  `----- edge3 <-- edge2 <-----'
***/

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
       /  __-+-__  \
      /_--       --_\
   A ----------------- B

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
  va->halfedge = he_ab;
  vb->halfedge = he_bc;
  vc->halfedge = he_cd;
  vd->halfedge = he_da;
  
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

  for (i = 0; i < 6; i++)
    {
      FLOATTYPE dist
        = vec3_distance_to_line (i < 3 ? loptr[i]->vertex
				       : hiptr[i - 3]->vertex,
				 loptr[extremities]->vertex,
				 hiptr[extremities]->vertex);

      dist = fabs (dist);

      if (furthest_extremity == -1 || dist > max_dist)
        {
	  furthest_extremity = i;
	  max_dist = dist;
	}
    }
  
 /* printf ("%f\n", vec3_distance_to_line (loptr[0]->vertex,
		    loptr[extremities]->vertex, hiptr[extremities]->vertex));
  printf ("%f\n", vec3_distance_to_line (loptr[1]->vertex,
		    loptr[extremities]->vertex, hiptr[extremities]->vertex));
  printf ("%f\n", vec3_distance_to_line (loptr[2]->vertex,
		    loptr[extremities]->vertex, hiptr[extremities]->vertex));
  printf ("%f\n", vec3_distance_to_line (hiptr[0]->vertex,
		    loptr[extremities]->vertex, hiptr[extremities]->vertex));
  printf ("%f\n", vec3_distance_to_line (hiptr[1]->vertex,
		    loptr[extremities]->vertex, hiptr[extremities]->vertex));
  printf ("%f\n", vec3_distance_to_line (hiptr[2]->vertex,
		    loptr[extremities]->vertex, hiptr[extremities]->vertex));*/

  printf ("furthest (%d) = %f\n", furthest_extremity, max_dist);

  return NULL;
}

void
idle(void)
{
  static float time;

  time = glutGet(GLUT_ELAPSED_TIME) / 50.0;

  rot = time;
  glutPostRedisplay();
}

void
display(void)
{
  int i;
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glPushMatrix ();
  
  glRotatef (rot * 5, 0.0, 1.0, 0.0);
  
  glEnable (GL_LIGHTING);
  
  glBegin (GL_TRIANGLES);
  glVertex3f (-1.0, -1.0, 0.0);
  glVertex3f (1.0, -1.0, 0.0);
  glVertex3f (0.0, 1.0, 0.0);
  glEnd ();
  
  glDisable (GL_LIGHTING);
  
  glBegin (GL_POINTS);
  for (i = 0; i < NUMPOINTS; i++)
    {
      glColor3f (1.0, 1.0, 1.0);
      glVertex3fv (points[i]);
    }
  glEnd ();
  
  glPopMatrix ();
  
/*  base();*/
  glutSwapBuffers();
}

void
gfxinit(void)
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
int main(int argc, char **argv)
{
  int i;
  vertex_info *vlist;
  
  for (i = 0; i < NUMPOINTS; i++)
    {
      points[i][0] = drand48 () * 3.0 - 1.5;
      points[i][1] = drand48 () * 3.0 - 1.5;
      points[i][2] = drand48 () * 3.0 - 1.5;
    }
  
  vlist = vertex_list (points, NUMPOINTS);
  locate_simplex_points (vlist);
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Convex hull");
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  gfxinit();
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
