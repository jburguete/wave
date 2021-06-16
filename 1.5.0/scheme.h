#ifndef SCHEME_H
#define SCHEME_H 1

#include "def.h"

typedef struct
{
  JBDOUBLE x, dx, ix, dxx, u, c, a, du, dF, dF2, Lp, Lm, L2p, L2m, C, Dp, E,
    iu, Dm, iup, ium, Ep, Em, uu, au, AA[4], BB[4];
} Parameters;

extern int scheme_type, flux_limiter_type, entropy_type, mesh_type,
  discretization_type, boundary_type, convergence;
extern int n, nc, nmax;
extern JBDOUBLE t, dt, tf, tmax;
extern JBDOUBLE cfl, o, pv, dxmax, initial_mass, final_mass;
extern int fix_in, fix_out;
extern int scheme_order, scheme_steps;
extern int simulating;
extern int parameters2;

extern Parameters *p;

#if JBW != JBW_NO
extern int mesh_cells, dialog_convergence, max_cells, convergence_number;
extern JBDOUBLE simulation_time, cfl_number, implicit, viscosity;
#endif

extern void draw ();

void simulate (FILE * file_error);
void execute (char *file_name);

#endif
