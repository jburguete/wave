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
 * \file config.h
 * \brief Configuration header file.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2010-2014.
 */
#ifndef CONFIG__H
#define CONFIG__H 1

/**
 * \def MAX_DIMENSIONS
 * \brief Number of maximum dimensions.
 */
#define MAX_DIMENSIONS 1

/**
 * \def MESH_RANDOM_SEED
 * \brief Seed of the pseudo-random numbers generator.
 */
#define MESH_RANDOM_SEED 7

// XML labels

#define XML_APPROACH	(const xmlChar*)"approach"
#define XML_DIMENSIONS	(const xmlChar*)"dimensions"
#define XML_DISCRETE	(const xmlChar*)"discrete"
#define XML_DOMAIN		(const xmlChar*)"domain"
#define XML_DX			(const xmlChar*)"dx"
#define XML_GAUSSIAN	(const xmlChar*)"gaussian"
#define XML_H			(const xmlChar*)"h"
#define XML_INTEGRAL	(const xmlChar*)"integral"
#define XML_LINEAR		(const xmlChar*)"linear"
#define XML_MESH		(const xmlChar*)"mesh"
#define XML_REGULAR		(const xmlChar*)"regular"
#define XML_RANDOM		(const xmlChar*)"random"
#define XML_SHAPE		(const xmlChar*)"shape"
#define XML_SINUSOIDAL	(const xmlChar*)"sinusoidal"
#define XML_SQUARE		(const xmlChar*)"square"
#define XML_XMAX		(const xmlChar*)"xmax"
#define XML_STEADY		(const xmlChar*)"steady"
#define XML_TMAX		(const xmlChar*)"tmax"
#define XML_TRIANGULAR	(const xmlChar*)"triangular"
#define XML_TYPE		(const xmlChar*)"type"
#define XML_U0			(const xmlChar*)"u0"
#define XML_VARIATION	(const xmlChar*)"variation"
#define XML_W			(const xmlChar*)"w"
#define XML_WAVE		(const xmlChar*)"wave"
#define XML_X0			(const xmlChar*)"x0"
#define XML_X1			(const xmlChar*)"x1"
#define XML_XMAX		(const xmlChar*)"xmax"

/**
 * \def DEBUG_DOMAIN_1D_OPEN_XML
 * \brief Macro to debug the domain_1d_open_xml function.
 */
#define DEBUG_DOMAIN_1D_OPEN_XML 1

/**
 * \def DEBUG_SHAPE_1D_OPEN_XML
 * \brief Macro to debug the shape_1d_open_xml function.
 */
#define DEBUG_SHAPE_1D_OPEN_XML 1

/**
 * \def DEBUG_MESH_1D_WRITE
 * \brief Macro to debug the mesh_1d_write function.
 */
#define DEBUG_MESH_1D_WRITE 1

/**
 * \def DEBUG_MESH_1D_CREATE
 * \brief Macro to debug the mesh_1d_create function.
 */
#define DEBUG_MESH_1D_CREATE 1

/**
 * \def DEBUG_MESH_1D_OPEN_XML
 * \brief Macro to debug the mesh_1d_open_xml function.
 */
#define DEBUG_MESH_1D_OPEN_XML 1

#endif
