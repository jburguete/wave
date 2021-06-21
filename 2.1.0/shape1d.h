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
 * \file shape1d.h
 * \brief Header file to define an 1D wave shape.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2010-2021.
*/
#ifndef SHAPE1D__H
#define SHAPE1D__H 1

/**
 * types of 1D wave shape.
 */
enum Shape1DType
{
  SHAPE_1D_TYPE_NONE = 0,       ///< not mesh shape.
  SHAPE_1D_TYPE_SQUARE = 1,     ///< square mesh shape.
  SHAPE_1D_TYPE_TRIANGULAR = 2, ///< triangular mesh shape.
  SHAPE_1D_TYPE_GAUSSIAN = 3,   ///< gaussian mesh shape.
  SHAPE_1D_TYPE_SINUSOIDAL = 4, ///< sinusoidal mesh shape.
  SHAPE_1D_TYPE_SINUSOIDAL_2 = 5,       ///< sinusoidal square mesh shape.
  SHAPE_1D_TYPE_STEADY = 6,     ///< steady mesh shape.
};

/**
 * \struct Shape1D
 * \brief Struct to define an 1D wave shape.
 */
typedef struct
{
  JBDOUBLE x0;                  ///< x-coordinate at the center.
  JBDOUBLE w;                   ///< width.
  JBDOUBLE u0;                  ///< base value.
  JBDOUBLE h;                   ///< height.
  unsigned int type;            ///< type.
} Shape1D;

int shape_1d_open_xml (Shape1D * shape, xmlNode * node);

#endif
