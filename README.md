# Amoeba
## Summary
Minimization algorithms using amoeba, random scanning and annealing.
The documentation can be found here: https://www.ge.infn.it/~chugon/Amoeba/html/

The examples on how to use each of the methods are in programs/test_*.cxx

##Application example
###Simple amoeba
This minimizer allows to find quickly a minimum, with a minimization of calculus. The test_simple.cxx demonstrate how to use it for a function:

    double value=-100*(exp(-((x_[0]*x_[0]-4))/7200)+exp(-((x_[1]*x_[1]+4))/7200)-sin (x_[0]/50)-sin(x_[1]/50));

as showed in the following picture, this function has multiple minimum. If the starts points of the amoeba is far enough in comparison of the minimum wells sizes, it has a good probability to do not fall inside.

![Function used for the simple amoeba example](./doc/images/illustre-simple.png)

## Compilation:

Amoeba uses cmake. It has only the standard libraries (c++11) as dependency:

    mkdir build
    cd build
    cmake ..
    make

## Using it as external library
No installation script is done yet. To compile with it, just configure the environement variable AMOEBA_ROOT, then add

    -L${AMOEBA_ROOT}/build -lamoeba -I${AMOEBA_ROOT}/include/amoeba

in your compilation commande