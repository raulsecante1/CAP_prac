#include <math.h>
#include "nbody.h"

/*****
 *Version usando SOA
 *****/

void bodyForce(body *p, float dt, int n) {

	for (int i = 0; i < n; i++) { 
		float Fx = 0.0f; float Fy = 0.0f; float Fz = 0.0f;

		for (int j = 0; j < n; j++) {
			if (i!=j) {
				float dx = p[j].x - p[i].x;
				float dy = p[j].y - p[i].y;
				float dz = p[j].z - p[i].z;
				//double softeningSquared = 1e-3;
				float softeningSquared = 1e-3;  //de double a float para evitar la convension de tipo
				float distSqr = dx*dx + dy*dy + dz*dz + softeningSquared;
				float invDist = 1.0f / sqrtf(distSqr);
				float invDist3 = invDist * invDist * invDist;

				//double G = 6.674e-11;
				float G = 6.674e-11;  //de double a float para evitar la convension de tipo
				float g_masses = G * p[j].m * p[i].m;

				Fx += g_masses * dx * invDist3; 
				Fy += g_masses * dy * invDist3; 
				Fz += g_masses * dz * invDist3;
			}
		}

		p[i].vx += dt*Fx/p[i].m; p[i].vy += dt*Fy/p[i].m; p[i].vz += dt*Fz/p[i].m;
	}
}

void bodyForce_SOA(bodySOA *p, float dt, int n) {

	for (int i = 0; i < n; i++) { 
		float Fx = 0.0f; float Fy = 0.0f; float Fz = 0.0f;

		for (int j = 0; j < n; j++) {
			if (i!=j) {
				float dx = p->x[j] - p->x[i];
				float dy = p->y[j] - p->y[i];
				float dz = p->z[j] - p->z[i];
				//double softeningSquared = 1e-3;
				float softeningSquared = 1e-3;  //de double a float para evitar la convension de tipo
				float distSqr = dx*dx + dy*dy + dz*dz + softeningSquared;
				float invDist = 1.0f / sqrtf(distSqr);
				float invDist3 = invDist * invDist * invDist;

				//double G = 6.674e-11;
				float G = 6.674e-11;  //de double a float para evitar la convension de tipo
				float g_masses = G * p->m[j] * p->m[i];

				Fx += g_masses * dx * invDist3; 
				Fy += g_masses * dy * invDist3; 
				Fz += g_masses * dz * invDist3;
			}
		}

		p->vx[i] += dt*Fx/p->m[i]; p->vy[i] += dt*Fy/p->m[i]; p->vz[i] += dt*Fz/p->m[i];
	}
}

void integrate(body *p, float dt, int n){
	for (int i = 0 ; i < n; i++) {
		p[i].x += p[i].vx*dt;
		p[i].y += p[i].vy*dt;
		p[i].z += p[i].vz*dt;
	}
}

void integrate_SOA(bodySOA *p, float dt, int n){
	#pragma ivdep
	for (int i = 0 ; i < n; i++) {
		p->x[i] += p->vx[i]*dt;
		p->y[i] += p->vy[i]*dt;
		p->z[i] += p->vz[i]*dt;
	}
}
