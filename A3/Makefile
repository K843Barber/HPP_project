CFLAGS=-Wall -g -Wpedantic
CFLAGS1=-O0
CFLAGS2=-O3
CFLAGS3=-Ofast -march=native
INCLUDES=-I/opt/X11/include
LDFLAGS=-lm -L/opt/X11/lib -lX11
VERSION=4

galsim: galsim.o graphics.o
	gcc -o galsim galsim.o graphics.o $(LDFLAGS)

galsim1: galsim1.o graphics.o
	gcc -o galsim1 galsim1.o graphics.o $(LDFLAGS)

galsim2: galsim2.o graphics.o
	gcc -o galsim2 galsim2.o graphics.o $(LDFLAGS)

galsim3: galsim3.o graphics.o
	gcc -o galsim3 galsim3.o graphics.o $(LDFLAGS)

galsim.o: galsim.c graphics.h
	gcc $(CFLAGS) $(CFLAGS3) $(INCLUDES) -DVERSION=$(VERSION) -c galsim.c

galsim1.o: galsim.c graphics.h
	gcc -o galsim1.o $(CFLAGS) $(CFLAGS1) $(INCLUDES) -DVERSION=$(VERSION) -c galsim.c

galsim2.o: galsim.c graphics.h
	gcc -o galsim2.o $(CFLAGS) $(CFLAGS2) $(INCLUDES) -DVERSION=$(VERSION) -c galsim.c

galsim3.o: galsim.c graphics.h
	gcc -o galsim3.o $(CFLAGS) $(CFLAGS3) $(INCLUDES) -DVERSION=$(VERSION) -c galsim.c

graphics.o: graphics.c graphics.h
	gcc $(CFLAGS) $(INCLUDES) -c graphics.c

time: galsim1 galsim2 galsim3
	echo "$(CFLAGS1)"
	./galsim1 3000 ../input_data/ellipse_N_03000.gal 100 1e-5 0
	echo "$(CFLAGS2)"
	./galsim2 3000 ../input_data/ellipse_N_03000.gal 100 1e-5 0
	echo "$(CFLAGS3)"
	./galsim3 3000 ../input_data/ellipse_N_03000.gal 100 1e-5 0

clean:
	rm -f galsim *.o galsim1 *.o galsim2 *.o galsim3 *.0