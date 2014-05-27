	.file	"transform.c"
	.text
	.p2align 4,,15
	.globl	mat33_identity
	.type	mat33_identity, @function
mat33_identity:
.LFB49:
	.cfi_startproc
	xorps	%xmm0, %xmm0
	movss	.LC0(%rip), %xmm1
	movss	%xmm1, (%rdi)
	movss	%xmm0, 4(%rdi)
	movss	%xmm0, 8(%rdi)
	movss	%xmm0, 12(%rdi)
	movss	%xmm1, 16(%rdi)
	movss	%xmm0, 20(%rdi)
	movss	%xmm0, 24(%rdi)
	movss	%xmm0, 28(%rdi)
	movss	%xmm1, 32(%rdi)
	ret
	.cfi_endproc
.LFE49:
	.size	mat33_identity, .-mat33_identity
	.p2align 4,,15
	.globl	vec3_zero
	.type	vec3_zero, @function
vec3_zero:
.LFB50:
	.cfi_startproc
	xorps	%xmm0, %xmm0
	movss	%xmm0, 8(%rdi)
	movss	%xmm0, 4(%rdi)
	movss	%xmm0, (%rdi)
	ret
	.cfi_endproc
.LFE50:
	.size	vec3_zero, .-vec3_zero
	.p2align 4,,15
	.globl	transp_mat33_mul_vec3
	.type	transp_mat33_mul_vec3, @function
transp_mat33_mul_vec3:
.LFB51:
	.cfi_startproc
	movss	(%rdx), %xmm3
	movss	4(%rdx), %xmm0
	movss	12(%rsi), %xmm1
	movss	(%rsi), %xmm4
	mulss	%xmm0, %xmm1
	mulss	%xmm3, %xmm4
	movss	8(%rdx), %xmm2
	addss	%xmm4, %xmm1
	movss	24(%rsi), %xmm4
	mulss	%xmm2, %xmm4
	addss	%xmm4, %xmm1
	movss	%xmm1, (%rdi)
	movss	16(%rsi), %xmm1
	movss	4(%rsi), %xmm4
	mulss	%xmm0, %xmm1
	mulss	%xmm3, %xmm4
	addss	%xmm4, %xmm1
	movss	28(%rsi), %xmm4
	mulss	%xmm2, %xmm4
	addss	%xmm4, %xmm1
	movss	%xmm1, 4(%rdi)
	mulss	20(%rsi), %xmm0
	mulss	8(%rsi), %xmm3
	mulss	32(%rsi), %xmm2
	addss	%xmm3, %xmm0
	addss	%xmm2, %xmm0
	movss	%xmm0, 8(%rdi)
	ret
	.cfi_endproc
.LFE51:
	.size	transp_mat33_mul_vec3, .-transp_mat33_mul_vec3
	.p2align 4,,15
	.globl	mul44_mul_vec3
	.type	mul44_mul_vec3, @function
mul44_mul_vec3:
.LFB52:
	.cfi_startproc
	movss	(%rdx), %xmm0
	movss	4(%rdx), %xmm3
	movss	(%rsi), %xmm1
	movss	4(%rsi), %xmm4
	mulss	%xmm0, %xmm1
	mulss	%xmm3, %xmm4
	movss	8(%rdx), %xmm2
	addss	%xmm4, %xmm1
	movss	8(%rsi), %xmm4
	mulss	%xmm2, %xmm4
	addss	12(%rsi), %xmm1
	addss	%xmm4, %xmm1
	movss	%xmm1, (%rdi)
	movss	16(%rsi), %xmm1
	movss	20(%rsi), %xmm4
	mulss	%xmm0, %xmm1
	mulss	%xmm3, %xmm4
	addss	%xmm4, %xmm1
	movss	24(%rsi), %xmm4
	mulss	%xmm2, %xmm4
	addss	28(%rsi), %xmm1
	addss	%xmm4, %xmm1
	movss	%xmm1, 4(%rdi)
	mulss	32(%rsi), %xmm0
	mulss	36(%rsi), %xmm3
	mulss	40(%rsi), %xmm2
	addss	%xmm3, %xmm0
	addss	44(%rsi), %xmm0
	addss	%xmm2, %xmm0
	movss	%xmm0, 8(%rdi)
	ret
	.cfi_endproc
.LFE52:
	.size	mul44_mul_vec3, .-mul44_mul_vec3
	.p2align 4,,15
	.globl	transp_mat44_mul_vec3
	.type	transp_mat44_mul_vec3, @function
transp_mat44_mul_vec3:
.LFB53:
	.cfi_startproc
	movss	(%rdx), %xmm0
	movss	4(%rdx), %xmm3
	movss	(%rsi), %xmm1
	movss	16(%rsi), %xmm4
	mulss	%xmm0, %xmm1
	mulss	%xmm3, %xmm4
	movss	8(%rdx), %xmm2
	addss	%xmm4, %xmm1
	movss	32(%rsi), %xmm4
	mulss	%xmm2, %xmm4
	addss	48(%rsi), %xmm1
	addss	%xmm4, %xmm1
	movss	%xmm1, (%rdi)
	movss	4(%rsi), %xmm1
	movss	20(%rsi), %xmm4
	mulss	%xmm0, %xmm1
	mulss	%xmm3, %xmm4
	addss	%xmm4, %xmm1
	movss	36(%rsi), %xmm4
	mulss	%xmm2, %xmm4
	addss	52(%rsi), %xmm1
	addss	%xmm4, %xmm1
	movss	%xmm1, 4(%rdi)
	mulss	8(%rsi), %xmm0
	mulss	24(%rsi), %xmm3
	mulss	40(%rsi), %xmm2
	addss	%xmm3, %xmm0
	addss	56(%rsi), %xmm0
	addss	%xmm2, %xmm0
	movss	%xmm0, 8(%rdi)
	ret
	.cfi_endproc
.LFE53:
	.size	transp_mat44_mul_vec3, .-transp_mat44_mul_vec3
	.p2align 4,,15
	.globl	mat44_mul_vec4
	.type	mat44_mul_vec4, @function
mat44_mul_vec4:
.LFB54:
	.cfi_startproc
	movss	(%rdx), %xmm4
	movss	4(%rdx), %xmm0
	movss	16(%rsi), %xmm1
	movss	(%rsi), %xmm5
	mulss	%xmm0, %xmm1
	mulss	%xmm4, %xmm5
	movss	8(%rdx), %xmm3
	movss	12(%rdx), %xmm2
	addss	%xmm5, %xmm1
	movss	32(%rsi), %xmm5
	mulss	%xmm3, %xmm5
	addss	%xmm5, %xmm1
	movss	48(%rsi), %xmm5
	mulss	%xmm2, %xmm5
	addss	%xmm5, %xmm1
	movss	%xmm1, (%rdi)
	movss	20(%rsi), %xmm1
	movss	4(%rsi), %xmm5
	mulss	%xmm0, %xmm1
	mulss	%xmm4, %xmm5
	addss	%xmm5, %xmm1
	movss	36(%rsi), %xmm5
	mulss	%xmm3, %xmm5
	addss	%xmm5, %xmm1
	movss	52(%rsi), %xmm5
	mulss	%xmm2, %xmm5
	addss	%xmm5, %xmm1
	movss	%xmm1, 4(%rdi)
	movss	24(%rsi), %xmm1
	movss	8(%rsi), %xmm5
	mulss	%xmm0, %xmm1
	mulss	%xmm4, %xmm5
	addss	%xmm5, %xmm1
	movss	40(%rsi), %xmm5
	mulss	%xmm3, %xmm5
	addss	%xmm5, %xmm1
	movss	56(%rsi), %xmm5
	mulss	%xmm2, %xmm5
	addss	%xmm5, %xmm1
	movss	%xmm1, 8(%rdi)
	mulss	28(%rsi), %xmm0
	mulss	12(%rsi), %xmm4
	mulss	44(%rsi), %xmm3
	mulss	60(%rsi), %xmm2
	addss	%xmm4, %xmm0
	addss	%xmm3, %xmm0
	addss	%xmm2, %xmm0
	movss	%xmm0, 12(%rdi)
	ret
	.cfi_endproc
.LFE54:
	.size	mat44_mul_vec4, .-mat44_mul_vec4
	.p2align 4,,15
	.globl	mat33_mul_mat33
	.type	mat33_mul_mat33, @function
mat33_mul_mat33:
.LFB55:
	.cfi_startproc
	movss	(%rsi), %xmm13
	movss	4(%rsi), %xmm14
	movss	8(%rdx), %xmm15
	movaps	%xmm14, %xmm0
	movss	%xmm15, -4(%rsp)
	movaps	%xmm13, %xmm15
	movss	(%rdx), %xmm7
	movss	12(%rdx), %xmm8
	mulss	%xmm7, %xmm15
	mulss	%xmm8, %xmm0
	movss	8(%rsi), %xmm12
	movss	24(%rdx), %xmm6
	movss	24(%rsi), %xmm1
	movss	32(%rsi), %xmm3
	addss	%xmm15, %xmm0
	movaps	%xmm12, %xmm15
	movss	%xmm1, -12(%rsp)
	mulss	%xmm6, %xmm15
	movss	20(%rdx), %xmm1
	movss	12(%rsi), %xmm10
	movss	16(%rsi), %xmm11
	movss	28(%rsi), %xmm2
	addss	%xmm15, %xmm0
	movss	20(%rsi), %xmm9
	movss	4(%rdx), %xmm4
	movss	16(%rdx), %xmm5
	movaps	%xmm13, %xmm15
	movss	%xmm1, -16(%rsp)
	movss	%xmm3, -8(%rsp)
	mulss	%xmm4, %xmm15
	movss	28(%rdx), %xmm3
	movss	32(%rdx), %xmm1
	movss	%xmm0, (%rdi)
	movaps	%xmm14, %xmm0
	mulss	-16(%rsp), %xmm14
	mulss	%xmm5, %xmm0
	addss	%xmm15, %xmm0
	movaps	%xmm12, %xmm15
	mulss	%xmm1, %xmm12
	mulss	%xmm3, %xmm15
	addss	%xmm15, %xmm0
	movss	-4(%rsp), %xmm15
	mulss	%xmm15, %xmm13
	movss	%xmm0, 4(%rdi)
	addss	%xmm13, %xmm14
	movss	-16(%rsp), %xmm13
	addss	%xmm12, %xmm14
	movaps	%xmm10, %xmm12
	mulss	%xmm7, %xmm12
	movss	%xmm14, 8(%rdi)
	movaps	%xmm11, %xmm14
	mulss	%xmm8, %xmm14
	mulss	%xmm2, %xmm8
	movaps	%xmm14, %xmm0
	movaps	%xmm11, %xmm14
	mulss	%xmm13, %xmm11
	addss	%xmm12, %xmm0
	movaps	%xmm9, %xmm12
	mulss	%xmm5, %xmm14
	mulss	%xmm6, %xmm12
	mulss	%xmm2, %xmm5
	mulss	%xmm13, %xmm2
	addss	%xmm12, %xmm0
	movaps	%xmm10, %xmm12
	mulss	%xmm15, %xmm10
	mulss	%xmm4, %xmm12
	movss	%xmm0, 12(%rdi)
	movaps	%xmm14, %xmm0
	addss	%xmm10, %xmm11
	addss	%xmm12, %xmm0
	movaps	%xmm9, %xmm12
	mulss	%xmm1, %xmm9
	mulss	%xmm3, %xmm12
	addss	%xmm9, %xmm11
	addss	%xmm12, %xmm0
	movss	%xmm11, 20(%rdi)
	movss	%xmm0, 16(%rdi)
	movss	-12(%rsp), %xmm0
	mulss	%xmm0, %xmm7
	mulss	%xmm0, %xmm4
	mulss	%xmm15, %xmm0
	addss	%xmm7, %xmm8
	movss	-8(%rsp), %xmm7
	mulss	%xmm7, %xmm6
	addss	%xmm4, %xmm5
	mulss	%xmm7, %xmm3
	addss	%xmm0, %xmm2
	mulss	%xmm7, %xmm1
	addss	%xmm6, %xmm8
	addss	%xmm3, %xmm5
	addss	%xmm1, %xmm2
	movss	%xmm8, 24(%rdi)
	movss	%xmm5, 28(%rdi)
	movss	%xmm2, 32(%rdi)
	ret
	.cfi_endproc
.LFE55:
	.size	mat33_mul_mat33, .-mat33_mul_mat33
	.p2align 4,,15
	.globl	mat33_rotate
	.type	mat33_rotate, @function
mat33_rotate:
.LFB56:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	leaq	8(%rsp), %rsi
	leaq	12(%rsp), %rdi
	call	sincosf
	movss	0(%rbp), %xmm2
	movaps	%xmm2, %xmm4
	movss	8(%rsp), %xmm5
	movss	.LC0(%rip), %xmm0
	subss	%xmm5, %xmm0
	mulss	%xmm2, %xmm4
	movss	4(%rbp), %xmm3
	movss	8(%rbp), %xmm1
	movss	12(%rsp), %xmm7
	mulss	%xmm0, %xmm4
	movaps	%xmm7, %xmm9
	movaps	%xmm7, %xmm8
	mulss	%xmm1, %xmm9
	mulss	%xmm3, %xmm8
	addss	%xmm5, %xmm4
	movss	%xmm4, (%rbx)
	movaps	%xmm3, %xmm4
	mulss	%xmm2, %xmm4
	mulss	%xmm0, %xmm4
	movaps	%xmm4, %xmm6
	addss	%xmm9, %xmm4
	subss	%xmm9, %xmm6
	movss	%xmm4, 4(%rbx)
	movaps	%xmm3, %xmm4
	movss	%xmm6, 12(%rbx)
	movaps	%xmm1, %xmm6
	mulss	%xmm3, %xmm4
	mulss	%xmm2, %xmm6
	mulss	%xmm1, %xmm3
	mulss	%xmm1, %xmm1
	mulss	%xmm0, %xmm4
	mulss	%xmm0, %xmm6
	mulss	%xmm0, %xmm3
	mulss	%xmm7, %xmm2
	addss	%xmm5, %xmm4
	mulss	%xmm0, %xmm1
	movaps	%xmm6, %xmm10
	subss	%xmm8, %xmm6
	addss	%xmm8, %xmm10
	movss	%xmm4, 16(%rbx)
	movaps	%xmm3, %xmm4
	addss	%xmm5, %xmm1
	subss	%xmm2, %xmm4
	movss	%xmm6, 8(%rbx)
	addss	%xmm2, %xmm3
	movss	%xmm10, 24(%rbx)
	movss	%xmm1, 32(%rbx)
	movss	%xmm4, 28(%rbx)
	movss	%xmm3, 20(%rbx)
	addq	$24, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE56:
	.size	mat33_rotate, .-mat33_rotate
	.p2align 4,,15
	.globl	vec2_rotate
	.type	vec2_rotate, @function
vec2_rotate:
.LFB57:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	leaq	8(%rsp), %rsi
	leaq	12(%rsp), %rdi
	call	sincosf
	movss	8(%rsp), %xmm4
	movss	12(%rsp), %xmm3
	movaps	%xmm4, %xmm1
	movaps	%xmm3, %xmm5
	movss	0(%rbp), %xmm2
	movss	4(%rbp), %xmm0
	mulss	%xmm2, %xmm1
	mulss	%xmm0, %xmm5
	mulss	%xmm3, %xmm2
	mulss	%xmm4, %xmm0
	subss	%xmm5, %xmm1
	addss	%xmm2, %xmm0
	movss	%xmm1, (%rbx)
	movss	%xmm0, 4(%rbx)
	addq	$24, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE57:
	.size	vec2_rotate, .-vec2_rotate
	.p2align 4,,15
	.globl	mat33_transpose
	.type	mat33_transpose, @function
mat33_transpose:
.LFB58:
	.cfi_startproc
	movss	(%rsi), %xmm8
	movss	4(%rsi), %xmm5
	movss	8(%rsi), %xmm2
	movss	12(%rsi), %xmm7
	movss	16(%rsi), %xmm4
	movss	20(%rsi), %xmm1
	movss	24(%rsi), %xmm6
	movss	28(%rsi), %xmm3
	movss	32(%rsi), %xmm0
	movss	%xmm0, 32(%rdi)
	movss	%xmm8, (%rdi)
	movss	%xmm7, 4(%rdi)
	movss	%xmm6, 8(%rdi)
	movss	%xmm5, 12(%rdi)
	movss	%xmm4, 16(%rdi)
	movss	%xmm3, 20(%rdi)
	movss	%xmm2, 24(%rdi)
	movss	%xmm1, 28(%rdi)
	ret
	.cfi_endproc
.LFE58:
	.size	mat33_transpose, .-mat33_transpose
	.p2align 4,,15
	.globl	mat33_invert
	.type	mat33_invert, @function
mat33_invert:
.LFB59:
	.cfi_startproc
	movss	16(%rsi), %xmm8
	movss	28(%rsi), %xmm10
	movaps	%xmm8, %xmm11
	movaps	%xmm10, %xmm0
	movaps	%xmm10, %xmm14
	movss	(%rsi), %xmm5
	movss	12(%rsi), %xmm6
	mulss	%xmm5, %xmm11
	mulss	%xmm5, %xmm0
	movss	24(%rsi), %xmm7
	mulss	%xmm6, %xmm14
	movaps	%xmm7, %xmm12
	movss	4(%rsi), %xmm9
	movaps	%xmm11, %xmm4
	movss	%xmm11, -8(%rsp)
	mulss	%xmm9, %xmm12
	movaps	%xmm0, %xmm11
	movss	%xmm0, -4(%rsp)
	movaps	%xmm14, %xmm0
	movss	8(%rsi), %xmm2
	movaps	%xmm6, %xmm15
	movss	32(%rsi), %xmm3
	mulss	%xmm2, %xmm0
	mulss	%xmm3, %xmm4
	movss	20(%rsi), %xmm1
	mulss	%xmm9, %xmm15
	movaps	%xmm7, %xmm13
	mulss	%xmm8, %xmm13
	addss	%xmm4, %xmm0
	movaps	%xmm12, %xmm4
	mulss	%xmm1, %xmm4
	addss	%xmm4, %xmm0
	movaps	%xmm11, %xmm4
	mulss	%xmm1, %xmm4
	subss	%xmm4, %xmm0
	movaps	%xmm15, %xmm4
	mulss	%xmm3, %xmm4
	subss	%xmm4, %xmm0
	movaps	%xmm13, %xmm4
	mulss	%xmm2, %xmm4
	subss	%xmm4, %xmm0
	comiss	.LC1(%rip), %xmm0
	je	.L18
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm4
	movsd	.LC2(%rip), %xmm0
	divsd	%xmm4, %xmm0
	unpcklpd	%xmm0, %xmm0
	cvtpd2ps	%xmm0, %xmm0
.L14:
	movaps	%xmm3, %xmm4
	movaps	%xmm10, %xmm11
	mulss	%xmm2, %xmm10
	mulss	%xmm8, %xmm4
	subss	-4(%rsp), %xmm12
	mulss	%xmm1, %xmm11
	subss	%xmm13, %xmm14
	mulss	%xmm2, %xmm8
	subss	%xmm11, %xmm4
	movss	-8(%rsp), %xmm11
	subss	%xmm15, %xmm11
	mulss	%xmm0, %xmm14
	mulss	%xmm0, %xmm12
	mulss	%xmm0, %xmm4
	mulss	%xmm0, %xmm11
	movss	%xmm14, 24(%rdi)
	movss	%xmm12, 28(%rdi)
	movss	%xmm4, (%rdi)
	movaps	%xmm3, %xmm4
	movss	%xmm11, 32(%rdi)
	mulss	%xmm9, %xmm4
	mulss	%xmm1, %xmm9
	subss	%xmm4, %xmm10
	movaps	%xmm7, %xmm4
	subss	%xmm8, %xmm9
	movaps	%xmm3, %xmm8
	mulss	%xmm1, %xmm4
	mulss	%xmm2, %xmm7
	mulss	%xmm6, %xmm8
	mulss	%xmm5, %xmm3
	mulss	%xmm6, %xmm2
	mulss	%xmm5, %xmm1
	subss	%xmm8, %xmm4
	mulss	%xmm0, %xmm10
	subss	%xmm7, %xmm3
	mulss	%xmm0, %xmm9
	subss	%xmm1, %xmm2
	mulss	%xmm0, %xmm4
	movss	%xmm10, 4(%rdi)
	mulss	%xmm0, %xmm3
	movss	%xmm9, 8(%rdi)
	mulss	%xmm0, %xmm2
	movss	%xmm4, 12(%rdi)
	movss	%xmm3, 16(%rdi)
	movss	%xmm2, 20(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L18:
	movss	.LC0(%rip), %xmm0
	jmp	.L14
	.cfi_endproc
.LFE59:
	.size	mat33_invert, .-mat33_invert
	.p2align 4,,15
	.globl	mat44_invert
	.type	mat44_invert, @function
mat44_invert:
.LFB60:
	.cfi_startproc
	leaq	-72(%rsp), %rax
	leaq	16(%rax), %rdx
.L22:
	movss	(%rsi), %xmm0
	addq	$4, %rax
	movss	%xmm0, -4(%rax)
	addq	$16, %rsi
	movss	-12(%rsi), %xmm0
	movss	%xmm0, 12(%rax)
	movss	-8(%rsi), %xmm0
	movss	%xmm0, 28(%rax)
	movss	-4(%rsi), %xmm0
	movss	%xmm0, 44(%rax)
	cmpq	%rdx, %rax
	jne	.L22
	movss	-40(%rsp), %xmm6
	xorl	%eax, %eax
	mulss	-16(%rsp), %xmm6
	movss	-20(%rsp), %xmm11
	mulss	-28(%rsp), %xmm11
	movss	-24(%rsp), %xmm7
	movss	-12(%rsp), %xmm4
	mulss	-32(%rsp), %xmm7
	movss	-32(%rsp), %xmm14
	mulss	%xmm4, %xmm14
	movss	-36(%rsp), %xmm2
	movss	-24(%rsp), %xmm5
	mulss	-16(%rsp), %xmm2
	movaps	%xmm6, %xmm1
	mulss	-36(%rsp), %xmm5
	movss	-40(%rsp), %xmm6
	mulss	-20(%rsp), %xmm6
	movaps	%xmm11, %xmm15
	movaps	%xmm14, %xmm0
	movss	%xmm7, -100(%rsp)
	movss	-48(%rsp), %xmm7
	mulss	%xmm7, %xmm15
	movss	-44(%rsp), %xmm8
	movss	%xmm5, -88(%rsp)
	movss	-16(%rsp), %xmm13
	movss	%xmm6, -92(%rsp)
	mulss	-28(%rsp), %xmm13
	movss	-52(%rsp), %xmm6
	mulss	%xmm6, %xmm0
	movss	-36(%rsp), %xmm9
	mulss	%xmm4, %xmm9
	movss	-20(%rsp), %xmm3
	mulss	-32(%rsp), %xmm3
	movss	-40(%rsp), %xmm12
	mulss	%xmm4, %xmm12
	movss	-24(%rsp), %xmm10
	addss	%xmm15, %xmm0
	movaps	%xmm2, %xmm15
	mulss	-28(%rsp), %xmm10
	movaps	%xmm0, %xmm5
	movss	%xmm3, -96(%rsp)
	movaps	%xmm2, %xmm0
	mulss	%xmm8, %xmm0
	addss	%xmm5, %xmm0
	movss	-56(%rsp), %xmm5
	movaps	%xmm0, %xmm2
	movaps	%xmm13, %xmm0
	mulss	%xmm6, %xmm0
	subss	%xmm0, %xmm2
	movaps	%xmm9, %xmm0
	mulss	%xmm7, %xmm0
	subss	%xmm0, %xmm2
	movaps	%xmm3, %xmm0
	movaps	%xmm12, %xmm3
	mulss	%xmm8, %xmm0
	mulss	%xmm7, %xmm3
	subss	%xmm0, %xmm2
	movaps	%xmm13, %xmm0
	mulss	%xmm5, %xmm0
	movss	%xmm2, -76(%rsp)
	movss	%xmm2, (%rdi)
	movss	-100(%rsp), %xmm2
	addss	%xmm0, %xmm3
	mulss	%xmm8, %xmm2
	movaps	%xmm14, %xmm0
	mulss	%xmm5, %xmm0
	addss	%xmm2, %xmm3
	movaps	%xmm1, %xmm2
	mulss	%xmm8, %xmm2
	subss	%xmm0, %xmm3
	movaps	%xmm10, %xmm0
	mulss	%xmm7, %xmm0
	subss	%xmm2, %xmm3
	movaps	%xmm10, %xmm2
	mulss	%xmm6, %xmm2
	subss	%xmm0, %xmm3
	movaps	%xmm9, %xmm0
	mulss	%xmm5, %xmm0
	movss	%xmm3, 4(%rdi)
	movss	%xmm1, -80(%rsp)
	mulss	%xmm6, %xmm1
	addss	%xmm0, %xmm2
	movss	-92(%rsp), %xmm0
	mulss	%xmm8, %xmm0
	movss	%xmm15, -84(%rsp)
	addss	%xmm0, %xmm2
	movss	-88(%rsp), %xmm0
	mulss	%xmm8, %xmm0
	subss	%xmm0, %xmm2
	movaps	%xmm11, %xmm0
	mulss	%xmm5, %xmm0
	subss	%xmm0, %xmm2
	movaps	%xmm12, %xmm0
	mulss	%xmm6, %xmm0
	subss	%xmm0, %xmm2
	movss	-96(%rsp), %xmm0
	mulss	%xmm5, %xmm0
	movss	%xmm2, 8(%rdi)
	addss	%xmm0, %xmm1
	movss	-88(%rsp), %xmm0
	mulss	%xmm7, %xmm0
	addss	%xmm0, %xmm1
	movaps	%xmm15, %xmm0
	movss	-92(%rsp), %xmm15
	mulss	%xmm5, %xmm0
	mulss	%xmm7, %xmm15
	subss	%xmm0, %xmm1
	movss	-100(%rsp), %xmm0
	mulss	%xmm6, %xmm0
	subss	%xmm15, %xmm1
	movss	-64(%rsp), %xmm15
	mulss	%xmm9, %xmm15
	subss	%xmm0, %xmm1
	movaps	%xmm15, %xmm0
	movss	-68(%rsp), %xmm15
	mulss	%xmm13, %xmm15
	movss	%xmm1, 12(%rdi)
	addss	%xmm15, %xmm0
	movss	-96(%rsp), %xmm15
	mulss	-60(%rsp), %xmm15
	addss	%xmm15, %xmm0
	movss	-84(%rsp), %xmm15
	mulss	-60(%rsp), %xmm15
	subss	%xmm15, %xmm0
	movss	-64(%rsp), %xmm15
	mulss	%xmm11, %xmm15
	subss	%xmm15, %xmm0
	movss	-68(%rsp), %xmm15
	mulss	%xmm14, %xmm15
	subss	%xmm15, %xmm0
	movss	-72(%rsp), %xmm15
	mulss	%xmm15, %xmm14
	mulss	%xmm15, %xmm13
	mulss	%xmm15, %xmm11
	movss	%xmm0, 16(%rdi)
	mulss	%xmm15, %xmm9
	movss	-64(%rsp), %xmm0
	mulss	%xmm10, %xmm0
	mulss	-68(%rsp), %xmm10
	addss	%xmm14, %xmm0
	movss	-80(%rsp), %xmm14
	mulss	-60(%rsp), %xmm14
	addss	%xmm14, %xmm0
	movss	-100(%rsp), %xmm14
	mulss	-60(%rsp), %xmm14
	subss	%xmm14, %xmm0
	movss	-64(%rsp), %xmm14
	mulss	%xmm12, %xmm14
	mulss	-68(%rsp), %xmm12
	subss	%xmm14, %xmm0
	movss	-88(%rsp), %xmm14
	addss	%xmm11, %xmm12
	movss	-92(%rsp), %xmm11
	subss	%xmm13, %xmm0
	movss	-60(%rsp), %xmm13
	mulss	%xmm14, %xmm13
	movss	%xmm0, 20(%rdi)
	addss	%xmm13, %xmm12
	subss	%xmm10, %xmm12
	movss	-60(%rsp), %xmm10
	mulss	%xmm11, %xmm10
	subss	%xmm9, %xmm12
	subss	%xmm10, %xmm12
	movss	%xmm12, 24(%rdi)
	movss	-100(%rsp), %xmm0
	mulss	-68(%rsp), %xmm0
	movss	-84(%rsp), %xmm9
	mulss	%xmm15, %xmm9
	movss	-64(%rsp), %xmm10
	mulss	%xmm11, %xmm10
	movss	-64(%rsp), %xmm13
	mulss	%xmm14, %xmm13
	movss	-80(%rsp), %xmm14
	mulss	-68(%rsp), %xmm14
	movss	-60(%rsp), %xmm12
	addss	%xmm9, %xmm0
	movss	-96(%rsp), %xmm9
	mulss	%xmm15, %xmm9
	movss	-68(%rsp), %xmm11
	mulss	%xmm6, %xmm12
	mulss	%xmm8, %xmm11
	addss	%xmm10, %xmm0
	movss	-60(%rsp), %xmm10
	mulss	%xmm5, %xmm10
	subss	%xmm13, %xmm0
	movss	-60(%rsp), %xmm13
	mulss	%xmm7, %xmm13
	subss	%xmm14, %xmm0
	movss	-64(%rsp), %xmm14
	mulss	%xmm8, %xmm14
	mulss	%xmm15, %xmm8
	subss	%xmm9, %xmm0
	movss	-68(%rsp), %xmm9
	mulss	%xmm7, %xmm9
	mulss	%xmm15, %xmm7
	movss	%xmm0, 28(%rdi)
	movss	-64(%rsp), %xmm0
	movss	%xmm7, -96(%rsp)
	mulss	%xmm6, %xmm0
	movss	-64(%rsp), %xmm7
	mulss	%xmm15, %xmm6
	mulss	%xmm5, %xmm7
	mulss	-68(%rsp), %xmm5
	movss	%xmm0, -100(%rsp)
	movss	-16(%rsp), %xmm0
	mulss	%xmm12, %xmm0
	movss	%xmm5, -92(%rsp)
	movss	-20(%rsp), %xmm5
	mulss	%xmm14, %xmm5
	addss	%xmm5, %xmm0
	movaps	%xmm9, %xmm5
	mulss	%xmm4, %xmm5
	addss	%xmm5, %xmm0
	movss	-100(%rsp), %xmm5
	mulss	%xmm4, %xmm5
	subss	%xmm5, %xmm0
	movss	-16(%rsp), %xmm5
	mulss	%xmm11, %xmm5
	subss	%xmm5, %xmm0
	movss	-20(%rsp), %xmm5
	mulss	%xmm13, %xmm5
	subss	%xmm5, %xmm0
	movss	-24(%rsp), %xmm5
	mulss	%xmm13, %xmm5
	movss	%xmm0, 32(%rdi)
	movss	-16(%rsp), %xmm0
	mulss	%xmm8, %xmm0
	addss	%xmm5, %xmm0
	movaps	%xmm7, %xmm5
	mulss	%xmm4, %xmm5
	addss	%xmm5, %xmm0
	movss	-24(%rsp), %xmm5
	mulss	%xmm14, %xmm5
	subss	%xmm5, %xmm0
	movss	-96(%rsp), %xmm5
	mulss	%xmm4, %xmm5
	subss	%xmm5, %xmm0
	movss	-16(%rsp), %xmm5
	mulss	%xmm10, %xmm5
	subss	%xmm5, %xmm0
	movss	-24(%rsp), %xmm5
	mulss	%xmm11, %xmm5
	movss	%xmm0, 36(%rdi)
	movss	-20(%rsp), %xmm0
	mulss	%xmm10, %xmm0
	addss	%xmm5, %xmm0
	movaps	%xmm6, %xmm5
	mulss	%xmm4, %xmm5
	mulss	-92(%rsp), %xmm4
	addss	%xmm5, %xmm0
	subss	%xmm4, %xmm0
	movss	-20(%rsp), %xmm4
	mulss	%xmm8, %xmm4
	subss	%xmm4, %xmm0
	movss	-24(%rsp), %xmm4
	mulss	%xmm12, %xmm4
	subss	%xmm4, %xmm0
	movss	%xmm0, 40(%rdi)
	movss	-96(%rsp), %xmm0
	mulss	-20(%rsp), %xmm0
	movss	-100(%rsp), %xmm5
	movss	-24(%rsp), %xmm4
	mulss	%xmm5, %xmm4
	addss	%xmm4, %xmm0
	movss	-92(%rsp), %xmm4
	mulss	-16(%rsp), %xmm4
	addss	%xmm4, %xmm0
	movss	-16(%rsp), %xmm4
	mulss	%xmm6, %xmm4
	subss	%xmm4, %xmm0
	movss	-20(%rsp), %xmm4
	mulss	%xmm7, %xmm4
	subss	%xmm4, %xmm0
	movss	-24(%rsp), %xmm4
	mulss	%xmm9, %xmm4
	subss	%xmm4, %xmm0
	movss	-32(%rsp), %xmm4
	mulss	%xmm11, %xmm4
	mulss	-40(%rsp), %xmm11
	movss	%xmm0, 44(%rdi)
	movss	-28(%rsp), %xmm0
	mulss	%xmm5, %xmm0
	movss	-96(%rsp), %xmm5
	addss	%xmm4, %xmm0
	movss	-36(%rsp), %xmm4
	mulss	%xmm13, %xmm4
	mulss	-40(%rsp), %xmm13
	addss	%xmm4, %xmm0
	movss	-32(%rsp), %xmm4
	mulss	%xmm12, %xmm4
	mulss	-40(%rsp), %xmm12
	subss	%xmm4, %xmm0
	movss	-36(%rsp), %xmm4
	mulss	%xmm14, %xmm4
	mulss	-40(%rsp), %xmm14
	subss	%xmm4, %xmm0
	movss	-28(%rsp), %xmm4
	mulss	%xmm9, %xmm4
	mulss	-40(%rsp), %xmm9
	subss	%xmm4, %xmm0
	movss	-28(%rsp), %xmm4
	mulss	%xmm5, %xmm4
	movss	%xmm0, 48(%rdi)
	addss	%xmm4, %xmm14
	movss	-32(%rsp), %xmm4
	mulss	%xmm10, %xmm4
	mulss	-36(%rsp), %xmm10
	addss	%xmm4, %xmm14
	movss	-28(%rsp), %xmm4
	mulss	%xmm7, %xmm4
	mulss	-36(%rsp), %xmm7
	subss	%xmm13, %xmm14
	movss	-28(%rsp), %xmm13
	subss	%xmm4, %xmm14
	movss	-32(%rsp), %xmm4
	mulss	%xmm8, %xmm4
	mulss	-36(%rsp), %xmm8
	subss	%xmm4, %xmm14
	movss	-28(%rsp), %xmm4
	mulss	%xmm6, %xmm4
	mulss	-32(%rsp), %xmm6
	movss	%xmm14, 52(%rdi)
	movss	-92(%rsp), %xmm14
	mulss	%xmm14, %xmm13
	addss	%xmm6, %xmm9
	movaps	%xmm13, %xmm0
	movss	-32(%rsp), %xmm13
	addss	%xmm8, %xmm0
	mulss	%xmm14, %xmm13
	addss	%xmm7, %xmm9
	movss	-36(%rsp), %xmm7
	mulss	%xmm5, %xmm7
	addss	%xmm12, %xmm0
	subss	%xmm7, %xmm9
	subss	%xmm11, %xmm0
	subss	%xmm13, %xmm9
	subss	%xmm4, %xmm0
	subss	%xmm10, %xmm0
	movss	%xmm0, 56(%rdi)
	movss	-100(%rsp), %xmm0
	mulss	-40(%rsp), %xmm0
	subss	%xmm0, %xmm9
	movss	%xmm9, 60(%rdi)
	mulss	-68(%rsp), %xmm3
	mulss	-76(%rsp), %xmm15
	mulss	-64(%rsp), %xmm2
	mulss	-60(%rsp), %xmm1
	addss	%xmm15, %xmm3
	addss	%xmm2, %xmm3
	addss	%xmm1, %xmm3
	movss	.LC0(%rip), %xmm1
	divss	%xmm3, %xmm1
	.p2align 4,,10
	.p2align 3
.L24:
	movss	(%rdi,%rax), %xmm0
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%rdi,%rax)
	addq	$4, %rax
	cmpq	$64, %rax
	jne	.L24
	rep ret
	.cfi_endproc
.LFE60:
	.size	mat44_invert, .-mat44_invert
	.p2align 4,,15
	.globl	vec3_cross_vec3
	.type	vec3_cross_vec3, @function
vec3_cross_vec3:
.LFB61:
	.cfi_startproc
	movss	4(%rsi), %xmm4
	movss	8(%rsi), %xmm1
	movaps	%xmm4, %xmm2
	movaps	%xmm1, %xmm7
	movss	(%rsi), %xmm5
	movss	(%rdx), %xmm3
	movss	4(%rdx), %xmm0
	mulss	%xmm3, %xmm1
	movss	8(%rdx), %xmm6
	mulss	%xmm0, %xmm7
	mulss	%xmm6, %xmm2
	mulss	%xmm5, %xmm0
	mulss	%xmm5, %xmm6
	mulss	%xmm4, %xmm3
	subss	%xmm7, %xmm2
	subss	%xmm6, %xmm1
	subss	%xmm3, %xmm0
	movss	%xmm2, (%rdi)
	movss	%xmm1, 4(%rdi)
	movss	%xmm0, 8(%rdi)
	ret
	.cfi_endproc
.LFE61:
	.size	vec3_cross_vec3, .-vec3_cross_vec3
	.p2align 4,,15
	.globl	vec3_dot_vec3
	.type	vec3_dot_vec3, @function
vec3_dot_vec3:
.LFB62:
	.cfi_startproc
	movss	4(%rdi), %xmm0
	movss	(%rdi), %xmm1
	mulss	4(%rsi), %xmm0
	mulss	(%rsi), %xmm1
	addss	%xmm1, %xmm0
	movss	8(%rdi), %xmm1
	mulss	8(%rsi), %xmm1
	addss	%xmm1, %xmm0
	ret
	.cfi_endproc
.LFE62:
	.size	vec3_dot_vec3, .-vec3_dot_vec3
	.p2align 4,,15
	.globl	vec3_add_vec3
	.type	vec3_add_vec3, @function
vec3_add_vec3:
.LFB63:
	.cfi_startproc
	movss	(%rsi), %xmm2
	addss	(%rdx), %xmm2
	movss	4(%rsi), %xmm1
	movss	8(%rsi), %xmm0
	movss	4(%rdx), %xmm4
	movss	8(%rdx), %xmm3
	addss	%xmm4, %xmm1
	addss	%xmm3, %xmm0
	movss	%xmm2, (%rdi)
	movss	%xmm1, 4(%rdi)
	movss	%xmm0, 8(%rdi)
	ret
	.cfi_endproc
.LFE63:
	.size	vec3_add_vec3, .-vec3_add_vec3
	.p2align 4,,15
	.globl	vec3_sub_vec3
	.type	vec3_sub_vec3, @function
vec3_sub_vec3:
.LFB64:
	.cfi_startproc
	movss	(%rsi), %xmm2
	subss	(%rdx), %xmm2
	movss	4(%rsi), %xmm1
	movss	8(%rsi), %xmm0
	movss	4(%rdx), %xmm4
	movss	8(%rdx), %xmm3
	subss	%xmm4, %xmm1
	subss	%xmm3, %xmm0
	movss	%xmm2, (%rdi)
	movss	%xmm1, 4(%rdi)
	movss	%xmm0, 8(%rdi)
	ret
	.cfi_endproc
.LFE64:
	.size	vec3_sub_vec3, .-vec3_sub_vec3
	.p2align 4,,15
	.globl	vec3_normalize
	.type	vec3_normalize, @function
vec3_normalize:
.LFB65:
	.cfi_startproc
	movss	(%rsi), %xmm3
	movss	4(%rsi), %xmm2
	movaps	%xmm3, %xmm0
	movaps	%xmm2, %xmm4
	mulss	%xmm3, %xmm0
	movss	8(%rsi), %xmm1
	mulss	%xmm2, %xmm4
	addss	%xmm4, %xmm0
	movaps	%xmm1, %xmm4
	mulss	%xmm1, %xmm4
	addss	%xmm4, %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	sqrtsd	%xmm0, %xmm4
	movsd	.LC2(%rip), %xmm0
	divsd	%xmm4, %xmm0
	unpcklpd	%xmm0, %xmm0
	cvtpd2ps	%xmm0, %xmm0
	mulss	%xmm0, %xmm3
	mulss	%xmm0, %xmm2
	mulss	%xmm0, %xmm1
	movss	%xmm3, (%rdi)
	movss	%xmm2, 4(%rdi)
	movss	%xmm1, 8(%rdi)
	ret
	.cfi_endproc
.LFE65:
	.size	vec3_normalize, .-vec3_normalize
	.p2align 4,,15
	.globl	vec3_length
	.type	vec3_length, @function
vec3_length:
.LFB66:
	.cfi_startproc
	movss	(%rdi), %xmm2
	movss	4(%rdi), %xmm0
	mulss	%xmm2, %xmm2
	mulss	%xmm0, %xmm0
	movss	8(%rdi), %xmm1
	mulss	%xmm1, %xmm1
	addss	%xmm2, %xmm0
	addss	%xmm1, %xmm0
	sqrtss	%xmm0, %xmm0
	ret
	.cfi_endproc
.LFE66:
	.size	vec3_length, .-vec3_length
	.p2align 4,,15
	.globl	mat33_fixortho
	.type	mat33_fixortho, @function
mat33_fixortho:
.LFB67:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	subq	$48, %rsp
	.cfi_def_cfa_offset 64
	movss	16(%rdi), %xmm0
	movq	%rsp, %rsi
	movss	28(%rdi), %xmm5
	movaps	%xmm0, %xmm7
	movaps	%xmm5, %xmm1
	movss	%xmm0, 20(%rsp)
	movss	12(%rdi), %xmm6
	movss	24(%rdi), %xmm3
	mulss	%xmm6, %xmm1
	mulss	%xmm3, %xmm7
	movss	4(%rdi), %xmm2
	movss	(%rdi), %xmm4
	mulss	%xmm2, %xmm6
	mulss	%xmm2, %xmm3
	movss	%xmm2, 16(%rsp)
	movq	%rsp, %rdi
	movss	%xmm5, 24(%rsp)
	subss	%xmm7, %xmm1
	movaps	%xmm5, %xmm7
	mulss	%xmm4, %xmm7
	mulss	%xmm0, %xmm4
	movss	%xmm1, 32(%rsp)
	subss	%xmm7, %xmm3
	subss	%xmm6, %xmm4
	movaps	%xmm3, %xmm7
	movss	%xmm3, 36(%rsp)
	movaps	%xmm4, %xmm6
	movss	%xmm4, 40(%rsp)
	mulss	%xmm5, %xmm7
	mulss	%xmm2, %xmm4
	mulss	%xmm1, %xmm5
	mulss	%xmm3, %xmm2
	mulss	%xmm0, %xmm6
	mulss	%xmm1, %xmm0
	subss	%xmm4, %xmm5
	subss	%xmm7, %xmm6
	subss	%xmm0, %xmm2
	movss	%xmm5, 4(%rsp)
	movss	%xmm6, (%rsp)
	movss	%xmm2, 8(%rsp)
	call	vec3_normalize
	leaq	16(%rsp), %rsi
	movq	%rsi, %rdi
	call	vec3_normalize
	leaq	32(%rsp), %rsi
	movq	%rsi, %rdi
	call	vec3_normalize
	movss	(%rsp), %xmm0
	movss	%xmm0, (%rbx)
	movss	4(%rsp), %xmm0
	movss	%xmm0, 12(%rbx)
	movss	8(%rsp), %xmm0
	movss	%xmm0, 24(%rbx)
	movss	16(%rsp), %xmm0
	movss	%xmm0, 4(%rbx)
	movss	20(%rsp), %xmm0
	movss	%xmm0, 16(%rbx)
	movss	24(%rsp), %xmm0
	movss	%xmm0, 28(%rbx)
	movss	32(%rsp), %xmm0
	movss	%xmm0, 8(%rbx)
	movss	36(%rsp), %xmm0
	movss	%xmm0, 20(%rbx)
	movss	40(%rsp), %xmm0
	movss	%xmm0, 32(%rbx)
	addq	$48, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE67:
	.size	mat33_fixortho, .-mat33_fixortho
	.p2align 4,,15
	.globl	mat44_from_mat33_translate
	.type	mat44_from_mat33_translate, @function
mat44_from_mat33_translate:
.LFB68:
	.cfi_startproc
	movss	(%rsi), %xmm0
	movss	%xmm0, (%rdi)
	movss	4(%rsi), %xmm0
	movss	%xmm0, 4(%rdi)
	movss	8(%rsi), %xmm0
	movss	%xmm0, 8(%rdi)
	xorps	%xmm0, %xmm0
	movss	%xmm0, 12(%rdi)
	movss	12(%rsi), %xmm1
	movss	%xmm1, 16(%rdi)
	movss	16(%rsi), %xmm1
	movss	%xmm1, 20(%rdi)
	movss	20(%rsi), %xmm1
	movss	%xmm1, 24(%rdi)
	movss	%xmm0, 28(%rdi)
	movss	24(%rsi), %xmm1
	movss	%xmm1, 32(%rdi)
	movss	28(%rsi), %xmm1
	movss	%xmm1, 36(%rdi)
	movss	32(%rsi), %xmm1
	movss	%xmm1, 40(%rdi)
	movss	%xmm0, 44(%rdi)
	movss	(%rdx), %xmm0
	movss	%xmm0, 48(%rdi)
	movss	4(%rdx), %xmm0
	movss	%xmm0, 52(%rdi)
	movss	8(%rdx), %xmm0
	movss	%xmm0, 56(%rdi)
	movl	$0x3f800000, 60(%rdi)
	ret
	.cfi_endproc
.LFE68:
	.size	mat44_from_mat33_translate, .-mat44_from_mat33_translate
	.p2align 4,,15
	.globl	mat44_identity
	.type	mat44_identity, @function
mat44_identity:
.LFB69:
	.cfi_startproc
	xorps	%xmm1, %xmm1
	xorl	%edx, %edx
.L35:
	xorl	%eax, %eax
.L39:
	cmpl	%eax, %edx
	je	.L41
	movaps	%xmm1, %xmm0
.L36:
	movss	%xmm0, (%rdi,%rax,4)
	addq	$1, %rax
	cmpq	$4, %rax
	jne	.L39
	addl	$1, %edx
	addq	$16, %rdi
	cmpl	$4, %edx
	jne	.L35
	rep ret
.L41:
	movss	.LC0(%rip), %xmm0
	jmp	.L36
	.cfi_endproc
.LFE69:
	.size	mat44_identity, .-mat44_identity
	.p2align 4,,15
	.globl	mat44_mul_mat44
	.type	mat44_mul_mat44, @function
mat44_mul_mat44:
.LFB70:
	.cfi_startproc
	xorps	%xmm2, %xmm2
	movl	$4, %r9d
.L43:
	leal	-4(%r9), %r8d
	movq	%rsi, %rcx
.L48:
	movaps	%xmm2, %xmm1
	xorl	%eax, %eax
.L46:
	movss	(%rdx,%rax), %xmm0
	mulss	(%rcx,%rax,4), %xmm0
	addq	$4, %rax
	cmpq	$16, %rax
	addss	%xmm0, %xmm1
	jne	.L46
	movslq	%r8d, %rax
	addl	$1, %r8d
	addq	$4, %rcx
	cmpl	%r9d, %r8d
	movss	%xmm1, -72(%rsp,%rax,4)
	jne	.L48
	leal	4(%r8), %r9d
	addq	$16, %rdx
	cmpl	$20, %r9d
	jne	.L43
	movq	-72(%rsp), %rax
	movq	%rax, (%rdi)
	movq	-64(%rsp), %rax
	movq	%rax, 8(%rdi)
	movq	-56(%rsp), %rax
	movq	%rax, 16(%rdi)
	movq	-48(%rsp), %rax
	movq	%rax, 24(%rdi)
	movq	-40(%rsp), %rax
	movq	%rax, 32(%rdi)
	movq	-32(%rsp), %rax
	movq	%rax, 40(%rdi)
	movq	-24(%rsp), %rax
	movq	%rax, 48(%rdi)
	movq	-16(%rsp), %rax
	movq	%rax, 56(%rdi)
	ret
	.cfi_endproc
.LFE70:
	.size	mat44_mul_mat44, .-mat44_mul_mat44
	.p2align 4,,15
	.globl	mat44_frustum
	.type	mat44_frustum, @function
mat44_frustum:
.LFB71:
	.cfi_startproc
	movaps	%xmm1, %xmm9
	movl	$0xbf800000, 56(%rdi)
	movaps	%xmm5, %xmm7
	subss	%xmm0, %xmm9
	movaps	%xmm4, %xmm10
	addss	%xmm1, %xmm0
	movaps	%xmm3, %xmm8
	subss	%xmm4, %xmm7
	addss	%xmm4, %xmm10
	subss	%xmm2, %xmm8
	divss	%xmm9, %xmm0
	addss	%xmm3, %xmm2
	movaps	%xmm10, %xmm6
	divss	%xmm9, %xmm6
	movss	%xmm0, 8(%rdi)
	movaps	%xmm5, %xmm0
	addss	%xmm4, %xmm0
	unpcklps	%xmm4, %xmm4
	unpcklps	%xmm5, %xmm5
	divss	%xmm7, %xmm0
	cvtps2pd	%xmm4, %xmm4
	cvtps2pd	%xmm5, %xmm5
	movss	%xmm6, (%rdi)
	mulsd	.LC4(%rip), %xmm4
	unpcklps	%xmm7, %xmm7
	divss	%xmm8, %xmm10
	movss	%xmm0, 40(%rdi)
	cvtps2pd	%xmm7, %xmm7
	mulsd	%xmm5, %xmm4
	divss	%xmm8, %xmm2
	movss	%xmm10, 20(%rdi)
	divsd	%xmm7, %xmm4
	movss	%xmm2, 24(%rdi)
	xorps	%xmm6, %xmm6
	unpcklpd	%xmm4, %xmm4
	movss	%xmm6, 16(%rdi)
	movss	%xmm6, 32(%rdi)
	movss	%xmm6, 48(%rdi)
	cvtpd2ps	%xmm4, %xmm1
	movss	%xmm6, 4(%rdi)
	movss	%xmm6, 36(%rdi)
	movss	%xmm6, 52(%rdi)
	movss	%xmm6, 12(%rdi)
	movss	%xmm6, 28(%rdi)
	movss	%xmm6, 60(%rdi)
	movss	%xmm1, 44(%rdi)
	ret
	.cfi_endproc
.LFE71:
	.size	mat44_frustum, .-mat44_frustum
	.p2align 4,,15
	.globl	mat44_perspective
	.type	mat44_perspective, @function
mat44_perspective:
.LFB72:
	.cfi_startproc
	unpcklps	%xmm0, %xmm0
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movaps	%xmm3, %xmm4
	movq	%rdi, %rbx
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	cvtps2pd	%xmm0, %xmm0
	subss	%xmm2, %xmm4
	movss	%xmm1, 28(%rsp)
	mulsd	.LC5(%rip), %xmm0
	movss	%xmm3, 24(%rsp)
	movss	%xmm2, 20(%rsp)
	movss	%xmm4, 16(%rsp)
	unpcklpd	%xmm0, %xmm0
	cvtpd2ps	%xmm0, %xmm5
	movaps	%xmm5, %xmm0
	movss	%xmm5, 12(%rsp)
	call	sinf
	movss	12(%rsp), %xmm5
	unpcklps	%xmm5, %xmm5
	movss	%xmm0, 8(%rsp)
	cvtps2pd	%xmm5, %xmm0
	call	cos
	movss	8(%rsp), %xmm5
	testb	$1, %bl
	movq	%rbx, %rdi
	movl	$64, %esi
	movss	16(%rsp), %xmm4
	cvtps2pd	%xmm5, %xmm5
	movss	20(%rsp), %xmm2
	divsd	%xmm5, %xmm0
	movss	24(%rsp), %xmm3
	movss	28(%rsp), %xmm1
	unpcklpd	%xmm0, %xmm0
	cvtpd2ps	%xmm0, %xmm0
	jne	.L76
	testb	$2, %dil
	jne	.L77
.L52:
	testb	$4, %dil
	jne	.L78
.L53:
	movl	%esi, %ecx
	xorl	%eax, %eax
	shrl	$3, %ecx
	testb	$4, %sil
	rep stosq
	je	.L54
	movl	$0, (%rdi)
	addq	$4, %rdi
.L54:
	testb	$2, %sil
	je	.L55
	xorl	%eax, %eax
	addq	$2, %rdi
	movw	%ax, -2(%rdi)
.L55:
	andl	$1, %esi
	je	.L56
	movb	$0, (%rdi)
.L56:
	movss	%xmm0, 20(%rbx)
	movaps	%xmm0, %xmm6
	movaps	%xmm3, %xmm0
	movl	$0xbf800000, 56(%rbx)
	divss	%xmm1, %xmm6
	movss	.LC6(%rip), %xmm1
	movl	$0x00000000, 60(%rbx)
	addss	%xmm2, %xmm0
	mulss	.LC7(%rip), %xmm2
	xorps	%xmm1, %xmm0
	movss	%xmm6, (%rbx)
	divss	%xmm4, %xmm0
	mulss	%xmm3, %xmm2
	movss	%xmm0, 40(%rbx)
	divss	%xmm4, %xmm2
	movss	%xmm2, 44(%rbx)
	addq	$32, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L76:
	.cfi_restore_state
	leaq	1(%rbx), %rdi
	movb	$0, (%rbx)
	movb	$63, %sil
	testb	$2, %dil
	je	.L52
	.p2align 4,,10
	.p2align 3
.L77:
	xorl	%edx, %edx
	addq	$2, %rdi
	subl	$2, %esi
	movw	%dx, -2(%rdi)
	testb	$4, %dil
	je	.L53
	.p2align 4,,10
	.p2align 3
.L78:
	movl	$0, (%rdi)
	subl	$4, %esi
	addq	$4, %rdi
	jmp	.L53
	.cfi_endproc
.LFE72:
	.size	mat44_perspective, .-mat44_perspective
	.p2align 4,,15
	.globl	mat44_ortho
	.type	mat44_ortho, @function
mat44_ortho:
.LFB73:
	.cfi_startproc
	movq	%rdi, %rdx
	movl	$64, %esi
	testb	$1, %dl
	jne	.L104
	testb	$2, %dil
	jne	.L105
.L81:
	testb	$4, %dil
	jne	.L106
.L82:
	movl	%esi, %ecx
	xorl	%eax, %eax
	shrl	$3, %ecx
	testb	$4, %sil
	rep stosq
	je	.L83
	movl	$0, (%rdi)
	addq	$4, %rdi
.L83:
	testb	$2, %sil
	je	.L84
	xorl	%eax, %eax
	addq	$2, %rdi
	movw	%ax, -2(%rdi)
.L84:
	andl	$1, %esi
	je	.L85
	movb	$0, (%rdi)
.L85:
	movaps	%xmm1, %xmm7
	movl	$0x3f800000, 60(%rdx)
	movss	.LC8(%rip), %xmm6
	subss	%xmm0, %xmm7
	movaps	%xmm6, %xmm8
	divss	%xmm7, %xmm8
	movaps	%xmm3, %xmm7
	subss	%xmm2, %xmm7
	divss	%xmm7, %xmm6
	movaps	%xmm5, %xmm7
	subss	%xmm4, %xmm7
	movss	%xmm8, (%rdx)
	movss	%xmm6, 20(%rdx)
	movss	.LC7(%rip), %xmm6
	divss	%xmm7, %xmm6
	movss	%xmm6, 40(%rdx)
	movaps	%xmm1, %xmm6
	addss	%xmm0, %xmm6
	subss	%xmm1, %xmm0
	divss	%xmm0, %xmm6
	movaps	%xmm3, %xmm0
	addss	%xmm2, %xmm0
	subss	%xmm3, %xmm2
	divss	%xmm2, %xmm0
	movss	%xmm6, 12(%rdx)
	movss	%xmm0, 28(%rdx)
	movaps	%xmm5, %xmm0
	addss	%xmm4, %xmm0
	subss	%xmm5, %xmm4
	divss	%xmm4, %xmm0
	movss	%xmm0, 44(%rdx)
	ret
	.p2align 4,,10
	.p2align 3
.L104:
	leaq	1(%rdx), %rdi
	movb	$0, (%rdx)
	movb	$63, %sil
	testb	$2, %dil
	je	.L81
	.p2align 4,,10
	.p2align 3
.L105:
	xorl	%ecx, %ecx
	addq	$2, %rdi
	subl	$2, %esi
	movw	%cx, -2(%rdi)
	testb	$4, %dil
	je	.L82
	.p2align 4,,10
	.p2align 3
.L106:
	movl	$0, (%rdi)
	subl	$4, %esi
	addq	$4, %rdi
	jmp	.L82
	.cfi_endproc
.LFE73:
	.size	mat44_ortho, .-mat44_ortho
	.p2align 4,,15
	.globl	mat44_translate_mat44
	.type	mat44_translate_mat44, @function
mat44_translate_mat44:
.LFB74:
	.cfi_startproc
	movq	(%rsi), %rax
	movss	(%rdx), %xmm0
	movss	4(%rdx), %xmm3
	movss	8(%rdx), %xmm2
	movq	%rax, (%rdi)
	movq	8(%rsi), %rax
	movq	%rax, 8(%rdi)
	movq	16(%rsi), %rax
	movq	%rax, 16(%rdi)
	movq	24(%rsi), %rax
	movq	%rax, 24(%rdi)
	movq	32(%rsi), %rax
	movq	%rax, 32(%rdi)
	movq	40(%rsi), %rax
	movq	%rax, 40(%rdi)
	movss	(%rsi), %xmm1
	movss	16(%rsi), %xmm4
	mulss	%xmm0, %xmm1
	mulss	%xmm3, %xmm4
	addss	%xmm4, %xmm1
	movss	32(%rsi), %xmm4
	mulss	%xmm2, %xmm4
	addss	48(%rsi), %xmm1
	addss	%xmm4, %xmm1
	movss	%xmm1, 48(%rdi)
	movss	4(%rsi), %xmm1
	movss	20(%rsi), %xmm4
	mulss	%xmm0, %xmm1
	mulss	%xmm3, %xmm4
	addss	%xmm4, %xmm1
	movss	36(%rsi), %xmm4
	mulss	%xmm2, %xmm4
	addss	52(%rsi), %xmm1
	addss	%xmm4, %xmm1
	movss	%xmm1, 52(%rdi)
	movss	8(%rsi), %xmm1
	movss	24(%rsi), %xmm4
	mulss	%xmm0, %xmm1
	mulss	%xmm3, %xmm4
	addss	%xmm4, %xmm1
	movss	40(%rsi), %xmm4
	mulss	%xmm2, %xmm4
	addss	56(%rsi), %xmm1
	addss	%xmm4, %xmm1
	movss	%xmm1, 56(%rdi)
	mulss	12(%rsi), %xmm0
	mulss	28(%rsi), %xmm3
	mulss	44(%rsi), %xmm2
	addss	%xmm3, %xmm0
	addss	60(%rsi), %xmm0
	addss	%xmm2, %xmm0
	movss	%xmm0, 60(%rdi)
	ret
	.cfi_endproc
.LFE74:
	.size	mat44_translate_mat44, .-mat44_translate_mat44
	.p2align 4,,15
	.globl	mat44_translate
	.type	mat44_translate, @function
mat44_translate:
.LFB75:
	.cfi_startproc
	xorps	%xmm0, %xmm0
	movss	.LC0(%rip), %xmm1
	movss	%xmm1, (%rdi)
	movss	%xmm0, 16(%rdi)
	movss	%xmm0, 32(%rdi)
	movss	(%rsi), %xmm2
	movss	%xmm2, 48(%rdi)
	movss	%xmm0, 4(%rdi)
	movss	%xmm1, 20(%rdi)
	movss	%xmm0, 36(%rdi)
	movss	4(%rsi), %xmm2
	movss	%xmm2, 52(%rdi)
	movss	%xmm0, 8(%rdi)
	movss	%xmm0, 24(%rdi)
	movss	%xmm1, 40(%rdi)
	movss	8(%rsi), %xmm2
	movss	%xmm2, 56(%rdi)
	movss	%xmm0, 12(%rdi)
	movss	%xmm0, 28(%rdi)
	movss	%xmm0, 44(%rdi)
	movss	%xmm1, 60(%rdi)
	ret
	.cfi_endproc
.LFE75:
	.size	mat44_translate, .-mat44_translate
	.p2align 4,,15
	.globl	mat44_lookat
	.type	mat44_lookat, @function
mat44_lookat:
.LFB76:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rcx, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rsi, %rbx
	subq	$112, %rsp
	.cfi_def_cfa_offset 144
	movss	(%rdx), %xmm2
	movq	%rsp, %rdi
	subss	(%rsi), %xmm2
	movss	4(%rdx), %xmm1
	movss	8(%rdx), %xmm0
	movss	4(%rsi), %xmm4
	movss	8(%rsi), %xmm3
	subss	%xmm4, %xmm1
	movq	%rsp, %rsi
	subss	%xmm3, %xmm0
	movss	%xmm2, (%rsp)
	movss	%xmm1, 4(%rsp)
	movss	%xmm0, 8(%rsp)
	call	vec3_normalize
	movss	4(%rbp), %xmm5
	leaq	16(%rsp), %rsi
	movss	8(%rbp), %xmm6
	movaps	%xmm5, %xmm7
	movaps	%xmm6, %xmm2
	movss	0(%rbp), %xmm4
	movq	%rsi, %rdi
	movss	(%rsp), %xmm0
	movss	4(%rsp), %xmm3
	mulss	%xmm0, %xmm6
	movss	8(%rsp), %xmm1
	mulss	%xmm5, %xmm0
	mulss	%xmm1, %xmm7
	mulss	%xmm3, %xmm2
	mulss	%xmm4, %xmm1
	mulss	%xmm4, %xmm3
	subss	%xmm7, %xmm2
	subss	%xmm6, %xmm1
	subss	%xmm3, %xmm0
	movss	%xmm2, 16(%rsp)
	movss	%xmm1, 20(%rsp)
	movss	%xmm0, 24(%rsp)
	call	vec3_normalize
	movss	4(%rsp), %xmm6
	leaq	32(%rsp), %rdx
	movss	8(%rsp), %xmm2
	leaq	48(%rsp), %rsi
	movaps	%xmm2, %xmm0
	movq	%r12, %rdi
	movaps	%xmm6, %xmm1
	movl	$0x3f800000, 108(%rsp)
	movss	20(%rsp), %xmm4
	movss	24(%rsp), %xmm7
	mulss	%xmm4, %xmm0
	mulss	%xmm7, %xmm1
	movss	(%rsp), %xmm5
	movaps	%xmm5, %xmm8
	movss	%xmm4, 64(%rsp)
	movaps	%xmm2, %xmm9
	mulss	%xmm5, %xmm4
	movss	16(%rsp), %xmm3
	mulss	%xmm7, %xmm8
	subss	%xmm1, %xmm0
	movaps	%xmm5, %xmm1
	mulss	%xmm3, %xmm9
	movss	%xmm3, 48(%rsp)
	mulss	%xmm6, %xmm3
	movss	%xmm7, 80(%rsp)
	movss	%xmm0, 52(%rsp)
	movss	.LC6(%rip), %xmm0
	subss	%xmm9, %xmm8
	xorps	%xmm0, %xmm1
	subss	%xmm4, %xmm3
	xorps	%xmm0, %xmm2
	movss	%xmm1, 56(%rsp)
	xorps	%xmm1, %xmm1
	movss	%xmm8, 68(%rsp)
	movaps	%xmm6, %xmm8
	movss	%xmm3, 84(%rsp)
	movss	%xmm1, 60(%rsp)
	xorps	%xmm0, %xmm8
	movss	%xmm1, 76(%rsp)
	movss	%xmm1, 92(%rsp)
	movss	%xmm1, 96(%rsp)
	movss	%xmm1, 100(%rsp)
	movss	%xmm1, 104(%rsp)
	movss	(%rbx), %xmm1
	xorps	%xmm0, %xmm1
	movss	%xmm8, 72(%rsp)
	movss	%xmm2, 88(%rsp)
	movss	%xmm1, 32(%rsp)
	movss	4(%rbx), %xmm1
	xorps	%xmm0, %xmm1
	movss	%xmm1, 36(%rsp)
	movss	8(%rbx), %xmm1
	xorps	%xmm0, %xmm1
	movss	%xmm1, 40(%rsp)
	call	mat44_translate_mat44
	addq	$112, %rsp
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE76:
	.size	mat44_lookat, .-mat44_lookat
	.p2align 4,,15
	.globl	mat44_rotate
	.type	mat44_rotate, @function
mat44_rotate:
.LFB77:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	subq	$48, %rsp
	.cfi_def_cfa_offset 64
	movq	%rsp, %rdi
	call	mat33_rotate
	movss	(%rsp), %xmm0
	movl	$0x3f800000, 60(%rbx)
	movss	12(%rsp), %xmm1
	movss	%xmm0, (%rbx)
	movss	%xmm1, 16(%rbx)
	movss	4(%rsp), %xmm0
	movss	16(%rsp), %xmm1
	movss	%xmm0, 4(%rbx)
	movss	%xmm1, 20(%rbx)
	movss	8(%rsp), %xmm0
	movss	20(%rsp), %xmm1
	movss	%xmm0, 8(%rbx)
	xorps	%xmm0, %xmm0
	movss	%xmm1, 24(%rbx)
	movss	24(%rsp), %xmm1
	movss	%xmm1, 32(%rbx)
	movss	28(%rsp), %xmm1
	movss	%xmm1, 36(%rbx)
	movss	32(%rsp), %xmm1
	movss	%xmm0, 12(%rbx)
	movss	%xmm0, 28(%rbx)
	movss	%xmm1, 40(%rbx)
	movss	%xmm0, 44(%rbx)
	movss	%xmm0, 48(%rbx)
	movss	%xmm0, 52(%rbx)
	movss	%xmm0, 56(%rbx)
	addq	$48, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE77:
	.size	mat44_rotate, .-mat44_rotate
	.p2align 4,,15
	.globl	mat44_extract_rotation
	.type	mat44_extract_rotation, @function
mat44_extract_rotation:
.LFB78:
	.cfi_startproc
	movss	(%rsi), %xmm0
	movss	%xmm0, (%rdi)
	movss	4(%rsi), %xmm0
	movss	%xmm0, 4(%rdi)
	movss	8(%rsi), %xmm0
	movss	%xmm0, 8(%rdi)
	xorps	%xmm0, %xmm0
	movss	%xmm0, 12(%rdi)
	movss	16(%rsi), %xmm1
	movss	%xmm1, 16(%rdi)
	movss	20(%rsi), %xmm1
	movss	%xmm1, 20(%rdi)
	movss	24(%rsi), %xmm1
	movss	%xmm1, 24(%rdi)
	movss	%xmm0, 28(%rdi)
	movss	32(%rsi), %xmm1
	movss	%xmm1, 32(%rdi)
	movss	36(%rsi), %xmm1
	movss	%xmm1, 36(%rdi)
	movss	40(%rsi), %xmm1
	movss	%xmm1, 40(%rdi)
	movl	$0x3f800000, 60(%rdi)
	movss	%xmm0, 44(%rdi)
	movss	%xmm0, 56(%rdi)
	movss	%xmm0, 52(%rdi)
	movss	%xmm0, 48(%rdi)
	ret
	.cfi_endproc
.LFE78:
	.size	mat44_extract_rotation, .-mat44_extract_rotation
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC9:
	.string	"Bad vectors! Normalise first.\n"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC11:
	.string	"rotate is nan\n"
.LC12:
	.string	"temp=%f\n"
	.text
	.p2align 4,,15
	.globl	rotate_vec3_to_vec3
	.type	rotate_vec3_to_vec3, @function
rotate_vec3_to_vec3:
.LFB79:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rsi, %rbx
	subq	$40, %rsp
	.cfi_def_cfa_offset 64
	movss	4(%rcx), %xmm5
	leaq	16(%rsp), %rdi
	movss	8(%rcx), %xmm2
	movaps	%xmm5, %xmm6
	movaps	%xmm2, %xmm7
	movss	4(%rdx), %xmm1
	movss	8(%rdx), %xmm0
	mulss	%xmm1, %xmm7
	mulss	%xmm0, %xmm6
	movss	(%rcx), %xmm4
	movss	(%rdx), %xmm3
	subss	%xmm7, %xmm6
	movaps	%xmm4, %xmm7
	mulss	%xmm0, %xmm7
	mulss	%xmm2, %xmm0
	movss	%xmm6, 16(%rsp)
	movaps	%xmm2, %xmm6
	mulss	%xmm3, %xmm6
	subss	%xmm7, %xmm6
	movaps	%xmm5, %xmm7
	mulss	%xmm3, %xmm7
	mulss	%xmm4, %xmm3
	movss	%xmm6, 20(%rsp)
	movaps	%xmm4, %xmm6
	mulss	%xmm1, %xmm6
	mulss	%xmm5, %xmm1
	subss	%xmm7, %xmm6
	addss	%xmm3, %xmm1
	movss	%xmm6, 24(%rsp)
	addss	%xmm0, %xmm1
	movss	%xmm1, 4(%rsp)
	call	vec3_length
	comiss	.LC0(%rip), %xmm0
	ja	.L122
	movss	.LC3(%rip), %xmm2
	comiss	%xmm0, %xmm2
	movss	4(%rsp), %xmm1
	ja	.L122
	xorps	%xmm2, %xmm2
	unpcklps	%xmm0, %xmm0
	comiss	%xmm1, %xmm2
	cvtps2pd	%xmm0, %xmm5
	movsd	%xmm5, 8(%rsp)
	movapd	%xmm5, %xmm0
	ja	.L126
	call	__asin_finite
	movsd	.LC10(%rip), %xmm1
	subsd	%xmm0, %xmm1
.L120:
	unpcklpd	%xmm1, %xmm1
	cvtpd2ps	%xmm1, %xmm4
	movaps	%xmm4, %xmm0
	movss	%xmm4, 4(%rsp)
	call	__isnanf
	testl	%eax, %eax
	jne	.L127
	leaq	16(%rsp), %rsi
	movq	%rbp, %rdi
	call	vec3_normalize
	movss	4(%rsp), %xmm3
	movl	$1, %eax
	movss	%xmm3, (%rbx)
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L127:
	.cfi_restore_state
	movq	stderr(%rip), %rcx
	movl	$14, %edx
	movl	$1, %esi
	movl	$.LC11, %edi
	call	fwrite
	movq	stderr(%rip), %rdi
	movsd	8(%rsp), %xmm0
	movl	$.LC12, %esi
	movl	$1, %eax
	call	fprintf
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L126:
	.cfi_restore_state
	call	__asin_finite
	movapd	%xmm0, %xmm1
	jmp	.L120
	.p2align 4,,10
	.p2align 3
.L122:
	movq	stderr(%rip), %rcx
	movl	$30, %edx
	movl	$1, %esi
	movl	$.LC9, %edi
	call	fwrite
	addq	$40, %rsp
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE79:
	.size	rotate_vec3_to_vec3, .-rotate_vec3_to_vec3
	.p2align 4,,15
	.globl	vec3_distance_vec3
	.type	vec3_distance_vec3, @function
vec3_distance_vec3:
.LFB80:
	.cfi_startproc
	movss	(%rdi), %xmm0
	movss	4(%rdi), %xmm2
	subss	(%rsi), %xmm0
	subss	4(%rsi), %xmm2
	movss	8(%rdi), %xmm1
	subss	8(%rsi), %xmm1
	mulss	%xmm0, %xmm0
	mulss	%xmm2, %xmm2
	mulss	%xmm1, %xmm1
	addss	%xmm2, %xmm0
	addss	%xmm1, %xmm0
	sqrtss	%xmm0, %xmm0
	ret
	.cfi_endproc
.LFE80:
	.size	vec3_distance_vec3, .-vec3_distance_vec3
	.section	.rodata.str1.1
.LC13:
	.string	"(%.4f %.4f %.4f)\n"
	.text
	.p2align 4,,15
	.globl	vec3_dump
	.type	vec3_dump, @function
vec3_dump:
.LFB81:
	.cfi_startproc
	movss	(%rsi), %xmm0
	movss	8(%rsi), %xmm2
	movl	$3, %eax
	movss	4(%rsi), %xmm1
	movl	$.LC13, %esi
	cvtps2pd	%xmm0, %xmm0
	cvtps2pd	%xmm2, %xmm2
	cvtps2pd	%xmm1, %xmm1
	jmp	fprintf
	.cfi_endproc
.LFE81:
	.size	vec3_dump, .-vec3_dump
	.p2align 4,,15
	.globl	mat33_dump
	.type	mat33_dump, @function
mat33_dump:
.LFB82:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$3, %eax
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rsi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movss	(%rsi), %xmm0
	movss	8(%rsi), %xmm2
	movss	4(%rsi), %xmm1
	movl	$.LC13, %esi
	cvtps2pd	%xmm0, %xmm0
	cvtps2pd	%xmm2, %xmm2
	cvtps2pd	%xmm1, %xmm1
	call	fprintf
	movss	12(%rbx), %xmm0
	movss	20(%rbx), %xmm2
	movq	%rbp, %rdi
	movss	16(%rbx), %xmm1
	movl	$.LC13, %esi
	movl	$3, %eax
	cvtps2pd	%xmm0, %xmm0
	cvtps2pd	%xmm2, %xmm2
	cvtps2pd	%xmm1, %xmm1
	call	fprintf
	movss	24(%rbx), %xmm0
	movss	32(%rbx), %xmm2
	movq	%rbp, %rdi
	movss	28(%rbx), %xmm1
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	movl	$.LC13, %esi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	cvtps2pd	%xmm0, %xmm0
	movl	$3, %eax
	cvtps2pd	%xmm2, %xmm2
	cvtps2pd	%xmm1, %xmm1
	jmp	fprintf
	.cfi_endproc
.LFE82:
	.size	mat33_dump, .-mat33_dump
	.section	.rodata.str1.1
.LC14:
	.string	"(%.4f %.4f %.4f %.4f)\n"
	.text
	.p2align 4,,15
	.globl	mat44_dump
	.type	mat44_dump, @function
mat44_dump:
.LFB83:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$4, %eax
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rsi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movss	(%rsi), %xmm0
	movss	12(%rsi), %xmm3
	movss	8(%rsi), %xmm2
	movss	4(%rsi), %xmm1
	movl	$.LC14, %esi
	cvtps2pd	%xmm0, %xmm0
	cvtps2pd	%xmm3, %xmm3
	cvtps2pd	%xmm2, %xmm2
	cvtps2pd	%xmm1, %xmm1
	call	fprintf
	movss	16(%rbx), %xmm0
	movss	28(%rbx), %xmm3
	movq	%rbp, %rdi
	movss	24(%rbx), %xmm2
	movss	20(%rbx), %xmm1
	movl	$.LC14, %esi
	cvtps2pd	%xmm0, %xmm0
	movl	$4, %eax
	cvtps2pd	%xmm3, %xmm3
	cvtps2pd	%xmm2, %xmm2
	cvtps2pd	%xmm1, %xmm1
	call	fprintf
	movss	32(%rbx), %xmm0
	movss	44(%rbx), %xmm3
	movq	%rbp, %rdi
	movss	40(%rbx), %xmm2
	movss	36(%rbx), %xmm1
	movl	$.LC14, %esi
	cvtps2pd	%xmm0, %xmm0
	movl	$4, %eax
	cvtps2pd	%xmm3, %xmm3
	cvtps2pd	%xmm2, %xmm2
	cvtps2pd	%xmm1, %xmm1
	call	fprintf
	movss	48(%rbx), %xmm0
	movss	60(%rbx), %xmm3
	movq	%rbp, %rdi
	movss	56(%rbx), %xmm2
	movss	52(%rbx), %xmm1
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC14, %esi
	movl	$4, %eax
	cvtps2pd	%xmm3, %xmm3
	cvtps2pd	%xmm2, %xmm2
	cvtps2pd	%xmm1, %xmm1
	jmp	fprintf
	.cfi_endproc
.LFE83:
	.size	mat44_dump, .-mat44_dump
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	1065353216
	.align 4
.LC1:
	.long	0
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC2:
	.long	0
	.long	1072693248
	.section	.rodata.cst4
	.align 4
.LC3:
	.long	3212836864
	.section	.rodata.cst8
	.align 8
.LC4:
	.long	0
	.long	-1073741824
	.align 8
.LC5:
	.long	2723323193
	.long	1065475910
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC6:
	.long	2147483648
	.long	0
	.long	0
	.long	0
	.section	.rodata.cst4
	.align 4
.LC7:
	.long	3221225472
	.align 4
.LC8:
	.long	1073741824
	.section	.rodata.cst8
	.align 8
.LC10:
	.long	1413754136
	.long	1074340347
	.ident	"GCC: (Debian 4.8.2-16) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
