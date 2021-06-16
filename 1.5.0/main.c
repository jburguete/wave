#include "scheme.h"

#if JBW == JBW_NO

#include <omp.h>
#include "wave.h"

int
main (int argn, char **argc)
{
  FILE *file;
  printf ("Maximum threads number=%d\n", omp_get_max_threads ());
  printf ("Processors number=%d\n", omp_get_num_procs ());
#if DEBUG_LEVEL && DEBUG_FILE
  stdout = fopen ("log", "w");
#endif
  file = fopen (argc[1], "r");
  fscanf (file, "%d%d", &wave_type, &shape_initial);
  fscanf (file, FRL FRL FRL FRL, &shape_position, &shape_width,
          &shape_amplitude, &wave_velocity);
  fscanf (file, FRL, &domain_width);
  fscanf (file, "%d%d%d%d%d", &scheme_type, &flux_limiter_type, &entropy_type,
          &mesh_type, &discretization_type);
  fscanf (file, "%d", &n);
  fscanf (file, FRL FRL, &tf, &cfl);
  fscanf (file, FRL FRL FRL, &o, &pv, &dxmax);
  fscanf (file, "%d%d%d", &convergence, &nmax, &nc);
  fclose (file);
#if DEBUG_LEVEL && DEBUG_SCHEME
  printf ("Wave type=%d initial=%d\n", wave_type, shape_initial);
  printf ("position=" FWL " width=" FWL " amplitude=" FWL " velocity=" FWL
          "\n", shape_position, shape_width, shape_amplitude, wave_velocity);
  printf ("Medium width=" FWL "\n", domain_width);
  printf ("Scheme type=%d flux_limiter=%d entropy=%d\n", scheme_type,
          flux_limiter_type, entropy_type);
  printf ("n=%d\n", n);
  printf ("t=" FWL " cfl=" FWL "\n", tf, cfl);
  printf ("o=" FWL " pv=" FWL " dxmax=" FWL "\n", o, pv, dxmax);
#endif
  execute (argc[2]);
  return 0;
}

#else

#include <locale.h>
#include "def.h"
#include "dialog_wave.h"
#include "dialog_options.h"
#include "dialog_scheme.h"
#include "simulator.h"

JBFLOAT draw_xmin, draw_xmax, draw_ymin, draw_ymax;
DialogSimulator dialog_simulator;

#if HAVE_FREEGLUT

static void
jbw_freeglut_idle ()
{
  GMainContext *context = g_main_context_default ();
  while (g_main_context_pending (context))
    g_main_context_iteration (context, 0);
}

static void
jbw_freeglut_draw_resize (int width, int height)
{
  jbw_graphic_resize (dialog_simulator.graphic, width, height);
  jbw_graphic_render (dialog_simulator.graphic);
}

static void
jbw_freeglut_draw_render ()
{
  jbw_graphic_render (dialog_simulator.graphic);
}

#elif HAVE_SDL

static void
jbw_sdl_draw_resize (int width, int height)
{
  jbw_graphic_resize (dialog_simulator.graphic, width, height);
  jbw_graphic_render (dialog_simulator.graphic);
}

static void
jbw_sdl_draw_render ()
{
  jbw_graphic_render (dialog_simulator.graphic);
}

#elif HAVE_GLFW

static void
jbw_glfw_draw_render ()
{
  jbw_graphic_render (dialog_simulator.graphic);
}

static void
jbw_glfw_window_close (JBWGraphic * graphic)
{
  jbw_main_loop_quit ();
  glfwSetWindowShouldClose (graphic->window, 1);
}

#endif

int
main (int argn, char **argc)
{
  JBWGraphic *graphic;
  GtkWindow *window;
#if HAVE_SDL
  SDL_Event exit_event[1];
#endif
#if DEBUG_LEVEL && DEBUG_FILE
  stderr = fopen ("log", "w");
#endif
  gtk_disable_setlocale ();
  setlocale (LC_ALL, "");
  setlocale (LC_NUMERIC, "C");
  bindtextdomain ("wave", "./po");
  bind_textdomain_codeset ("wave", "UTF-8");
  textdomain ("wave");
  jbw_init (&argn, &argc);
  dialog_simulator_create ();
  graphic = dialog_simulator.graphic;
  window = dialog_simulator.window;
#if HAVE_GTKGLAREA
  jbw_graphic_show (graphic);
#elif HAVE_FREEGLUT
  jbw_graphic_init (graphic);
  jbw_main_idle = jbw_freeglut_idle;
  jbw_main_resize = jbw_freeglut_draw_resize;
  jbw_main_render = jbw_freeglut_draw_render;
#elif HAVE_SDL
  exit_event->type = SDL_QUIT;
  jbw_graphic_init (graphic);
  jbw_main_resize = jbw_sdl_draw_resize;
  jbw_main_render = jbw_sdl_draw_render;
  g_signal_connect_swapped (window, "delete-event", (GCallback) SDL_PushEvent,
                            exit_event);
#elif HAVE_GLFW
  jbw_graphic_init (graphic);
  jbw_main_render = jbw_glfw_draw_render;
  g_signal_connect_swapped (window, "delete-event",
                            (GCallback) jbw_glfw_window_close, graphic);
#endif
  jbw_main_loop ();
  return 0;
}


#endif
