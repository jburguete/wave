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
 * \file domain1d.c
 * \brief Source file to define an 1D domain.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2010-2013.
*/
#include "def.h"
#include "jb/jb_xml.h"
#include "domain1d.h"

/**
 * \fn void domain_1d_message(char *text)
 * \brief Function to make a message in the 1D domain context.
 * \param text
 * \brief message string.
*/
void domain_1d_message(char *text)
{
	message = g_strconcat("Domain1D:\n", gettext(text), NULL);
}

/**
 * \fn int domain_1d_open_xml(Domain1D *domain, xmlNode *node)
 * \brief Function to read an 1D domain of a XML node.
 * \param domain
 * \brief 1D domain struct.
 * \param node
 * \brief XML node.
 * \return 0 on error, 1 on success.
*/
int domain_1d_open_xml(Domain1D *domain, xmlNode *node)
{
	int err1, err2;
	char *err_msg;

#if DEBUG_DOMAIN_1D_OPEN_XML
	fprintf(stderr,"domain_1d_open_xml: start\n");
#endif

	// Checking the name of the XML node
	if (xmlStrcmp(node->name, XML_DOMAIN))
	{
		err_msg = "Bad XML";
		goto error1;
	}

	// Reading the propierties of the domain
	domain->xmax = jb_xml_node_get_float(node, XML_XMAX, &err1);
	domain->tmax = jb_xml_node_get_float(node, XML_TMAX, &err2);
	if (err1 != 1 || err2 != 1 || domain->xmax <= 0. || domain->tmax <= 0.)
	{
		err_msg = "Bad defined";
		goto error1;
	}
#if DEBUG_DOMAIN_1D_OPEN_XML
	fprintf(stderr,"domain_1d_open_xml: end\n");
#endif
	return 1;

error1:
	// Making the error message
	domain_1d_message(err_msg);
#if DEBUG_DOMAIN_1D_OPEN_XML
	fprintf(stderr,"domain_1d_open_xml: end\n");
#endif
	return 0;
}

