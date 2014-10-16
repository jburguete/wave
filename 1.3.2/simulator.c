#define _GNU_SOURCE
#include <time.h>
#include "wave.h"
#include "scheme.h"
#include "dialog_wave.h"
#include "dialog_scheme.h"
#include "dialog_options.h"
#include "simulator.h"

extern void draw_graphic();

void DialogHelpCreate()
{
	jbw_show_message(gettext("Help"),
		gettext("wave v. 1.3.2 english\n"
			"Unsteady propagation wave program\n"
			"Author: Javier Burguete\n"
			"E-mail: jburguete@eead.csic.es\n"
			"Phone: (+034) 976491996"),
		GTK_MESSAGE_INFO);
}

void DialogSimulatorClose()
{
	#if DEBUG_LEVEL && DEBUG_FILE
		fclose(stdout);
	#endif
	gtk_widget_destroy((GtkWidget*)dialog_simulator.window);
	glutLeaveMainLoop();
}

void DialogSimulatorStart()
{
	char *file_name="1";
	n=mesh_cells;
	tf=simulation_time;
	cfl=cfl_number;
	o=implicit;
	pv=viscosity;
	convergence=dialog_convergence;
	nmax=max_cells;
	nc=convergence_number;
	if (simulating) simulating=0;
	else
	{
		dialog_simulator.time0 = time(NULL);
		execute(file_name);
		DialogSimulatorActualize();
	}
}

void DialogSimulatorSaveGraphical()
{
	char *buffer=0;
	GtkFileChooserDialog *dlg;
	dlg = (GtkFileChooserDialog*)gtk_file_chooser_dialog_new
		(gettext("Save graphical"), dialog_simulator.window,
		GTK_FILE_CHOOSER_ACTION_SAVE,
		GTK_STOCK_OK,GTK_RESPONSE_OK,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,NULL);
	gtk_container_set_border_width((GtkContainer*)dlg,10);
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dlg),1);
	if (gtk_dialog_run((GtkDialog*)dlg)==GTK_RESPONSE_OK)
		buffer=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dlg));
	gtk_widget_destroy((GtkWidget*)dlg);
	if (buffer)
	{
		while (gtk_events_pending()) gtk_main_iteration();
		jbw_graphic_expose_event();
		jbw_graphic_save(dialog_simulator.graphic,buffer,JBW_GRAPHIC_TYPE_PNG);
		g_free(buffer);
	}
}

void DialogSimulatorActualize()
{
	register DialogSimulator *dlg=&dialog_simulator;
	gtk_action_set_sensitive(dlg->action[4],!simulating);
	gtk_action_set_sensitive(dlg->action[5],simulating);
	glutPostRedisplay();
}

#if JBW==JBW_GTK_GL
void DialogSimulatorCreate(GdkGLConfig *glconfig)
#else
void DialogSimulatorCreate()
#endif
{
	int i;
	DialogSimulator *dlg=&dialog_simulator;

	char *str_menu[]={gettext("_Wave type"),gettext("_Numerical scheme"),
		gettext("_Drawing options"),0,gettext("S_tart simulation"),
		gettext("Sto_p simulation"),gettext("_Save graphical"),0,
		gettext("_Help"),0,gettext("E_xit")},
	*image_menu[]={GTK_STOCK_PREFERENCES,GTK_STOCK_PROPERTIES,
		GTK_STOCK_CONVERT,0,GTK_STOCK_EXECUTE,
		GTK_STOCK_STOP,GTK_STOCK_SAVE,0,
		GTK_STOCK_HELP,0,GTK_STOCK_QUIT},
	*str_tooltip[]={gettext("Wave type"),gettext("Numerical scheme"),
		gettext("Drawing options"),0,gettext("Start simulation"),
		gettext("Stop simulation"),gettext("Save graphical"),0,
		gettext("Help"),0,gettext("Exit")};

	dlg->menu_bar=(GtkMenuBar*)gtk_menu_bar_new();
	dlg->menu=
		(GtkMenuItem*)gtk_menu_item_new_with_mnemonic(gettext("_Menu"));
	gtk_menu_shell_append((GtkMenuShell*)dlg->menu_bar,(GtkWidget*)dlg->menu);

	dlg->menu_items=(GtkMenu*)gtk_menu_new();
	dlg->toolbar=(GtkToolbar*)gtk_toolbar_new();
	for (i=0; i<N_DIALOG_SIMULATOR_ITEMS; ++i)
	{
		if (str_menu[i])
		{
			dlg->action[i]=gtk_action_new
				(str_menu[i],str_menu[i],str_tooltip[i],image_menu[i]);
			dlg->item[i]=
				(GtkMenuItem*)gtk_action_create_menu_item(dlg->action[i]);
			dlg->tool_item[i]=
				(GtkToolItem*)gtk_action_create_tool_item(dlg->action[i]);
		}
		else
		{
			dlg->item[i]=(GtkMenuItem*)gtk_separator_menu_item_new();
			dlg->tool_item[i]=gtk_separator_tool_item_new();
		}
		gtk_menu_shell_append
			((GtkMenuShell*)dlg->menu_items,(GtkWidget*)dlg->item[i]);
		gtk_toolbar_insert(dlg->toolbar,dlg->tool_item[i],-1);
	}
	gtk_menu_item_set_submenu
		((GtkMenuItem*)dlg->menu,(GtkWidget*)dlg->menu_items);

	g_signal_connect(dlg->action[0],"activate",dialog_wave_create,NULL);
	g_signal_connect(dlg->action[1],"activate",dialog_scheme_create,NULL);
	g_signal_connect(dlg->action[2],"activate",DialogOptionsCreate,NULL);
	g_signal_connect(dlg->action[4],"activate",DialogSimulatorStart,NULL);
	g_signal_connect(dlg->action[5],"activate",DialogSimulatorStart,NULL);
	g_signal_connect
		(dlg->action[6],"activate",DialogSimulatorSaveGraphical,NULL);
	g_signal_connect(dlg->action[8],"activate",DialogHelpCreate,NULL);
	g_signal_connect(dlg->action[10],"activate",DialogSimulatorClose,NULL);

	dlg->graphic=jbw_graphic_new(0,6,6,0,draw_graphic);
	jbw_graphic_set_xlabel(dlg->graphic,"x");
	jbw_graphic_set_ylabel(dlg->graphic,"y");
	dlg->graphic->resize = 0;

	dlg->label_simulation_time=
		(GtkLabel*)gtk_label_new(gettext("Simulation time"));
	gtk_widget_set_sensitive
		((GtkWidget*)dlg->label_simulation_time,0);
	dlg->progress_simulation_time=(GtkProgressBar*)gtk_progress_bar_new();
	gtk_widget_set_sensitive
		((GtkWidget*)dlg->progress_simulation_time,0);
	
	dlg->label_cpu_time=(GtkLabel*)gtk_label_new(gettext("CPU time"));
	gtk_widget_set_sensitive((GtkWidget*)dlg->label_cpu_time,0);
	dlg->entry_cpu_time=(JBWFloatEntry*)jbw_float_entry_new();
	gtk_widget_set_sensitive((GtkWidget*)dlg->entry_cpu_time,0);
	dlg->label_initial_mass=(GtkLabel*)gtk_label_new(gettext("Initial mass"));
	gtk_widget_set_sensitive((GtkWidget*)dlg->label_initial_mass,0);
	dlg->entry_initial_mass=(JBWFloatEntry*)jbw_float_entry_new();
	gtk_widget_set_sensitive((GtkWidget*)dlg->entry_initial_mass,0);
	dlg->label_final_mass=(GtkLabel*)gtk_label_new(gettext("Final mass"));
	gtk_widget_set_sensitive((GtkWidget*)dlg->label_final_mass,0);
	dlg->entry_final_mass=(JBWFloatEntry*)jbw_float_entry_new();
	gtk_widget_set_sensitive((GtkWidget*)dlg->entry_final_mass,0);

	dlg->box_bar=(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_box_pack_start
		(dlg->box_bar,(GtkWidget*)dlg->label_simulation_time,0,1,0);
	gtk_container_add
		(GTK_CONTAINER(dlg->box_bar),(GtkWidget*)dlg->progress_simulation_time);
	gtk_box_pack_start
		(dlg->box_bar,(GtkWidget*)dlg->label_cpu_time,0,1,0);
	gtk_container_add
		(GTK_CONTAINER(dlg->box_bar),(GtkWidget*)dlg->entry_cpu_time);
	gtk_box_pack_start
		(dlg->box_bar,(GtkWidget*)dlg->label_initial_mass,0,1,0);
	gtk_container_add
		(GTK_CONTAINER(dlg->box_bar),(GtkWidget*)dlg->entry_initial_mass);
	gtk_box_pack_start
		(dlg->box_bar,(GtkWidget*)dlg->label_final_mass,0,1,0);
	gtk_container_add
		(GTK_CONTAINER(dlg->box_bar),(GtkWidget*)dlg->entry_final_mass);

	dlg->button_bar=(GtkButton*)gtk_button_new();
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_bar,0);
	gtk_container_add((GtkContainer*)dlg->button_bar,(GtkWidget*)dlg->box_bar);

	dlg->box=(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_box_pack_start(dlg->box,(GtkWidget*)dlg->menu_bar,0,1,0);
	gtk_box_pack_start(dlg->box,(GtkWidget*)dlg->toolbar,0,1,0);
	gtk_box_pack_start(dlg->box,(GtkWidget*)dlg->button_bar,0,1,0);

	dlg->window=(GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(dlg->window,gettext("Waves simulator"));
	gtk_container_add((GtkContainer*)dlg->window,(GtkWidget*)dlg->box);
	gtk_widget_show_all((GtkWidget*)dlg->window);
	g_signal_connect(dlg->window,"delete_event",DialogSimulatorClose,NULL);

	window_parent = dlg->window;

	DialogSimulatorActualize();

	draw();
}
