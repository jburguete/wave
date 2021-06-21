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
 * \file wave1d.h
 * \brief Header file to define an 1D mesh.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2010-2014.
 */
#ifndef MESH1D__H
#define MESH1D__H 1

typedef struct
{
  Wave1D wave[1];
  Cell1D *cell;
  Wall1D *wall;
  JBDOUBLE dx_max, initial_mass, final_mass, inlet_mass, outlet_mass;
  int type, focus, n_cells, n_walls;
} Mesh1D;

#define MESH_1D(mesh) ((Mesh1D*)mesh)

void mesh_1d_message(char *text);
void mesh_1d_write(Mesh1D *mesh, FILE *file);
int mesh_1d_create(Mesh1D *mesh);
int mesh_1d_open_xml(Mesh1D *mesh, xmlNode *node);

#endif
