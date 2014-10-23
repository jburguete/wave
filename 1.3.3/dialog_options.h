#ifndef DIALOG_OPTIONS_H
#define DIALOG_OPTIONS_H 1

#include "jb/jb_win.h"

extern void draw();

void DialogOptionsCreate();

typedef struct
{
	GtkLabel *label_xmax, *label_xmin, *label_ymax, *label_ymin;
	JBWFloatEntry *entry_xmax, *entry_xmin, *entry_ymax, *entry_ymin;
	GtkGrid *table_x, *table_y;
	GtkFrame *frame_x, *frame_y;
	GtkDialog *window;
} DialogOptions;

extern JBDOUBLE draw_xmin, draw_xmax, draw_ymin, draw_ymax;

#endif
