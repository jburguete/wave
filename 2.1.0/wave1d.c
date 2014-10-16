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
 * \file wave1d.c
 * \brief Source file to define an 1D wave.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2010-2013.
 */
#include "def.h"
#include "jb/jb_xml.h"
#include "domain1d.h"
#include "shape1d.h"
#include "wave1d.h"

JBDOUBLE vx_1d_linear(JBDOUBLE u)
{
	return vx_linear;
}

JBDOUBLE vx_1d_Burgers(JBDOUBLE u)
{
	return u;
}

void adjust_1d_linear(Wave1D *wave)
{
	register JBDOUBLE k = 0.5 * wave->shape->w;
	wave->xb = wave->shape->x0 + vx_linear * t;
	wave->xa = wave->xb - k;
	wave->xc = wave->xb + k;
}

JBDOUBLE u_1d_linear_square(Wave1D *wave, JBDOUBLE x)
{
	if (x <= wave->xa || x >= wave->xc) return wave->shape->u0;
	return wave->shape->u0 + wave->shape->h;
}

JBDOUBLE mass_1d_linear_square(Wave1D *wave, JBDOUBLE x1, JBDOUBLE x2)
{
	register JBDOUBLE k1,k2;
	k1 = (x2 - x1) * wave->shape->u0;
	if (x2 <= wave->xa || x1 >= wave->xc) return k1;
	if (x1 > wave->xa)
	{
		if (x2 > wave->xc) k2 = wave->xc - x1; else k2 = x2 - x1;
	}
	else
	{
		if (x2 < wave->xc) k2 = x2 - wave->xa; else k2 = wave->shape->w;
	}
	return k1 + wave->shape->h * k2;
}

JBDOUBLE flux_1d_linear_square(Wave1D *wave, JBDOUBLE x)
{
	register JBDOUBLE k1, k2, ta, tc;
	k1 = x - wave->shape->x0;
	k2 = 0.5 * wave->shape->w;
	ta = (k1 + k2) / vx_linear;
	tc = (k1 - k2) / vx_linear;
	k1 = t * vx_linear * wave->shape->u0;
	if (vx_linear > 0.)
	{
		if (ta <= 0. || t <= tc) return k1;
		if (tc > 0.)
		{
			if (t < ta) k2 = vx_linear * (t - tc); else k2 = wave->shape->w;
		}
		else
		{
			if (t < ta) k2 = vx_linear * t; else k2 = vx_linear * ta;
		}
	}
	else
	{
		if (tc <= 0. || t <= ta) return k1;
		if (ta > 0.)
		{
			if (t < tc) k2 = vx_linear * (t - ta); else k2 = wave->shape->w;
		}
		else
		{
			if (t < tc) k2 = vx_linear * t; else k2 = vx_linear * tc;
		}
	}
	return k1 + wave->shape->h * k2;
}

JBDOUBLE u_1d_linear_triangular(Wave1D *wave, JBDOUBLE x)
{
	register JBDOUBLE k1, k2;
	k1 = wave->shape->u0;
	if (x <= wave->xa || x >= wave->xc) return k1;
	if (x <= wave->xb) k2 = x - wave->xa;
	else k2 = wave->xc - x;
	return k1 + 2 * wave->shape->h * k2 / wave->shape->w;
}

JBDOUBLE mass_1d_linear_triangular(Wave1D *wave, JBDOUBLE x1, JBDOUBLE x2)
{
	register JBDOUBLE k1, k2;
	k1 = (x2 - x1) * wave->shape->u0;
	if (x2 <= wave->xa || x1 >= wave->xc) return k1;
	if (x1 > wave->xb)
	{
		if (x2 >= wave->xc) k2 = jbm_fsqr(wave->xc - x1) / wave->shape->w;
		else k2 = (x1 - x2) * (x1 + x2 - 2 * wave->xc) / wave->shape->w;
	}
	else if (x1 > wave->xa)
	{
		if (x2 > wave->xc) k2 = 0.25 * wave->shape->w +
			(wave->xb - x1) * (wave->xb + x1 - 2 * wave->xa) / wave->shape->w;
		else if (x2 > wave->xb) k2 = ((wave->xb - x1) *
				(wave->xb + x1 - 2 * wave->xa) + (wave->xb - x2) *
				(x2 + wave->xb - 2 * wave->xc)) / wave->shape->w;
		else k2 = (x2 - x1) * (x1 + x2 - 2 * wave->xa) / wave->shape->w;
	}
	else
	{
		if (x2 >= wave->xc) k2 = 0.5 * wave->shape->w;
		else if (x2 > wave->xb) k2 = 0.25 * wave->shape->w +
			(wave->xb - x2) * (x2 + wave->xb - 2 * wave->xc) / wave->shape->w;
		else k2 = jbm_fsqr(x2 - wave->xa) / wave->shape->w;
	}
	return k1 + wave->shape->h * k2;
}

JBDOUBLE flux_1d_linear_triangular(Wave1D *wave, JBDOUBLE x)
{
	register JBDOUBLE k1, k2, ta, tb, tc;
	tb = (x - wave->shape->x0) / vx_linear;
	k2 = 0.5 * wave->shape->w / vx_linear;
	ta = tb + k2;
	tc = tb - k2;
	k1 = t * vx_linear * wave->shape->u0;
	if (vx_linear > 0.)
	{
		if (ta <= 0. || t <= tc) return k1;
		if (tc > 0.)
		{
			if (t >= ta) k2 = wave->shape->w;
			else if (t > tb) k2 = 0.5 * wave->shape->w +
				vx_linear * (t - tb) * (t + tb - 2 * ta) / (tb - ta);
			else k2 = vx_linear * jbm_fsqr(t - tc) / (tb - tc);
		}
		else if (tb > 0.)
		{
			if (t > ta) k2 = 0.5 * wave->shape->w +
				vx_linear * tb * (tb - 2 * tc) / (tb - tc);
			else if (t > tb) k2 = vx_linear * ((t - tb) * (t + tb - 2 * ta) 
				+ tb * (tb - 2 * tc)) / (tb - ta);
			else k2 = vx_linear * t * (t - 2 * tc) / (tb - tc);
		}
		else
		{
			if (t < ta) k2 = vx_linear * t * (t  - 2 * ta) / (tb - ta);
			else k2 = vx_linear * ta * ta / (ta - tb);
		}
	}
	else
	{
		if (tc <= 0. || t <= ta) return k1;
		if (ta > 0.)
		{
			if (t >= tc) k2 = wave->shape->w;
			else if (t > tb) k2 = 0.5 * wave->shape->w +
				vx_linear * (t - tb) * (t + tb - 2 * tc) / (tb - tc);
			else k2 = vx_linear * jbm_fsqr(t - ta) / (tb - ta);
		}
		else if (tb > 0.)
		{
			if (t > tc) k2 = 0.5 * wave->shape->w +
				vx_linear * tb * (tb - 2 * ta) / (tb - ta);
			else if (t > tb) k2 = vx_linear * ((t - tb) * (t + tb - 2 * tc) 
				+ tb * (tb - 2 * ta)) / (tb - tc);
			else k2 = vx_linear * t * (t - 2 * ta) / (tb - ta);
		}
		else
		{
			if (t < tc) k2 = vx_linear * t * (t  - 2 * tc) / (tb - tc);
			else k2 = vx_linear * tc * tc / (tc - tb);
		}
	}
	return k1 + 0.5 * wave->shape->h * k2;
}

JBDOUBLE u_1d_linear_sinusoidal(Wave1D *wave, JBDOUBLE x)
{
	if (x <= wave->xa || x >= wave->xc) return wave->shape->u0;
	return wave->shape->u0 +
		wave->shape->h * jbm_fsqr(cos(M_PI * (x - wave->xb) / wave->shape->w));
}

JBDOUBLE mass_1d_linear_sinusoidal(Wave1D *wave, JBDOUBLE x1, JBDOUBLE x2)
{
	register JBDOUBLE k1, k2;
	k1 = (x2 - x1) * wave->shape->u0;
	if (x2 <= wave->xa || x1 >= wave->xc) return k1;
	if (x1 > wave->xa)
	{
		if (x2 > wave->xc) k2 = wave->xc - x1 - wave->shape->w / (2 * M_PI) *
			sin(2 * M_PI * (x1 - wave->xa) / wave->shape->w);
		else k2 = x2 - x1 - wave->shape->w / (2 * M_PI) *
			(sin(2 * M_PI * (x2 -wave->xa) / wave->shape->w) -
			sin(2 * M_PI * (x1 - wave->xa) / wave->shape->w));
	}
	else
	{
		if (x2 < wave->xc) k2 = x2 - wave->xa - wave->shape->w / (2 * M_PI) *
			sin(2 * M_PI * (x2 -wave->xa) / wave->shape->w);
		else k2 = wave->shape->w;
	}
	return k1 + 0.5 * wave->shape->h * k2;
}

JBDOUBLE flux_1d_linear_sinusoidal(Wave1D *wave, JBDOUBLE x)
{
	register JBDOUBLE k1, k2, ta, tc;
	k1 = x - wave->shape->x0;
	k2 = 0.5 * wave->shape->w;
	ta = (k1 + k2) / vx_linear;
	tc = (k1 - k2) / vx_linear;
	k1 = t * vx_linear * wave->shape->u0;
	if (vx_linear > 0.)
	{
		if (ta <= 0. || t <= tc) return k1;
		if (tc > 0.)
		{
			if (t < ta) k2 = vx_linear * (t - tc) - wave->shape->w /
				(2 * M_PI) * sin (2 * M_PI * (x - wave->xa) / wave->shape->w);
			else k2 = wave->shape->w;
		}
		else
		{
			if (t < ta) k2 = vx_linear * t + wave->shape->w / (2 * M_PI) *
				(sin (2 * M_PI * (x - wave->shape->x0) / wave->shape->w) -
				sin (2 * M_PI * (x - wave->xa) / wave->shape->w));
			else k2 = vx_linear * ta + wave->shape->w / (2 * M_PI) *
				sin (2 * M_PI * (x - wave->shape->x0) / wave->shape->w);
		}
	}
	else
	{
		if (tc <= 0. || t <= ta) return k1;
		if (ta > 0.)
		{
			if (t < tc) k2 = vx_linear * (t - ta) - wave->shape->w /
				(2 * M_PI) * sin (2 * M_PI * (x - wave->xa) / wave->shape->w);
			else k2 = wave->shape->w;
		}
		else
		{
			if (t < tc) k2 = vx_linear * t + wave->shape->w / (2 * M_PI) *
				(sin (2 * M_PI * (x - wave->shape->x0) / wave->shape->w) -
				sin (2 * M_PI * (x - wave->xa) / wave->shape->w));
			else k2 = vx_linear * tc + wave->shape->w / (2 * M_PI) *
				sin (2 * M_PI * (x - wave->shape->x0) / wave->shape->w);
		}
	}
	return k1 + 0.5 * wave->shape->h * k2;
}

JBDOUBLE u_1d_linear_gaussian(Wave1D *wave, JBDOUBLE x)
{
	return wave->shape->u0 +
		wave->shape->h * exp(-jbm_fsqr(2 * (x - wave->xb) / wave->shape->w));
}

JBDOUBLE mass_1d_linear_gaussian(Wave1D *wave, JBDOUBLE x1, JBDOUBLE x2)
{
	return (x2 - x1) * wave->shape->u0 + 0.25 * sqrt(M_PI) * wave->shape->w *
		wave->shape->h * (erf(2 * (x2 - wave->xb) / wave->shape->w) -
		erf(2 * (x1 - wave->xb) / wave->shape->w));
}

JBDOUBLE flux_1d_linear_gaussian(Wave1D *wave, JBDOUBLE x)
{
	return vx_linear * t * wave->shape->u0 + 0.25 * sqrt(M_PI) *
		wave->shape->w * wave->shape->h *
		(erf(2 * (x - wave->xb) / wave->shape->w) -
		erf(2 * (x - wave->shape->x0) / wave->shape->w));
}

void wave_1d_message(char *text)
{
	char *buffer;
	buffer = message;
	message = g_strconcat("Wave1D:\n", text, NULL);
	if (buffer) g_free(buffer);
}

