#include "wave.h"
#include "dialog_wave.h"

static void
dialog_wave_update (DialogWave * dlg)
{
  int k;
  k = jbw_array_radio_buttons_get_active (dlg->array_buttons_type);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->array_buttons_initial[4], k);
  k = !k;
  gtk_widget_set_sensitive ((GtkWidget *) dlg->array_buttons_initial[2], k);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->array_buttons_initial[3], k);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->label_wave_velocity, k);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->entry_wave_velocity, k);
  if (jbw_array_radio_buttons_get_active (dlg->array_buttons_initial) == 4)
    k = 0;
  else
    k = 1;
  gtk_widget_set_sensitive ((GtkWidget *) dlg->label_shape_position, k);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->entry_shape_position, k);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->label_shape_width, k);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->entry_shape_width, k);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->label_shape_amplitude, k);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->entry_shape_amplitude, k);
}

void
dialog_wave_create ()
{
  int i;
  char *string_type[N_WAVES_TYPE] = { _("Linear"), "Burger's" };
  char *string_initial[N_WAVES_INITIAL] = { _("Square"), _("Triangular"),
    _("Sinusoidal"), _("Gaussian"), _("Steady")
  };
  DialogWave dlg[1];

  dlg->array_buttons_type[0] = NULL;
  dlg->box_wave_type = (GtkBox *) gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  for (i = 0; i < N_WAVES_TYPE; ++i)
    {
      dlg->array_buttons_type[i] = (GtkRadioButton *)
        gtk_radio_button_new_with_label_from_widget
        (dlg->array_buttons_type[0], string_type[i]);
      g_signal_connect_swapped (dlg->array_buttons_type[i],
                                "clicked", (void (*)()) dialog_wave_update,
                                dlg);
      gtk_container_add (GTK_CONTAINER (dlg->box_wave_type),
                         (GtkWidget *) dlg->array_buttons_type[i]);
    }
  dlg->frame_wave_type = (GtkFrame *) gtk_frame_new (_("Equation type"));
  gtk_container_add (GTK_CONTAINER (dlg->frame_wave_type),
                     (GtkWidget *) dlg->box_wave_type);

  dlg->array_buttons_initial[0] = 0;
  dlg->box_shape_initial = (GtkBox *) gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  for (i = 0; i < N_WAVES_INITIAL; ++i)
    {
      dlg->array_buttons_initial[i] = (GtkRadioButton *)
        gtk_radio_button_new_with_label_from_widget
        (dlg->array_buttons_initial[0], string_initial[i]);
      g_signal_connect_swapped (dlg->array_buttons_initial[i],
                                "clicked", (void (*)()) dialog_wave_update,
                                dlg);
      gtk_container_add (GTK_CONTAINER (dlg->box_shape_initial),
                         (GtkWidget *) dlg->array_buttons_initial[i]);
    }
  dlg->frame_shape_initial = (GtkFrame *) gtk_frame_new (_("Initial shape"));
  gtk_container_add ((GtkContainer *) dlg->frame_shape_initial,
                     (GtkWidget *) dlg->box_shape_initial);

  dlg->label_shape_position = (GtkLabel *) gtk_label_new (_("Shape position"));
  dlg->entry_shape_position = (JBWFloatEntry *) jbw_float_entry_new ();
  dlg->label_shape_width = (GtkLabel *) gtk_label_new (_("Shape width"));
  dlg->entry_shape_width = (JBWFloatEntry *) jbw_float_entry_new ();
  dlg->label_shape_amplitude
    = (GtkLabel *) gtk_label_new (_("Shape amplitude"));
  dlg->entry_shape_amplitude = (JBWFloatEntry *) jbw_float_entry_new ();
  dlg->label_shape_base = (GtkLabel *) gtk_label_new (_("Base height"));
  dlg->entry_shape_base = (JBWFloatEntry *) jbw_float_entry_new ();
  dlg->label_domain_width = (GtkLabel *) gtk_label_new (_("Domain width"));
  dlg->entry_domain_width = (JBWFloatEntry *) jbw_float_entry_new ();
  dlg->label_wave_velocity = (GtkLabel *) gtk_label_new (_("Wave velocity"));
  dlg->entry_wave_velocity = (JBWFloatEntry *) jbw_float_entry_new ();

  dlg->box_frames = (GtkBox *) gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add
    (GTK_CONTAINER (dlg->box_frames), (GtkWidget *) dlg->frame_wave_type);
  gtk_container_add
    (GTK_CONTAINER (dlg->box_frames), (GtkWidget *) dlg->frame_shape_initial);

  dlg->table = (GtkGrid *) gtk_grid_new ();
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->box_frames, 0, 0, 1, 7);
  gtk_grid_attach (dlg->table,
                   (GtkWidget *) dlg->label_shape_position, 1, 0, 1, 1);
  gtk_grid_attach (dlg->table,
                   (GtkWidget *) dlg->entry_shape_position, 2, 0, 1, 1);
  gtk_grid_attach (dlg->table,
                   (GtkWidget *) dlg->label_shape_width, 1, 1, 1, 1);
  gtk_grid_attach (dlg->table,
                   (GtkWidget *) dlg->entry_shape_width, 2, 1, 1, 1);
  gtk_grid_attach (dlg->table,
                   (GtkWidget *) dlg->label_shape_amplitude, 1, 2, 1, 1);
  gtk_grid_attach (dlg->table,
                   (GtkWidget *) dlg->entry_shape_amplitude, 2, 2, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->label_shape_base, 1, 3, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->entry_shape_base, 2, 3, 1, 1);
  gtk_grid_attach (dlg->table,
                   (GtkWidget *) dlg->label_domain_width, 1, 4, 1, 1);
  gtk_grid_attach (dlg->table,
                   (GtkWidget *) dlg->entry_domain_width, 2, 4, 1, 1);
  gtk_grid_attach (dlg->table,
                   (GtkWidget *) dlg->label_wave_velocity, 1, 5, 1, 1);
  gtk_grid_attach (dlg->table,
                   (GtkWidget *) dlg->entry_wave_velocity, 2, 5, 1, 1);

  dlg->window =
    (GtkDialog *) gtk_dialog_new_with_buttons (_("Wave type"),
                                               window_parent, GTK_DIALOG_MODAL,
                                               _("_OK"), GTK_RESPONSE_OK,
                                               _("_Cancel"),
                                               GTK_RESPONSE_CANCEL, NULL);
  gtk_container_add (GTK_CONTAINER (gtk_dialog_get_content_area (dlg->window)),
                     GTK_WIDGET (dlg->table));
  gtk_widget_show_all ((GtkWidget *) dlg->window);

  jbw_float_entry_set_value (dlg->entry_shape_position, shape_position);
  jbw_float_entry_set_value (dlg->entry_shape_width, shape_width);
  jbw_float_entry_set_value (dlg->entry_shape_amplitude, shape_amplitude);
  jbw_float_entry_set_value (dlg->entry_shape_base, shape_base);
  jbw_float_entry_set_value (dlg->entry_domain_width, domain_width);
  jbw_float_entry_set_value (dlg->entry_wave_velocity, wave_velocity);

  jbw_array_radio_buttons_set_active (dlg->array_buttons_type, wave_type, 1);
  jbw_array_radio_buttons_set_active
    (dlg->array_buttons_initial, shape_initial, 1);

  dialog_wave_update (dlg);

  if (gtk_dialog_run (dlg->window) == GTK_RESPONSE_OK)
    {
      shape_position = jbw_float_entry_get_value (dlg->entry_shape_position);
      shape_width = jbw_float_entry_get_value (dlg->entry_shape_width);
      shape_amplitude = jbw_float_entry_get_value (dlg->entry_shape_amplitude);
      shape_base = jbw_float_entry_get_value (dlg->entry_shape_base);
      domain_width = jbw_float_entry_get_value (dlg->entry_domain_width);
      wave_velocity = jbw_float_entry_get_value (dlg->entry_wave_velocity);
      wave_type = jbw_array_radio_buttons_get_active (dlg->array_buttons_type);
      shape_initial =
        jbw_array_radio_buttons_get_active (dlg->array_buttons_initial);
    }
  gtk_widget_destroy ((GtkWidget *) dlg->window);
}
