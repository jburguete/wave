#define _GNU_SOURCE
#include <string.h>
#include "simulator.h"
#include "dialog_options.h"

void
dialog_options_create ()
{
  GtkWidget *container;
  DialogOptions dlg[1];

  dlg->label_xmax = (GtkLabel *) gtk_label_new (_("Maximum"));
  dlg->entry_xmax = (JBWFloatEntry *) jbw_float_entry_new ();
  jbw_float_entry_set_value (dlg->entry_xmax, draw_xmax);

  dlg->label_xmin = (GtkLabel *) gtk_label_new (_("Minimum"));
  dlg->entry_xmin = (JBWFloatEntry *) jbw_float_entry_new ();
  jbw_float_entry_set_value (dlg->entry_xmin, draw_xmin);

  dlg->table_x = (GtkGrid *) gtk_grid_new ();
  gtk_grid_attach (dlg->table_x, (GtkWidget *) dlg->label_xmax, 0, 0, 1, 1);
  gtk_grid_attach (dlg->table_x, (GtkWidget *) dlg->entry_xmax, 1, 0, 1, 1);
  gtk_grid_attach (dlg->table_x, (GtkWidget *) dlg->label_xmin, 0, 1, 1, 1);
  gtk_grid_attach (dlg->table_x, (GtkWidget *) dlg->entry_xmin, 1, 1, 1, 1);

  dlg->frame_x = (GtkFrame *) gtk_frame_new (_("Position"));
  gtk_container_add ((GtkContainer *) dlg->frame_x, (GtkWidget *) dlg->table_x);

  dlg->label_ymax = (GtkLabel *) gtk_label_new (_("Maximum"));
  dlg->entry_ymax = (JBWFloatEntry *) jbw_float_entry_new ();
  jbw_float_entry_set_value (dlg->entry_ymax, draw_ymax);

  dlg->label_ymin = (GtkLabel *) gtk_label_new (_("Minimum"));
  dlg->entry_ymin = (JBWFloatEntry *) jbw_float_entry_new ();
  jbw_float_entry_set_value (dlg->entry_ymin, draw_ymin);

  dlg->table_y = (GtkGrid *) gtk_grid_new ();
  gtk_grid_attach (dlg->table_y, (GtkWidget *) dlg->label_ymax, 0, 0, 1, 1);
  gtk_grid_attach (dlg->table_y, (GtkWidget *) dlg->entry_ymax, 1, 0, 1, 1);
  gtk_grid_attach (dlg->table_y, (GtkWidget *) dlg->label_ymin, 0, 1, 1, 1);
  gtk_grid_attach (dlg->table_y, (GtkWidget *) dlg->entry_ymin, 1, 1, 1, 1);

  dlg->frame_y = (GtkFrame *) gtk_frame_new (_("Variable"));
  gtk_container_add ((GtkContainer *) dlg->frame_y, (GtkWidget *) dlg->table_y);

  dlg->window =
    (GtkDialog *) gtk_dialog_new_with_buttons (_("Drawing options"),
                                               window_parent, GTK_DIALOG_MODAL,
                                               _("_OK"), GTK_RESPONSE_OK,
                                               _("_Cancel"),
                                               GTK_RESPONSE_CANCEL, NULL);
  container = gtk_dialog_get_content_area (dlg->window);
  gtk_container_add (GTK_CONTAINER (container), GTK_WIDGET (dlg->frame_x));
  gtk_container_add (GTK_CONTAINER (container), GTK_WIDGET (dlg->frame_y));
  gtk_widget_show_all ((GtkWidget *) dlg->window);

  jbw_float_entry_set_value (dlg->entry_xmin, draw_xmin);
  jbw_float_entry_set_value (dlg->entry_xmax, draw_xmax);
  jbw_float_entry_set_value (dlg->entry_ymin, draw_ymin);
  jbw_float_entry_set_value (dlg->entry_ymax, draw_ymax);

  if (gtk_dialog_run (dlg->window) == GTK_RESPONSE_OK)
    {
      DialogSimulator *dlg2 = &dialog_simulator;
      dlg2->graphic->xmin = draw_xmin =
        jbw_float_entry_get_value (dlg->entry_xmin);
      dlg2->graphic->xmax = draw_xmax =
        jbw_float_entry_get_value (dlg->entry_xmax);
      dlg2->graphic->ymin = draw_ymin =
        jbw_float_entry_get_value (dlg->entry_ymin);
      dlg2->graphic->ymax = draw_ymax =
        jbw_float_entry_get_value (dlg->entry_ymax);
      draw ();
    }
  gtk_widget_destroy ((GtkWidget *) dlg->window);
}
