#include "wave.h"

JBDOUBLE t, vx_linear;
void *mesh = NULL;
char *message = NULL;

void wave_message(char *text)
{
	char *buffer;
	buffer = message;
	message = g_strconcat("Wave:\n", text, NULL);
	if (buffer) g_free(buffer);
	jbw_show_error(message);
}

void wave_open_xml(char *input_file, char *output_file)
{
	int type, dimensions, err;
	char *buffer;
	FILE *file;
	xmlDoc *doc;
	xmlNode *node;
	doc = xmlParseFile(input_file);
	if (!doc)
	{
		wave_message(gettext("Unable to open the file"));
		return;
	}
	node = xmlDocGetRootElement(doc);
	if (!node || xmlStrcmp(node->name, XML_WAVE))
	{
		wave_message(gettext("Bad XML file"));
		return;
	}
	if (!xmlHasProp(node, XML_TYPE))
	{
		wave_message(gettext("Not type"));
		return;
	}
	buffer = xmlGetProp(node, XML_TYPE);
	if (!buffer)
	{
		wave_message(gettext("Unable to open the type"));
		return;
	}
	if (!xmlStrcmp(buffer, XML_LINEAR)) type = WAVE_TYPE_LINEAR;
	else
	{
		wave_message(gettext("Unknown type"));
		return;
	}
	dimensions = jb_xml_node_get_int(node, XML_DIMENSIONS, &err);
	if (err != 1 || dimensions < 1 || dimensions > MAX_DIMENSIONS)
	{
		wave_message(gettext("Bad dimensions number"));
		return;
	}
	t = 0.;
	switch (dimensions)
	{
	case 1:
		mesh = jb_realloc(mesh, sizeof(Mesh1D));
		MESH_1D(mesh)->parameters = NULL;
		node = node->children;
		if (!node)
		{
			wave_message(gettext("Not domain"));
			return;
		}
		if (!domain_1d_open_xml(MESH_1D(mesh)->wave->domain, node))
		{
			wave_message(message);
			return;
		}
		node = node->next;
		if (!node)
		{
			wave_message(gettext("Not shape"));
			return;
		}
		if (!shape_1d_open_xml(MESH_1D(mesh)->wave->shape, node))
		{
			wave_message(message);
			return;
		}
		switch (type)
		{
		case WAVE_TYPE_LINEAR:
			MESH_1D(mesh)->wave->adjust = adjust_1d_linear;
			MESH_1D(mesh)->wave->vx = vx_1d_linear;
			switch (MESH_1D(mesh)->wave->shape->type)
			{
			case SHAPE_1D_TYPE_SQUARE:
				MESH_1D(mesh)->wave->u = u_1d_linear_square;
				MESH_1D(mesh)->wave->mass = mass_1d_linear_square;
				MESH_1D(mesh)->wave->flux = flux_1d_linear_square;
				break;
			case SHAPE_1D_TYPE_TRIANGULAR:
				MESH_1D(mesh)->wave->u = u_1d_linear_triangular;
				MESH_1D(mesh)->wave->mass = mass_1d_linear_triangular;
				MESH_1D(mesh)->wave->flux = flux_1d_linear_triangular;
				break;
			case SHAPE_1D_TYPE_SINUSOIDAL:
				MESH_1D(mesh)->wave->u = u_1d_linear_sinusoidal;
				MESH_1D(mesh)->wave->mass = mass_1d_linear_sinusoidal;
				MESH_1D(mesh)->wave->flux = flux_1d_linear_sinusoidal;
				break;
			case SHAPE_1D_TYPE_GAUSSIAN:
				MESH_1D(mesh)->wave->u = u_1d_linear_gaussian;
				MESH_1D(mesh)->wave->mass = mass_1d_linear_gaussian;
				MESH_1D(mesh)->wave->flux = flux_1d_linear_gaussian;
				break;
			default:
				jb_free_null(&mesh);
				wave_message(gettext("Bad shape type"));
				return;
			}
		}
		MESH_1D(mesh)->wave->adjust(MESH_1D(mesh)->wave);
		node = node->next;
		if (!node)
		{
			wave_message(gettext("Not mesh"));
			return;
		}
		if (!mesh_1d_open_xml((Mesh1D*)mesh, node))
		{
			wave_message(message);
			return;
		}
		file = fopen(output_file, "w");
		if (!file)
		{
			wave_message(gettext("Unable to open the output file"));
			return;
		}
		mesh_1d_write((Mesh1D*)mesh, file);
		fclose(file);
	}
}

int main(int argn,char **argc)
{
	if (argn != 3)
	{
		wave_message(gettext("Usage is:\n./wave input_file.xml output_file"));
		return 0;
	}
	xmlKeepBlanksDefault(0);
	wave_open_xml(argc[1], argc[2]);
	return 0;
}
