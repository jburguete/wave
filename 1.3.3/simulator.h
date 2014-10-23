#ifndef SIMULATOR_H
#define SIMULATOR_H 1

#include "jb/jb_win.h"

#define N_DIALOG_SIMULATOR_ITEMS 11

typedef struct
{
	long time0;
	GtkLabel *label_simulation_time,*label_cpu_time,*label_initial_mass,
		*label_final_mass;
	GtkAction *action[N_DIALOG_SIMULATOR_ITEMS];
	GtkToolItem *tool_item[N_DIALOG_SIMULATOR_ITEMS];
	GtkButton *button_bar;
	GtkEntry *entry_cpu_time,*entry_initial_mass,*entry_final_mass;
	JBWGraphic *graphic;
	GtkProgressBar *progress_simulation_time;
	GtkMenuItem *item[N_DIALOG_SIMULATOR_ITEMS];
	GtkMenuItem *menu;
	GtkMenu *menu_items;
	GtkMenuBar *menu_bar;
	GtkBox *box_bar, *box;
	GtkToolbar *toolbar;
	GtkWindow *window;
} DialogSimulator;

extern DialogSimulator dialog_simulator;

void DialogHelpCreate();
void DialogSimulatorClose();
void DialogSimulatorStart();
void DialogSimulatorActualize();
void DialogSimulatorCreate();

#endif
