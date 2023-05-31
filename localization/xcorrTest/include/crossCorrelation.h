#ifndef CROSSCORRELATION_H
#define CROSSCORRELATION_H

#include <eigen3/Eigen/Dense>
#include <vector>
#include <complex>
#include <fftw3.h>

typedef int16_t SAMP_DTYPE;
typedef std::complex<SAMP_DTYPE> RX_DTYPE;
//typedef compint64_t std::complex<int64_t>;

// Finds the index of the smallest element in an array
//  @param        *array The array to search
//  @param   numElements The number of elements to search
//  @return              The index of the minimum value in the array
int CrossCorrelate(std::vector<RX_DTYPE>& s1, std::vector<RX_DTYPE>& s2, int length);

int maximumIndex(std::complex<double>* array,int numElements);
//int maximumIndex(fftw_complex* array,int numElements);

#endif
