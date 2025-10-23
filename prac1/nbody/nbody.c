#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "nbody.h"
#include "gtime.h"
#include "memory.h"
#include "nbody_routines.h"


double solutionPos(body *p, int n)
{
	float pos_private = 0.0f;
	double pos_global  = 0.0f;

	for (int i = 0 ; i < n; i++) {
		pos_private = sqrtf(p[i].x*p[i].x+p[i].y*p[i].y+p[i].z*p[i].z);
		pos_global+=pos_private;
	}
	return(pos_global);
}

float solutionPos_SOA(bodySOA *p, int n)
{
	float pos_private = 0.0f;
	float pos_global  = 0.0f;

	for (int i = 0 ; i < n; i++) {
		pos_private = sqrtf(p->x[i]*p->x[i]+p->y[i]*p->y[i]+p->z[i]*p->z[i]);
		pos_global+=pos_private;
	}
	return(pos_global);
}

int main(const int argc, const char** argv) {

	int nBodies = 5000;
	if (argc > 1) nBodies = atoi(argv[1]);

	const float dt = 0.01f; // time step
	const int nIters = 100;  // simulation iterations

	//body *p = get_memory(nBodies);  replaced by SOA

	bodySOA *chg = get_memory_SOA(nBodies);  //using SOA

	//randomizeBodies(p, nBodies); // Init pos / vel data

	randomizeBodies_SOA(chg, nBodies); // Init pos / vel data using SOA

	double t0 = get_time();

	for (int iter = 1; iter <= nIters; iter++) {
		bodyForce_SOA(chg, dt, nBodies); // compute interbody forces
		integrate_SOA(chg, dt, nBodies); // integrate position
	}

	double totalTime = get_time()-t0;
	//double solPos = solutionPos(p, nBodies);  // use SoA instead
	float solPos = solutionPos_SOA(chg, nBodies);  //usando SoA
	
	printf("%d Bodies with %d iterations: %0.3f Millions Interactions/second\n", nBodies, nIters, 1e-6 * nBodies * nBodies / totalTime);
	printf("pos=%e\n", solPos);

	free_memory_SOA(chg);  //usando de version SoA
}
