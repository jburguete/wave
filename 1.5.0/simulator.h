#ifndef SIMULATOR_H
#define SIMULATOR_H 1

#include "jb/jb_win.h"

#define N_DIALOG_SIMULATOR_ITEMS 8

typedef struct
{
  long time0;
  GtkLabel *label_simulation_time, *label_cpu_time, *label_initial_mass,
    *label_final_mass;
  GtkButton *button_item[N_DIALOG_SIMULATOR_ITEMS];
  GtkEntry *entry_cpu_time, *entry_initial_mass, *entry_final_mass;
  JBWGraphic *graphic;
  GtkProgressBar *progress_simulation_time;
  GtkBox *box_items, *box_bar, *box;
  GtkWindow *window;
} DialogSimulator;

extern DialogSimulator dialog_simulator;

void dialog_simulator_update ();
void dialog_simulator_create ();

#endif
