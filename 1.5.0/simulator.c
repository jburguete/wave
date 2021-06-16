#define _GNU_SOURCE
#include <time.h>
#include "wave.h"
#include "scheme.h"
#include "dialog_wave.h"
#include "dialog_scheme.h"
#include "dialog_options.h"
#include "simulator.h"

extern void draw_graphic ();

static void
dialog_help_create ()
{
  gchar *authors[] = {
    "Javier Burguete Tolosa (jburguete@eead.csic.es)",
    NULL
  };
  gtk_show_about_dialog (dialog_simulator.window,
                         "program_name",
                         "Wave",
                         "authors",
                         authors,
                         "version",
                         "1.5.0",
                         "copyright",
                         "Copyright 2009-2021 Javier Burguete Tolosa",
//              "logo",
//              dialog_simulator.logo,
                         "website-label",
                         _("Website"),
                         "website", "https://github.com/jburguete/wave", NULL);
}

static void
dialog_simulator_close ()
{
#if HAVE_SDL
  SDL_Event exit_event[1];
  exit_event->type = SDL_QUIT;
#endif
#if DEBUG_LEVEL && DEBUG_FILE
  fclose (stdout);
#endif
  gtk_widget_destroy ((GtkWidget *) dialog_simulator.window);
#if HAVE_FREEGLUT
  glutLeaveMainLoop ();
#elif HAVE_SDL
  SDL_PushEvent (exit_event);
#endif
}

void
dialog_simulator_update ()
{
  register DialogSimulator *dlg = &dialog_simulator;
  gtk_widget_set_sensitive (GTK_WIDGET (dlg->button_item[3]), !simulating);
  gtk_widget_set_sensitive (GTK_WIDGET (dlg->button_item[4]), simulating);
  jbw_graphic_render (dlg->graphic);
}

static void
dialog_simulator_start ()
{
  char *file_name = "1";
  n = mesh_cells;
  tf = simulation_time;
  cfl = cfl_number;
  o = implicit;
  pv = viscosity;
  convergence = dialog_convergence;
  nmax = max_cells;
  nc = convergence_number;
  if (simulating)
    simulating = 0;
  else
    {
      dialog_simulator.time0 = time (NULL);
      execute (file_name);
      dialog_simulator_update ();
    }
}

static void
dialog_simulator_save_graphical ()
{
  char *buffer = 0;
  GtkFileChooserDialog *dlg;
  dlg = (GtkFileChooserDialog *) gtk_file_chooser_dialog_new
    (_("Save graphical"), dialog_simulator.window,
     GTK_FILE_CHOOSER_ACTION_SAVE,
     _("_OK"), GTK_RESPONSE_OK, _("_Cancel"), GTK_RESPONSE_CANCEL, NULL);
  gtk_container_set_border_width ((GtkContainer *) dlg, 10);
  gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dlg), 1);
  if (gtk_dialog_run ((GtkDialog *) dlg) == GTK_RESPONSE_OK)
    buffer = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dlg));
  gtk_widget_destroy ((GtkWidget *) dlg);
  if (buffer)
    {
      while (gtk_events_pending ())
        gtk_main_iteration ();
      jbw_graphic_render (dialog_simulator.graphic);
      jbw_graphic_save (dialog_simulator.graphic, buffer);
      g_free (buffer);
    }
}

#if JBW==JBW_GTK_GL
void
dialog_simulator_create (GdkGLConfig * glconfig)
#else
void
dialog_simulator_create ()
#endif
{
  int i;
  DialogSimulator *dlg = &dialog_simulator;

  void (*function_item[]) = { &dialog_wave_create, &dialog_scheme_create,
    &dialog_options_create, &dialog_simulator_start, &dialog_simulator_start,
    &dialog_simulator_save_graphical, &dialog_help_create,
    &dialog_simulator_close
  };
  char *image_item[] = { "preferences-system", "document-properties",
    "preferences-desktop", "system-run", "process-stop",
    "document-save", "help-about",
    "application-exit"
  }, *str_tooltip[] = { _("Wave type"), _("Numerical scheme"),
    _("Drawing options"), _("Start simulation"), _("Stop simulation"),
    _("Save graphical"), _("Help"),
    _("Exit")
  };

  dlg->box_items = (GtkBox *) gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  for (i = 0; i < N_DIALOG_SIMULATOR_ITEMS; ++i)
    {
      dlg->button_item[i] = (GtkButton *)
        gtk_button_new_from_icon_name (image_item[i], GTK_ICON_SIZE_BUTTON);
      gtk_box_pack_start (dlg->box_items, dlg->button_item[i], 0, 0, 0);
      gtk_widget_set_tooltip_text (GTK_WIDGET (dlg->button_item[i]),
                                   str_tooltip[i]);
      g_signal_connect (dlg->button_item[i], "clicked", function_item[i], NULL);
    }

  dlg->graphic = jbw_graphic_new (6, 6, 0, draw_graphic, "Wave");
  jbw_graphic_set_xlabel (dlg->graphic, "x");
  jbw_graphic_set_ylabel (dlg->graphic, "y");
  dlg->graphic->resize = 0;

  dlg->label_simulation_time =
    (GtkLabel *) gtk_label_new (_("Simulation time"));
  gtk_widget_set_sensitive ((GtkWidget *) dlg->label_simulation_time, 0);
  dlg->progress_simulation_time = (GtkProgressBar *) gtk_progress_bar_new ();

  dlg->label_cpu_time = (GtkLabel *) gtk_label_new (_("CPU time"));
  gtk_widget_set_sensitive ((GtkWidget *) dlg->label_cpu_time, 0);
  dlg->entry_cpu_time = (JBWFloatEntry *) jbw_float_entry_new ();
  gtk_widget_set_sensitive ((GtkWidget *) dlg->entry_cpu_time, 0);
  dlg->label_initial_mass = (GtkLabel *) gtk_label_new (_("Initial mass"));
  gtk_widget_set_sensitive ((GtkWidget *) dlg->label_initial_mass, 0);
  dlg->entry_initial_mass = (JBWFloatEntry *) jbw_float_entry_new ();
  gtk_widget_set_sensitive ((GtkWidget *) dlg->entry_initial_mass, 0);
  dlg->label_final_mass = (GtkLabel *) gtk_label_new (_("Final mass"));
  gtk_widget_set_sensitive ((GtkWidget *) dlg->label_final_mass, 0);
  dlg->entry_final_mass = (JBWFloatEntry *) jbw_float_entry_new ();
  gtk_widget_set_sensitive ((GtkWidget *) dlg->entry_final_mass, 0);

  dlg->box_bar = (GtkBox *) gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start
    (dlg->box_bar, (GtkWidget *) dlg->label_simulation_time, 0, 1, 0);
  gtk_box_pack_start
    (dlg->box_bar, (GtkWidget *) dlg->progress_simulation_time, 0, 0, 0);
  gtk_box_pack_start (dlg->box_bar, (GtkWidget *) dlg->label_cpu_time, 0, 1, 0);
  gtk_box_pack_start
    (dlg->box_bar, (GtkWidget *) dlg->entry_cpu_time, 0, 0, 0);
  gtk_box_pack_start
    (dlg->box_bar, (GtkWidget *) dlg->label_initial_mass, 0, 1, 0);
  gtk_box_pack_start
    (dlg->box_bar, (GtkWidget *) dlg->entry_initial_mass, 0, 0, 0);
  gtk_box_pack_start
    (dlg->box_bar, (GtkWidget *) dlg->label_final_mass, 0, 1, 0);
  gtk_box_pack_start
    (dlg->box_bar, (GtkWidget *) dlg->entry_final_mass, 0, 0, 0);

  dlg->box = (GtkBox *) gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_pack_start (dlg->box, (GtkWidget *) dlg->box_items, 0, 1, 0);
  gtk_box_pack_start (dlg->box, (GtkWidget *) dlg->box_bar, 0, 1, 0);

  dlg->window = (GtkWindow *) gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (dlg->window, _("Waves simulator"));
  gtk_container_add ((GtkContainer *) dlg->window, (GtkWidget *) dlg->box);
  gtk_widget_show_all ((GtkWidget *) dlg->window);
  g_signal_connect (dlg->window, "delete_event", dialog_simulator_close, NULL);

  window_parent = dlg->window;

  dialog_simulator_update ();

  draw ();
}
