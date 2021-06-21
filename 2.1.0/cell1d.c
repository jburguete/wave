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
 * \file cell1d.c
 * \brief Source file to define an 1D mesh cell.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2010-2021.
*/
#include <stdio.h>
#include "config.h"
#include "jb/jb_math.h"
#include "tools.h"
#include "cell1d.h"

/**
 * function to write the content of an 1D mesh cell on a file.
 */
void
cell_1d_write (Cell1D * cell,   ///< Cell1D struct.
               FILE * file)     ///< output file.
{
#if DEBUG_CELL_1D
  fprintf (stderr, "cell_1d_write: start\n");
#endif
  fprintf (file, FWL " " FWL " " FWL " " FWL "\n",
           cell->x, cell->U, cell->vx, cell->dx);
#if DEBUG_CELL_1D
  fprintf (stderr, "cell_1d_write: end\n");
#endif
}
