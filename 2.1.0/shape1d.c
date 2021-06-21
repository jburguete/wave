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
 * \file shape1d.c
 * \brief Source file to define an 1D wave shape.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2010-2021.
 */
#include "config.h"
#include "jb/jb_xml.h"
#include "tools.h"
#include "shape1d.h"

/**
 * function to make an error message in the 1D shape context.
 */
static inline void
shape_1d_error (char *text)     ///< message string.
{
  error_message ("Shape1D", text);
}

/**
 * function to read an 1D shape of a XML node.
 *
 * \return 0 on error, 1 on success.
 */
int
shape_1d_open_xml (Shape1D * shape,     ///< Shape1D struct.
                   xmlNode * node)      ///< XML node.
{
  xmlChar *buffer;
  const char *msg;
  int err1, err2, err3, err4;

#if DEBUG_SHAPE_1D
  fprintf (stderr, "shape_1d_open_xml: start\n");
#endif

  // Checking the name of the XML node
  if (xmlStrcmp (node->name, XML_SHAPE))
    {
      msg = _("Bad XML");
      goto error1;
    }

  // Reading the type of shape
  if (!xmlHasProp (node, XML_TYPE))
    {
      msg = _("Not type");
      goto error1;
    }
  buffer = xmlGetProp (node, XML_TYPE);
  if (!buffer)
    {
      msg = _("Unable to open the type");
      goto error1;
    }
  if (!xmlStrcmp (buffer, XML_SQUARE))
    shape->type = SHAPE_1D_TYPE_SQUARE;
  else if (!xmlStrcmp (buffer, XML_TRIANGULAR))
    shape->type = SHAPE_1D_TYPE_TRIANGULAR;
  else if (!xmlStrcmp (buffer, XML_SINUSOIDAL))
    shape->type = SHAPE_1D_TYPE_SINUSOIDAL;
  else if (!xmlStrcmp (buffer, XML_GAUSSIAN))
    shape->type = SHAPE_1D_TYPE_GAUSSIAN;
  else if (!xmlStrcmp (buffer, XML_STEADY))
    shape->type = SHAPE_1D_TYPE_STEADY;
  else
    {
      xmlFree (buffer);
      msg = _("Unknown type");
      goto error1;
    }
  xmlFree (buffer);

  // Reading the propierties of the shape
  shape->x0 = jb_xml_node_get_float (node, XML_X0, &err1);
  shape->w = jb_xml_node_get_float (node, XML_W, &err2);
  shape->u0 = jb_xml_node_get_float (node, XML_U0, &err3);
  shape->h = jb_xml_node_get_float (node, XML_H, &err4);
  if (err1 != 1 || err2 != 1 || err3 != 1 || err4 != 1)
    {
      msg = _("Bad parameters");
      goto error1;
    }

#if DEBUG_SHAPE_1D
  fprintf (stderr, "shape_1d_open_xml: end\n");
#endif
  return 1;

error1:
  // Making the error message
  shape_1d_error ((char *) msg);
#if DEBUG_SHAPE_1D
  fprintf (stderr, "shape_1d_open_xml: end\n");
#endif
  return 0;
}
