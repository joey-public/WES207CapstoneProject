#ifndef PROC_H
#define PROC_H

#include <iostream>
#include <complex>
#include <vector>
#include <cmath>
#include <uhd/usrp/multi_usrp.hpp>

#include <Eigen/Core>

#include "TypeDefs.h"
#include "Settings.h"
#include "UtilFuncs.h"

namespace proc{

struct dsp_struct {
    uint64_t start_idx;
    uint64_t end_idx;
    std::vector<RX_DTYPE> pulse_data;
};


//void process_data(std::vector<RX_DTYPE> &data_buff);
dsp_struct process_data(std::vector<RX_DTYPE> &data_buff, double fs);  

//std::vector<SAMP_DTYPE> calc_mag(std::vector<RX_DTYPE>& complexVector);
std::vector<float> calc_mag(std::vector<std::complex<float>>& complexVector); 
std::vector<float> calc_mag_and_norm(std::vector<std::complex<float>>& complexVector, float max_val);

//std::vector<SAMP_DTYPE> calc_phase(std::vector<RX_DTYPE>& complexVector);
std::vector<float> calc_phase(std::vector<std::complex<float>>& complexVector);

//int detect_threshold(std::vector<SAMP_DTYPE>& values, SAMP_DTYPE threshold, int offset);
uint64_t detect_threshold(std::vector<float>& values, float threshold, uint64_t offset);

float fir_complex(std::vector<float>& coeffs, std::vector<RX_DTYPE>& input, 
                 std::vector<std::complex<float>>& output);

void divide_vec_by_scalar(std::vector<std::complex<float>>& vec, float scalar); 


}//end namespace
#endif
