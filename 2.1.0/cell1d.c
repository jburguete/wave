#include "cell1d.h"

void cell_1d_write(Cell1D *cell, FILE *file)
{
#if DEBUG_CELL_1D_WRITE
	fprintf(stderr, "cell_1d_write: start\n");
#endif
	fprintf(file, "%lg %lg %lg %lg\n",
		cell->x,
		cell->U,
		cell->vx,
		cell->dx);
#if DEBUG_CELL_1D_WRITE
	fprintf(stderr, "cell_1d_write: end\n");
#endif
}


