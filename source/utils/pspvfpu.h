#ifndef FPU_PSP_H
#define FPU_PSP_H

#include "../types.h"

float sceFpuRsqrt(float fs);

static float sceFpuSqrt(float x) {
	float result;
	__asm__ volatile (
		"mtv     %1, S000\n"
		"vsqrt.s S000, S000\n"
		"mfv     %0, S000\n"
		: "=r"(result) : "r"(x));
	return result;
}



static unsigned int sceFpuFloor(float x) {
	unsigned int result;
	__asm__ volatile (
		"mtv     %1, S000\n"
		"vf2id.s S000, S000, 0\n"
		"vi2f.s S000, S000, 0\n"
		"mfv     %0, S000\n"
		: "=r"(result) : "r"(x));
	return result;
}


static float sceFpuMul(float x, float y) {
	float result;
	__asm__ volatile (
		"mtv     %1, S001\n"
		"mtv     %2, S002\n"
		"vmul.s S000, S001, S002\n"
		"mfv     %0, S000\n"
		: "=r"(result) : "r"(x), "r"(y));
	return result;
//	return x * y;
}

static float sceFpuDiv(float x, float y) {
	float result;
	__asm__ volatile (
		"mtv     %1, S001\n"
		"mtv     %2, S002\n"
		"vdiv.s S000, S001, S002\n"
		"mfv     %0, S000\n"
		: "=r"(result) : "r"(x) , "r"(y));
	return result;
}


static s32 sceMatrix4FixedMul(s32 x, s32 y, s32 z, s32 w, const s32* matrx) {
	s32 result = 0;

	//printf("%d %d %d %d\n", x, y, w, z);

	__asm__ volatile (
		"mult   %1, %5			\n"
		"mflo   $t0				\n"
		"mfhi   $t1				\n"
		"sll    $t1, $t1, 15	\n"
		"add	%0, $t0, $t1	\n"

		"mult   %2, %6			\n"
		"mflo   $t0				\n"
		"mfhi   $t1				\n"
		"sll    $t1, $t1, 15	\n"
		"add	$t2, $t0, $t1	\n"
		"add	%0, %0,  $t2	\n"

		"mult   %3, %7			\n"
		"mflo   $t0				\n"
		"mfhi   $t1				\n"
		"sll    $t1, $t1, 15	\n"
		"add	$t2, $t0, $t1	\n"
		"add	%0, %0 ,  $t2	\n"

		"mult   %4, %8			\n"
		"mflo   $t0				\n"
		"mfhi   $t1				\n"
		"sll    $t1, $t1, 15	\n"
		"add	$t2, $t0, $t1	\n"

		"sll    $t2, $t2, 12	\n"

		"add	%0, %0 ,  $t2	\n"

		: "=r"(result) : "r"((s64)matrx[0]), "r"((s64)matrx[4]), "r"((s64)matrx[8]), "r"((s64)matrx[12]), "r"((s64)x), "r"((s64)y), "r"((s64)z), "r"((s64)w));
	return result;

}

static float sceMatrix4Mul(float x, float y, float z, float w, const float* matrx) {
	float result = 0;

	__asm__ volatile (

		"mtv		 %1, S001		\n"
		"mtv		 %5, S010		\n"
		"vmul.s		 S000,S001,S010	\n"
		"mtv		 %2, S001		\n"
		"mtv		 %6, S010		\n"
		"vmul.s		 S003,S001,S010	\n"
		"vadd.s		 S000,S000,S003	\n"
		"mtv		 %3, S001		\n"
		"mtv		 %7, S010		\n"
		"vmul.s		 S003,S001,S010	\n"
		"vadd.s		 S000,S000,S003	\n"
		"mtv		 %4, S001		\n"
		"mtv		 %8, S010		\n"
		"vmul.s		 S003,S001,S010	\n"
		"vadd.s		 S000,S000,S003	\n"
		"mfv		 %0, S000		\n"

		: "=r"(result) : "r"(matrx[0]), "r"(matrx[4]), "r"(matrx[8]), "r"(matrx[12]), "r"(x), "r"(y), "r"(z), "r"(w));
	return result;
}


static float sceMatrix3Mul(float x, float y, float z, const float* matrx) {
	float result = 0;

	__asm__ volatile (

		"mtv		 %1, S001		\n"
		"mtv		 %4, S010		\n"
		"vmul.s		 S000,S001,S010	\n"
		"mtv		 %2, S001		\n"
		"mtv		 %5, S010		\n"
		"vmul.s		 S003,S001,S010	\n"
		"vadd.s		 S000,S000,S003	\n"
		"mtv		 %3, S001		\n"
		"mtv		 %6, S010		\n"
		"vmul.s		 S003,S001,S010	\n"
		"vadd.s		 S000,S000,S003	\n"
		"mfv		 %0, S000		\n"

		: "=r"(result) : "r"(matrx[0]), "r"(matrx[4]), "r"(matrx[8]), "r"(x), "r"(y), "r"(z));
	return result;
}

static float * sceFpuDivQ(float* q,float val) {
	float result[4];
	
	/*
	__asm__ volatile (
		"mtv     %1, V000\n"
		"mtv     %2, S010\n"
		"vdiv.t  V020, V000, S010\n"
		"sv.q    V020, %0\n"
		: "=r"(*result) : "r"(*q), "r"(val));
	*/

	/*__asm__ volatile (
		"mtv     %1, S000\n"
		"mtv     %2, S010\n"
		"vdiv.s  S020, S000, S010\n"
		"vdiv.s  S024, S004, S010\n"
		"vdiv.s  S028, S008, S010\n"
		"sv.q    S020, %0\n"
		: "=r"(*result) : "r"(*q), "r"(val));*/

	return result;
}

void memcpy_vfpu(void* dst, const void* src, u32 size);
void memcpy_vfpu_byteswap(void* dst, const void* src, u32 size);

#define fast_memcpy 		memcpy_vfpu
#define fast_memcpy_swizzle memcpy_vfpu_byteswap

#endif