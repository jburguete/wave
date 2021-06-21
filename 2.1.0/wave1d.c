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
 * \file wave1d.c
 * \brief Source file to define an 1D wave.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2010-2021.
 */
#include "config.h"
#include "jb/jb_xml.h"
#include "tools.h"
#include "domain1d.h"
#include "shape1d.h"
#include "wave1d.h"

/**
 * function to make an error message in the 1D wave context.
 */
static inline void
wave_1d_error (char *text)      ///< message string.
{
  error_message ("Wave1D", text);
}

/**
 * function to set the x component of the propagation velocity on a linear 1D
 * wave.
 *
 * \return x component of the propagation velocity.
 */
JBDOUBLE
vx_1d_linear (JBDOUBLE u __attribute__((unused)))       ///< variable.
{
#if DEBUG_WAVE_1D
  fprintf (stderr, "vx_1d_linear: start\n");
  fprintf (stderr, "vx_1d_linear: velocity=" FWL "\n", vx_linear);
  fprintf (stderr, "vx_1d_linear: end\n");
#endif
  return vx_linear;
}

/**
 * function to set the x component of the propagation velocity on a Burgers' 1D
 * wave.
 *
 * \return x component of the propagation velocity.
 */
JBDOUBLE
vx_1d_Burgers (JBDOUBLE u)      ///< variable.
{
#if DEBUG_WAVE_1D
  fprintf (stderr, "vx_1d_Burgers: start\n");
  fprintf (stderr, "vx_1d_Burgers: velocity=" FWL "\n", u);
  fprintf (stderr, "vx_1d_Burgers: end\n");
#endif
  return u;
}

/**
 * function to adjust the shape propagated parameters on a linear 1D wave.
 */
void
adjust_1d_linear (Wave1D * wave)        ///< Wave1D struct.
{
  register JBDOUBLE k;
#if DEBUG_WAVE_1D
  fprintf (stderr, "adjust_1d_linear: start\n");
#endif
  k = 0.5L * wave->shape->w;
  wave->xb = wave->shape->x0 + vx_linear * t;
  wave->xa = wave->xb - k;
  wave->xc = wave->xb + k;
#if DEBUG_WAVE_1D
  fprintf (stderr, "adjust_1d_linear: xa=" FWL " xb=" FWL " xc=" FWL "\n",
           wave->xa, wave->xb, wave->xc);
  fprintf (stderr, "adjust_1d_linear: end\n");
#endif
}

/**
 * function to get the variable at a position on a square linear 1D wave.
 *
 * \return variable value.
 */
JBDOUBLE
u_1d_linear_square (Wave1D * wave,      ///< Wave1D struct.
                    JBDOUBLE x) ///< x-coordinate.
{
  register JBDOUBLE k;
#if DEBUG_WAVE_1D
  fprintf (stderr, "u_1d_linear_square: start\n");
#endif
  k = wave->shape->u0;
  if (x > wave->xa && x < wave->xc)
    k += wave->shape->h;
#if DEBUG_WAVE_1D
  fprintf (stderr, "u_1d_linear_square: u=" FWL " x=" FWL "\n", k, x);
  fprintf (stderr, "u_1d_linear_square: end\n");
#endif
  return k;
}

/**
 * function to get the mass at an interval on a square linear 1D wave.
 *
 * \return mass value.
 */
JBDOUBLE
mass_1d_linear_square (Wave1D * wave,   ///< Wave1D struct.
                       JBDOUBLE x1,     ///< minimum x-coordinate.
                       JBDOUBLE x2)     ///< maximum x-coordinate.
{
  register JBDOUBLE k1, k2;
#if DEBUG_WAVE_1D
  fprintf (stderr, "mass_1d_linear_square: start\n");
#endif
  k1 = (x2 - x1) * wave->shape->u0;
  if (x2 > wave->xa && x1 < wave->xc)
    {
      if (x1 > wave->xa)
        {
          if (x2 > wave->xc)
            k2 = wave->xc - x1;
          else
            k2 = x2 - x1;
        }
      else
        {
          if (x2 < wave->xc)
            k2 = x2 - wave->xa;
          else
            k2 = wave->shape->w;
        }
      k1 += wave->shape->h * k2;
    }
#if DEBUG_WAVE_1D
  fprintf (stderr,
           "mass_1d_linear_square: mass=" FWL " x1=" FWL " x2=" FWL "\n", k1,
           x1, x2);
  fprintf (stderr, "mass_1d_linear_square: end\n");
#endif
  return k1;
}

/**
 * function to get the flux at a position on a square linear 1D wave.
 *
 * \return flux value.
 */
JBDOUBLE
flux_1d_linear_square (Wave1D * wave,   ///< Wave1D struct.
                       JBDOUBLE x)      ///< x-coordinate.
{
  register JBDOUBLE k1, k2, ta, tc;
#if DEBUG_WAVE_1D
  fprintf (stderr, "flux_1d_linear_square: start\n");
#endif
  k1 = x - wave->shape->x0;
  k2 = 0.5L * wave->shape->w;
  ta = (k1 + k2) / vx_linear;
  tc = (k1 - k2) / vx_linear;
  k1 = t * vx_linear * wave->shape->u0;
  if (vx_linear > 0.L)
    {
      if (ta > 0.L && t > tc)
        {
          if (tc > 0.L)
            {
              if (t < ta)
                k2 = vx_linear * (t - tc);
              else
                k2 = wave->shape->w;
            }
          else
            {
              if (t < ta)
                k2 = vx_linear * t;
              else
                k2 = vx_linear * ta;
            }
          k1 += wave->shape->h * k2;
        }
    }
  else
    {
      if (tc > 0.L && t > ta)
        {
          if (ta > 0.L)
            {
              if (t < tc)
                k2 = vx_linear * (t - ta);
              else
                k2 = wave->shape->w;
            }
          else
            {
              if (t < tc)
                k2 = vx_linear * t;
              else
                k2 = vx_linear * tc;
            }
          k1 += wave->shape->h * k2;
        }
    }
#if DEBUG_WAVE_1D
  fprintf (stderr, "flux_1d_linear_square: flux=" FWL " x=" FWL "\n", k1, x);
  fprintf (stderr, "flux_1d_linear_square: end\n");
#endif
  return k1;
}

/**
 * function to get the variable at a position on a triangular linear 1D wave.
 *
 * \return variable value.
 */
JBDOUBLE
u_1d_linear_triangular (Wave1D * wave,  ///< Wave1D struct.
                        JBDOUBLE x)     ///< x-coordinate.
{
  register JBDOUBLE k1, k2;
#if DEBUG_WAVE_1D
  fprintf (stderr, "u_1d_linear_triangular: start\n");
#endif
  k1 = wave->shape->u0;
  if (x > wave->xa && x < wave->xc)
    {
      if (x <= wave->xb)
        k2 = x - wave->xa;
      else
        k2 = wave->xc - x;
      k1 += 2.L * wave->shape->h * k2 / wave->shape->w;
    }
#if DEBUG_WAVE_1D
  fprintf (stderr, "u_1d_linear_triangular: u=" FWL " x=" FWL "\n", k1, x);
  fprintf (stderr, "u_1d_linear_triangular: end\n");
#endif
  return k1;
}

/**
 * function to get the mass at an interval on a triangular linear 1D wave.
 *
 * \return mass value.
 */
JBDOUBLE
mass_1d_linear_triangular (Wave1D * wave,       ///< Wave1D struct.
                           JBDOUBLE x1, ///< minimum x-coordinate.
                           JBDOUBLE x2) ///< maximum x-coordinate.
{
  register JBDOUBLE k1, k2;
#if DEBUG_WAVE_1D
  fprintf (stderr, "mass_1d_linear_triangular: start\n");
#endif
  k1 = (x2 - x1) * wave->shape->u0;
  if (x2 > wave->xa && x1 < wave->xc)
    {
      if (x1 > wave->xb)
        {
          if (x2 >= wave->xc)
            k2 = jbm_fsqrl (wave->xc - x1) / wave->shape->w;
          else
            k2 = (x1 - x2) * (x1 + x2 - 2 * wave->xc) / wave->shape->w;
        }
      else if (x1 > wave->xa)
        {
          if (x2 > wave->xc)
            k2 = 0.25L * wave->shape->w +
              (wave->xb - x1) * (wave->xb + x1 - 2 * wave->xa) / wave->shape->w;
          else if (x2 > wave->xb)
            k2 = ((wave->xb - x1) *
                  (wave->xb + x1 - 2 * wave->xa) + (wave->xb - x2) *
                  (x2 + wave->xb - 2 * wave->xc)) / wave->shape->w;
          else
            k2 = (x2 - x1) * (x1 + x2 - 2 * wave->xa) / wave->shape->w;
        }
      else
        {
          if (x2 >= wave->xc)
            k2 = 0.5L * wave->shape->w;
          else if (x2 > wave->xb)
            k2 = 0.25L * wave->shape->w +
              (wave->xb - x2) * (x2 + wave->xb - 2 * wave->xc) / wave->shape->w;
          else
            k2 = jbm_fsqrl (x2 - wave->xa) / wave->shape->w;
        }
      k1 += wave->shape->h * k2;
    }
#if DEBUG_WAVE_1D
  fprintf (stderr,
           "mass_1d_linear_triangular: mass=" FWL " x1=" FWL " x2=" FWL "\n",
           k1, x1, x2);
  fprintf (stderr, "mass_1d_linear_triangular: end\n");
#endif
  return k1;
}

/**
 * function to get the flux at a position on a triangular linear 1D wave.
 *
 * \return flux value.
 */
JBDOUBLE
flux_1d_linear_triangular (Wave1D * wave,       ///< Wave1D struct.
                           JBDOUBLE x)  ///< x-coordinate.
{
  register JBDOUBLE k1, k2, ta, tb, tc;
#if DEBUG_WAVE_1D
  fprintf (stderr, "flux_1d_linear_triangular: start\n");
#endif
  tb = (x - wave->shape->x0) / vx_linear;
  k2 = 0.5L * wave->shape->w / vx_linear;
  ta = tb + k2;
  tc = tb - k2;
  k1 = t * vx_linear * wave->shape->u0;
  if (vx_linear > 0.L)
    {
      if (ta > 0.L && t > tc)
        {
          if (tc > 0.L)
            {
              if (t >= ta)
                k2 = wave->shape->w;
              else if (t > tb)
                k2 = 0.5L * wave->shape->w +
                  vx_linear * (t - tb) * (t + tb - 2 * ta) / (tb - ta);
              else
                k2 = vx_linear * jbm_fsqrl (t - tc) / (tb - tc);
            }
          else if (tb > 0.L)
            {
              if (t > ta)
                k2 = 0.5L * wave->shape->w +
                  vx_linear * tb * (tb - 2 * tc) / (tb - tc);
              else if (t > tb)
                k2 = vx_linear * ((t - tb) * (t + tb - 2 * ta)
                                  + tb * (tb - 2 * tc)) / (tb - ta);
              else
                k2 = vx_linear * t * (t - 2 * tc) / (tb - tc);
            }
          else
            {
              if (t < ta)
                k2 = vx_linear * t * (t - 2 * ta) / (tb - ta);
              else
                k2 = vx_linear * ta * ta / (ta - tb);
            }
          k1 += 0.5L * wave->shape->h * k2;
        }
    }
  else
    {
      if (tc > 0.L && t > ta)
        {
          if (ta > 0.L)
            {
              if (t >= tc)
                k2 = wave->shape->w;
              else if (t > tb)
                k2 = 0.5L * wave->shape->w +
                  vx_linear * (t - tb) * (t + tb - 2 * tc) / (tb - tc);
              else
                k2 = vx_linear * jbm_fsqrl (t - ta) / (tb - ta);
            }
          else if (tb > 0.L)
            {
              if (t > tc)
                k2 = 0.5L * wave->shape->w +
                  vx_linear * tb * (tb - 2 * ta) / (tb - ta);
              else if (t > tb)
                k2 = vx_linear * ((t - tb) * (t + tb - 2 * tc)
                                  + tb * (tb - 2 * ta)) / (tb - tc);
              else
                k2 = vx_linear * t * (t - 2 * ta) / (tb - ta);
            }
          else
            {
              if (t < tc)
                k2 = vx_linear * t * (t - 2 * tc) / (tb - tc);
              else
                k2 = vx_linear * tc * tc / (tc - tb);
            }
          k1 += 0.5L * wave->shape->h * k2;
        }
    }
#if DEBUG_WAVE_1D
  fprintf (stderr, "flux_1d_linear_triangular: flux=" FWL " x=" FWL "\n", k1,
           x);
  fprintf (stderr, "flux_1d_linear_triangular: end\n");
#endif
  return k1;
}

/**
 * function to get the variable at a position on a sinusoidal linear 1D wave.
 *
 * \return variable value.
 */
JBDOUBLE
u_1d_linear_sinusoidal (Wave1D * wave,  ///< Wave1D struct.
                        JBDOUBLE x)     ///< x-coordinate.
{
  register JBDOUBLE k;
#if DEBUG_WAVE_1D
  fprintf (stderr, "u_1d_linear_sinusoidal: start\n");
#endif
  k = wave->shape->u0;
  if (x > wave->xa && x < wave->xc)
    k += wave->shape->h
      * jbm_fsqrl (cosl (M_PIl * (x - wave->xb) / wave->shape->w));
#if DEBUG_WAVE_1D
  fprintf (stderr, "u_1d_linear_sinusoidal: u=" FWL " x=" FWL "\n", k, x);
  fprintf (stderr, "u_1d_linear_sinusoidal: end\n");
#endif
  return k;
}

/**
 * function to get the mass at an interval on a sinusoidal linear 1D wave.
 *
 * \return mass value.
 */
JBDOUBLE
mass_1d_linear_sinusoidal (Wave1D * wave,       ///< Wave1D struct.
                           JBDOUBLE x1, ///< minimum x-coordinate.
                           JBDOUBLE x2) ///< maximum x-coordinate.
{
  register JBDOUBLE k1, k2;
#if DEBUG_WAVE_1D
  fprintf (stderr, "mass_1d_linear_sinusoidal: start\n");
#endif
  k1 = (x2 - x1) * wave->shape->u0;
  if (x2 > wave->xa && x1 < wave->xc)
    {
      if (x1 > wave->xa)
        {
          if (x2 > wave->xc)
            k2 = wave->xc - x1 - wave->shape->w / (2.L * M_PIl) *
              sinl (2.L * M_PIl * (x1 - wave->xa) / wave->shape->w);
          else
            k2 = x2 - x1 - wave->shape->w / (2 * M_PIl) *
              (sinl (2.L * M_PIl * (x2 - wave->xa) / wave->shape->w) -
               sinl (2.L * M_PIl * (x1 - wave->xa) / wave->shape->w));
        }
      else
        {
          if (x2 < wave->xc)
            k2 = x2 - wave->xa - wave->shape->w / (2 * M_PIl) *
              sinl (2.L * M_PIl * (x2 - wave->xa) / wave->shape->w);
          else
            k2 = wave->shape->w;
        }
      k1 += 0.5L * wave->shape->h * k2;
    }
#if DEBUG_WAVE_1D
  fprintf (stderr,
           "mass_1d_linear_sinusoidal: mass=" FWL " x1=" FWL " x2=" FWL "\n",
           k1, x1, x2);
  fprintf (stderr, "mass_1d_linear_sinusoidal: end\n");
#endif
  return k1;
}

/**
 * function to get the flux at a position on a sinusoidal linear 1D wave.
 *
 * \return flux value.
 */
JBDOUBLE
flux_1d_linear_sinusoidal (Wave1D * wave,       ///< Wave1D struct.
                           JBDOUBLE x)  ///< x-coordinate.
{
  register JBDOUBLE k1, k2, ta, tc;
#if DEBUG_WAVE_1D
  fprintf (stderr, "flux_1d_linear_sinusoidal: start\n");
#endif
  k1 = x - wave->shape->x0;
  k2 = 0.5L * wave->shape->w;
  ta = (k1 + k2) / vx_linear;
  tc = (k1 - k2) / vx_linear;
  k1 = t * vx_linear * wave->shape->u0;
  if (vx_linear > 0.L)
    {
      if (ta > 0.L && t > tc)
        {
          if (tc > 0.L)
            {
              if (t < ta)
                k2 = vx_linear * (t - tc)
                  - wave->shape->w / (2.L * M_PIl)
                  * sinl (2.L * M_PIl * (x - wave->xa) / wave->shape->w);
              else
                k2 = wave->shape->w;
            }
          else
            {
              if (t < ta)
                k2 = vx_linear * t + wave->shape->w / (2.L * M_PIl) *
                  (sinl (2.L * M_PIl * (x - wave->shape->x0) / wave->shape->w) -
                   sinl (2.L * M_PIl * (x - wave->xa) / wave->shape->w));
              else
                k2 = vx_linear * ta + wave->shape->w / (2.L * M_PIl) *
                  sinl (2.L * M_PIl * (x - wave->shape->x0) / wave->shape->w);
            }
          k1 += 0.5L * wave->shape->h * k2;
        }
    }
  else
    {
      if (tc > 0.L && t > ta)
        {
          if (ta > 0.L)
            {
              if (t < tc)
                k2 = vx_linear * (t - ta)
                  - wave->shape->w / (2.L * M_PIl)
                  * sinl (2.L * M_PIl * (x - wave->xa) / wave->shape->w);
              else
                k2 = wave->shape->w;
            }
          else
            {
              if (t < tc)
                k2 = vx_linear * t + wave->shape->w / (2.L * M_PIl) *
                  (sinl (2.L * M_PIl * (x - wave->shape->x0) / wave->shape->w) -
                   sinl (2.L * M_PIl * (x - wave->xa) / wave->shape->w));
              else
                k2 = vx_linear * tc + wave->shape->w / (2.L * M_PIl) *
                  sinl (2.L * M_PIl * (x - wave->shape->x0) / wave->shape->w);
            }
          k1 += 0.5L * wave->shape->h * k2;
        }
    }
#if DEBUG_WAVE_1D
  fprintf (stderr, "flux_1d_linear_sinusoidal: flux=" FWL " x=" FWL "\n", k1,
           x);
  fprintf (stderr, "flux_1d_linear_sinusoidal: end\n");
#endif
  return k1;
}

JBDOUBLE
u_1d_linear_gaussian (Wave1D * wave, JBDOUBLE x)
{
  return wave->shape->u0 +
    wave->shape->h * expl (-jbm_fsqrl (2 * (x - wave->xb) / wave->shape->w));
}

JBDOUBLE
mass_1d_linear_gaussian (Wave1D * wave, JBDOUBLE x1, JBDOUBLE x2)
{
  return (x2 - x1) * wave->shape->u0 + 0.25L * sqrtl (M_PIl) * wave->shape->w *
    wave->shape->h * (erfl (2 * (x2 - wave->xb) / wave->shape->w) -
                      erfl (2 * (x1 - wave->xb) / wave->shape->w));
}

JBDOUBLE
flux_1d_linear_gaussian (Wave1D * wave, JBDOUBLE x)
{
  return vx_linear * t * wave->shape->u0 + 0.25L * sqrtl (M_PIl) *
    wave->shape->w * wave->shape->h *
    (erfl (2 * (x - wave->xb) / wave->shape->w) -
     erfl (2 * (x - wave->shape->x0) / wave->shape->w));
}
