# High Performance Programming

This repo contains work from a course called High Performance Programming. The goal was to create a simulation of particles interacting with each other and to make the program as efficient as possible. I have recently reviewed the report and updated with some stylistic additions.

## CODE

To run the code in each folder in the `code` folder, you can simply run `make time` and this will run the code with different flag optimizers. To see the time for each flag this needs to be manually changed manually in the `Makefile`. Alternatively, if you wish to run your own parameters, then an example of this is as follows:
`./galsim 1000 ../input_data/ellipse_N_01000.gal 100 1e-5 1`

## REPORTS

A prerequesite to run the command shown below, you need to install pdflatex in the command line with:

`sudo apt-get install texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra`.

To run the tex files and build the reports, you can run the following command:

`texfot pdflatex -interaction=nonstopmode -output-directory=../report -jobname=report main.tex; bibtex report; texfot pdflatex -interaction=nonstopmode -output-directory=../report -jobname=report main.tex; texfot pdflatex -interaction=nonstopmode -output-directory=../report -jobname=report main.tex`

## SIMULATIONS

### Simulation of particle evolution representing stars in a galaxy (Assignment 3)
<p align="center">
  <img src="https://github.com/K843Barber/HPP_project/blob/main/space.gif" width=400px;>
</p>



The time required for the calculation is: 3.427271


Output from computation of A4: `./galsim 3000 ../input_data/ellipse_N_03000.gal 100 1e-5 0.01 1`
`....................................................................................................`
`quadtime: 0.098491 `
`forcetime: 35.855026 `
`The total time required for the calculation is: 37.015067.`

```
➜  A5 git:(main) ✗ ./galsim 3000 ../input_data/ellipse_N_03000.gal 100 1e-5 1e-3 1 1
....................................................................................................
quadtime: 0.094956
forcetime: 26.998334
The total time required for the calculation is: 28.141161.

➜  A5 git:(main) ✗ ./galsim 3000 ../input_data/ellipse_N_03000.gal 100 1e-5 1e-3 1 2
....................................................................................................
quadtime: 0.092072
forcetime: 15.779427
The total time required for the calculation is: 16.916033.

➜  A5 git:(main) ✗ ./galsim 3000 ../input_data/ellipse_N_03000.gal 100 1e-5 1e-3 1 4
....................................................................................................
quadtime: 0.097179
forcetime: 13.025987
The total time required for the calculation is: 14.170618.

➜  A5 git:(main) ✗ ./galsim 3000 ../input_data/ellipse_N_03000.gal 100 1e-5 1e-3 1 8
....................................................................................................
```

➜  A6 git:(main) ✗ ./galsim 3000 ../input_data/ellipse_N_03000.gal 100 1e-5 1e-3 1
....................................................................................................
quadtime: 0.110113
forcetime: 10.489650
 partytime 2.312356
The total time required for the calculation is: 14.178464.
