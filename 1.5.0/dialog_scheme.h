#ifndef DIALOG_SCHEME_H
#define DIALOG_SCHEME_H 1

#include "jb/jb_win.h"

#define N_SCHEMES 18
#define N_LIMITERS 11
#define N_ENTROPY 6
#define N_MESHES 3
#define N_DISCRETIZATIONS 2

typedef struct
{
  GtkLabel *label_mesh_cells, *label_simulation_time, *label_cfl,
    *label_implicit, *label_viscosity, *label_dxmax,
    *label_max_cells, *label_convergence_number;
  GtkSpinButton *entry_mesh_cells, *entry_max_cells, *entry_convergence_number;
  JBWFloatEntry *entry_simulation_time, *entry_cfl,
    *entry_implicit, *entry_viscosity, *entry_dxmax;
  GtkCheckButton *convergence;
  GtkRadioButton *array_schemes[N_SCHEMES], *array_limiters[N_LIMITERS],
    *array_entropy[N_ENTROPY], *array_meshes[N_MESHES],
    *array_discretizations[N_DISCRETIZATIONS];
  GtkBox *box_schemes, *box_limiters, *box_entropy, *box_meshes,
    *box_discretizations;
  GtkFrame *frame_schemes, *frame_limiters, *frame_entropy, *frame_meshes,
    *frame_convergence, *frame_discretizations;
  GtkGrid *table;
  GtkDialog *window;
} DialogScheme;

void dialog_scheme_create ();

#endif
