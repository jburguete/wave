#include "def.h"

#if JBW==JBW_GTK

#include <time.h>
#include "jb/jb_math.h"
#include "wave.h"
#include "scheme.h"
#include "simulator.h"
#include "draw.h"

void draw_graphic()
{
	register int i,j;
	DialogSimulator *dlg=&dialog_simulator;
	#if DEBUG_LEVEL & DEBUG_DRAW_GRAPHIC
		fprintf(stderr,"Draw graphic: start\n");
	#endif
	if (p)
	{
		j=n+1;
		for (i=j; --i>=0;)
			p[i].uu = y(p[i].x);
		jbw_graphic_draw_linesv(dlg->graphic,&(p->x),&(p->u),&(p->uu),0,0,
			sizeof(Parameters),j);
	}
	#if DEBUG_LEVEL & DEBUG_DRAW_GRAPHIC
		fprintf(stderr,"DG w=%d h=%d\n",
			((GtkWidget*)dlg->graphic->drawing_area)->allocation.width,
			((GtkWidget*)dlg->graphic->drawing_area)->allocation.height);
		fprintf(stderr,"Draw graphic: end\n");
	#endif
}

void draw()
{
	DialogSimulator *dlg=&dialog_simulator;
	#if DEBUG_LEVEL & DEBUG_DRAW
		fprintf(stderr,"Draw: start\n");
	#endif
jbw_graphic_expose_event();
	glutPostRedisplay();
	gtk_progress_bar_set_fraction(dlg->progress_simulation_time, t / tf);
	if (simulating)
	{
		jbw_int_entry_set_value_with_format(dlg->entry_cpu_time,"%ld s",
			time(0)-dlg->time0);
		jbw_float_entry_set_value(dlg->entry_initial_mass,initial_mass);
		jbw_float_entry_set_value(dlg->entry_final_mass,final_mass);
	}
	while (gtk_events_pending()) gtk_main_iteration();
	#if DEBUG_LEVEL & DEBUG_DRAW
		fprintf(stderr,"Draw: end\n");
	#endif
}

#endif
