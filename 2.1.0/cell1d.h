#ifndef CELL1D__H
#define CELL1D__H 1

#include "def.h"

typedef struct _Cell1D
{
	JBDOUBLE x, dx, dxx, U, vx;
} Cell1D;

void cell_1d_write(Cell1D *cell, FILE *file);

#endif
