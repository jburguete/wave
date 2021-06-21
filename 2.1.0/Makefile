.PHONY: strip clean

CC = gcc -flto -g
obj = tools.o domain1d.o shape1d.o wave1d.o cell1d.o mesh1d.o wave.o
pkg = glib-2.0 libxml-2.0 libgtop-2.0
inc = `pkg-config --cflags $(pkg)`
lib = -lm `pkg-config --libs $(pkg)` -Ljb -ljb
flags = -c -O3 -Wall -Wextra -DJBM_PRECISION=5 -DJBW=1
jb = jb/jb_xml.h jb/jb_win.h jb/jb_math.h jb/jb_def.h
config = config.h $(jb) Makefile
tools = tools.h $(config)
wave1d = wave1d.h shape1d.h domain1d.h $(tools) 

all: wave wave.pdf

tools.o: tools.c $(tools)
	$(CC) tools.c -o tools.o $(inc) $(flags)

domain1d.o: domain1d.c domain1d.h $(tools)
	$(CC) domain1d.c -o domain1d.o $(inc) $(flags)

shape1d.o: shape1d.c shape1d.h $(tools)
	$(CC) shape1d.c -o shape1d.o $(inc) $(flags)

wave1d.o: wave1d.c $(wave1d)
	$(CC) wave1d.c -o wave1d.o $(inc) $(flags)

cell1d.o: cell1d.c cell1d.h $(tools)
	$(CC) cell1d.c -o cell1d.o $(inc) $(flags)

mesh1d.o: mesh1d.c mesh1d.h mesh_type.h cell1d.h wave1d.h shape1d.h domain1d.h \
	$(config)
	$(CC) mesh1d.c -o mesh1d.o $(inc) $(flags)

wave.o: wave.c wave.h mesh1d.h cell1d.h wave1d.h shape1d.h domain1d.h $(config)
	$(CC) wave.c -o wave.o $(inc) $(flags)

wave: $(obj)
	$(CC) $(obj) -o wave $(lib)

wave.pdf: wave.tex makefile
	latex wave
	latex wave
	dvipdf wave

strip:
	make
	strip wave

clean:
	rm wave *.o
