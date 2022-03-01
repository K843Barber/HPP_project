CFLAGS=-Wall -g -Wpedantic -p
COPTS=-Ofast -march=native
INCLUDES=-I/opt/X11/include
LDFLAGS=-lm -L/opt/X11/lib -lX11
VERSION=4

galsim: build/galsim.o build/forces.o build/quad.o build/particle.o build/vector.o build/graphics.o build/box_bounds.o
	gcc -o $@ $^ $(LDFLAGS)

build/graphics.o: graphics/graphics.c
	gcc -o $@ $(CFLAGS) $(COPTS) $(INCLUDES) -c graphics/graphics.c

build/%.o: src/%.c | build
	gcc -o $@ $(CFLAGS) $(COPTS) $(INCLUDES) -DVERSION=$(VERSION) -c $<

clean: 
	rm -f -r build *.o galsim

build: 
	mkdir build

A4.tar.gz: src/*.c src/*.h Makefile graphics/graphics.c graphics/graphics.h best_timing.txt report.pdf input_data/* ref_output_data/* compare_gal_files/*
	tar -czvf $@ $^