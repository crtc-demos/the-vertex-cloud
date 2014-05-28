#ifndef TRANSFORM_H
#define TRANSFORM_H 1

#ifndef FLOATTYPE
#error FLOATTYPE not defined.
#endif

#include <stdio.h>

typedef FLOATTYPE vec2[2];
typedef int ivec2[2];
typedef FLOATTYPE vec3[3];
typedef FLOATTYPE vec4[4];

typedef FLOATTYPE mat33[9];
typedef FLOATTYPE mat44[16];

extern void mat33_identity (mat33 mat);
extern void vec3_zero (vec3 vec);
extern void mat33_mul_vec3 (vec3 dest, const mat33 mat, const vec3 src);
extern void transp_mat33_mul_vec3 (vec3 dest, const mat33 mat, const vec3 src);
extern void mat44_mul_vec3 (vec3 dest, const mat44 mat, const vec3 src);
extern void transp_mat44_mul_vec3 (vec3 dest, const mat44 mat, const vec3 src);
extern void mat44_mul_vec4 (vec4 dest, const mat44 mat, const vec4 src);
extern void mat33_mul_mat33 (mat33 dest, const mat33 a, const mat33 b);
extern void mat33_rotate (mat33 dest, const vec3 axis, FLOATTYPE angle);
extern void vec2_rotate (vec2 dest, const vec2 src, FLOATTYPE angle);
extern void mat33_transpose (mat33 dest, const mat33 src);
extern void mat33_invert (mat33 dest, const mat33 src);
extern void mat44_invert (mat44 dst, const mat44 mat);
extern void vec3_cross_vec3 (vec3 r, const vec3 u, const vec3 v);
extern FLOATTYPE vec3_dot_vec3 (const vec3 a, const vec3 b);
extern void vec3_add_vec3 (vec3 dest, const vec3 src1, const vec3 src2);
extern void vec3_sub_vec3 (vec3 dest, const vec3 src1, const vec3 src2);
extern void vec3_normalize (vec3 dest, const vec3 pt);
extern FLOATTYPE vec3_length (const vec3 vec);
extern void mat33_fixortho (mat33 m);
extern void mat44_from_mat33 (mat44 out, const mat33 mat);
extern void mat44_identity (mat44 out);
extern void mat44_mul_mat44 (mat44 out, const mat44 a, const mat44 b);
extern void mat44_frustum (mat44 mat, FLOATTYPE left, FLOATTYPE right,
			   FLOATTYPE bottom, FLOATTYPE top, FLOATTYPE near,
			   FLOATTYPE far);
extern void mat44_perspective (mat44 mat, FLOATTYPE fovy, FLOATTYPE aspect,
			       FLOATTYPE nearz, FLOATTYPE farz);
extern void mat44_ortho (mat44 mat, FLOATTYPE left, FLOATTYPE right,
			 FLOATTYPE bottom, FLOATTYPE top, FLOATTYPE near,
			 FLOATTYPE far);
extern void mat44_translate_mat44 (mat44 dst, const mat44 mat,
				   const vec3 xlate);
extern void mat44_translate (mat44 dst, const vec3 xlate);
extern void mat44_lookat (mat44 dst, const vec3 eye, const vec3 centre,
			  const vec3 up);
extern void mat44_rotate (mat44 dst, vec3 axis, FLOATTYPE angle);
extern void mat44_extract_rotation (mat44 dst, const mat44 src);
extern int rotate_vec3_to_vec3 (vec3 axis, FLOATTYPE *angle, const vec3 from,
				const vec3 to);
extern FLOATTYPE vec3_distance_vec3 (const vec3 a, const vec3 b);
extern void vec3_scale (vec3, const vec3, FLOATTYPE);
extern FLOATTYPE vec3_distance_to_line (const vec3, const vec3, const vec3);
extern void plane_from_triangle (vec3, FLOATTYPE *, const vec3, const vec3,
				 const vec3);
extern void vec3_dump (FILE* to, const vec3 vec);
extern void mat33_dump (FILE* to, const mat33 mat);
extern void mat44_dump (FILE* to, const mat44 mat);

#endif
