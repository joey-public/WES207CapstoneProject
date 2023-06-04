#ifndef CROSSCORRELATION_H
#define CROSSCORRELATION_H

#include <eigen3/Eigen/Dense>
#include <vector>
#include <complex>
#include <fftw3.h>
#include "TypeDefs.h"

// Finds the index of the largest value of the cross correlation between the 2 input vectors
//  @param	s1 first signal
//  @param   	s2 second signal
//  @return	The lag of the maximum cross correlation magnitude
int CrossCorrelate(std::vector<RX_DTYPE>& s1, std::vector<RX_DTYPE>& s2, int length);

// Finds the index of the largest element in an array
//  @param        *array The array to search
//  @param   numElements The number of elements to search
//  @return              The index of the maximum value in the array
int maximumIndex(std::complex<double>* array,int numElements);

#endif
