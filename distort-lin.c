
/*  Triangle subdivision perlin noise blob distortion demo
 *  (c) Julian Brown 2002
 *
 *  Some code nicked from glut examples...
 */

#ifndef DREAMCAST_KOS
#include <GL/glut.h>
#else
#include <kos.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "perlin.h"

GLfloat light_diffuse[] =
{0.9, 0.7, 0.2, 1.0};
GLfloat light_position[] =
{1.0, 1.0, 1.0, 0.0};

GLfloat light2_diffuse[] =
{0.0, 0.1, 0.4, 1.0};
GLfloat light2_position[] =
{-1.0, -1.0, -0.5, 0.0};

GLfloat mat_specular[] = {0.3f, 0.4f, 0.5f, 1.0f};
GLfloat mat_shininess[] = {20.0f};

GLfloat*** sphdata;
int nstrip;
int stripl;
GLfloat rot;

#define T_TOP 0
#define T_LEFT 1
#define T_FRONT 2
#define T_RIGHT 3
#define T_BACK 4
#define T_BOTTOM 5

void normalise(GLfloat* x)
{
  GLfloat leng = 1.0/sqrt(x[2]*x[2] + x[1]*x[1] + x[0]*x[0]);
  x[0] *= leng;
  x[1] *= leng;
  x[2] *= leng;
}

void mid(GLfloat* d, GLfloat* s1, GLfloat* s2)
{
  d[0] = (s1[0]+s2[0])/2.0;
  d[1] = (s1[1]+s2[1])/2.0;
  d[2] = (s1[2]+s2[2])/2.0;
}

#define COPYVEC(A,B) memcpy((A), (B), sizeof(GLfloat)*3);

GLfloat*** mkspheredata(int depth, int* nstrips, int* stripl)
{
  GLfloat points[][3] =
  { {0.0, 1.0, 0.0},  /* top */
    {-1.0, 0.0, 0.0},  /* left */
    {0.0, 0.0, -1.0},  /* front */
    {1.0, 0.0, 0.0},   /* right */
    {0.0, 0.0, 1.0},  /* back */
    {0.0, -1.0, 0.0} };  /* bottom */
  GLfloat*** strips, ***strops;
    
  int striplength = 4;
  int numstrips = 4;
  int i;
  int recur;

  strips = calloc(sizeof(GLfloat**), numstrips);
  for (i=0; i<numstrips; i++)
  {
    int j;
    strips[i] = calloc(sizeof(GLfloat*), striplength);
    for (j=0; j<striplength; j++)
    {
      strips[i][j] = calloc(sizeof(GLfloat), 3);
    }
  }

  COPYVEC(strips[0][0], &points[T_BOTTOM]);
  COPYVEC(strips[0][1], &points[T_LEFT]);
  COPYVEC(strips[0][2], &points[T_FRONT]);
  COPYVEC(strips[0][3], &points[T_TOP]);

  COPYVEC(strips[1][0], &points[T_BOTTOM]);
  COPYVEC(strips[1][1], &points[T_FRONT]);
  COPYVEC(strips[1][2], &points[T_RIGHT]);
  COPYVEC(strips[1][3], &points[T_TOP]);
  
  COPYVEC(strips[2][0], &points[T_BOTTOM]);
  COPYVEC(strips[2][1], &points[T_RIGHT]);
  COPYVEC(strips[2][2], &points[T_BACK]);
  COPYVEC(strips[2][3], &points[T_TOP]);
  
  COPYVEC(strips[3][0], &points[T_BOTTOM]);
  COPYVEC(strips[3][1], &points[T_BACK]);
  COPYVEC(strips[3][2], &points[T_LEFT]);
  COPYVEC(strips[3][3], &points[T_TOP]);
  
  for (recur=0; recur<depth; recur++)
  {
    int newlength = ((striplength-2)*2)+2;
    strops = calloc(sizeof(GLfloat**), numstrips*2);
    for (i=0; i<numstrips*2; i++)
    {
      int j;
      strops[i] = calloc(sizeof(GLfloat*), newlength);
      for (j=0; j<newlength; j++)
      {
        strops[i][j] = calloc(sizeof(GLfloat), 3);
      }
    }

    for (i=0; i<numstrips; i++)
    {
      GLfloat edge[3], side[3], lastedge[3], lastside[3];
      int j, alt = 0;
      int strop_a = 0, strop_b = 0;
      int k = i*2;

  /*    mid(side, strips[i][0], strips[i][1]);
      mid(edge, strips[i][0], strips[i][2]);

      normalise(&side[0]);
      normalise(&edge[0]);*/

      for (j=0; j<striplength-2; j++)
      {
        COPYVEC(lastedge, edge);
        COPYVEC(lastside, side);

        mid(side, strips[i][j], strips[i][j+1]);
        mid(edge, strips[i][j], strips[i][j+2]);

        normalise(&side[0]);
        normalise(&edge[0]);

        if (alt)  // 1st, 3rd, etc.
        {
          COPYVEC(strops[k][strop_a++], lastside);
          COPYVEC(strops[k][strop_a++], lastedge);
          COPYVEC(strops[k][strop_a++], side);

          COPYVEC(strops[k+1][strop_b++], side);
          COPYVEC(strops[k+1][strop_b++], edge);
        }
        else  // 0th, 2nd, 4th eth
        {
          COPYVEC(strops[k][strop_a++], strips[i][j]);

          COPYVEC(strops[k+1][strop_b++], side);
          COPYVEC(strops[k+1][strop_b++], strips[i][j+1]);
        }
        
        alt = !alt;
      }

      /* finish off strop_a */
      mid(side, strips[i][striplength-2], strips[i][striplength-1]);
      normalise(&side[0]);

      COPYVEC(strops[k][strop_a++], strips[i][striplength-2]);
      COPYVEC(strops[k][strop_a++], side);

      /* finish off strop_b */
      COPYVEC(strops[k+1][strop_b++], side);
      COPYVEC(strops[k+1][strop_b++], strips[i][striplength-1]);
    }

    for (i=0; i<numstrips; i++)
    {
      int j;
      for (j=0; j<striplength; j++)
      {
        free(strips[i][j]);
      }
      free(strips[i]);
    }
    free(strips);
    
    strips = strops;

    numstrips *= 2;
    striplength = newlength;
  }
  
  *nstrips = numstrips;
  *stripl = striplength;
  
  return strips;
}

void delspheredata(GLfloat*** strips, int numstrips)
{
  int i;

  for (i=0; i<numstrips; i++)
  {
    int j;
    for (j=0; j<3; j++)
    {
      free(strips[i][j]);
    }
    free(strips[i]);
  }
  free(strips);
}

void mangle(GLfloat*** data, int numstrips, int striplength, GLfloat offx,
  GLfloat offy, GLfloat offz)
{
  int i;
  static int max = 0;
  int rend = 0;

  glPushMatrix();

/*  glLoadIdentity();*/
  glRotatef(rot*5, 0.5, 0.5, 0);

  for (i=0; i<numstrips; i++)
  {
    int j;
    glBegin(GL_TRIANGLE_STRIP);
    
    for (j=0; j<striplength; j++)
    {
      GLfloat n[3], x;
      COPYVEC(n, data[i][j]);
      n[0] += offx;
      n[1] += offy;
      n[2] += offz;
      x = perlin_noise_2D(15+3*n[1]+rot*0.3, 15+2*n[0]+rot*0.4, 4)*0.35;
      n[0] += x * 0.3;
      n[1] += x * 0.12;
      n[2] += x * 0.05;
      glNormal3fv(data[i][j]);
      glVertex3fv(n);
    }
    
    glEnd();
  }
  glPopMatrix();
}


void
idle(void)
{
  static float time;

  time = glutGet(GLUT_ELAPSED_TIME) / 500.0;

  rot = time;
  glutPostRedisplay();
}

void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  /* render sphere display list */
  mangle(sphdata, nstrip, stripl, 0.0, 0.0, 0.0); 
/*  base();*/
  glutSwapBuffers();
}

void
gfxinit(void)
{
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light2_diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, light2_position);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_CULL_FACE);
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* field of view in degree */ 40.0,
  /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */
  glTranslatef(0.0, 0.0, -1.0);
}

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Distort");
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  gfxinit();
  sphdata = mkspheredata(7, &nstrip, &stripl);
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
