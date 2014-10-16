/*
WAVE: a software to calculate numerical propagation of waves.

AUTHORS: Javier Burguete Tolosa.

Copyright 2010-2014, AUTHORS.

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
 * \copyright Copyright 2010-2014.
*/
#ifndef SHAPE1D__H
#define SHAPE1D__H 1

/**
 * \enum Shape1DType
 * \brief Types of 1D wave shape.
 * \var SHAPE_1D_TYPE_NONE
 * \brief not mesh shape.
 * \var SHAPE_1D_TYPE_SQUARE
 * \brief square mesh shape.
 * \var SHAPE_1D_TYPE_TRIANGULAR
 * \brief triangular mesh shape.
 * \var SHAPE_1D_TYPE_GAUSSIAN
 * \brief gaussian mesh shape.
 * \var SHAPE_1D_TYPE_SINUSOIDAL
 * \brief sinusoidal mesh shape.
 * \var SHAPE_1D_TYPE_SINUSOIDAL_2
 * \brief sinusoidal square mesh shape.
 */
enum Shape1DType
{
	SHAPE_1D_TYPE_NONE = 0,
	SHAPE_1D_TYPE_SQUARE = 1,
	SHAPE_1D_TYPE_TRIANGULAR = 2,
	SHAPE_1D_TYPE_GAUSSIAN = 3,
	SHAPE_1D_TYPE_SINUSOIDAL = 4,
	SHAPE_1D_TYPE_SINUSOIDAL_2 = 5,
	SHAPE_1D_TYPE_PARTICULAR = 4
};

/**
 * \struct Shape1D
 * \brief Struct to define an 1D wave shape.
 */
typedef struct
{
/**
 * \var x0
 * \brief x-coordinate at the center of the wave shape.
 * \var w
 * \brief width of the wave shape.
 * \var u0
 * \brief base value of the wave shape.
 * \var h
 * \brief height of the wave shape.
 * \var type
 * \brief type of shape.
 */
	JBDOUBLE x0, w, u0, h;
	unsigned int type;
} Shape1D;

int shape_1d_open_xml(Shape1D *shape, xmlNode *node);

#endif
