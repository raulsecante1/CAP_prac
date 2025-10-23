#ifndef MEMORY_H
#define MEMORY_H

body* get_memory(int nBodies);
bodySOA* get_memory_SOA(int nBodies);
void free_memory(body *p);

void randomizeBodies(body *data, int n);

void randomizeBodies_SOA(bodySOA *data, int n);

void free_memory_SOA(bodySOA *chg);

#endif
