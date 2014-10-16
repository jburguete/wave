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
 * \file mesh1d.c
 * \brief Source file to define an 1D mesh.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2010-2014.
 */
#include "def.h"
#include "jb/jb_xml.h"
#include "cell1d.h"
//#include "wall1d.h"
#include "mesh_type.h"
#include "mesh1d.h"

void mesh_1d_message(char *text)
{
	message = g_strconcat("Mesh 1D:\n", gettext(text), NULL);
}

void mesh_1d_write(Mesh1D *mesh, FILE *file)
{
	int i;
#if DEBUG_MESH_1D_WRITE
	fprintf(stderr, "mesh_1d_write: start\n");
#endif
	for (i = 0; i < mesh->n_cells; ++i)
		cell_1d_write(mesh->cell + i, file);
#if DEBUG_MESH_1D_WRITE
	fprintf(stderr, "mesh_1d_write: end\n");
#endif
}

int mesh_1d_create(Mesh1D *mesh)
{
#if DEBUG_MESH_1D_CREATE
	fprintf(stderr, "mesh_1d_create: start\n");
#endif
	mesh->cell = (Cell1D*)
		jb_realloc(mesh->cell, mesh->n_cells * sizeof(Cell1D));
	if (!mesh->cell)
	{
		mesh_1d_message("Not enough memory");
#if DEBUG_MESH_1D_CREATE
		fprintf(stderr, "mesh_1d_create: end\n");
#endif
		return 0;
	}
#if DEBUG_MESH_1D_CREATE
	fprintf(stderr, "mesh_1d_create: end\n");
#endif
	return 1;
}

int mesh_1d_open_xml(Mesh1D *mesh, xmlNode *node)
{
	int i, err;
	JBDOUBLE x, dx, alpha, beta;
	char *buffer;
	GRand *rand;

#if DEBUG_MESH_1D_OPEN_XML
	fprintf(stderr, "mesh_1d_open_xml: start\n");
#endif

	if (xmlStrcmp(node->name, XML_MESH))
	{
		buffer = "Bad XML";
		goto error1;
	}
	if (!xmlHasProp(node, XML_DX))
	{
		buffer = "Not x mesh size";
		goto error1;
	}
	mesh->dx_max = jb_xml_node_get_float(node, XML_DX, &err);
	if (err != 1 || mesh->dx_max <= 0.)
	{
		buffer = "Bad x mesh size";
		goto error1;
	}
	if (!xmlHasProp(node, XML_TYPE))
	{
		buffer = "Not type";
		goto error1;
	}
	buffer = xmlGetProp(node, XML_TYPE);
	if (!buffer)
	{
		buffer = "Unable to open the type";
		goto error1;
	}
	if (!xmlStrcmp(buffer, XML_REGULAR)) mesh->type = MESH_SPACING_REGULAR;
	else if (!xmlStrcmp(buffer, XML_RANDOM)) mesh->type = MESH_SPACING_RANDOM;
	else
	{
		buffer = "Unknown type";
		goto error1;
	}
	xmlFree(buffer);
	if (!xmlHasProp(node, XML_APPROACH))
	{
		buffer = "Not focus";
		goto error1;
	}
	buffer = xmlGetProp(node, XML_APPROACH);
	if (!buffer)
	{
		buffer = "Unable to open the focus";
		goto error1;
	}
	if (!xmlStrcmp(buffer, XML_INTEGRAL))
		mesh->focus = MESH_APPROACH_FINITE_VOLUMES;
	else if (!xmlStrcmp(buffer, XML_DISCRETE)) 
		mesh->focus = MESH_APPROACH_FINITE_DIFFERENCES;
	else
	{
		buffer = "Unknown focus";
		goto error1;
	}
	xmlFree(buffer);
	switch (mesh->type)
	{
	case MESH_SPACING_REGULAR:
		dx = mesh->wave->domain->xmax;
		mesh->n_cells = ceil(dx / mesh->dx_max);
		if (mesh->n_cells < 2) mesh->n_cells = 2;
		dx /= mesh->n_cells;
		switch (mesh->focus)
		{
		case MESH_APPROACH_FINITE_VOLUMES:
			if (!mesh_1d_create(mesh)) goto error1;
			for (i = 0; i < mesh->n_cells; ++i)
			{
				mesh->cell[i].x = (i + 0.5) * dx;
				mesh->cell[i].dx = mesh->cell[i].ix = dx;
				mesh->cell[i].U =
					mesh->wave->mass(mesh->wave, i * dx, (i + 1) * dx) / dx;
			}
			break;
		case MESH_APPROACH_FINITE_DIFFERENCES:
			++mesh->n_cells;
			if (!mesh_1d_create(mesh)) goto error1;
			for (i = 0; i < mesh->n_cells; ++i)
			{
				mesh->cell[i].x = x = i * dx;
				mesh->cell[i].U = mesh->wave->u(mesh->wave, x);
				mesh->cell[i].ix = dx;
			}
			for (i = 0, mesh->cell[0].dx = 0.; i < mesh->n_cells - 1;)
				mesh->cell[i].dx += mesh->cell[i + 1].dx = 
					0.5 * mesh->cell[i].ix;
		}
		break;
	case MESH_SPACING_RANDOM:
		alpha = jb_xml_node_get_float(node, XML_VARIATION, &err);
		if (err != 1)
		{
			buffer = "Not variation coefficient";
			goto error1;
		}
		if (alpha < 0.)
		{
			buffer = "Bad variation coefficient";
			goto error1;
		}
		rand = g_rand_new_with_seed(MESH_RANDOM_SEED);
		dx = mesh->wave->domain->xmax;
		mesh->n_cells = ceil((1. + alpha) * dx / mesh->dx_max);
		if (mesh->n_cells < 2) mesh->n_cells = 2;
		switch (mesh->focus)
		{
		case MESH_APPROACH_FINITE_VOLUMES:
			if (!mesh_1d_create(mesh)) goto error1;
			for (i = 0, beta = 0.; i < mesh->n_cells; ++i)
				beta += mesh->cell[i].dx =
					1. + alpha * g_rand_double(rand);
			dx /= beta;
			for (i = 0; i < mesh->n_cells; ++i) mesh->cell[i].dx *= dx;
			mesh->cell[0].x = 0.5 * mesh->cell[0].dx;
			for (i = 0; i < mesh->n_cells - 1; ++i)
			{
				mesh->cell[i].ix = 0.5 * (mesh->cell[i + 1].dx +
					mesh->cell[i].dx);
				mesh->cell[i + 1].x =
					mesh->cell[i].x + mesh->cell[i].ix;
			}
			for (i = 0; i < mesh->n_cells; ++i)
			{
				dx = 0.5 * mesh->cell[i].dx;
				mesh->cell[i].U = mesh->wave->mass(mesh->wave,
					mesh->cell[i].x - dx, mesh->cell[i].x + dx) /
					mesh->cell[i].dx;
			}
			break;
		case MESH_APPROACH_FINITE_DIFFERENCES:
			++mesh->n_cells;
			if (!mesh_1d_create(mesh)) goto error1;
			for (i = 0, beta = 0.; i < mesh->n_cells - 1; ++i)
				beta += mesh->cell[i].ix =
					1. + alpha * g_rand_double(rand);
			dx /= beta;
			mesh->cell[0].x = mesh->cell[0].dx = 0.;
			for (i = 0; i < mesh->n_cells - 1; ++i)
			{
				mesh->cell[i].ix *= dx;
				mesh->cell[i + 1].x =
					mesh->cell[i].x + mesh->cell[i].ix;
				mesh->cell[i].dx += mesh->cell[i + 1].dx = 
					0.5 * mesh->cell[i].ix;
			}
			for (i = 0; i < mesh->n_cells; ++i)
				mesh->cell[i].U =
					mesh->wave->u(mesh->wave, mesh->cell[i].x);
		}
	}
#if DEBUG_MESH_1D_OPEN_XML
	fprintf(stderr, "mesh_1d_open_xml: end\n");
#endif
	return 1;

error1:
	mesh_1d_message(buffer);
#if DEBUG_MESH_1D_OPEN_XML
	fprintf(stderr, "mesh_1d_open_xml: end\n");
#endif
	return 0;
}

