# Amoeba
## Summary
Minimization algorithms using amoeba, random scanning and annealing.
The documentation can be found here: https://www.ge.infn.it/~chugon/Amoeba/html/

The examples on how to use each of the methods are in programs/test_*.cxx

## Compilation:

Amoeba uses cmake. It has only the standard libraries (c++11) as dependency:

    mkdir build
    cd build
    cmake ..
    make

## Using it as external library
No installation script is done yet. To compile with it, just configure the environement variable AMOEBA_ROOT, then add

    -L${AMOEBA_ROOT}/build -lamoeba -I${AMOEBA_ROOT}/include/amoeba

in your compilation command. A script amoeba-config will be created soon to facilitate the linking.

##Application example
###Simple amoeba
This minimizer allows to find quickly a minimum, with a minimization of number of calculated points. The test_simple.cxx demonstrates how to use it for a function defined by:

    double value=-100*(exp(-((x_[0]*x_[0]-4))/7200)+exp(-((x_[1]*x_[1]+4))/7200)-sin (x_[0]/50)-sin(x_[1]/50));

as showed in the following picture, this function has multiple minimums. If the starts points of the amoeba is not too far enough in comparison of the minimum wells sizes, it has a good probability to do not fall inside. This example find the minimum in about 40 steps with a precision of 1°. This method is optimal for minimization from which we already have information and/or for prefit cases.

![Function used for the annealing example](./doc/images/illustre-simple.png)

###Annealing and nosy amoeba
This, still under improvment, method uses first a random scan of the map with an arbitrary number of scanners (called noses). This scan can be homogeneous, or distributed with a normal distribution around the current minimum point.
The annealing uses the noses scans as a base to decide, in function of the current temperature, if a non smaller value is accepted or not. The example test_annealing.cxx shows how to use this method. The next figure shows the more complex used function, defined by

    double value=-100*(exp(-((x_[0]*x_[0]-4))/7200)+exp(-((x_[1]*x_[1]+4))/7200)-sin (x_[0]/20)-sin(x_[1]/20));

![Function used for the simple amoeba example](./doc/images/illustre-annealing.png)

The simple amoeba is generally unable to find the real minimum and will tend to fall in a local minimum. The next figure shows the minimums found by the annealing, using the command

    ./test_annealing -n 2 -s 70 -minmax 0 400

The -n gives a number of noses, -s the sigma of the normal distribution around the current minimum and -minmax define the temperature differential we want to use in this case (400°, the annealing algorithm has been tuned in a way that the order of magnitude between the minimum and maximum is a good value). -s 70 is the order of magnitude of the x-y size minimum wells, optimizing the probability to escape from it. The next figure shows the probability to find the real minimum (-380) of to fall in other minimums.

![Probability to find the real minimum and wrong minimum](./doc/images/proba-annealing.png)

As can be seen, the probability to find the wrong minimum is ~30%. Rerun a fair enough number of times the algorithm permit to reduce significantly this probability.