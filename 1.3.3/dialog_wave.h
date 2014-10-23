#ifndef DIALOG_WAVE_H
#define DIALOG_WAVE_H 1

#include "jb/jb_win.h"

#define N_WAVES_TYPE 2
#define N_WAVES_INITIAL 5

typedef struct
{
	GtkRadioButton *array_buttons_type[N_WAVES_TYPE], 
		*array_buttons_initial[N_WAVES_INITIAL];
	GtkLabel *label_shape_position, *label_shape_width, *label_shape_amplitude, 
		*label_shape_base, *label_domain_width, *label_wave_velocity;
	JBWFloatEntry *entry_shape_position, *entry_shape_width, 
		*entry_shape_amplitude, *entry_shape_base, *entry_domain_width, 
		*entry_wave_velocity;
	GtkBox *box_wave_type, *box_shape_initial, *box_frames;
	GtkFrame *frame_wave_type, *frame_shape_initial;
	GtkGrid *table;
	GtkDialog *window;
} DialogWave;

void dialog_wave_actualize(DialogWave*);
void dialog_wave_create();

#endif
