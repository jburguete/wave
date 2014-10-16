/*
WAVE: a software to calculate numerical propagation of waves.

AUTHORS: Javier Burguete Tolosa.

Copyright 2010-2013, AUTHORS.

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
 * \file shape1d.c
 * \brief Source file to define an 1D wave shape.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2010-2013.
 */
#include "def.h"
#include "jb/jb_xml.h"
#include "shape1d.h"

/**
 * \fn void shape_1d_message(char *text)
 * \brief Function to make a message in the 1D shape context.
 * \param text
 * \brief message string.
 */
void shape_1d_message(char *text)
{
	message = g_strconcat("Shape1D:\n", gettext(text), NULL);
}

/**
 * \fn int shape_1d_open_xml(Shape1D *shape, xmlNode *node)
 * \brief Function to read an 1D shape of a XML node.
 * \param shape
 * \brief 1D shape struct.
 * \param node
 * \brief XML node.
 * \return 0 on error, 1 on success.
 */
int shape_1d_open_xml(Shape1D *shape, xmlNode *node)
{
	int err1, err2, err3, err4;
	char *buffer;

#if DEBUG_SHAPE_1D_OPEN_XML
	fprintf(stderr,"shape_1d_open_xml: start\n");
#endif

	// Checking the name of the XML node
	if (xmlStrcmp(node->name, XML_SHAPE))
	{
		buffer = "Bad XML";
		goto error1;
	}

	// Reading the type of shape
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
	if (!xmlStrcmp(buffer, XML_SQUARE)) shape->type = SHAPE_1D_TYPE_SQUARE;
	else if (!xmlStrcmp(buffer, XML_TRIANGULAR))
		shape->type = SHAPE_1D_TYPE_TRIANGULAR;
	else if (!xmlStrcmp(buffer, XML_SINUSOIDAL))
		shape->type = SHAPE_1D_TYPE_SINUSOIDAL;
	else if (!xmlStrcmp(buffer, XML_GAUSSIAN))
		shape->type = SHAPE_1D_TYPE_GAUSSIAN;
	else if (!xmlStrcmp(buffer, XML_STEADY))
		shape->type = SHAPE_1D_TYPE_STEADY;
	else
	{
		xmlFree(buffer);
		buffer = "Unknown type";
		goto error1;
	}
	xmlFree(buffer);

	// Reading the propierties of the shape
	shape->x0 = jb_xml_node_get_float(node, XML_X0, &err1);
	shape->w = jb_xml_node_get_float(node, XML_W, &err2);
	shape->u0 = jb_xml_node_get_float(node, XML_U0, &err3);
	shape->h = jb_xml_node_get_float(node, XML_H, &err4);
	if (err1 != 1 || err2 != 1 || err3 != 1 || err4 != 1)
	{
		buffer = "Bad parameters";
		goto error1;
	}

#if DEBUG_SHAPE_1D_OPEN_XML
	fprintf(stderr,"shape_1d_open_xml: end\n");
#endif
	return 1;

error1:
	// Making the error message
	shape_1d_message(buffer);
#if DEBUG_SHAPE_1D_OPEN_XML
	fprintf(stderr,"shape_1d_open_xml: end\n");
#endif
	return 0;
}

