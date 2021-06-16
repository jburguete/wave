#include "scheme.h"
#include "dialog_scheme.h"

static void
dialog_scheme_update (DialogScheme * dlg)
{
  int k;
  gtk_widget_set_sensitive ((GtkWidget *) dlg->entry_viscosity, 0);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->label_viscosity, 0);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->entry_implicit, 0);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->label_implicit, 0);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->frame_limiters, 0);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->frame_entropy, 0);
  k = jbw_array_radio_buttons_get_active (dlg->array_schemes);
  if (k > 1)
    gtk_widget_set_sensitive ((GtkWidget *) dlg->frame_entropy, 1);
  if (k == 0)
    {
      gtk_widget_set_sensitive ((GtkWidget *) dlg->entry_viscosity, 1);
      gtk_widget_set_sensitive ((GtkWidget *) dlg->label_viscosity, 1);
    }
  else if (k >= 11 && k <= 15)
    {
      if (k != 12 && k != 14)
        {
          gtk_widget_set_sensitive ((GtkWidget *) dlg->entry_implicit, 1);
          gtk_widget_set_sensitive ((GtkWidget *) dlg->label_implicit, 1);
        }
    }
  else if (k >= 6)
    gtk_widget_set_sensitive ((GtkWidget *) dlg->frame_limiters, 1);
  k = jbw_array_radio_buttons_get_active (dlg->array_meshes);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->entry_dxmax, k);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->label_dxmax, k);
  k = gtk_toggle_button_get_active ((GtkToggleButton *) dlg->convergence);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->entry_max_cells, k);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->label_max_cells, k);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->entry_convergence_number, k);
  gtk_widget_set_sensitive ((GtkWidget *) dlg->label_convergence_number, k);
}

void
dialog_scheme_create ()
{
  int i;
  char *string_schemes[N_SCHEMES] = { "Lax-Friedrichs",
    _("Lax-Friedrichs optimized"), _("1st order upwind"),
    "Lax-Wendroff", "Warming-Beam", "McCormack",
    _("2nd order upwind"), _("Classical spatial 2nd order TVD"),
    _("Spatial 2nd order TVD"), "Roe-Sweby TVD",
    _("2nd order TVD"), _("Tridiagonal upwind implicit"),
    _("Optimized tridiagonal implicit"),
    _("Bidiagonal upwind implicit"),
    _("Optimized bidiagonal implicit"), "Casier",
    _("Tridiagonal upwind semi-explicit"),
    _("Bidiagonal upwind semi-explicit")
  };
  char *string_limiters[N_LIMITERS] = { _("Total"), _("None"), _("Centered"),
    "Minmod", "Superbee", "Van-Leer", "Van-Albada", "Minsuper", "Supermin",
    _("Monotonized central"), _("Mean")
  };
  char *string_entropy[N_ENTROPY] = { _("None"), "Harten-Hyman",
    _("Mean"), _("Double mean"), _("Interpolated"),
    _("Interpolated mean")
  };
  char *string_meshes[N_MESHES] =
    { _("Regular"), _("Progressive"), _("Aleatory") };
  char *string_discretizations[N_DISCRETIZATIONS] =
    { _("Finite differences"), _("Finite volumes") };
  DialogScheme dlg[1];

  dlg->entry_mesh_cells = (GtkSpinButton *)
    gtk_spin_button_new_with_range (0., (gdouble) G_MAXULONG, 1.);
  dlg->label_mesh_cells = (GtkLabel *) gtk_label_new (_("Mesh cells"));

  dlg->entry_simulation_time = (JBWFloatEntry *) jbw_float_entry_new ();
  dlg->label_simulation_time =
    (GtkLabel *) gtk_label_new (_("Simulation time"));

  dlg->array_schemes[0] = 0;
  dlg->box_schemes = (GtkBox *) gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  for (i = 0; i < N_SCHEMES; ++i)
    {
      dlg->array_schemes[i] = (GtkRadioButton *)
        gtk_radio_button_new_with_label_from_widget
        (dlg->array_schemes[0], string_schemes[i]);
      g_signal_connect_swapped (dlg->array_schemes[i], "clicked",
                                (void (*)()) dialog_scheme_update, dlg);
      gtk_container_add (GTK_CONTAINER (dlg->box_schemes),
                         GTK_WIDGET (dlg->array_schemes[i]));
    }
  dlg->frame_schemes = (GtkFrame *) gtk_frame_new (_("Numerical scheme"));
  gtk_container_add ((GtkContainer *) dlg->frame_schemes,
                     (GtkWidget *) dlg->box_schemes);

  dlg->array_limiters[0] = 0;
  dlg->box_limiters = (GtkBox *) gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  for (i = 0; i < N_LIMITERS; ++i)
    {
      dlg->array_limiters[i] = (GtkRadioButton *)
        gtk_radio_button_new_with_label_from_widget
        (dlg->array_limiters[0], string_limiters[i]);
      gtk_container_add (GTK_CONTAINER (dlg->box_limiters),
                         GTK_WIDGET (dlg->array_limiters[i]));
    }
  dlg->frame_limiters = (GtkFrame *) gtk_frame_new (_("Flux limiter"));
  gtk_container_add ((GtkContainer *) dlg->frame_limiters,
                     (GtkWidget *) dlg->box_limiters);

  dlg->array_entropy[0] = 0;
  dlg->box_entropy = (GtkBox *) gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  for (i = 0; i < N_ENTROPY; ++i)
    {
      dlg->array_entropy[i] = (GtkRadioButton *)
        gtk_radio_button_new_with_label_from_widget
        (dlg->array_entropy[0], string_entropy[i]);
      gtk_container_add (GTK_CONTAINER (dlg->box_entropy),
                         GTK_WIDGET (dlg->array_entropy[i]));
    }
  dlg->frame_entropy = (GtkFrame *) gtk_frame_new (_("Entropy correction"));
  gtk_container_add ((GtkContainer *) dlg->frame_entropy,
                     (GtkWidget *) dlg->box_entropy);

  dlg->array_meshes[0] = 0;
  dlg->box_meshes = (GtkBox *) gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  for (i = 0; i < N_MESHES; ++i)
    {
      dlg->array_meshes[i] = (GtkRadioButton *)
        gtk_radio_button_new_with_label_from_widget
        (dlg->array_meshes[0], string_meshes[i]);
      g_signal_connect_swapped (dlg->array_meshes[i], "clicked",
                                (void (*)()) dialog_scheme_update, dlg);
      gtk_container_add (GTK_CONTAINER (dlg->box_meshes),
                         GTK_WIDGET (dlg->array_meshes[i]));
    }
  dlg->frame_meshes = (GtkFrame *) gtk_frame_new (_("Mesh"));
  gtk_container_add
    ((GtkContainer *) dlg->frame_meshes, (GtkWidget *) dlg->box_meshes);

  dlg->array_discretizations[0] = 0;
  dlg->box_discretizations =
    (GtkBox *) gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  for (i = 0; i < N_DISCRETIZATIONS; ++i)
    {
      dlg->array_discretizations[i] = (GtkRadioButton *)
        gtk_radio_button_new_with_label_from_widget
        (dlg->array_discretizations[0], string_discretizations[i]);
      gtk_container_add (GTK_CONTAINER (dlg->box_discretizations),
                         GTK_WIDGET (dlg->array_discretizations[i]));
    }
  dlg->frame_discretizations = (GtkFrame *) gtk_frame_new (_("Discretization"));
  gtk_container_add ((GtkContainer *) dlg->frame_discretizations,
                     (GtkWidget *) dlg->box_discretizations);

  dlg->entry_cfl = (JBWFloatEntry *) jbw_float_entry_new ();
  dlg->label_cfl = (GtkLabel *) gtk_label_new (_("CFL Number"));

  dlg->entry_implicit = (JBWFloatEntry *) jbw_float_entry_new ();
  dlg->label_implicit = (GtkLabel *) gtk_label_new (_("Implicit number"));

  dlg->entry_viscosity = (JBWFloatEntry *) jbw_float_entry_new ();
  dlg->label_viscosity = (GtkLabel *) gtk_label_new (_("Artificial viscosity"));

  dlg->entry_dxmax = (JBWFloatEntry *) jbw_float_entry_new ();
  dlg->label_dxmax = (GtkLabel *) gtk_label_new (_("Mesh variation"));

  dlg->convergence = (GtkCheckButton *)
    gtk_check_button_new_with_mnemonic (_("_Convergence"));
  g_signal_connect_swapped (dlg->convergence, "clicked",
                            (void (*)()) dialog_scheme_update, dlg);

  dlg->entry_max_cells = (GtkSpinButton *)
    gtk_spin_button_new_with_range (0., (gdouble) G_MAXULONG, 1.);
  dlg->label_max_cells = (GtkLabel *) gtk_label_new (_("Maximum mesh cells"));

  dlg->entry_convergence_number = (GtkSpinButton *)
    gtk_spin_button_new_with_range (0., (gdouble) G_MAXULONG, 1.);
  dlg->label_convergence_number =
    (GtkLabel *) gtk_label_new (_("Convergence test number"));

  dlg->table = (GtkGrid *) gtk_grid_new ();
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->convergence, 0, 0, 2, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->frame_discretizations,
                   0, 1, 2, 3);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->frame_meshes, 0, 4, 2, 4);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->label_mesh_cells, 0, 8, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->entry_mesh_cells, 1, 8, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->label_simulation_time,
                   0, 9, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->entry_simulation_time,
                   1, 9, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->label_cfl, 0, 10, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->entry_cfl, 1, 10, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->label_implicit, 0, 11, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->entry_implicit, 1, 11, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->label_viscosity, 0, 12, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->entry_viscosity, 1, 12, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->label_dxmax, 0, 13, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->entry_dxmax, 1, 13, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->label_max_cells, 0, 14, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->entry_max_cells, 1, 14, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->label_convergence_number,
                   0, 15, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->entry_convergence_number,
                   1, 15, 1, 1);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->frame_schemes, 3, 0, 1, 19);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->frame_entropy, 4, 0, 1, 7);
  gtk_grid_attach (dlg->table, (GtkWidget *) dlg->frame_limiters, 4, 7, 1, 12);

  dlg->window =
    (GtkDialog *) gtk_dialog_new_with_buttons (_("Numerical scheme"),
                                               window_parent, GTK_DIALOG_MODAL,
                                               _("_OK"), GTK_RESPONSE_OK,
                                               _("_Cancel"),
                                               GTK_RESPONSE_CANCEL, NULL);
  gtk_container_add (GTK_CONTAINER (gtk_dialog_get_content_area (dlg->window)),
                     GTK_WIDGET (dlg->table));
  gtk_widget_show_all ((GtkWidget *) dlg->window);

  gtk_spin_button_set_value (dlg->entry_mesh_cells, mesh_cells);
  jbw_float_entry_set_value (dlg->entry_simulation_time, simulation_time);
  jbw_float_entry_set_value (dlg->entry_cfl, cfl_number);
  jbw_float_entry_set_value (dlg->entry_implicit, implicit);
  jbw_float_entry_set_value (dlg->entry_viscosity, viscosity);
  jbw_float_entry_set_value (dlg->entry_dxmax, dxmax);

  jbw_array_radio_buttons_set_active (dlg->array_schemes, scheme_type, 1);
  jbw_array_radio_buttons_set_active
    (dlg->array_limiters, flux_limiter_type, 1);
  jbw_array_radio_buttons_set_active (dlg->array_entropy, entropy_type, 1);
  jbw_array_radio_buttons_set_active (dlg->array_meshes, mesh_type, 1);
  jbw_array_radio_buttons_set_active
    (dlg->array_discretizations, discretization_type, 1);

  gtk_toggle_button_set_active
    ((GtkToggleButton *) dlg->convergence, dialog_convergence);
  gtk_spin_button_set_value (dlg->entry_max_cells, max_cells);
  gtk_spin_button_set_value (dlg->entry_convergence_number, convergence_number);

  dialog_scheme_update (dlg);

  if (gtk_dialog_run (dlg->window) == GTK_RESPONSE_OK)
    {
      mesh_cells = gtk_spin_button_get_value (dlg->entry_mesh_cells);
      simulation_time = jbw_float_entry_get_value (dlg->entry_simulation_time);
      cfl_number = jbw_float_entry_get_value (dlg->entry_cfl);
      implicit = jbw_float_entry_get_value (dlg->entry_implicit);
      viscosity = jbw_float_entry_get_value (dlg->entry_viscosity);
      dxmax = jbw_float_entry_get_value (dlg->entry_dxmax);
      scheme_type = jbw_array_radio_buttons_get_active (dlg->array_schemes);
      flux_limiter_type =
        jbw_array_radio_buttons_get_active (dlg->array_limiters);
      entropy_type = jbw_array_radio_buttons_get_active (dlg->array_entropy);
      mesh_type = jbw_array_radio_buttons_get_active (dlg->array_meshes);
      discretization_type =
        jbw_array_radio_buttons_get_active (dlg->array_discretizations);
      dialog_convergence =
        gtk_toggle_button_get_active ((GtkToggleButton *) dlg->convergence);
      max_cells = gtk_spin_button_get_value (dlg->entry_max_cells);
      convergence_number =
        gtk_spin_button_get_value (dlg->entry_convergence_number);
    }
  gtk_widget_destroy ((GtkWidget *) dlg->window);
}
