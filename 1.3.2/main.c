#include "scheme.h"

#if JBW == JBW_NO

#include <omp.h>
#include "wave.h"

int main(int argn,char **argc)
{
	FILE *file;
	printf("Maximum threads number=%d\n",omp_get_max_threads());
	printf("Processors number=%d\n",omp_get_num_procs());
	#if DEBUG_LEVEL && DEBUG_FILE
		stdout=fopen("log","w");
	#endif
	file=fopen(argc[1],"r");
	fscanf(file,"%d%d",&wave_type,&shape_initial);
	fscanf(file,FRL FRL FRL FRL,
		&shape_position,&shape_width,&shape_amplitude,&wave_velocity);
	fscanf(file,FRL,&domain_width);
	fscanf(file,"%d%d%d%d%d",&scheme_type,&flux_limiter_type,&entropy_type,
		&mesh_type,&discretization_type);
	fscanf(file,"%d",&n);
	fscanf(file,FRL FRL,&tf,&cfl);
	fscanf(file,FRL FRL FRL,&o,&pv,&dxmax);
	fscanf(file,"%d%d%d",&convergence,&nmax,&nc);
	fclose(file);
	#if DEBUG_LEVEL && DEBUG_SCHEME
		printf("Wave type=%d initial=%d\n",wave_type,shape_initial);
		printf("position="FWL" width="FWL" amplitude="FWL" velocity="FWL
		 "\n",shape_position,shape_width,shape_amplitude,wave_velocity);
		printf("Medium width="FWL"\n",domain_width);
		printf("Scheme type=%d flux_limiter=%d entropy=%d\n",
			scheme_type,flux_limiter_type,entropy_type);
		printf("n=%d\n",n);
		printf("t="FWL" cfl="FWL"\n",tf,cfl);
		printf("o="FWL" pv="FWL" dxmax="FWL"\n",o,pv,dxmax);
	#endif
	execute(argc[2]);
	return 0;
}

#else

#include <locale.h>
#include "def.h"
#include "dialog_wave.h"
#include "dialog_options.h"
#include "dialog_scheme.h"
#include "simulator.h"

JBFLOAT draw_xmin,draw_xmax,draw_ymin,draw_ymax;

GtkWindow *window_parent;

DialogSimulator dialog_simulator;

int main(int argn,char **argc)
{
	#if DEBUG_LEVEL && DEBUG_FILE
		stderr=fopen("log","w");
	#endif
	gtk_disable_setlocale();
	setlocale(LC_ALL,"");
	setlocale(LC_NUMERIC,"C");
	bindtextdomain("wave","./po");
	bind_textdomain_codeset("wave","UTF-8");
	textdomain("wave");
	jbw_graphic_init(&argn,&argc);
	DialogSimulatorCreate();
	glutIdleFunc((void(*))gtk_main_iteration);
	glutMainLoop();
	return 0;
}

#endif
