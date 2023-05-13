#ifndef PROC_H
#define PROC_H

#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

void process_data(std::vector<std::complex<float>> &data_buff);

std::vector<float> calc_mag(std::vector<std::complex<float>>& complexVector);
int detect_threshold(std::vector<float>& values, float threshold);
#endif
