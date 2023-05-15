#ifndef PROC_H
#define PROC_H

#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

#include <Eigen/Core>

void process_data(std::vector<std::complex<float>> &data_buff);

std::vector<float> calc_mag(std::vector<std::complex<float>>& complexVector);

std::vector<float> calc_phase(std::vector<std::complex<float>>& complexVector);

int detect_threshold(std::vector<float>& values, float threshold, int offset);

std::vector<std::complex<float>> xcorr_eigen(const std::vector<std::complex<float>>& signalA, const std::vector<std::complex<float>>& signalB);
#endif
