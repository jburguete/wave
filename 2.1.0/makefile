obj = jb_def.o jb_math.o jb_win.o jb_xml.o domain1d.o shape1d.o wave1d.o \
	mesh1d.o wave.o
inc = `pkg-config --cflags glib-2.0 libxml-2.0`
lib = -lm `pkg-config --libs glib-2.0 libxml-2.0`
flags = -c -O3 -DJBW=1 -DJBW_DRAW=2 -DJBW_GRAPHIC=2
jb = jb/jb_xml.h jb/jb_win.h jb/jb_math.h jb/jb_def.h makefile
config = def.h config.h $(jb)

all: wave wave.pdf

jb_def.o: jb/jb_def.c jb/jb_def.h jb/jb_config.h makefile
	gcc jb/jb_def.c -o jb_def.o $(inc) $(flags)

jb_math.o: jb/jb_math.c jb/jb_def.h jb/jb_config.h makefile
	gcc jb/jb_math.c -o jb_math.o $(inc) $(flags)

jb_win.o: jb/jb_win.c jb/jb_def.h jb/jb_config.h makefile
	gcc jb/jb_win.c -o jb_win.o $(inc) $(flags)

jb_xml.o: jb/jb_xml.c jb/jb_def.h jb/jb_config.h makefile
	gcc jb/jb_xml.c -o jb_xml.o $(inc) $(flags)

domain1d.o: domain1d.c domain1d.h $(config)
	gcc domain1d.c -o domain1d.o $(inc) $(flags)

shape1d.o: shape1d.c shape1d.h $(config)
	gcc shape1d.c -o shape1d.o $(inc) $(flags)

wave1d.o: wave1d.c wave1d.h shape1d.h domain1d.h $(config)
	gcc wave1d.c -o wave1d.o $(inc) $(flags)

cell1d.o: cell1d.c cell1d.h $(config)
	gcc cell1d.c -o cell1d.o $(inc) $(flags)

mesh1d.o: mesh1d.c mesh1d.h mesh_type.h cell1d.h wave1d.h shape1d.h domain1d.h \
	$(config)
	gcc mesh1d.c -o mesh1d.o $(inc) $(flags)

wave.o: wave.c wave.h mesh1d.h cell1d.h wave1d.h shape1d.h domain1d.h $(config)
	gcc wave.c -o wave.o $(inc) $(flags)

wave: $(obj)
	gcc $(obj) -o wave $(lib)

wave.pdf: wave.tex makefile
	latex wave
	latex wave
	dvipdf wave
