#include <math.h>
#include <string.h>

#include <GL/gl.h>

#define FLOATTYPE GLfloat

#include "transform.h"

void
test1 (void)
{
  vec3 vec = { 1.0, 0.0, 0.0 };
  vec3 up = { 0.0, 1.0, 0.0 };
  mat33 rot;

  printf ("test 1:\n");

  vec3_dump (stdout, vec);
  
  mat33_rotate (rot, up, M_PI / 2.0);
  mat33_mul_vec3 (vec, rot, vec);
  
  vec3_dump (stdout, vec);
}

void
test2 (void)
{
  vec3 vec1 = { 1.0, 2.0, 3.0 };
  vec3 vec2 = { 0.0, 0.0, 0.0 };
  vec3 axis = { 0.0, 1.0, 0.0 };
  mat44 mat;
  mat44 xlate;
  
  printf ("test 2:\n");
  
  mat44_rotate (mat, axis, M_PI / 4);
  mat44_translate (xlate, (vec3) { 2.0, 0.0, 0.0 });
  
  mat44_mul_mat44 (mat, xlate, mat);
  
  //mat44_translate_mat44 (mat, mat, (vec3) { 2.0, 0.0, 0.0 });
  
  mat44_mul_vec3 (vec1, mat, vec1);
  mat44_mul_vec3 (vec2, mat, vec2);
  
  vec3_dump (stdout, vec1);
  vec3_dump (stdout, vec2);
  
  mat44_invert (mat, mat);
  
  mat44_mul_vec3 (vec1, mat, vec1);
  mat44_mul_vec3 (vec2, mat, vec2);
  
  vec3_dump (stdout, vec1);
  vec3_dump (stdout, vec2);
}

void
test3 (void)
{
  vec3 vec1 = { 1.0, 2.0, 3.0 };
  vec3 vec2 = { 0.0, 0.0, 0.0 };
  vec3 axis = { 0.0, 1.0, 0.0 };
  mat33 mat;
  mat33 xlate;
  
  printf ("test 3:\n");
  
  mat33_rotate (mat, axis, M_PI / 4);
  
  //mat44_translate_mat44 (mat, mat, (vec3) { 2.0, 0.0, 0.0 });
  
  mat33_mul_vec3 (vec1, mat, vec1);
  mat33_mul_vec3 (vec2, mat, vec2);
  
  vec3_dump (stdout, vec1);
  vec3_dump (stdout, vec2);
  
  mat33_invert (mat, mat);
  
  mat33_mul_vec3 (vec1, mat, vec1);
  mat33_mul_vec3 (vec2, mat, vec2);
  
  vec3_dump (stdout, vec1);
  vec3_dump (stdout, vec2);
}

void test4 (void)
{
  vec3 a = { 5.0, 3.0, -1.0 };
  vec3 b = { 4.0, 3.0, -1.0 };
  vec3 c = { 0.0, 3.0, 1.0 };
  vec3 norm;
  FLOATTYPE offset;
  
  printf ("test 4:\n");
  
  plane_from_triangle (norm, &offset, a, b, c);
  
  vec3_dump (stdout, norm);
  printf ("offset: %f\n", offset);
  
  memcpy (a, (vec3) { 5.0, -1.0, -1.0 }, sizeof (GLfloat) * 3);
  memcpy (b, (vec3) { 5.0,  1.0, -1.0 }, sizeof (GLfloat) * 3);
  memcpy (c, (vec3) { 5.0,  0.0,  1.0 }, sizeof (GLfloat) * 3);
  
  plane_from_triangle (norm, &offset, a, b, c);
  
  vec3_dump (stdout, norm);
  printf ("offset: %f\n", offset);
}

int main (int argc, char* argv[])
{
  test1 ();
  test2 ();
  test3 ();
  test4 ();
  return 0;
}
