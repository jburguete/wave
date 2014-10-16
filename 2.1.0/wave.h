#ifndef WAVE__H
#define WAVE__H 1

#include "mesh1d.h"

enum WaveType
{
	WAVE_TYPE_LINEAR=0,
	WAVE_TYPE_BURGERS=1
};

void wave_message(char *text);
void wave_open_xml(char *input_file, char *output_file);

#endif
