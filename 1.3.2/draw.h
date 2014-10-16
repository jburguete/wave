#ifndef DRAW_H
#define DRAW_H 1

#include "def.h"

#if JBW == JBW_NO

	extern JBDOUBLE t,initial_mass,final_mass;
	static inline void draw()
	{
		printf("t="FWL" m0="FWL" m="FWL" e="FWL"\n",
			t,initial_mass,final_mass,1.-final_mass/initial_mass);
	}

#else

	extern JBDOUBLE draw_xmin,draw_xmax,draw_ymin,draw_ymax;

	void draw_graphic();
	void draw();

#endif

#endif
