# Amoeba
##Summary
Minimization algorithms using amoeba, random scanning and annealing.
The documentation can be found here: https://www.ge.infn.it/~chugon/Amoeba/html/

The examples on how to use each of the methods are in programs/test_*.cxx

##Compilation:

Amoeba uses cmake. It has only the standard libraries (c++11) as dependency:

    mkdir build
    cd build
    cmake ..
    make

##Using it as external library
No installation script is done yet. To compile with it, just configure the environement variable AMOEBA_ROOT, then add

    -L${AMOEBA_ROOT}/build -lamoeba -I${AMOEBA_ROOT}/include/amoeba

in your compilation commande