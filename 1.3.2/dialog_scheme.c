#include "scheme.h"
#include "dialog_scheme.h"

void dialog_scheme_actualize(DialogScheme *dlg)
{
	int k;
	gtk_widget_set_sensitive((GtkWidget*)dlg->entry_viscosity,0);
	gtk_widget_set_sensitive((GtkWidget*)dlg->label_viscosity,0);
	gtk_widget_set_sensitive((GtkWidget*)dlg->entry_implicit,0);
	gtk_widget_set_sensitive((GtkWidget*)dlg->label_implicit,0);
	gtk_widget_set_sensitive((GtkWidget*)dlg->frame_limiters,0);
	gtk_widget_set_sensitive((GtkWidget*)dlg->frame_entropy,0);
	k=jbw_array_radio_buttons_get_active(dlg->array_schemes);
	if (k>1) gtk_widget_set_sensitive((GtkWidget*)dlg->frame_entropy,1);
	if (k==0)
	{
		gtk_widget_set_sensitive((GtkWidget*)dlg->entry_viscosity,1);
		gtk_widget_set_sensitive((GtkWidget*)dlg->label_viscosity,1);
	}
	else if (k>=11 && k<=15)
	{
		if (k!=12 && k!=14)
		{
			gtk_widget_set_sensitive((GtkWidget*)dlg->entry_implicit,1);
			gtk_widget_set_sensitive((GtkWidget*)dlg->label_implicit,1);
		}
	}
	else if (k>=6)
		gtk_widget_set_sensitive((GtkWidget*)dlg->frame_limiters,1);
	k=jbw_array_radio_buttons_get_active(dlg->array_meshes);
	gtk_widget_set_sensitive((GtkWidget*)dlg->entry_dxmax,k);
	gtk_widget_set_sensitive((GtkWidget*)dlg->label_dxmax,k);
	k=gtk_toggle_button_get_active((GtkToggleButton*)dlg->convergence);
	gtk_widget_set_sensitive((GtkWidget*)dlg->entry_max_cells,k);
	gtk_widget_set_sensitive((GtkWidget*)dlg->label_max_cells,k);
	gtk_widget_set_sensitive((GtkWidget*)dlg->entry_convergence_number,k);
	gtk_widget_set_sensitive((GtkWidget*)dlg->label_convergence_number,k);
}

void dialog_scheme_create()
{
	int i;
	char *string_schemes[N_SCHEMES]={"Lax-Friedrichs",
		gettext("Lax-Friedrichs optimized"),gettext("1st order upwind"),
		"Lax-Wendroff","Warming-Beam","McCormack",gettext("2nd order upwind"),
		gettext("Classical spatial 2nd order TVD"),
		gettext("Spatial 2nd order TVD"),"Roe-Sweby TVD",
		gettext("2nd order TVD"),gettext("Tridiagonal upwind implicit"),
		gettext("Optimized tridiagonal implicit"),
		gettext("Bidiagonal upwind implicit"),
		gettext("Optimized bidiagonal implicit"),"Casier",
		gettext("Tridiagonal upwind semi-explicit"),
		gettext("Bidiagonal upwind semi-explicit")};
	char *string_limiters[N_LIMITERS]={"Superbee","Van-Leer","Van-Albada",
		"Minmod","Supermin","Minsuper",gettext("None"),gettext("Centered"),
		gettext("Total"),"Burguete",gettext("Mean")};
	char *string_entropy[N_ENTROPY]={gettext("None"),"Harten-Hyman",
		gettext("Mean"),gettext("Double mean"),gettext("Interpolated"),
		gettext("Interpolated mean")};
	char *string_meshes[N_MESHES]=
		{gettext("Regular"),gettext("Progressive"),gettext("Aleatory")};
	char *string_discretizations[N_DISCRETIZATIONS]=
		{gettext("Finite differences"),gettext("Finite volumes")};
	DialogScheme dlg[1];

	dlg->entry_mesh_cells=(JBWIntEntry*)jbw_int_entry_new();
	dlg->label_mesh_cells=(GtkLabel*)gtk_label_new(gettext("Mesh cells"));

	dlg->entry_simulation_time=(JBWFloatEntry*)jbw_float_entry_new();
	dlg->label_simulation_time=
		(GtkLabel*)gtk_label_new(gettext("Simulation time"));

	dlg->array_schemes[0]=0;
	dlg->box_schemes=(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	for (i=0; i<N_SCHEMES; ++i)
	{
		dlg->array_schemes[i]=(GtkRadioButton*)
			gtk_radio_button_new_with_label_from_widget
				(dlg->array_schemes[0],string_schemes[i]);
		g_signal_connect_swapped(dlg->array_schemes[i],"clicked",
			(void(*)())dialog_scheme_actualize,dlg);
		gtk_container_add(GTK_CONTAINER(dlg->box_schemes),
			GTK_WIDGET(dlg->array_schemes[i]));
	}
	dlg->frame_schemes=(GtkFrame*)gtk_frame_new(gettext("Numerical scheme"));
	gtk_container_add((GtkContainer*)dlg->frame_schemes,
		(GtkWidget*)dlg->box_schemes);

	dlg->array_limiters[0]=0;
	dlg->box_limiters=(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	for (i=0; i<N_LIMITERS; ++i)
	{
		dlg->array_limiters[i]=(GtkRadioButton*)
			gtk_radio_button_new_with_label_from_widget
				(dlg->array_limiters[0],string_limiters[i]);
		gtk_container_add(GTK_CONTAINER(dlg->box_limiters),
			GTK_WIDGET(dlg->array_limiters[i]));
	}
	dlg->frame_limiters=(GtkFrame*)gtk_frame_new(gettext("Flux limiter"));
	gtk_container_add((GtkContainer*)dlg->frame_limiters,
		(GtkWidget*)dlg->box_limiters);

	dlg->array_entropy[0]=0;
	dlg->box_entropy=(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	for (i=0; i<N_ENTROPY; ++i)
	{
		dlg->array_entropy[i]=(GtkRadioButton*)
			gtk_radio_button_new_with_label_from_widget
				(dlg->array_entropy[0],string_entropy[i]);
		gtk_container_add(GTK_CONTAINER(dlg->box_entropy),
			GTK_WIDGET(dlg->array_entropy[i]));
	}
	dlg->frame_entropy=(GtkFrame*)gtk_frame_new(gettext("Entropy correction"));
	gtk_container_add((GtkContainer*)dlg->frame_entropy,
		(GtkWidget*)dlg->box_entropy);

	dlg->array_meshes[0]=0;
	dlg->box_meshes=(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	for (i=0; i<N_MESHES; ++i)
	{
		dlg->array_meshes[i]=(GtkRadioButton*)
			gtk_radio_button_new_with_label_from_widget
				(dlg->array_meshes[0],string_meshes[i]);
		g_signal_connect_swapped(dlg->array_meshes[i],"clicked",
			(void(*)())dialog_scheme_actualize,dlg);
		gtk_container_add(GTK_CONTAINER(dlg->box_meshes),
			GTK_WIDGET(dlg->array_meshes[i]));
	}
	dlg->frame_meshes=(GtkFrame*)gtk_frame_new(gettext("Mesh"));
	gtk_container_add
		((GtkContainer*)dlg->frame_meshes,(GtkWidget*)dlg->box_meshes);

	dlg->array_discretizations[0]=0;
	dlg->box_discretizations=(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	for (i=0; i<N_DISCRETIZATIONS; ++i)
	{
		dlg->array_discretizations[i]=(GtkRadioButton*)
			gtk_radio_button_new_with_label_from_widget
				(dlg->array_discretizations[0],string_discretizations[i]);
		gtk_container_add(GTK_CONTAINER(dlg->box_discretizations),
			GTK_WIDGET(dlg->array_discretizations[i]));
	}
	dlg->frame_discretizations=
		(GtkFrame*)gtk_frame_new(gettext("Discretization"));
	gtk_container_add((GtkContainer*)dlg->frame_discretizations,
		(GtkWidget*)dlg->box_discretizations);

	dlg->entry_cfl=(JBWFloatEntry*)jbw_float_entry_new();
	dlg->label_cfl=(GtkLabel*)gtk_label_new(gettext("CFL Number"));
	
	dlg->entry_implicit=(JBWFloatEntry*)jbw_float_entry_new();
	dlg->label_implicit=(GtkLabel*)gtk_label_new(gettext("Implicit number"));
	
	dlg->entry_viscosity=(JBWFloatEntry*)jbw_float_entry_new();
	dlg->label_viscosity=
		(GtkLabel*)gtk_label_new(gettext("Artificial viscosity"));

	dlg->entry_dxmax=(JBWFloatEntry*)jbw_float_entry_new();
	dlg->label_dxmax=(GtkLabel*)gtk_label_new(gettext("Mesh variation"));

	dlg->convergence=(GtkCheckButton*)
		gtk_check_button_new_with_mnemonic(gettext("_Convergence"));
	g_signal_connect_swapped(dlg->convergence,"clicked",
		(void(*)())dialog_scheme_actualize,dlg);

	dlg->entry_max_cells=(JBWIntEntry*)jbw_int_entry_new();
	dlg->label_max_cells=
		(GtkLabel*)gtk_label_new(gettext("Maximum mesh cells"));

	dlg->entry_convergence_number=(JBWIntEntry*)jbw_int_entry_new();
	dlg->label_convergence_number=
		(GtkLabel*)gtk_label_new(gettext("Convergence test number"));

	dlg->table=(GtkTable*)gtk_table_new(0,0,0);
	gtk_table_attach_defaults(dlg->table,
		(GtkWidget*)dlg->frame_schemes,0,1,0,2);
	gtk_table_attach(dlg->table,
		(GtkWidget*)dlg->frame_entropy,0,1,2,8,GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,(GtkWidget*)dlg->frame_discretizations,0,1,8,11,
		GTK_FILL|GTK_EXPAND,GTK_FILL,0,0);
	gtk_table_attach_defaults(dlg->table,
		(GtkWidget*)dlg->frame_limiters,1,3,0,1);
	gtk_table_attach_defaults(dlg->table,
		(GtkWidget*)dlg->frame_meshes,1,3,1,2);
	gtk_table_attach(dlg->table,
		(GtkWidget*)dlg->label_mesh_cells,1,2,2,3,GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,(GtkWidget*)dlg->entry_mesh_cells,2,3,2,3,
		GTK_EXPAND|GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,
		(GtkWidget*)dlg->label_simulation_time,1,2,3,4,GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,(GtkWidget*)dlg->entry_simulation_time,2,3,3,4,
		GTK_EXPAND|GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,
		(GtkWidget*)dlg->label_cfl,1,2,4,5,GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,
		(GtkWidget*)dlg->entry_cfl,2,3,4,5,GTK_EXPAND|GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,
		(GtkWidget*)dlg->label_implicit,1,2,5,6,GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,(GtkWidget*)dlg->entry_implicit,2,3,5,6,
		GTK_EXPAND|GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,
		(GtkWidget*)dlg->label_viscosity,1,2,6,7,GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,(GtkWidget*)dlg->entry_viscosity,2,3,6,7,
		GTK_EXPAND|GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,
		(GtkWidget*)dlg->label_dxmax,1,2,7,8,GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,
		(GtkWidget*)dlg->entry_dxmax,2,3,7,8,GTK_EXPAND|GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,
		(GtkWidget*)dlg->label_max_cells,1,2,8,9,GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,(GtkWidget*)dlg->entry_max_cells,2,3,8,9,
		GTK_EXPAND|GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,(GtkWidget*)dlg->label_convergence_number,
		1,2,9,10,GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,(GtkWidget*)dlg->entry_convergence_number,
		2,3,9,10,GTK_EXPAND|GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(dlg->table,(GtkWidget*)dlg->convergence,1,3,10,11,
		GTK_EXPAND|GTK_FILL,GTK_FILL,0,0);

	dlg->window=(GtkDialog*)gtk_dialog_new_with_buttons(
		gettext("Numerical scheme"),window_parent,GTK_DIALOG_MODAL,
		GTK_STOCK_OK,GTK_RESPONSE_OK,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,NULL);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dlg->window)),
		GTK_WIDGET(dlg->table));
	gtk_widget_show_all((GtkWidget*)dlg->window);

	jbw_int_entry_set_value(dlg->entry_mesh_cells,mesh_cells);
	jbw_float_entry_set_value(dlg->entry_simulation_time,simulation_time);
	jbw_float_entry_set_value(dlg->entry_cfl,cfl_number);
	jbw_float_entry_set_value(dlg->entry_implicit,implicit);
	jbw_float_entry_set_value(dlg->entry_viscosity,viscosity);
	jbw_float_entry_set_value(dlg->entry_dxmax,dxmax);

	jbw_array_radio_buttons_set_active(dlg->array_schemes,scheme_type,1);
	jbw_array_radio_buttons_set_active(dlg->array_limiters,flux_limiter_type,1);
	jbw_array_radio_buttons_set_active(dlg->array_entropy,entropy_type,1);
	jbw_array_radio_buttons_set_active(dlg->array_meshes,mesh_type,1);
	jbw_array_radio_buttons_set_active
		(dlg->array_discretizations,discretization_type,1);

	gtk_toggle_button_set_active
		((GtkToggleButton*)dlg->convergence,dialog_convergence);
	jbw_int_entry_set_value(dlg->entry_max_cells,max_cells);
	jbw_int_entry_set_value(dlg->entry_convergence_number,convergence_number);

	dialog_scheme_actualize(dlg);

	if (gtk_dialog_run(dlg->window)==GTK_RESPONSE_OK)
	{
		mesh_cells=jbw_int_entry_get_value(dlg->entry_mesh_cells);
		simulation_time=jbw_float_entry_get_value(dlg->entry_simulation_time);
		cfl_number=jbw_float_entry_get_value(dlg->entry_cfl);
		implicit=jbw_float_entry_get_value(dlg->entry_implicit);
		viscosity=jbw_float_entry_get_value(dlg->entry_viscosity);
		dxmax=jbw_float_entry_get_value(dlg->entry_dxmax);
		scheme_type=jbw_array_radio_buttons_get_active(dlg->array_schemes);
		flux_limiter_type=
			jbw_array_radio_buttons_get_active(dlg->array_limiters);
		entropy_type=jbw_array_radio_buttons_get_active(dlg->array_entropy);
		mesh_type=jbw_array_radio_buttons_get_active(dlg->array_meshes);
		discretization_type=
			jbw_array_radio_buttons_get_active(dlg->array_discretizations);
		dialog_convergence=
			gtk_toggle_button_get_active((GtkToggleButton*)dlg->convergence);
		max_cells=jbw_int_entry_get_value(dlg->entry_max_cells);
		convergence_number=
			jbw_int_entry_get_value(dlg->entry_convergence_number);
	}
	gtk_widget_destroy((GtkWidget*)dlg->window);
}
