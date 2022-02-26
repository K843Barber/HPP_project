COPTS=-O0
make clean > /dev/null
make COPTS=$COPTS > /dev/null
echo $COPTS
./galsim 5000 ../input_data/ellipse_N_05000.gal 100 1e-5 0.25 0 6

COPTS=-O3
make clean > /dev/null
make COPTS=$COPTS > /dev/null
echo $COPTS
./galsim 5000 ../input_data/ellipse_N_05000.gal 100 1e-5 0.25 0 6

COPTS=-Ofast
make clean > /dev/null
make COPTS=$COPTS > /dev/null
echo $COPTS
./galsim 05000 ../input_data/ellipse_N_05000.gal 100 1e-5 0.25 0 6

COPTS="-Ofast -march=native"
make clean > /dev/null
make COPTS="$COPTS" > /dev/null
echo $COPTS
./galsim 5000 ../input_data/ellipse_N_05000.gal 100 1e-5 0.25 0 6

COPTS=-O4
make clean > /dev/null
make COPTS="$COPTS" > /dev/null
echo $COPTS
./galsim 5000 ../input_data/ellipse_N_05000.gal 100 1e-5 0.25 0 6