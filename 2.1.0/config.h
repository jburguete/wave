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
 * \file config.h
 * \brief Configuration header file.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2010-2021.
 */
#ifndef CONFIG__H
#define CONFIG__H 1

#define MAX_DIMENSIONS 1        ///< maximum number of dimensions.
#define MESH_RANDOM_SEED 7
///< seed of the pseudo-random numbers generator.

// debug macros

#define DEBUG_CELL_1D 1
///< macro to debug the cell_1d functions.
#define DEBUG_DOMAIN_1D 1
///< macro to debug the domain_1d_functions.
#define DEBUG_MESH_1D 1
///< macro to debug the mesh_1d functions.
#define DEBUG_SHAPE_1D 1
///< macro to debug the shape_1d functions.
#define DEBUG_WAVE_1D 1
///< macro to debug the wape_1d functions.

// XML labels

#define XML_APPROACH            (const xmlChar*)"approach"
///< approach XML label.
#define XML_DIMENSIONS          (const xmlChar*)"dimensions"
///< dimensions XML label.
#define XML_DISCRETE            (const xmlChar*)"discrete"
///< discrete XML label.
#define XML_DOMAIN              (const xmlChar*)"domain"
///< domain XML label.
#define XML_DX                  (const xmlChar*)"dx"
///< dx XML label.
#define XML_GAUSSIAN            (const xmlChar*)"gaussian"
///< gaussian XML label.
#define XML_H                   (const xmlChar*)"h"
///< h XML label.
#define XML_INTEGRAL            (const xmlChar*)"integral"
///< integral XML label.
#define XML_LINEAR              (const xmlChar*)"linear"
///< linear XML label.
#define XML_MESH                (const xmlChar*)"mesh"
///< mesh XML label.
#define XML_RANDOM              (const xmlChar*)"random"
///< random XML label.
#define XML_REGULAR             (const xmlChar*)"regular"
///< regular XML label.
#define XML_SHAPE               (const xmlChar*)"shape"
///< shape XML label.
#define XML_SINUSOIDAL          (const xmlChar*)"sinusoidal"
///< sinusoidal XML label.
#define XML_SQUARE              (const xmlChar*)"square"
///< square XML label.
#define XML_STEADY              (const xmlChar*)"steady"
///< steady XML label.
#define XML_TMAX                (const xmlChar*)"tmax"
///< tmax XML label.
#define XML_TRIANGULAR          (const xmlChar*)"triangular"
///< triangular XML label.
#define XML_TYPE                (const xmlChar*)"type"
///< type XML label.
#define XML_U0                  (const xmlChar*)"u0"
///< u0 XML label.
#define XML_VARIATION           (const xmlChar*)"variation"
///< variation XML label.
#define XML_W                   (const xmlChar*)"w"
///< w XML label.
#define XML_WAVE                (const xmlChar*)"wave"
///< wave XML label.
#define XML_X0                  (const xmlChar*)"x0"
///< x0 XML label.
#define XML_X1                  (const xmlChar*)"x1"
///< x1 XML label.
#define XML_XMAX                (const xmlChar*)"xmax"
///< xmax XML label.

#endif
