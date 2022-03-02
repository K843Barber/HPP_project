# High Performance Programming

This repo contains work from a course called High Performance Programming. The goal was to create a simulation of particles interacting with each other and to make the program as efficient as possible. I have recently reviewed the report and updated with some stylistic additions.

## CODE

To run the code in each folder in the `code` folder, you can simply run 'make time' and this will run the code with different flag optimizers. To see the time for each flag this needs to be manually changed manually in the `Makefile`. Alternatively, if you wish to run your own parameters, then an example of this is as follows:
`./galsim 1000 ../input_data/ellipse_N_01000.gal 100 1e-5 1`

## TEX

A prerequesite to run the command shown below, you need to install pdflatex in the command line with: `sudo apt-get install texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra`.

To run the tex files and build the reports, you can run the following command: `texfot pdflatex -interaction=nonstopmode -output-directory=../report -jobname=report main.tex; bibtex report; texfot pdflatex -interaction=nonstopmode -output-directory=../report -jobname=report main.tex; texfot pdflatex -interaction=nonstopmode -output-directory=../report -jobname=report main.tex`

### Simulation of particle evolution representing stars in a galaxy (Assignment 3)

<img src="https://github.com/K843Barber/HPP_project/blob/main/space.gif" width=600px;>
