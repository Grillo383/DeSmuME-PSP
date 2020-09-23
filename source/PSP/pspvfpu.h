#ifndef FPU_PSP_H
#define FPU_PSP_H

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

#endif