/*
WAVE: a software to calculate numerical propagation of waves.

AUTHORS: Javier Burguete Tolosa.

Copyright 2010-2021, AUTHORS.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY AUTHORS ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL AUTHORS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

/**
 * \file wave1d.h
 * \brief Header file to define an 1D wave.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2010-2021.
 */
#ifndef WAVE1D__H
#define WAVE1D__H 1

struct Wave1D;

/**
 * \struct Wave1D
 * \brief Struct to define an 1D wave.
 */
typedef struct
{
  Shape1D shape[1];             ///< Shape1D struct.
  Domain1D domain[1];           ///< Domain1D struct.
  void (*adjust) (struct Wave1D *);
  ///< pointer to the function to get the calculation parameters.
    JBDOUBLE (*vx) (JBDOUBLE);
  ///< pointer to the function to get the velocity in a position.
    JBDOUBLE (*u) (struct Wave1D *, JBDOUBLE);
  ///< pointer to the function to get the variable in a position
    JBDOUBLE (*mass) (struct Wave1D *, JBDOUBLE, JBDOUBLE);
  ///< pointer to the function to get the mass in an intervale.
    JBDOUBLE (*flux) (struct Wave1D *, JBDOUBLE);
  ///< pointer to the function to get the numerical flux in an intervale.
  JBDOUBLE xa;                  ///< parameter of position (see wave.pdf).
  JBDOUBLE xb;                  ///< parameter of position (see wave.pdf).
  JBDOUBLE xc;                  ///< parameter of position (see wave.pdf).
  JBDOUBLE tt;                  ///< parameter of time (see wave.pdf).
  int type;                     ///< type.
} Wave1D;

extern JBDOUBLE t, vx_linear;

JBDOUBLE vx_1d_linear (JBDOUBLE u);
void adjust_1d_linear (Wave1D * wave);

JBDOUBLE u_1d_linear_square (Wave1D * wave, JBDOUBLE x);
JBDOUBLE mass_1d_linear_square (Wave1D * wave, JBDOUBLE x1, JBDOUBLE x2);
JBDOUBLE flux_1d_linear_square (Wave1D * wave, JBDOUBLE x);

JBDOUBLE u_1d_linear_triangular (Wave1D * wave, JBDOUBLE x);
JBDOUBLE mass_1d_linear_triangular (Wave1D * wave, JBDOUBLE x1, JBDOUBLE x2);
JBDOUBLE flux_1d_linear_triangular (Wave1D * wave, JBDOUBLE x);

JBDOUBLE u_1d_linear_sinusoidal (Wave1D * wave, JBDOUBLE x);
JBDOUBLE mass_1d_linear_sinusoidal (Wave1D * wave, JBDOUBLE x1, JBDOUBLE x2);
JBDOUBLE flux_1d_linear_sinusoidal (Wave1D * wave, JBDOUBLE x);

JBDOUBLE u_1d_linear_gaussian (Wave1D * wave, JBDOUBLE x);
JBDOUBLE mass_1d_linear_gaussian (Wave1D * wave, JBDOUBLE x1, JBDOUBLE x2);
JBDOUBLE flux_1d_linear_gaussian (Wave1D * wave, JBDOUBLE x);

#endif
