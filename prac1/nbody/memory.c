#include <immintrin.h>
#include <malloc.h>
#include <stdlib.h>
#include "nbody.h"

body* get_memory(int nBodies)
{
	body *p = (body*)malloc(nBodies*sizeof(body));
	return p;
}

bodySOA* get_memory_SOA(int nBodies)
{
	const int alignment=64;
	bodySOA *chg = (bodySOA *)malloc(sizeof(bodySOA));
	chg->m = (float*)_mm_malloc(sizeof(float)*nBodies, alignment);
	chg->x = (float*)_mm_malloc(sizeof(float)*nBodies, alignment);
    chg->y = (float*)_mm_malloc(sizeof(float)*nBodies, alignment);
    chg->z = (float*)_mm_malloc(sizeof(float)*nBodies, alignment);
	chg->vx = (float*)_mm_malloc(sizeof(float)*nBodies, alignment);
    chg->vy = (float*)_mm_malloc(sizeof(float)*nBodies, alignment);
    chg->vz = (float*)_mm_malloc(sizeof(float)*nBodies, alignment);
	return chg;
}

void free_memory(body *p)
{
	free(p);
}

void free_memory_SOA(bodySOA *chg)
{
	free(chg->m);
	free(chg->x);
	free(chg->y);
	free(chg->z);
	free(chg->vx);
	free(chg->vy);
	free(chg->vz);
}

void randomizeBodies(body *data, int n) {
	for (int i = 0; i < n; i++) {
		data[i].m  = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;

		data[i].x  = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
		data[i].y  = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
		data[i].z  = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;

		data[i].vx = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
		data[i].vy = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
		data[i].vz = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
	}
}

void randomizeBodies_SOA(bodySOA *data, int n) {
	for (int i = 0; i < n; i++) {
		data->m[i]  = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;

		data->x[i]  = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
		data->y[i]  = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
		data->z[i]  = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;

		data->vx[i] = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
		data->vy[i] = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
		data->vz[i] = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
	}
}

