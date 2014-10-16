#define _GNU_SOURCE
#include <stdio.h>
#include "jb/jb_math.h"
#include "wave.h"

extern JBFLOAT t;

int wave_type,shape_initial;
JBFLOAT wave_velocity,shape_position,shape_width,shape_amplitude,shape_height,
	shape_base,domain_width;
JBFLOAT xa,xb,xc,ta0,tb0,tc0,taf,tbf,tcf;

JBDOUBLE (*propagation)(JBDOUBLE);
JBDOUBLE (*y)(JBDOUBLE), (*yy0)(JBDOUBLE), (*yyf)(JBDOUBLE);

JBDOUBLE propagation_linear(JBDOUBLE u)
{
	return wave_velocity;
}

JBDOUBLE propagation_Burgers(JBDOUBLE u)
{
	return u;
}

void y_adjust()
{
	register JBDOUBLE m,p,tt;
	#if DEBUG_Y_ADJUST
		fprintf(stderr,"y_adjust: start\n");
	#endif
	shape_height = shape_base + shape_amplitude;
	if (wave_type == 0)
	{
		m = 0.5 * shape_width;
		xb = shape_position + wave_velocity * t;
		xa=xb-m;
		xc=xb+m;
		if (wave_velocity > 0.)
		{
			tb0 = -shape_position / wave_velocity;
			ta0 = tb0 - m / wave_velocity;
			tc0 = tb0 + m / wave_velocity;
		}
		else
		{
			tb0 = (domain_width - shape_position) / wave_velocity;
			ta0 = tb0 + m / wave_velocity;
			tc0 = tb0 - m / wave_velocity;
		}
	}
	else if (shape_initial == 3)
	{
		xb = 0.5 * domain_width;
		xa = fmin(xb, shape_base * t);
		xc = domain_width - xa;
	}
	else
	{
		if (shape_amplitude > 0.) p=1.; else p=-1.;
		xa = shape_position + shape_base * t - 0.5 * p * shape_width;
		if (shape_initial == 0)
		{
			tt = p * jbm_fdbl(shape_width / shape_amplitude);
			if (t<tt)
			{
				xb = xa + shape_amplitude * t;
				xc = xb + p * shape_width - 0.5 * shape_amplitude * t;
			}
			else
			{
				m = sqrtl(1. + p * (t - tt) * shape_amplitude /
					jbm_fdbl(shape_width));
				xb = xc = xa +  p * shape_width * jbm_fdbl(m);
				shape_height = shape_base + shape_amplitude / m;
			}
		}
		else
		{
			tt = 0.5 * p * shape_width / shape_amplitude;
			if (t<tt)
			{
				xb = xa + 0.5 * p * shape_width + shape_amplitude * t;
				xc = xb + p * shape_width;
			}
			else
			{
				m = sqrtl(1. + p * (t - tt) * shape_amplitude / shape_width);
				xb = xc = xa +  p * shape_width * m;
				shape_height = shape_base + shape_amplitude / m;
			}
		}
	}
	#if DEBUG_Y_ADJUST
		fprintf(stderr,"YA xa="FWL" xb="FWL" xc="FWL"\n",xa,xb,xc);
		fprintf(stderr,"y_adjust: end\n");
	#endif
}

JBDOUBLE y_square(JBDOUBLE x)
{
	if (x<=xa || x>=xc) return shape_base;
	return shape_height;
}

JBDOUBLE yy_square(JBDOUBLE t)
{
	register JBDOUBLE k;
	if (t<=ta0) k = shape_base * t;
	else if (t<=tc0) k = shape_base * t + shape_amplitude * (t - ta0);
	else k = shape_base * t + shape_amplitude * (tc0 - ta0);
	return k * wave_velocity;
}

JBDOUBLE y_triangular(JBDOUBLE x)
{
	if (x<=xa || x>=xc) return shape_base;
	if (x<xb) return jbm_extrapolate(x,xa,xb,shape_base,shape_height);
	return jbm_extrapolate(x,xb,xc,shape_height,shape_base);
}

JBDOUBLE yy_triangular(JBDOUBLE t)
{
	register JBDOUBLE k;
	if (t<=ta0) k = shape_base * t;
	else if (t<=tb0) k = shape_base * t +
		0.5 * (t - ta0) * jbm_extrapolate(t,ta0,tb0,0,shape_amplitude);
	else if (t<tc0) k = shape_base * t + 0.5 * shape_amplitude * (tc0 - ta0) +
		0.5 * (t - tc0) * jbm_extrapolate(t,tb0,tc0,shape_amplitude,0);
	else k = shape_base * t + 0.5 * shape_width * shape_amplitude;
	return k * wave_velocity;
}

JBDOUBLE y_sinusoidal(JBDOUBLE x)
{
	if (x<=xa || x>=xc) return shape_base;
	return shape_base + shape_amplitude * sinl(M_PI * (x - xa) / shape_width);
}

JBDOUBLE yy_sinusoidal(JBDOUBLE t)
{
	register JBDOUBLE k;
	if (t<=ta0) k = shape_base * t;
	else if (t<tc0) k = shape_base * t + shape_amplitude * (tc0 - ta0) *
		(1. - cos(M_PI * (t - ta0)/ (tc0 - ta0))) / M_PI;
	else k = shape_base * t + 2. / M_PI * shape_amplitude * (tc0 - ta0);
	return k * wave_velocity;
}

JBDOUBLE y_Gaussian(JBDOUBLE x)
{
	return shape_base + shape_amplitude * expl(-jbm_fsqr((x - xb) / (xc - xb)));
}

/*
	int(0,t) exp(-x**2)dx = sqrt(pi) / 2 * erf(t) =>
	int(0,t) exp(-((t-t1)/a)**2)dt = a int(-t1/a, (t-t1)/a) exp(-u**2)du =
		sqrt(pi) * a / 2 * (erf((t-t1)/a) - erf(-t1/a))
*/
JBDOUBLE yy_Gaussian(JBDOUBLE t)
{
	return (shape_base * t + 0.5 * sqrtl(M_PI) * shape_amplitude * (tc0 - tb0) *
		(erfl((t - tb0) / (tc0 - tb0)) - erfl(-tb0 / (tc0 - tb0)))) *
		wave_velocity;
}

JBDOUBLE y_square_Burgers(JBDOUBLE x)
{
	if (shape_amplitude > 0.)
	{
		if (x<=xa || x>=xc) return shape_base;
		if (x<xb) return jbm_extrapolate(x,xa,xb,shape_base,shape_height);
		return shape_height;
	}
	if (x>=xa || x<=xc) return shape_base;
	if (x>xb) return jbm_extrapolate(x,xa,xb,shape_base,shape_height);
	return shape_height;	
}

JBDOUBLE yy0_square_Burgers(JBDOUBLE t)
{
	return 0.5 * jbm_fsqr(shape_base) * t;
}

JBDOUBLE yyf_square_Burgers(JBDOUBLE t)
{
	return 0.5 * jbm_fsqr(shape_base) * t;
}
JBDOUBLE y_triangular_Burgers(JBDOUBLE x)
{
	if (shape_amplitude > 0.)
	{
		if (x<=xa || x>=xc) return shape_base;
		if (xb<xc)
		{
			if (x<=xb)
				return jbm_extrapolate(x,xa,xb,shape_base,shape_height);
			return jbm_extrapolate(x,xb,xc,shape_height,shape_base);
		}
		return jbm_extrapolate(x,xa,xc,shape_base,shape_height);
	}
	if (x>=xa || x<=xc) return shape_base;
	if (xb>xc)
	{
		if (x<=xb)
			return jbm_extrapolate(x,xc,xb,shape_base,shape_height);
		return jbm_extrapolate(x,xb,xa,shape_height,shape_base);
	}
	return jbm_extrapolate(x,xa,xc,shape_base,shape_height);
}

JBDOUBLE yy0_triangular_Burgers(JBDOUBLE t)
{
	return 0.5 * jbm_fsqr(shape_base) * t;
}

JBDOUBLE yyf_triangular_Burgers(JBDOUBLE t)
{
	return 0.5 * jbm_fsqr(shape_base) * t;
}
JBDOUBLE y_steady_Burgers(JBDOUBLE x)
{
	if (shape_base < 0.) return 0.;
	if (x<=xa || x>=xc) return shape_base;
	return jbm_extrapolate(x,xa,xc,shape_base,-shape_base);
}

JBDOUBLE yy_steady_Burgers(JBDOUBLE t)
{
	if (shape_base < 0.) return 0.;
	return 0.5 * jbm_fsqr(shape_base) * t;
}

void initial_wave()
{
	#if DEBUG_INITIAL_WAVE
		printf("initial_wave: start\n");
	#endif
	y_adjust();
	switch (wave_type)
	{
	case 0:
		propagation = propagation_linear;
		switch (shape_initial)
		{
		case 0:
			y = y_square;
			yy0 = yyf = yy_square;
			break;
		case 1:
			y = y_triangular;
			yy0 = yyf = yy_triangular;
			break;
		case 2:
			y = y_sinusoidal;
			yy0 = yyf = yy_sinusoidal;
			break;
		default:
			y = y_Gaussian;
			yy0 = yyf = yy_Gaussian;
		}
		break;
	default:
		propagation = propagation_Burgers;
		switch (shape_initial)
		{
		case 0:
			y = y_square_Burgers;
			yy0 = yy0_square_Burgers;
			yyf = yyf_square_Burgers;
			break;
		case 1:
			y = y_triangular_Burgers;
			yy0 = yy0_triangular_Burgers;
			yyf = yyf_triangular_Burgers;
			break;
		default:
			y = y_steady_Burgers;
			yy0 = yyf = yy_steady_Burgers;
		}
	}
	#if DEBUG_INITIAL_WAVE
		printf("initial_wave: end\n");
	#endif
}
