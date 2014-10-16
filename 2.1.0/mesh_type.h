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
 * \file mesh_type.h
 * \brief Header file to define mesh types.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2010-2014.
 */
#ifndef MESH_TYPE__H
#define MESH_TYPE__H 1

/**
 * \enum MeshSpacing
 * \brief Enum to define the mesh spacing type.
 * \var MESH_SPACING_REGULAR
 * \brief Regular mesh spacing
 * \var MESH_SPACING_RANDOM
 * \brief Random mesh spacing
 */
enum MeshSpacing
{
	MESH_SPACING_REGULAR=0,
	MESH_SPACING_RANDOM=1
};

/**
 * \enum MeshApproach
 * \brief Enum to define the mesh approach (finite differences or finite \
 *   volumes).
 * \var MESH_APPROACH_FINITE_DIFFERENCES
 * \brief Finite differences mesh approach.
 * \var MESH_APPROACH_FINITE_VOLUMES
 * \brief Finite volumes mesh approach.
 */
enum MeshApproach
{
	MESH_APPROACH_FINITE_DIFFERENCES=0,
	MESH_APPROACH_FINITE_VOLUMES=1
};

#endif
