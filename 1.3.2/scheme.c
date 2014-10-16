#define _GNU_SOURCE
#include <omp.h>
#include "jb/jb_win.h"
#include "wave.h"
#include "draw.h"
#include "scheme.h"

#define SEED 7

int scheme_type=0,flux_limiter_type=0,entropy_type=0,mesh_type=0,
	discretization_type=1,boundary_type=0,convergence=0;

int n,nmax,nc;
JBDOUBLE t,dt,tf,tmax;
JBDOUBLE cfl,o,pv,dxmax=0.,initial_mass,final_mass;
Parameters *p=NULL,*p2;
int fix_in,fix_out;
int scheme_order,scheme_steps;
int simulating;

#if JBW != JBW_NO
	int mesh_cells=0,dialog_convergence=0,max_cells=0,convergence_number=0;
	JBDOUBLE simulation_time=0.,cfl_number=0.,implicit=0.,viscosity=0.;
	extern void DialogSimulatorActualize();
#endif

JBDOUBLE (*limiter)(JBDOUBLE,JBDOUBLE);
JBDOUBLE (*entropy_correction_viscosity)(Parameters*);
void (*viscosity_artificial)(Parameters*);

void (*parameters_cell)(Parameters*);
void (*waves)(Parameters*);
void (*wavesb)(Parameters*);
void (*wavesc)(Parameters*);
void (*waves2)(Parameters*);
void (*waves2b)(Parameters*);
void (*waves2c)(Parameters*);
void (*solve)(Parameters*);
void (*step_internal)(Parameters*,void(*)(Parameters*),void(*)(Parameters*),
	void(*)(Parameters*));
void (*step)();

void variables_destroy()
{
	#if DEBUG_VARIABLES_DESTROY
		fprintf(stderr,"variables_destroy: start\n");
	#endif
	jb_free_null((void**)&p);
	#if DEBUG_VARIABLES_DESTROY
		fprintf(stderr,"variables_destroy: end\n");
	#endif
}

size_t variables_start()
{
	register int i,j;
	#if DEBUG_VARIABLES_START
		fprintf(stderr,"variables_start: start\n");
	#endif

	variables_destroy();

	i=n+1;
	i*=scheme_steps;
	p=(Parameters*)g_malloc(i*sizeof(Parameters));
	if (!p) jbw_show_error(gettext("No enough memory to start the variables"));
	while (--i>=0) for (j=4; --j>=0;) p[i].BB[j] = p[i].AA[j] = 0.;
	#if DEBUG_VARIABLES_START
		fprintf(stderr,"variables_start: end\n");
	#endif
	return (size_t)p;
}

void initial_mesh()
{
	register int i;
	register JBDOUBLE k,k2,k3,k4;
	GRand *r;
	#if DEBUG_MESH_START
		fprintf(stderr,"mesh_start: start\n");
	#endif
	switch (mesh_type)
	{
	case 0:
		for (i=0; i<=n; ++i)
			p[i].x = i * domain_width / n;
		break;
	case 1:
		for (i=0; i<=n; ++i)
			p[i].x = (i + 0.5L) * domain_width / (n + 1);
		break;
	default:
		r=g_rand_new_with_seed(SEED);
		p[0].x = 0.L;
		for (i=0; ++i<=n;)
			p[i].x = p[i-1].x + 1.L + g_rand_double(r) * dxmax;
		k = domain_width / p[n].x;
		for (i=0; ++i<=n;) p[i].x *= k;
	}
	for (i=0; i<=n; ++i)
	{
		p[i].u = y(p[i].x);
		#if DEBUG_MESH_START
			fprintf(stderr,"MS i=%d x="FWL" u="FWL"\n",i,p[i].x,p[i].u);
		#endif
	}
	for (i=n; --i>=0;) p[i].ix = p[i+1].x - p[i].x;
	p[n].dx = 0.L;
	for (i=n; --i>=0;)
	{
		k = 0.5L * p[i].ix;
		p[i].dx = k;
		p[i+1].dx += k;
	}
	if (mesh_type==1)
	{
		p[0].dx += p[0].x;
		p[0].x = 0.L;
		p[n].dx += domain_width - p[n].x;
		p[n].x = domain_width;
	}
	switch (scheme_order)
	{
	case 3:
		for (i=n+1; --i>=0;) p[i].dxx = 0.;
		k = 0.5 * p[0].ix;
		p[0].dxx += k;
		p[1].dxx += k;
		for (i=0; ++i<(n-1);)
		{
			k = p[i].ix;
			k2 = k + p[i-1].ix;
			k3 = k + p[i+1].ix;
			k4 = k2 + p[i+1].ix;
			k /= 12.;
			p[i-1].dxx -= k * (k3 + p[i+1].ix) / (p[i-1].ix * k2 * k4);
			p[i].dxx += k * (p[i].ix * (k3 + p[i+1].ix) + 2. * p[i-1].ix *
				(2. * k3 + p[i+1].ix)) / (p[i-1].ix * k3);
			p[i+1].dxx += k * (p[i].ix * (k2 + p[i-1].ix) + 2. * p[i+1].ix *
				(2. * k2 + p[i-1].ix)) / (p[i+1].ix * k2);
			p[i+2].dxx -= k * (k2 + p[i-1].ix) / (p[i+1].ix * k3 * k4);
		}
		k = 0.5 * p[i].ix;
		p[i].dxx += k;
		p[n].dxx += k;
		break;
	default:
		for (i=n+1; --i>=0;) p[i].dxx = p[i].dx;
	}
	#if DEBUG_MESH_START
		fprintf(stderr,"mesh_start: end\n");
	#endif
}

void initial_conditions()
{
	register int i;
	#if DEBUG_INITIAL_CONDITIONS
		fprintf(stderr,"initial conditions: start\n");
	#endif
	t=0.L;
	initial_wave();
	#if DEBUG_INITIAL_CONDITIONS
		fprintf(stderr,"IC n=%d\n",n);
	#endif
	initial_mesh();
	switch (scheme_steps)
	{
	case 2:
		for (i=n+1, p2=p+i; --i>=0;)
		{
			p2[i].x = p[i].x;
			p2[i].dx = p[i].dx;
			p2[i].ix = p[i].ix;
			p2[i].dxx = p[i].dxx;
		}
		break;
	}
	#if DEBUG_INITIAL_CONDITIONS
		fprintf(stderr,"Initial conditions open\n");
	#endif
}

static inline void parameters_cell_base_finite_volumes(Parameters *p)
{
	#if DEBUG_PARAMETERS_CELL_BASE_FINITE_VOLUMES
		fprintf(stderr,"parameters_cell_base_finite_volumes: start\n");
	#endif
	p->au = (p+1)->u - p->u;
	p->a = 0.5 * ((p+1)->c + p->c);
	p->du = p->au;
	p->dF = p->a * p->du;
	p->dF2 = 0.5 * p->a * p->dF / p->ix;
	p->au *= p->ix;
	#if DEBUG_PARAMETERS_CELL_BASE_FINITE_VOLUMES
		fprintf(stderr,"PCBFV a="FWL" du="FWL" dF="FWL"\n",p->a,p->du,p->dF);
		fprintf(stderr,"parameters_cell_base_finite_volumes: end\n");
	#endif	
}

static inline void parameters_cell_base_finite_differences(Parameters *p)
{
	#if DEBUG_PARAMETERS_CELL_BASE_FINITE_DIFFERENCES
		fprintf(stderr,"parameters_cell_base_finite_differences: start\n");
	#endif
	p->au = (p+1)->u - p->u;
	p->a = 0.5 * ((p+1)->c + p->c);
	p->du = p->au / p->ix;
	p->dF = p->a * p->du;
	p->dF2 = 0.5 * p->a * p->dF;
	#if DEBUG_PARAMETERS_CELL_BASE_FINITE_DIFFERENCES
		fprintf(stderr,"PCBFD a="FWL" du="FWL" dF="FWL"\n",p->a,p->du,p->dF);
		fprintf(stderr,"parameters_cell_base_finite_differences: end\n");
	#endif	
}

void parameters_cell_TVD_finite_volumes(Parameters *p)
{
	#if DEBUG_PARAMETERS_CELL_TVD_FINITE_VOLUMES
		fprintf(stderr,"parameters_cell_TVD_finite_volumes: start\n");
	#endif	
	parameters_cell_base_finite_volumes(p);
	if (p->a > 0.) p->Lp = p->dF / p->ix, p->Lm = 0.;
	else p->Lm = p->dF / p->ix, p->Lp = 0.;
	#if DEBUG_PARAMETERS_CELL_TVD_FINITE_VOLUMES
		fprintf(stderr,"PCTVDFV Lp="FWL" Lm="FWL"\n",p->Lp,p->Lm);
		fprintf(stderr,"parameters_cell_TVD_finite_volumes: end\n");
	#endif
}

void parameters_cell_TVD_finite_differences(Parameters *p)
{
	#if DEBUG_PARAMETERS_CELL_TVD_FINITE_DIFFERENCES
		fprintf(stderr,"parameters_cell_TVD_finite_differences: start\n");
	#endif	
	parameters_cell_base_finite_differences(p);
	if (p->a > 0.) p->Lp = p->dF, p->Lm = 0.;
	else p->Lm = p->dF, p->Lp = 0.;
	#if DEBUG_PARAMETERS_CELL_TVD_FINITE_DIFFERENCES
		fprintf(stderr,"PCTVDFD Lp="FWL" Lm="FWL"\n",p->Lp,p->Lm);
		fprintf(stderr,"parameters_cell_TVD_finite_differences: end\n");
	#endif	
}

void parameters_cell_TVD2_finite_volumes(Parameters *p)
{
	#if DEBUG_PARAMETERS_CELL_TVD2_FINITE_VOLUMES
		fprintf(stderr,"parameters_cell_TVD2_finite_volumes: start\n");
	#endif	
	parameters_cell_base_finite_volumes(p);
	if (p->a > 0.)
	{
		p->Lp = p->dF / p->ix;
		p->L2p = p->dF2;
		p->L2m = p->Lm = 0.;
	}
	else
	{
		p->Lm = p->dF / p->ix;
		p->L2m = p->dF2;
		p->L2p = p->Lp = 0.;
	}
	#if DEBUG_PARAMETERS_CELL_TVD2_FINITE_VOLUMES
		fprintf(stderr,"PCTVD2FV Lp="FWL" Lm="FWL"\n",p->Lp,p->Lm);
		fprintf(stderr,"PCTVD2FV L2p="FWL" L2m="FWL"\n",p->L2p,p->L2m);
		fprintf(stderr,"parameters_cell_TVD2_finite_volumes: end\n");
	#endif
}

void parameters_cell_TVD2_finite_differences(Parameters *p)
{
	#if DEBUG_PARAMETERS_CELL_TVD2_FINITE_DIFFERENCES
		fprintf(stderr,"parameters_cell_TVD2_finite_differences: start\n");
	#endif	
	parameters_cell_base_finite_differences(p);
	if (p->a > 0.)
	{
		p->Lp = p->dF;
		p->L2p = p->dF2;
		p->L2m = p->Lm = 0.;
	}
	else
	{
		p->Lm = p->dF;
		p->L2m = p->dF2;
		p->L2p = p->Lp = 0.;
	}
	#if DEBUG_PARAMETERS_CELL_TVD2_FINITE_DIFFERENCES
		fprintf(stderr,"PCTVD2FD Lp="FWL" Lm="FWL"\n",p->Lp,p->Lm);
		fprintf(stderr,"PCTVD2FD L2p="FWL" L2m="FWL"\n",p->L2p,p->L2m);
		fprintf(stderr,"parameters_cell_TVD2_finite_differences: end\n");
	#endif	
}

JBDOUBLE parameters_all(Parameters *p)
{
	register int i;
	JBDOUBLE k;
	#if DEBUG_PARAMETERS_ALL
		fprintf(stderr,"parameters_all: start\n");
	#endif
	p[n].c = propagation(p[n].u);
	k=cfl/tmax;
#pragma omp parallel for default(none) \
shared(p,n,discretization_type,k,propagation,parameters_cell,stderr) private(i)
	for (i=n-1; i>=0; --i)
	{
		p[i].c = propagation(p[i].u);
		parameters_cell(p+i);
		if (p[i].a < 0.) k = fmaxl(k, -p[i].a / p[i].dx);
		else k = fmaxl(k, p[i].a / p[i+1].dx);
		#if DEBUG_PARAMETERS_ALL
			fprintf(stderr,"PA i=%d k="FWL" ",i,k);
		#endif
	}
	if (k>0.) k=cfl/k; else k=tmax-t;
	#if DEBUG_PARAMETERS_ALL
		fprintf(stderr,"PA dt="FWL" ",k);
		fprintf(stderr,"parameters_all: end\n");
	#endif
	return k;
}

JBDOUBLE entropy_correction_viscosity_no(Parameters *p)
{
	return 0.;
}

JBDOUBLE entropy_correction_viscosity_Harten(Parameters *p)
{
	return 0.5 * ((p+1)->c - p->c);
}

JBDOUBLE entropy_correction_viscosity_mean(Parameters *p)
{
	return 0.5 * fminl((p+1)->c, -p->c);
}

JBDOUBLE entropy_correction_viscosity_mean_mean(Parameters *p)
{
	return 0.25 * ((p+1)->c - p->c - fabs((p+1)->c + p->c));
}

JBDOUBLE entropy_correction_viscosity_interpolated(Parameters *p)
{
	register JBDOUBLE k,kp,km;
	if (p->a > 0.) kp = p->a, km = 0.; else km = p->a, kp = 0.;
	k = 1.L + ((p+1)->c - p->c) * dt / p->ix;
	k = fmaxl((p+1)->c / k - kp, km - p->c / k);
	return k;
}

JBDOUBLE entropy_correction_viscosity_interpolated_mean(Parameters *p)
{
	register JBDOUBLE k,kp,km;
	if (p->a > 0.L) kp = p->a, km = 0.L; else km = p->a, kp = 0.L;
	k = 1.L + ((p+1)->c - p->c) * dt / p->ix;
	k = 0.5L * ((p+1)->c / k - kp + km - p->c / k);
	return k;
}

void entropy_correction(Parameters *p)
{
	register JBDOUBLE k;
	#if DEBUG_ENTROPY_CORRECTION
		fprintf(stderr,"entropy_correction: start\n");
	#endif
	if (p->c < 0.L && (p+1)->c > 0.L)
	{
		k = dt * p->du * entropy_correction_viscosity(p);
		(p+1)->iu -= k;
		p->iu += k;
		#if DEBUG_ENTROPY_CORRECTION
			fprintf(stderr,"EC k="FWL"\n",k);
		#endif
	} 
	#if DEBUG_ENTROPY_CORRECTION
		fprintf(stderr,"entropy_correction: end\n");
	#endif
}

void entropy_correction_implicit(Parameters *p)
{
	register JBDOUBLE k;
	#if DEBUG_ENTROPY_CORRECTION_IMPLICIT
		fprintf(stderr,"entropy_correction_implicit: start\n");
	#endif
	if (p->c < 0.L && (p+1)->c > 0.L)
	{
		k = dt * p->du * entropy_correction_viscosity(p);
		(p+1)->iup -= k;
		p->ium += k;
		#if DEBUG_ENTROPY_CORRECTION_IMPLICIT
			fprintf(stderr,"ECI k="FWL"\n",k);
		#endif
	} 
	#if DEBUG_ENTROPY_CORRECTION_IMPLICIT
		fprintf(stderr,"entropy_correction_implicit: end\n");
	#endif
}

void viscosity_LaxFriedrichs(Parameters *p)
{
	register JBDOUBLE k;
	k = 0.5 * pv * p->au;
	(p+1)->iu -= k;
	p->iu += k;
}

void viscosity_LaxFriedrichs_optimized(Parameters *p)
{
	register JBDOUBLE k;
	k = 0.5 * dt * fmaxl(fabsl(p->c), fabsl((p+1)->c)) * p->au / p->ix;
	(p+1)->iu -= k;
	p->iu += k;
}

void waves_centered(Parameters *p)
{
	p->AA[1] = p->AA[2] = 0.5;
}

void waves_right(Parameters *p)
{
	p->AA[2] = 1.;
}

void waves_left(Parameters *p)
{
	p->AA[1] = 1.;
}

void waves_upwind(Parameters *p)
{
	if (p->a > 0.) p->AA[1] = 1., p->AA[2] = 0.;
	else p->AA[2] = 1., p->AA[1] = 0.;
}

void waves_LaxWendroff(Parameters *p)
{
	p->AA[1] = p->AA[2] = 0.5;
	p->BB[2] = 1.;
	p->BB[1] = -1.;
}

void waves_WarmingBeam(Parameters *p)
{
	if (p->a > 0.)
	{
		p->AA[0] = -0.5;
		p->AA[1] = 1.5;
		p->BB[1] = 1.;
		p->BB[0] = -1.;
		p->AA[2] = p->AA[3] = p->BB[2] = p->BB[3] = 0.;
	}
	else
	{
		p->AA[3] = -0.5;
		p->AA[2] = 1.5;
		p->BB[3] = 1.;
		p->BB[2] = -1.;
		p->AA[0] = p->AA[1] = p->BB[0] = p->BB[1] = 0.;
	}
}

void waves_upwind2(Parameters *p)
{
	if (p->a > 0.)
	{
		p->AA[0] = -0.5 * (p+1)->ix / p->ix;
		p->AA[1] = 1. - p->AA[0];
		p->BB[1] = 1.;
		p->BB[0] = -1.;
		p->AA[2] = p->AA[3] = p->BB[2] = p->BB[3] = 0.;
	}
	else
	{
		p->AA[3] = -0.5 * (p-1)->ix / p->ix;
		p->AA[2] = 1. - p->AA[3];
		p->BB[3] = 1.;
		p->BB[2] = -1.;
		p->AA[0] = p->AA[1] = p->BB[0] = p->BB[1] = 0.;
	}
}

void waves_TVD_classical(Parameters *p)
{
	if (p->a > 0.)
	{
		p->AA[0] = -0.5 * limiter((p+1)->Lp, p->Lp);
		p->AA[1] = 1. - p->AA[0];
		p->AA[2] = p->AA[3] = 0.;
	}
	else
	{
		p->AA[3] = -0.5 * limiter((p-1)->Lm, p->Lm);
		p->AA[2] = 1. - p->AA[3];
		p->AA[0] = p->AA[1] = 0.;
	}
}

void waves_TVD(Parameters *p)
{
	if (p->a > 0.)
	{
		p->AA[0] = -0.5 * limiter((p+1)->Lp, p->Lp) * (p+1)->ix / p->ix;
		p->AA[1] = 1. - p->AA[0];
		p->AA[2] = p->AA[3] = 0.;
	}
	else
	{
		p->AA[3] = -0.5 * limiter((p-1)->Lm, p->Lm) * (p-1)->ix / p->ix;
		p->AA[2] = 1. - p->AA[3];
		p->AA[0] = p->AA[1] = 0.;
	}
}

void waves_TVD_RoeSweby(Parameters *p)
{
	register JBDOUBLE k;
	if (p->a > 0.)
	{
		k = (p+1)->Lp - 2 * dt * (p+1)->L2p / (p+1)->ix;
		k = limiter(k, p->Lp - 2 * dt * p->L2p / p->ix);
		p->BB[1] = k;
		p->BB[0] = -k;
		k *= -0.5;
		p->AA[0] = k;
		p->AA[1] = 1. - k;
		p->AA[2] = p->AA[3] = p->BB[2] = p->BB[3] = 0.;
	}
	else
	{
		k = (p-1)->Lm + 2 * dt * (p-1)->L2m / (p-1)->ix;
		k = limiter(k, p->Lm + 2 * dt * p->L2m / p->ix);
		p->BB[3] = k;
		p->BB[2] = -k;
		k *= -0.5;
		p->AA[3] = k;
		p->AA[2] = 1. - k;
		p->AA[0] = p->AA[1] = p->BB[0] = p->BB[1] = 0.;
	}
}

void waves_TVD2(Parameters *p)
{
	register JBDOUBLE k;
	if (p->a > 0.)
	{
		k = (p+1)->Lp - 2 * dt * (p+1)->L2p / (p+1)->ix;
		k = limiter(k, p->Lp - 2 * dt * p->L2p / (p+1)->ix);
		p->BB[1] = k;
		p->BB[0] = -k;
		k *= -0.5 * (p+1)->ix / p->ix;
		p->AA[0] = k;
		p->AA[1] = 1. - k;
		p->AA[2] = p->AA[3] = p->BB[2] = p->BB[3] = 0.;
	}
	else
	{
		k = (p-1)->Lm + 2 * dt * (p-1)->L2m / (p-1)->ix;
		k = limiter(k, p->Lm + 2 * dt * p->L2m / (p-1)->ix);
		p->BB[3] = k;
		p->BB[2] = -k;
		k *= -0.5 * (p-1)->ix / p->ix;
		p->AA[3] = k;
		p->AA[2] = 1. - k;
		p->AA[0] = p->AA[1] = p->BB[0] = p->BB[1] = 0.;
	}
}

void waves_upwind_implicit(Parameters *p)
{
	register JBDOUBLE k;
	k = dt * p->dF;
	if (p->a > 0) (p+1)->iup -= k; else p->ium -= k;
}

void waves_right_implicit(Parameters *p)
{
	p->ium -= dt * p->dF;
}

void waves_left_implicit(Parameters *p)
{
	(p+1)->iup -= dt * p->dF;
}

void semi_explicit_parameters(Parameters *p)
{
	int i;
	JBDOUBLE k;
	#if DEBUG_SEMI_EXPLICIT_PARAMETERS
		fprintf(stderr,"semi_explicit_parameters: start\n");
	#endif
	if (discretization_type)
	{
		p[0].Ep = k = fmaxl(p[0].c * dt / p[0].dxx - 1.L, 0.L);
		for (i=0; ++i<=n;)
		{
			k = fmaxl(fmaxl(k * p[i-1].dxx / p[i].dxx, p[i-1].a * dt / p[i].dxx)
				- 1.L, 0.L);
			p[i].Ep = k;
			#if DEBUG_SEMI_EXPLICIT_PARAMETERS
				fprintf(stderr,"SEP i=%d Ep="FWL"\n",i,k);
			#endif
		}
		i=n;
		p[i].Em = k = fminl(p[i].c * dt / p[i].dxx + 1.L, 0.L);
		while (--i>=0)
		{
			k = fminl(fminl(k * p[i+1].dxx / p[i].dxx, p[i].a * dt / p[i].dxx)
				+ 1.L, 0.L);
			p[i].Em = k;
			#if DEBUG_SEMI_EXPLICIT_PARAMETERS
				fprintf(stderr,"SEP i=%d Em="FWL"\n",i,k);
			#endif
		}
	}
	else
	{
		p[0].Ep = k = fmaxl(p[0].c * dt / p[0].ix - 1.L, 0.L);
		for (i=0; ++i<=n;)
		{
			k = fmaxl(fmaxl(k, p[i-1].a * dt / p[i-1].ix) - 1.L, 0.L);
			p[i].Ep = k;
			#if DEBUG_SEMI_EXPLICIT_PARAMETERS
				fprintf(stderr,"SEP i=%d Ep="FWL"\n",i,k);
			#endif
		}
		i=n;
		p[i].Em = k = fminl(p[i].c * dt / p[i-1].ix + 1.L, 0.L);
		while (--i>=0)
		{
			k = fminl(fminl(k, p[i].a * dt / p[i].ix) + 1.L, 0.L);
			p[i].Em = k;
			#if DEBUG_SEMI_EXPLICIT_PARAMETERS
				fprintf(stderr,"SEP i=%d Em="FWL"\n",i,k);
			#endif
		}
	}
	#if DEBUG_SEMI_EXPLICIT_PARAMETERS
		fprintf(stderr,"semi_explicit_parameters: end\n");
	#endif
}

void semi_explicit_waves(Parameters *p)
{
	p->Dp = 1.L + p->Ep;
	p->C = -p->Ep;
	p->Dm = 1.L + p->Em;
	p->E = -p->Em;
}

void implicit_waves_upwind(Parameters *p)
{
	register JBDOUBLE k;
	k = o * p->c * dt / p->dxx;
	p->Dp = p->Dm = 1.L;
	if (k<0.L)
	{
		p->C = 0.L;
		p->E = k;
		p->Dm -= k;
	}
	else
	{
		p->E = 0.L,
		p->C = -k;
		p->Dp += k;
	}
}

void implicit_waves_upwind_optimized(Parameters *p)
{
	register JBDOUBLE k;
	k = p->c * dt / p->dxx;
	p->Dp = p->Dm = 1.L;
	if (k<0.L)
	{
		p->C = 0.L;
		k *= fmaxl(0.L, 1.L + 1.L / k);
		p->E = k;
		p->Dm -= k;
	}
	else
	{
		p->E = 0.L,
		k *= fmaxl(0.L, 1.L - 1.L / k);
		p->C = -k;
		p->Dp += k;
	}
}

void implicit_waves_right(Parameters *p)
{
	register JBDOUBLE k;
	k = o * fabs(p->c) * dt / p->dxx;
	p->Dp = p->Dm = 1.L;
	p->C = 0.L;
	p->E = -k;
	p->Dm += k;
}

void implicit_waves_left(Parameters *p)
{
	register JBDOUBLE k;
	k = o * fabs(p->c) * dt / p->dxx;
	p->Dp = p->Dm = 1.L;
	p->E = 0.L;
	p->C = -k;
	p->Dp += k;
}

void scheme_explicit(Parameters *p)
{
	register JBDOUBLE k,k2;
	viscosity_artificial(p);
	k = dt * p->dF;
	k2 = dt * dt * p->dF2;
	(p-1)->iu += p->BB[3] * k2 - p->AA[3] * k;
	p->iu += p->BB[2] * k2 - p->AA[2] * k;
	(p+1)->iu += p->BB[1] * k2 - p->AA[1] * k;
	(p+2)->iu += p->BB[0] * k2 - p->AA[0] * k;
}

void scheme_explicit_inlet(Parameters *p)
{
	register JBDOUBLE k,k2;
	viscosity_artificial(p);
	k = dt * p->dF;
	k2 = dt * dt * p->dF2;
	p->iu += p->BB[2] * k2 - p->AA[2] * k;
	(p+1)->iu += p->BB[1] * k2 - p->AA[1] * k;
	(p+2)->iu += p->BB[0] * k2 - p->AA[0] * k;
}

void scheme_explicit_outlet(Parameters *p)
{
	register JBDOUBLE k,k2;
	viscosity_artificial(p);
	k = dt * p->dF;
	k2 = dt * dt * p->dF2;
	(p-1)->iu += p->BB[3] * k2 - p->AA[3] * k;
	p->iu += p->BB[2] * k2 - p->AA[2] * k;
	(p+1)->iu += p->BB[1] * k2 - p->AA[1] * k;
}

JBDOUBLE inlet(JBDOUBLE u)
{
	register JBDOUBLE k,h;
	#if DEBUG_INLET
		fprintf(stderr,"inlet: start\n");
	#endif
	h = y(p[0].x);
	k=propagation(h);
	#if DEBUG_INLET
		fprintf(stderr,"I u="FWL" h="FWL" k="FWL" c="FWL"\n",u,h,k,p[0].c);
	#endif
	if (k >= 0.L || p[0].c >= 0.L)
	{
		fix_in=1;
		#if DEBUG_INPUT
			fprintf(stderr,"I fix_in=1\n");
			fprintf(stderr,"inlet: end\n");
		#endif
		if (!boundary_type) return h-u;
		return p[0].iu + (yy0(t) - yy0(t-dt) - p[0].u * p[0].c * dt) / p[0].dx;
	}
	fix_in=0;
	#if DEBUG_INLET
		fprintf(stderr,"I fix_in=0\n");
		fprintf(stderr,"inlet: end\n");
	#endif
	return 0.;
}

JBDOUBLE outlet(JBDOUBLE u)
{
	register JBDOUBLE k,h;
	#if DEBUG_OUTLET
		fprintf(stderr,"outlet: start\n");
	#endif
	h = y(p[n].x);
	k=propagation(h);
	if (k <= 0.L || p[n].c <= 0.L)
	{
		fix_out=1;	
		#if DEBUG_OUTLET
			fprintf(stderr,"O fix_out=1\n");
			fprintf(stderr,"outlet: end\n");
		#endif
		return h-u;
	}
	fix_out=0;
	#if DEBUG_OUTLET
		fprintf(stderr,"O fix_out=0\n");
		fprintf(stderr,"outlet: end\n");
	#endif
	return 0.;
}

void solve_bidiagonal(Parameters *p)
{
	int i;
	#if DEBUG_SOLVE_BIDIAGONAL
		fprintf(stderr,"solve_bidiagonal: start\n");
		for (i=0; i<=n; ++i)
		{
			fprintf(stderr,"SB i=%d iup="FWL" ium="FWL"\n",
				i,p[i].iup,p[i].ium);
			fprintf(stderr,"SB Dp="FWL" Dm="FWL"\n",p[i].Dp,p[i].Dm);
			fprintf(stderr,"SB C="FWL" E="FWL"\n",p[i].C,p[i].E);
		}
	#endif
	for (i=0; ++i<=n;) p[i].iup = (p[i].iup - p[i-1].C * p[i-1].iup) / p[i].Dp;
	for (i=n; --i>=0;) p[i].ium = (p[i].ium - p[i+1].E * p[i+1].ium) / p[i].Dm;
#pragma omp parallel for default(none) shared(p,n) private(i)
	for (i=n; i>=0; --i) p[i].iu = p[i].iup + p[i].ium;
	#if DEBUG_SOLVE_BIDIAGONAL
		for (i=n+1; --i>=0;)
			fprintf(stderr,"SB i=%d iu="FWL" iup="FWL" ium="FWL"\n",
				i,p[i].iu,p[i].iup,p[i].ium);
		fprintf(stderr,"solve_bidiagonal: end\n");
	#endif
}

void solve_tridiagonal(Parameters *p)
{
	register int i;
	#if DEBUG_SOLVE_TRIDIAGONAL
		fprintf(stderr,"solve_tridiagonal: start\n");
	#endif
#pragma omp parallel for default(none) shared(p,n) private(i)
	for (i=n; i>=0; --i)
	{
		p[i].iu = p[i].iup + p[i].ium;
		p[i].Dp += p[i].Dm - 1.L;
	}
	#if DEBUG_SOLVE_TRIDIAGONAL
		for (i=0; i<=n; ++i)
		{
			fprintf(stderr,"ST i=%d iup="FWL" ium="FWL"\n",
				i,p[i].iup,p[i].ium);
			fprintf(stderr,"ST Dp="FWL" Dm="FWL"\n",p[i].Dp,p[i].Dm);
			fprintf(stderr,"ST C="FWL" E="FWL"\n",p[i].C,p[i].E);
		}
	#endif
	jbm_varray_solve_tridiagonal
		(&p[0].C, &p[0].Dp, &p[0].E, &p[0].iu, sizeof(Parameters), n);
	#if DEBUG_SOLVE_TRIDIAGONAL
		for (i=n+1; --i>=0;)
			fprintf(stderr,"ST i=%d iu="FWL" iup="FWL" ium="FWL"\n",
				i,p[i].iu,p[i].iup,p[i].ium);
		fprintf(stderr,"solve_tridiagonal: end\n");
	#endif
}


void step_explicit(Parameters *p,void (*waves)(Parameters*),
	void (*waves_inlet)(Parameters*),void (*waves_outlet)(Parameters*))
{
	register int i;
	register JBDOUBLE k;
	#if DEBUG_STEP_EXPLICIT
		fprintf(stderr,"step_explicit: start\n");
	#endif
#pragma omp parallel for default(none) shared(p,n) private(i)
	for (i=n; i>=0; --i) p[i].iu = 0.;
	waves_outlet(p+n-1);
	scheme_explicit_outlet(p+n-1);
#pragma omp parallel for default(none) shared(p,n,dt,waves) private(i)
	for (i=n-2; i>0; --i)
	{
		waves(p+i);
		scheme_explicit(p+i);
	}
	waves_inlet(p);
	scheme_explicit_inlet(p);
	k = outlet(p[n].u);
	if (discretization_type) k *= p[n].dxx;
	if (fix_out) p[n].iu = k;
	k = inlet(p[0].u);
	if (discretization_type) k *= p[0].dxx;
	if (fix_in) p[0].iu = k;
	#if DEBUG_STEP_EXPLICIT
		for (i=0; i<=n; ++i) fprintf(stderr,"SE i=%d iu="FWL"\n",i,p[i].iu);
		fprintf(stderr,"step_explicit: end\n");
	#endif
}

void step_implicit(Parameters *p,void (*waves)(Parameters*),
	void (*implicit_waves)(Parameters*),void (*unused)(Parameters*))
{
	register int i;
	#if DEBUG_STEP_IMPLICIT
		fprintf(stderr,"step_implicit: start\n");
	#endif
#pragma omp parallel for default(none) shared(p,n) private(i)
	for (i=n; i>=0; --i) p[i].iup = p[i].ium = 0.L;
	#if DEBUG_STEP_IMPLICIT
		fprintf(stderr,"SI waves\n");
	#endif
	p[n].iu = outlet(p[n].u);
	implicit_waves(p+n);
#pragma omp parallel for default(none) \
shared(p,n,dt,waves,implicit_waves,viscosity_artificial) private(i)
	for (i=n-1; i>=0; --i)
	{
		implicit_waves(p+i);
		viscosity_artificial(p+i);
		waves(p+i);
	}
	p[0].iu = inlet(p[0].u);
	if (fix_in)
	{
		p[1].E = p[0].ium = 0.L;
		p[0].Dp = 1.L;
		p[0].iup = p[0].iu;
	}
	if (fix_out)
	{
		p[n-1].C = p[n].iup = 0.L;
		p[n].Dm = 1.L;
		p[n].ium = p[n].iu;
	}
	#if DEBUG_STEP_IMPLICIT
		for (i=0; i<=n; ++i)
		{
			fprintf(stderr,"SI i=%d iup="FWL" ium="FWL"\n",i,p[i].iup,p[i].ium);
			fprintf(stderr,"SI Dp="FWL" Dm="FWL"\n",p[i].Dp,p[i].Dm);
			fprintf(stderr,"SI C="FWL" E="FWL"\n",p[i].C,p[i].E);
		}
	#endif
	solve(p);
	#if DEBUG_STEP_IMPLICIT
		fprintf(stderr,"step_implicit: end\n");
	#endif
}

void step_semi_explicit(Parameters *p,void (*waves)(Parameters*),
	void (*implicit_waves)(Parameters*),void (*unused)(Parameters*))
{
	semi_explicit_parameters(p);
	step_implicit(p,waves,implicit_waves,unused);
}

void step1()
{
	register int i;
	#if DEBUG_STEP1
		fprintf(stderr,"step1: start\n");
	#endif
	dt=parameters_all(p);
	if (t+dt>tmax) dt=tmax-t; else tmax=t+dt;
	t=tmax;
	#if DEBUG_STEP1
		fprintf(stderr,"S1 t="FWL" dt="FWL" tmax="FWL"\n",t,dt,tmax);
	#endif
	y_adjust();
	step_internal(p,waves,wavesb,wavesc);
	if (discretization_type)
#pragma omp parallel for default(none) shared(p,n) private(i)
	   for (i=n; i>=0; --i) p[i].u += p[i].iu / p[i].dxx;
	else
#pragma omp parallel for default(none) shared(p,n) private(i)
		for (i=n; i>=0; --i) p[i].u += p[i].iu;
	#if DEBUG_STEP1
		for (i=0; i<=n; ++i)
			fprintf(stderr,"S1 i=%d u="FWL" iu="FWL"\n",i,p[i].u,p[i].iu);
		fprintf(stderr,"step1: end\n");
	#endif
}

void step2()
{
	register int i;
	#if DEBUG_STEP2
		fprintf(stderr,"step2: start\n");
	#endif
	dt=parameters_all(p);
	#if DEBUG_STEP2
		fprintf(stderr,"S2 parameters all started\n");
	#endif
	if (t+dt>tmax) dt=tmax-t; else tmax=t+dt;
	t=tmax;
	#if DEBUG_STEP2
		fprintf(stderr,"S2 t="FWL" dt="FWL" tmax="FWL"\n",t,dt,tmax);
	#endif
	y_adjust();
	#if DEBUG_STEP2
		fprintf(stderr,"S2 y adjusted\n");
	#endif
	step_internal(p,waves,wavesb,wavesc);
	#if DEBUG_STEP2
		fprintf(stderr,"S2 step1 started\n");
	#endif
	if (discretization_type)
#pragma omp parallel for default(none) shared(p,p2,n) private(i)
		for (i=n; i>=0; --i) p2[i].u = p[i].u + p[i].iu / p[i].dxx;
	else
#pragma omp parallel for default(none) shared(p,p2,n) private(i)
		for (i=n; i>=0; --i) p2[i].u = p[i].u + p[i].iu;
	#if DEBUG_STEP2
		fprintf(stderr,"S2 p2 started\n");
	#endif
	parameters_all(p2);
	#if DEBUG_STEP2
		fprintf(stderr,"S2 parameters2 started\n");
	#endif
	t+=dt;
	#if DEBUG_STEP2
		fprintf(stderr,"S2 t="FWL" dt="FWL" tmax="FWL"\n",t,dt,tmax);
	#endif
	y_adjust();
	#if DEBUG_STEP2
		fprintf(stderr,"S2 y2 adjusted\n");
	#endif
	step_internal(p2,waves2,waves2b,waves2c);
	#if DEBUG_STEP2
		fprintf(stderr,"S2 step2 started\n");
	#endif
	t=tmax;
	#if DEBUG_STEP2
		fprintf(stderr,"S2 t="FWL" dt="FWL" tmax="FWL"\n",t,dt,tmax);
	#endif
	y_adjust();
	if (discretization_type)
#pragma omp parallel for default(none) shared(p,p2,n) private(i)
		for (i=n; i>=0; --i)
		{
			p2[i].u += p2[i].iu / p2[i].dxx;
			p[i].u = 0.5 * (p2[i].u + p[i].u);
		}
	else
#pragma omp parallel for default(none) shared(p,p2,n) private(i)
		for (i=n; i>=0; --i)
		{
			p2[i].u += p2[i].iu;
			p[i].u = 0.5 * (p2[i].u + p[i].u);
		}
	#if DEBUG_STEP2
		for (i=0; i<=n; ++i)
			fprintf(stderr,"S2 i=%d u2="FWL" u="FWL"\n",i,p2[i].u,p[i].u);
		fprintf(stderr,"step2: end\n");
	#endif
}

void scheme_start()
{
	#if DEBUG_SCHEME_START
		fprintf(stderr,"scheme_start: start\n");
	#endif
	scheme_steps = scheme_order = 1;
	limiter = jbm_flux_limiter_select(flux_limiter_type);
	if (discretization_type)
		parameters_cell = parameters_cell_base_finite_volumes;
	else parameters_cell = parameters_cell_base_finite_differences;
	viscosity_artificial = entropy_correction;
	solve = solve_bidiagonal;
	if (scheme_type > 10)
	{
		waves = waves_upwind_implicit;
		wavesb = implicit_waves_upwind_optimized;
		step_internal = step_implicit;
	}
	else
	{
		waves = wavesb = wavesc = waves_upwind;
		step_internal = step_explicit;
	}
	step = step1;
	switch (scheme_type)
	{
	case 0:
		viscosity_artificial = viscosity_LaxFriedrichs;
		waves = wavesb = wavesc = waves_centered;
		break;
	case 1:
		viscosity_artificial = viscosity_LaxFriedrichs_optimized;
		waves = wavesb = wavesc = waves_centered;
		break;
	case 3:
		waves = wavesb = wavesc = waves_LaxWendroff;
		break;
	case 4:
		waves = waves_WarmingBeam;
		break;
	case 5:
		scheme_steps = 2;
		waves = wavesb = wavesc = waves_right;
		waves2 = waves2b = waves2c = waves_left;
		step = step2;
		break;
	case 6:
		waves = waves_upwind2;
		break;
	case 7:
		if (discretization_type)
			parameters_cell = parameters_cell_TVD_finite_volumes;
		else parameters_cell = parameters_cell_TVD_finite_differences;
		waves = waves_TVD_classical;
		break;
	case 8:
		if (discretization_type)
			parameters_cell = parameters_cell_TVD_finite_volumes;
		else parameters_cell = parameters_cell_TVD_finite_differences;
		waves = waves_TVD;
		break;
	case 9:
		if (discretization_type)
			parameters_cell = parameters_cell_TVD2_finite_volumes;
		else parameters_cell = parameters_cell_TVD2_finite_differences;
		waves = waves_TVD_RoeSweby;
		break;
	case 10:
		if (discretization_type)
			parameters_cell = parameters_cell_TVD2_finite_volumes;
		else parameters_cell = parameters_cell_TVD2_finite_differences;
		waves = waves_TVD2;
		break;
	case 11:
		wavesb = implicit_waves_upwind;
	case 12:
		solve = solve_tridiagonal;
		break;
	case 13:
		wavesb = implicit_waves_upwind;
		break;
	case 15:
		scheme_steps = 2;
		waves = waves_right_implicit;
		wavesb = implicit_waves_right;
		waves2 = waves_left_implicit;
		waves2b = implicit_waves_left;
		step = step2;
		break;
	case 16:
		solve = solve_tridiagonal;
	case 17:
		wavesb = semi_explicit_waves;
		step_internal = step_semi_explicit;
	}
	switch (entropy_type)
	{
	case 0:
		entropy_correction_viscosity = entropy_correction_viscosity_no;
		break;
	case 1:
		entropy_correction_viscosity = entropy_correction_viscosity_Harten;
		break;
	case 2:
		entropy_correction_viscosity = entropy_correction_viscosity_mean;
		break;
	case 3:
		entropy_correction_viscosity = entropy_correction_viscosity_mean_mean;
		break;
	case 4:
		entropy_correction_viscosity =
			entropy_correction_viscosity_interpolated;
		break;
	default:
		entropy_correction_viscosity =
			entropy_correction_viscosity_interpolated_mean;
	}
	#if DEBUG_SCHEME_START
		fprintf(stderr,"scheme_start: end\n");
	#endif
}

void simulate(FILE *file_error)
{
	register int i;
	register JBDOUBLE k,k2;
	int nsteps=0;
	FILE *file;
	#if DEBUG_SIMULATE
		fprintf(stderr,"simulate: start\n");
	#endif
	scheme_start();
	#if DEBUG_SIMULATE
		fprintf(stderr,"S scheme started\n");
	#endif
	if (!variables_start()) return;
	#if DEBUG_SIMULATE
		fprintf(stderr,"S variables started\n");
	#endif
	initial_conditions();
	#if DEBUG_SIMULATE
		fprintf(stderr,"S initial conditions started\n");
	#endif
	for (i=0, initial_mass=0.; i<=n; ++i) initial_mass += p[i].u * p[i].dx;
	while (t<tf && simulating)
	{
		tmax=tf;
		step();
		for (i=0, final_mass=0.; i<=n; ++i) final_mass += p[i].u * p[i].dx;
		++nsteps;
		if (!convergence) draw();
		#if JBW != JBW_NO
			while (gtk_events_pending()) gtk_main_iteration();
		#endif
	}
	draw();
	file=fopen("0","w");
	for (i=0, k=0.; i<=n; ++i)
	{
		k2=y(p[i].x);
		fprintf(file,FWL" "FWL" "FWL"\n",p[i].x,p[i].u,k2);
		k2-=p[i].u;
		k+=k2*k2;
	}
	fclose(file);
	if (convergence)
	{
		k=sqrtl(k/(n+1));
		fprintf(stderr,"Square error="FWL"\n",k);
		fprintf(file_error,"%d "FWL"\n",nsteps,k);
	}
	#if DEBUG_SIMULATE
		fprintf(stderr,"simulate: end\n");
	#endif
}

void execute(char *file_name)
{
	int i;
	JBDOUBLE r1,r2;
	FILE *file;
	#if DEBUG_EXECUTE
		fprintf(stderr,"execute: start\n");
	#endif
	simulating=1;
	#if JBW != JBW_NO
		DialogSimulatorActualize();
	#endif
	if (convergence)
	{
		r1=n;
		r2=powl(nmax/r1,1./(JBDOUBLE)nc);
		file=fopen(file_name,"w");
		for (i=0; i<=nc; ++i, r1*=r2)
		{
			n=(int)r1;
			fprintf(stderr,"Case %d/%d n=%d\n",i,nc,n);
			simulate(file);
			if (!simulating) break;
		}
		fclose(file);
	}
	else simulate(NULL);
	simulating=0;
	#if DEBUG_EXECUTE
		fprintf(stderr,"execute: end\n");
	#endif
}
