#ifndef DIALOG_OPTIONS_H
#define DIALOG_OPTIONS_H 1

#include "jb/jb_win.h"

typedef struct
{
  GtkLabel *label_xmax, *label_xmin, *label_ymax, *label_ymin;
  JBWFloatEntry *entry_xmax, *entry_xmin, *entry_ymax, *entry_ymin;
  GtkGrid *table_x, *table_y;
  GtkFrame *frame_x, *frame_y;
  GtkDialog *window;
} DialogOptions;

void dialog_options_create ();

extern void draw ();
extern JBDOUBLE draw_xmin, draw_xmax, draw_ymin, draw_ymax;

#endif
