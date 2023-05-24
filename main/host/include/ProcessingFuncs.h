#ifndef PROC_H
#define PROC_H

#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

#include <Eigen/Core>

#include "TypeDefs.h"

namespace proc{

void process_data(std::vector<RX_DTYPE> &data_buff);

std::vector<SAMP_DTYPE> calc_mag(std::vector<RX_DTYPE>& complexVector);

std::vector<SAMP_DTYPE> calc_phase(std::vector<RX_DTYPE>& complexVector);

int detect_threshold(std::vector<SAMP_DTYPE>& values, SAMP_DTYPE threshold, int offset);

std::vector<RX_DTYPE> xcorr_eigen(const std::vector<RX_DTYPE>& signalA, const std::vector<RX_DTYPE>& signalB);

void fir(int16_t *coeffs, RX_DTYPE *input, RX_DTYPE *output, int length, int filterLength);

}//end namespace
#endif
