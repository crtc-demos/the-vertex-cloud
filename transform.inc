#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "transform.h"

#define M33(M,R,C) ((M)[(R) * 3 + (C)])
#define M44(M,R,C) ((M)[(R) * 4 + (C)])

void
mat33_identity (mat33 mat)
{
  int i;
  
  for (i = 0; i < 3; i++)
    {
      M33(mat, i, 0) = (FLOATTYPE) (i == 0);
      M33(mat, i, 1) = (FLOATTYPE) (i == 1);
      M33(mat, i, 2) = (FLOATTYPE) (i == 2);
    }
}

void
vec3_zero (vec3 vec)
{
  vec[0] = vec[1] = vec[2] = 0.0;
}

void
mat33_mul_vec3 (vec3 dest, const mat33 mat, const vec3 src)
{
  FLOATTYPE x = src[0], y = src[1], z = src[2];
  
  dest[0] = M33(mat, 0, 0) * x + M33(mat, 0, 1) * y + M33(mat, 0, 2) * z;
  dest[1] = M33(mat, 1, 0) * x + M33(mat, 1, 1) * y + M33(mat, 1, 2) * z;
  dest[2] = M33(mat, 2, 0) * x + M33(mat, 2, 1) * y + M33(mat, 2, 2) * z;
}

void
transp_mat33_mul_vec3 (vec3 dest, const mat33 mat, const vec3 src)
{
  FLOATTYPE x = src[0], y = src[1], z = src[2];
  
  dest[0] = M33(mat, 0, 0) * x + M33(mat, 1, 0) * y + M33(mat, 2, 0) * z;
  dest[1] = M33(mat, 0, 1) * x + M33(mat, 1, 1) * y + M33(mat, 2, 1) * z;
  dest[2] = M33(mat, 0, 2) * x + M33(mat, 1, 2) * y + M33(mat, 2, 2) * z;
}

/* Transform pt (dest,1) by 4x4 GL-style matrix:
   ( [0][0]  [0][1]  [0][2]  [0][3] ) ( src[0] )
   ( [1][0]  [1][1]  [1][2]  [1][3] ) ( src[1] )
   ( [2][0]  [2][1]  [2][2]  [2][3] ) ( src[2] )
   ( [3][0]  [3][1]  [3][2]  [3][3] ) (  1.0   )
*/

void
mat44_mul_vec3 (vec3 dest, const mat44 mat, const vec3 src)
{
  FLOATTYPE x = src[0], y = src[1], z = src[2];
  
  dest[0] = M44(mat, 0, 0) * x + M44(mat, 0, 1) * y
	    + M44(mat, 0, 2) * z + M44(mat, 0, 3);
  dest[1] = M44(mat, 1, 0) * x + M44(mat, 1, 1) * y
	    + M44(mat, 1, 2) * z + M44(mat, 1, 3);
  dest[2] = M44(mat, 2, 0) * x + M44(mat, 2, 1) * y
	    + M44(mat, 2, 2) * z + M44(mat, 2, 3);
}

void
transp_mat44_mul_vec3 (vec3 dest, const mat44 mat, const vec3 src)
{
  FLOATTYPE x = src[0], y = src[1], z = src[2];
  
  dest[0] = M44(mat, 0, 0) * x + M44(mat, 1, 0) * y
	    + M44(mat, 2, 0) * z + M44(mat, 3, 0);
  dest[1] = M44(mat, 0, 1) * x + M44(mat, 1, 1) * y
	    + M44(mat, 2, 1) * z + M44(mat, 3, 1);
  dest[2] = M44(mat, 0, 2) * x + M44(mat, 1, 2) * y
	    + M44(mat, 2, 2) * z + M44(mat, 3, 2);
}

void
mat44_mul_vec4 (vec4 dest, const mat44 mat, const vec4 src)
{
  FLOATTYPE x = src[0], y = src[1], z = src[2], w = src[3];
  
  dest[0] = M44(mat, 0, 0) * x + M44(mat, 1, 0) * y
	    + M44(mat, 2, 0) * z + M44(mat, 3, 0) * w;
  dest[1] = M44(mat, 0, 1) * x + M44(mat, 1, 1) * y
	    + M44(mat, 2, 1) * z + M44(mat, 3, 1) * w;
  dest[2] = M44(mat, 0, 2) * x + M44(mat, 1, 2) * y
	    + M44(mat, 2, 2) * z + M44(mat, 3, 2) * w;
  dest[3] = M44(mat, 0, 3) * x + M44(mat, 1, 3) * y
	    + M44(mat, 2, 3) * z + M44(mat, 3, 3) * w;
}

/*
   ( [0][0]  [0][1]  [0][2] ) ( [0][0]  [0][1]  [0][2] )
   ( [1][0]  [1][1]  [1][2] ) ( [1][0]  [1][1]  [1][2] )
   ( [2][0]  [2][1]  [2][2] ) ( [2][0]  [2][1]  [2][2] )
*/

void
mat33_mul_mat33 (mat33 dest, const mat33 a, const mat33 b)
{
  FLOATTYPE s00 = M33(a, 0, 0), s10 = M33(a, 1, 0), s20 = M33(a, 2, 0);
  FLOATTYPE s01 = M33(a, 0, 1), s11 = M33(a, 1, 1), s21 = M33(a, 2, 1);
  FLOATTYPE s02 = M33(a, 0, 2), s12 = M33(a, 1, 2), s22 = M33(a, 2, 2);

  FLOATTYPE t00 = M33(b, 0, 0), t10 = M33(b, 1, 0), t20 = M33(b, 2, 0);
  FLOATTYPE t01 = M33(b, 0, 1), t11 = M33(b, 1, 1), t21 = M33(b, 2, 1);
  FLOATTYPE t02 = M33(b, 0, 2), t12 = M33(b, 1, 2), t22 = M33(b, 2, 2);

  M33(dest, 0, 0) = s00 * t00 + s01 * t10 + s02 * t20;
  M33(dest, 0, 1) = s00 * t01 + s01 * t11 + s02 * t21;
  M33(dest, 0, 2) = s00 * t02 + s01 * t12 + s02 * t22;

  M33(dest, 1, 0) = s10 * t00 + s11 * t10 + s12 * t20;
  M33(dest, 1, 1) = s10 * t01 + s11 * t11 + s12 * t21;
  M33(dest, 1, 2) = s10 * t02 + s11 * t12 + s12 * t22;

  M33(dest, 2, 0) = s20 * t00 + s21 * t10 + s22 * t20;
  M33(dest, 2, 1) = s20 * t01 + s21 * t11 + s22 * t21;
  M33(dest, 2, 2) = s20 * t02 + s21 * t12 + s22 * t22;
}

void
mat33_rotate (mat33 dest, const vec3 axis, FLOATTYPE angle)
{
  FLOATTYPE s = sin (angle), c = cos (angle);
  FLOATTYPE v = 1.0 - c, sx = axis[0], sy = axis[1], sz = axis[2];

  M33(dest, 0, 0) = sx * sx * v + c;
  M33(dest, 0, 1) = sx * sy * v + sz * s;
  M33(dest, 0, 2) = sx * sz * v - sy * s;

  M33(dest, 1, 0) = sx * sy * v - sz * s;
  M33(dest, 1, 1) = sy * sy * v + c;
  M33(dest, 1, 2) = sy * sz * v + sx * s;

  M33(dest, 2, 0) = sx * sz * v + sy * s;
  M33(dest, 2, 1) = sy * sz * v - sx * s;
  M33(dest, 2, 2) = sz * sz * v + c;
}

void
vec2_rotate (vec2 dest, const vec2 src, FLOATTYPE angle)
{
  FLOATTYPE cosa = cos (angle), sina = sin (angle);
  FLOATTYPE sx = src[0], sy = src[1];
  dest[0] = cosa * sx - sina * sy;
  dest[1] = sina * sx + cosa * sy;
}

void
mat33_transpose (mat33 dest, const mat33 src)
{
  FLOATTYPE s00 = M33(src, 0, 0), s01 = M33(src, 0, 1), s02 = M33(src, 0, 2),
	    s10 = M33(src, 1, 0), s11 = M33(src, 1, 1), s12 = M33(src, 1, 2),
	    s20 = M33(src, 2, 0), s21 = M33(src, 2, 1), s22 = M33(src, 2, 2);

  M33(dest, 0, 0) = s00; M33(dest, 0, 1) = s10; M33(dest, 0, 2) = s20;
  M33(dest, 1, 0) = s01; M33(dest, 1, 1) = s11; M33(dest, 1, 2) = s21;
  M33(dest, 2, 0) = s02; M33(dest, 2, 1) = s12; M33(dest, 2, 2) = s22;
}

void
mat33_invert (mat33 dest, const mat33 src)
{
  FLOATTYPE e11 = M33(src, 0, 0), e12 = M33(src, 0, 1), e13 = M33(src, 0, 2);
  FLOATTYPE e21 = M33(src, 1, 0), e22 = M33(src, 1, 1), e23 = M33(src, 1, 2);
  FLOATTYPE e31 = M33(src, 2, 0), e32 = M33(src, 2, 1), e33 = M33(src, 2, 2);
  FLOATTYPE det = e11 * e22 * e33 - e11 * e32 * e23
        	  + e21 * e32 * e13 - e21 * e12 * e33
        	  + e31 * e12 * e23 - e31 * e22 * e13;
  FLOATTYPE idet = (det == 0.0) ? 1.0 : 1.0 / det;
  
  M33(dest, 0, 0) =  (e22 * e33 - e23 * e32) * idet;
  M33(dest, 0, 1) = -(e12 * e33 - e13 * e32) * idet;
  M33(dest, 0, 2) =  (e12 * e23 - e13 * e22) * idet;
  
  M33(dest, 1, 0) = -(e21 * e33 - e23 * e31) * idet;
  M33(dest, 1, 1) =  (e11 * e33 - e13 * e31) * idet;
  M33(dest, 1, 2) = -(e11 * e23 - e13 * e21) * idet;
  
  M33(dest, 2, 0) =  (e21 * e32 - e22 * e31) * idet;
  M33(dest, 2, 1) = -(e11 * e32 - e12 * e31) * idet;
  M33(dest, 2, 2) =  (e11 * e22 - e12 * e21) * idet;
}

/* stolen from Intel, tsk tsk
 * Matrix inversion using Cramer's rule
 */
void
mat44_invert (mat44 dst, const mat44 mat)
{
  FLOATTYPE tmp[12];
  FLOATTYPE src[16];
  FLOATTYPE det;
  int i;
  
  /* transpose matrix */
  for (i = 0; i < 4; i++)
  {
    src[i] = M44(mat, i, 0);
    src[i + 4] = M44(mat, i, 1);
    src[i + 8] = M44(mat, i, 2);
    src[i + 12] = M44(mat, i, 3);
  }
  
  /* calculate pairs for first 8 elements (cofactors) */
  tmp[0] = src[10] * src[15];
  tmp[1] = src[11] * src[14];
  tmp[2] = src[9] * src[15];
  tmp[3] = src[11] * src[13];
  tmp[4] = src[9] * src[14];
  tmp[5] = src[10] * src[13];
  tmp[6] = src[8] * src[15];
  tmp[7] = src[11] * src[12];
  tmp[8] = src[8] * src[14];
  tmp[9] = src[10] * src[12];
  tmp[10] = src[8] * src[13];
  tmp[11] = src[9] * src[12];
  
  /* calculate first 8 elements (cofactors) */
  M44(dst, 0, 0) = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7]
        	   - tmp[1]*src[5] - tmp[2]*src[6] - tmp[5]*src[7];
  M44(dst, 0, 1) = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7]
        	   - tmp[0]*src[4] - tmp[7]*src[6] - tmp[8]*src[7];
  M44(dst, 0, 2) = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7]
        	   - tmp[3]*src[4] - tmp[6]*src[5] - tmp[11]*src[7];
  M44(dst, 0, 3) = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6]
        	   - tmp[4]*src[4] - tmp[9]*src[5] - tmp[10]*src[6];
  M44(dst, 1, 0) = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3]
        	   - tmp[0]*src[1] - tmp[3]*src[2] - tmp[4]*src[3];
  M44(dst, 1, 1) = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3]
        	   - tmp[1]*src[0] - tmp[6]*src[2] - tmp[9]*src[3];
  M44(dst, 1, 2) = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3]
        	   - tmp[2]*src[0] - tmp[7]*src[1] - tmp[10]*src[3];
  M44(dst, 1, 3) = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2]
        	   - tmp[5]*src[0] - tmp[8]*src[1] - tmp[11]*src[2];

  /* calculate pairs for second 8 elements (cofactors) */
  tmp[0] = src[2]*src[7];
  tmp[1] = src[3]*src[6];
  tmp[2] = src[1]*src[7];
  tmp[3] = src[3]*src[5];
  tmp[4] = src[1]*src[6];
  tmp[5] = src[2]*src[5];
  tmp[6] = src[0]*src[7];
  tmp[7] = src[3]*src[4];
  tmp[8] = src[0]*src[6];
  tmp[9] = src[2]*src[4];
  tmp[10] = src[0]*src[5];
  tmp[11] = src[1]*src[4];

  /* calculate second 8 elements (cofactors) */
  M44(dst, 2, 0) = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15]
        	   - tmp[1]*src[13] - tmp[2]*src[14] - tmp[5]*src[15];
  M44(dst, 2, 1) = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15]
        	   - tmp[0]*src[12] - tmp[7]*src[14] - tmp[8]*src[15];
  M44(dst, 2, 2) = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15]
        	   - tmp[3]*src[12] - tmp[6]*src[13] - tmp[11]*src[15];
  M44(dst, 2, 3) = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14]
        	   - tmp[4]*src[12] - tmp[9]*src[13] - tmp[10]*src[14];
  M44(dst, 3, 0) = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9]
        	   - tmp[4]*src[11] - tmp[0]*src[9] - tmp[3]*src[10];
  M44(dst, 3, 1) = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10]
        	   - tmp[6]*src[10] - tmp[9]*src[11] - tmp[1]*src[8];
  M44(dst, 3, 2) = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8]
        	   - tmp[10]*src[11] - tmp[2]*src[8] - tmp[7]*src[9];
  M44(dst, 3, 3) = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9]
        	   - tmp[8]*src[9] - tmp[11]*src[10] - tmp[5]*src[8];

  /* calculate determinant */
  det = src[0] * M44(dst, 0, 0) + src[1] * M44(dst, 0, 1)
	+ src[2] * M44(dst, 0, 2) + src[3] * M44(dst, 0, 3);

  /* calculate matrix inverse */
  det = 1 / det;

  for (i = 0; i < 16; i++)
    dst[i] *= det;
}

void
vec3_cross_vec3 (vec3 r, const vec3 u, const vec3 v)
{
  FLOATTYPE ux = u[0], uy = u[1], uz = u[2], vx = v[0], vy = v[1], vz = v[2];
  r[0] = uy * vz - uz * vy;
  r[1] = uz * vx - ux * vz;
  r[2] = ux * vy - uy * vx;
}

FLOATTYPE
vec3_dot_vec3 (const vec3 a, const vec3 b)
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void
vec3_add_vec3 (vec3 dest, const vec3 src1, const vec3 src2)
{
  FLOATTYPE x0 = src1[0], y0 = src1[1], z0 = src1[2];
  FLOATTYPE x1 = src2[0], y1 = src2[1], z1 = src2[2];

  dest[0] = x0 + x1;
  dest[1] = y0 + y1;
  dest[2] = z0 + z1;
}

void
vec3_sub_vec3 (vec3 dest, const vec3 src1, const vec3 src2)
{
  FLOATTYPE x0 = src1[0], y0 = src1[1], z0 = src1[2];
  FLOATTYPE x1 = src2[0], y1 = src2[1], z1 = src2[2];

  dest[0] = x0 - x1;
  dest[1] = y0 - y1;
  dest[2] = z0 - z1;
}

void
vec3_normalize (vec3 dest, const vec3 pt)
{
  FLOATTYPE x = pt[0], y = pt[1], z = pt[2];
  FLOATTYPE len = sqrt (x * x + y * y + z * z), rlen;
  
  assert (len != 0.0);
  rlen = 1.0 / len;
  
  dest[0] = x * rlen;
  dest[1] = y * rlen;
  dest[2] = z * rlen;
}

FLOATTYPE
vec3_length (const vec3 vec)
{
  FLOATTYPE x = vec[0], y = vec[1], z = vec[2];
  return sqrt (x * x + y * y + z * z);
}

/* Fix matrix drifting away from orthogonality.  */

void
mat33_fixortho (mat33 m)
{
  vec3 col0, col1, col2;

  col0[0] = M33(m, 0, 0);
  col0[1] = M33(m, 1, 0);
  col0[2] = M33(m, 2, 0);
  
  col1[0] = M33(m, 0, 1);
  col1[1] = M33(m, 1, 1);
  col1[2] = M33(m, 2, 1);
  
  vec3_cross_vec3 (col2, col0, col1);
  vec3_cross_vec3 (col0, col1, col2);
  vec3_normalize (col0, col0);
  vec3_normalize (col1, col1);
  vec3_normalize (col2, col2);
  
  M33(m, 0, 0) = col0[0];
  M33(m, 1, 0) = col0[1];
  M33(m, 2, 0) = col0[2];
  
  M33(m, 0, 1) = col1[0];
  M33(m, 1, 1) = col1[1];
  M33(m, 2, 1) = col1[2];
  
  M33(m, 0, 2) = col2[0];
  M33(m, 1, 2) = col2[1];
  M33(m, 2, 2) = col2[2];
}

/* Create a mat44 from a mat33.  */

void
mat44_from_mat33 (mat44 out, const mat33 mat)
{
  M44(out, 0, 0) = M33(mat, 0, 0);
  M44(out, 0, 1) = M33(mat, 0, 1);
  M44(out, 0, 2) = M33(mat, 0, 2);
  M44(out, 0, 3) = 0.0;
  
  M44(out, 1, 0) = M33(mat, 1, 0);
  M44(out, 1, 1) = M33(mat, 1, 1);
  M44(out, 1, 2) = M33(mat, 1, 2);
  M44(out, 1, 3) = 0.0;
  
  M44(out, 2, 0) = M33(mat, 2, 0);
  M44(out, 2, 1) = M33(mat, 2, 1);
  M44(out, 2, 2) = M33(mat, 2, 2);
  M44(out, 2, 3) = 0.0;
  
  M44(out, 3, 0) = 0.0;
  M44(out, 3, 1) = 0.0;
  M44(out, 3, 2) = 0.0;
  M44(out, 3, 3) = 1.0;
}

void
mat44_identity (mat44 out)
{
  int i, j;

  for (j = 0; j < 4; j++)
    for (i = 0; i < 4; i++)
      M44(out, j, i) = (i == j) ? 1.0 : 0.0;
}

/* Matrix-matrix multiply:
  ( a00 a01 a02 a03 )  ( b00 b01 b02 b03 )
  ( a10 a11 a12 a13 )  ( b10 b11 b12 b13 )
  ( a20 a21 a22 a23 )  ( b20 b21 b22 b23 )
  ( a30 a31 a32 a33 )  ( b30 b31 b32 b33 )

*/
void
mat44_mul_mat44 (mat44 out, const mat44 a, const mat44 b)
{
  int row, col, k;
  FLOATTYPE tmp[16];
  
  for (row = 0; row < 4; row++)
    for (col = 0; col < 4; col++)
      {
	FLOATTYPE sum = 0.0;

	for (k = 0; k < 4; k++)
          sum += M44(a, row, k) * M44(b, k, col);

	M44(tmp, row, col) = sum;
      }
  
  memcpy (out, tmp, sizeof (FLOATTYPE) * 16);
}

/* GL-equivalent frustum.  */
void
mat44_frustum (mat44 mat, FLOATTYPE left, FLOATTYPE right, FLOATTYPE bottom,
	       FLOATTYPE top, FLOATTYPE near, FLOATTYPE far)
{
  FLOATTYPE a = (right + left) / (right - left);
  FLOATTYPE b = (top + bottom) / (top - bottom);
  FLOATTYPE c = (far + near) / (far - near);
  FLOATTYPE d = (-2.0 * far * near) / (far - near);

  M44(mat, 0, 0) = 2 * near / (right - left);
  M44(mat, 1, 0) = 0;
  M44(mat, 2, 0) = 0;
  M44(mat, 3, 0) = 0;

  M44(mat, 0, 1) = 0;
  M44(mat, 1, 1) = 2 * near / (top - bottom);
  M44(mat, 2, 1) = 0;
  M44(mat, 3, 1) = 0;

  M44(mat, 0, 2) = a;
  M44(mat, 1, 2) = b;
  M44(mat, 2, 2) = c;
  M44(mat, 3, 2) = -1;

  M44(mat, 0, 3) = 0;
  M44(mat, 1, 3) = 0;
  M44(mat, 2, 3) = d;
  M44(mat, 3, 3) = 0;
}

void
mat44_perspective (mat44 mat, FLOATTYPE fovy, FLOATTYPE aspect,
		   FLOATTYPE nearz, FLOATTYPE farz)
{
  FLOATTYPE sine, cot, fov_rad = fovy / 360.0 * M_PI, deltaz;
  
  deltaz = farz - nearz;
  sine = sin (fov_rad);
  cot = cos (fov_rad) / sine;
    
  memset (mat, 0, sizeof (FLOATTYPE) * 16);
  M44(mat, 0, 0) = cot / aspect;
  M44(mat, 1, 1) = cot;
  M44(mat, 2, 2) = -(farz + nearz) / deltaz;
  M44(mat, 3, 2) = -1;
  M44(mat, 2, 3) = -2 * nearz * farz / deltaz;
  M44(mat, 3, 3) = 0;
}

void
mat44_ortho (mat44 mat, FLOATTYPE left, FLOATTYPE right, FLOATTYPE bottom,
	     FLOATTYPE top, FLOATTYPE near, FLOATTYPE far)
{
  memset (mat, 0, sizeof (FLOATTYPE) * 16);
  M44(mat, 0, 0) = 2.0 / (right - left);
  M44(mat, 1, 1) = 2.0 / (top - bottom);
  M44(mat, 2, 2) = -2.0 / (far - near);
  M44(mat, 0, 3) = -(right + left) / (right - left);
  M44(mat, 1, 3) = -(top + bottom) / (top - bottom);
  M44(mat, 2, 3) = -(far + near) / (far - near);
  M44(mat, 3, 3) = 1.0;
}

void
mat44_translate_mat44 (mat44 dst, const mat44 mat, const vec3 xlate)
{
  FLOATTYPE dx = xlate[0], dy = xlate[1], dz = xlate[2];
  FLOATTYPE tmp[16];

  memcpy (tmp, mat, sizeof (FLOATTYPE) * 12);

  M44(tmp, 3, 0) = M44(mat, 0, 0) * dx + M44(mat, 1, 0) * dy
		   + M44(mat, 2, 0) * dz + M44(mat, 3, 0);
  M44(tmp, 3, 1) = M44(mat, 0, 1) * dx + M44(mat, 1, 1) * dy
		   + M44(mat, 2, 1) * dz + M44(mat, 3, 1);
  M44(tmp, 3, 2) = M44(mat, 0, 2) * dx + M44(mat, 1, 2) * dy
		   + M44(mat, 2, 2) * dz + M44(mat, 3, 2);
  M44(tmp, 3, 3) = M44(mat, 0, 3) * dx + M44(mat, 1, 3) * dy
		   + M44(mat, 2, 3) * dz + M44(mat, 3, 3);

  memcpy (dst, tmp, sizeof (FLOATTYPE) * 16);
}

void
mat44_translate (mat44 dst, const vec3 xlate)
{
  M44(dst, 0, 0) = 1;
  M44(dst, 0, 1) = 0;
  M44(dst, 0, 2) = 0;
  M44(dst, 0, 3) = xlate[0];
  
  M44(dst, 1, 0) = 0;
  M44(dst, 1, 1) = 1;
  M44(dst, 1, 2) = 0;
  M44(dst, 1, 3) = xlate[1];
  
  M44(dst, 2, 0) = 0;
  M44(dst, 2, 1) = 0;
  M44(dst, 2, 2) = 1;
  M44(dst, 2, 3) = xlate[2];
  
  M44(dst, 3, 0) = 0;
  M44(dst, 3, 1) = 0;
  M44(dst, 3, 2) = 0;
  M44(dst, 3, 3) = 1;
}

/* GL-equivalent lookat */
void
mat44_lookat (mat44 dst, const vec3 eye, const vec3 centre, const vec3 up)
{
  vec3 l, s, uq, minus_eye;
  mat44 mat;

  vec3_sub_vec3 (l, centre, eye);
  vec3_normalize (l, l);
  vec3_cross_vec3 (s, l, up);
  vec3_normalize (s, s);
  vec3_cross_vec3 (uq, s, l);

  /* Note: this is transposed, for the inverse transform.  */
  M44(mat, 0, 0) = s[0];
  M44(mat, 0, 1) = uq[0];
  M44(mat, 0, 2) = -l[0];
  M44(mat, 0, 3) = 0;
  
  M44(mat, 1, 0) = s[1];
  M44(mat, 1, 1) = uq[1];
  M44(mat, 1, 2) = -l[1];
  M44(mat, 1, 3) = 0;
  
  M44(mat, 2, 0) = s[2];
  M44(mat, 2, 1) = uq[2];
  M44(mat, 2, 2) = -l[2];
  M44(mat, 2, 3) = 0;
  
  M44(mat, 3, 0) = 0;
  M44(mat, 3, 1) = 0;
  M44(mat, 3, 2) = 0;
  M44(mat, 3, 3) = 1;

  minus_eye[0] = -eye[0];
  minus_eye[1] = -eye[1];
  minus_eye[2] = -eye[2];
  
  mat44_translate_mat44 (dst, mat, minus_eye);
}

void
mat44_rotate (mat44 dst, vec3 axis, FLOATTYPE angle)
{
  mat33 tmp;
  
  mat33_rotate (tmp, axis, angle);
  mat44_from_mat33 (dst, tmp);
}

void
mat44_extract_rotation (mat44 dst, const mat44 src)
{
  M44(dst, 0, 0) = M44(src, 0, 0);
  M44(dst, 0, 1) = M44(src, 0, 1);
  M44(dst, 0, 2) = M44(src, 0, 2);
  M44(dst, 0, 3) = 0.0;
  
  M44(dst, 1, 0) = M44(src, 1, 0);
  M44(dst, 1, 1) = M44(src, 1, 1);
  M44(dst, 1, 2) = M44(src, 1, 2);
  M44(dst, 1, 3) = 0.0;
  
  M44(dst, 2, 0) = M44(src, 2, 0);
  M44(dst, 2, 1) = M44(src, 2, 1);
  M44(dst, 2, 2) = M44(src, 2, 2);
  M44(dst, 2, 3) = 0.0;
  
  M44(dst, 3, 0) = M44(dst, 3, 1) = M44(dst, 3, 2) = 0.0;
  M44(dst, 3, 3) = 1.0;
}

/* Given two unit-length vectors from & to, find the axis-angle rotation
   to map 'from' onto 'to' (output angle in radians).  Return 1 if the
   operation succeeded, 0 otherwise. */

int
rotate_vec3_to_vec3 (vec3 axis, FLOATTYPE *angle, const vec3 from,
		     const vec3 to)
{
  vec3 perp;
  int side;
  FLOATTYPE temp, rotate;
  
  vec3_cross_vec3 (perp, to, from);
  side = (vec3_dot_vec3 (to, from) < 0.0) ? 1 : 0;
  
  temp = vec3_length (perp);

  if (temp < -1.0 || temp > 1.0)
    {
      fprintf (stderr, "Bad vectors! Normalise first.\n");
      return 0;
    }

  rotate = side ? asin (temp) : M_PI - asin (temp);

  if (isnan (rotate))
    {
      fprintf (stderr, "rotate is nan\n");
      fprintf (stderr, "temp=%f\n", temp);
      return 0;
    }

  vec3_normalize (axis, perp);
  *angle = rotate;
  
  return 1;
}

/* distance between two points */

FLOATTYPE
vec3_distance_vec3 (const vec3 a, const vec3 b)
{
  FLOATTYPE ax = a[0], ay = a[1], az = a[2];
  FLOATTYPE bx = b[0], by = b[1], bz = b[2];

  ax -= bx;
  ay -= by;
  az -= bz;

  return sqrt (ax * ax + ay * ay + az * az);
}

void
vec3_scale (vec3 out, const vec3 vec, FLOATTYPE scale)
{
  int i;
  
  for (i = 0; i < 3; i++)
    out[i] = vec[i] * scale;
}

/* Distance from point to line (defined by two endpoints).  */

FLOATTYPE
vec3_distance_to_line (const vec3 pt, const vec3 a, const vec3 b)
{
  vec3 line;
  vec3 a_to_pt;
  vec3 intercept;
  FLOATTYPE dotprod;
  
  vec3_sub_vec3 (line, b, a);
  vec3_normalize (line, line);
  vec3_sub_vec3 (a_to_pt, pt, a);
  dotprod = vec3_dot_vec3 (line, a_to_pt);
  
  vec3_scale (intercept, line, dotprod);
  
  return vec3_distance_vec3 (a_to_pt, intercept);
}

void
plane_from_triangle (plane *theplane, const vec3 a, const vec3 b, const vec3 c)
{
  vec3 ab, ac;
  vec3 norm, scaled_norm;
  
  vec3_sub_vec3 (ab, b, a);
  vec3_sub_vec3 (ac, c, a);
  vec3_cross_vec3 (norm, ab, ac);
  vec3_normalize (norm, norm);
  
  memcpy (theplane->normal, norm, sizeof (FLOATTYPE) * 3);
   
  theplane->offset = vec3_dot_vec3 (a, norm);
}

FLOATTYPE
vec3_distance_to_plane (const vec3 pt, const plane *theplane)
{
  FLOATTYPE dp = vec3_dot_vec3 (theplane->normal, pt);
  return dp - theplane->offset;
}

void
vec3_dump (FILE* to, const vec3 vec)
{
  fprintf (to, "(%.4f %.4f %.4f)\n", vec[0], vec[1], vec[2]);
}

void
mat33_dump (FILE* to, const mat33 mat)
{
  fprintf (to, "(%.4f %.4f %.4f)\n",
	   M33(mat, 0, 0), M33(mat, 0, 1), M33(mat, 0, 2));
  fprintf (to, "(%.4f %.4f %.4f)\n",
	   M33(mat, 1, 0), M33(mat, 1, 1), M33(mat, 1, 2));
  fprintf (to, "(%.4f %.4f %.4f)\n",
	   M33(mat, 2, 0), M33(mat, 2, 1), M33(mat, 2, 2));
}

void
mat44_dump (FILE* to, const mat44 mat)
{
  fprintf (to, "(%.4f %.4f %.4f %.4f)\n",
	   M44(mat, 0, 0), M44(mat, 0, 1), M44(mat, 0, 2), M44(mat, 0, 3));
  fprintf (to, "(%.4f %.4f %.4f %.4f)\n",
	   M44(mat, 1, 0), M44(mat, 1, 1), M44(mat, 1, 2), M44(mat, 1, 3));
  fprintf (to, "(%.4f %.4f %.4f %.4f)\n",
	   M44(mat, 2, 0), M44(mat, 2, 1), M44(mat, 2, 2), M44(mat, 2, 3));
  fprintf (to, "(%.4f %.4f %.4f %.4f)\n",
	   M44(mat, 3, 0), M44(mat, 3, 1), M44(mat, 3, 2), M44(mat, 3, 3));
}
