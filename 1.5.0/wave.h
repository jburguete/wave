#ifndef WAVE_H
#define WAVE_H 1

#include "def.h"

extern int wave_type, shape_initial;
extern JBFLOAT wave_velocity, shape_position, shape_width, shape_amplitude,
  shape_height, shape_base, domain_width;
extern JBFLOAT xa, xb, xc, ta0, tb0, tc0, taf, tbf, tcf, tt;

extern JBDOUBLE (*propagation) (JBDOUBLE);

extern JBDOUBLE (*y) (JBDOUBLE), (*yy0) (JBDOUBLE), (*yyf) (JBDOUBLE);

JBDOUBLE propagation_Linear (JBDOUBLE u);
JBDOUBLE propagation_Burgers (JBDOUBLE u);

void y_adjust ();

JBDOUBLE y_square (JBDOUBLE x);
JBDOUBLE yy_square (JBDOUBLE t);

JBDOUBLE y_triangular (JBDOUBLE x);
JBDOUBLE yy_triangular (JBDOUBLE t);

JBDOUBLE y_sinusoidal (JBDOUBLE x);
JBDOUBLE yy_sinusoidal (JBDOUBLE t);

JBDOUBLE y_Gaussian (JBDOUBLE x);
JBDOUBLE yy_Gaussian (JBDOUBLE t);

JBDOUBLE y_square_Burgers (JBDOUBLE x);
JBDOUBLE yy0_square_Burgers (JBDOUBLE x);
JBDOUBLE yyf_square_Burgers (JBDOUBLE x);

JBDOUBLE y_triangular_Burgers (JBDOUBLE x);
JBDOUBLE yy0_triangular_Burgers (JBDOUBLE x);
JBDOUBLE yyf_triangular_Burgers (JBDOUBLE x);

JBDOUBLE y_steady_Burgers (JBDOUBLE x);
JBDOUBLE yy_steady_Burgers (JBDOUBLE t);

void initial_wave ();

#endif
