#include <iostream>
#include <cmath>

#include "fftw3.h"

int main() {
    std::cout << "Starting FFTW linkage test..." << std::endl;

    double *in, *out;
    int N = 10;
    
    in = (double*) fftw_malloc(sizeof(double) * N);
    
    if (in == nullptr) {
        std::cerr << "Error: fftw_malloc failed (or linking failed)." << std::endl;
        return 1;
    }
    
    fftw_free(in);
    
    std::cout << "SUCCESS! FFTW libraries linked and basic functions called." << std::endl;
    
    return 0;
}