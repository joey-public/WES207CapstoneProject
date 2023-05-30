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
    int start_idx;
    int end_idx;
    std::vector<RX_DTYPE> pulse_data;
};

void stream_and_process_data(uhd::usrp::multi_usrp::sptr usrp);


//void process_data(std::vector<RX_DTYPE> &data_buff);
dsp_struct process_data(std::vector<RX_DTYPE> &data_buff, double fs);  

//std::vector<SAMP_DTYPE> calc_mag(std::vector<RX_DTYPE>& complexVector);
std::vector<float> calc_mag(std::vector<std::complex<float>>& complexVector); 

std::vector<SAMP_DTYPE> calc_phase(std::vector<RX_DTYPE>& complexVector);

//int detect_threshold(std::vector<SAMP_DTYPE>& values, SAMP_DTYPE threshold, int offset);
int detect_threshold(std::vector<float>& values, float threshold, int offset);

std::vector<RX_DTYPE> xcorr_eigen(const std::vector<RX_DTYPE>& signalA, 
                                  const std::vector<RX_DTYPE>& signalB);

float fir_complex(std::vector<float>& coeffs, std::vector<RX_DTYPE>& input, 
                 std::vector<std::complex<float>>& output);

void divide_vec_by_scalar(std::vector<std::complex<float>>& vec, float scalar); 

std::vector<SAMP_DTYPE> calc_norm_mag(std::vector<RX_DTYPE>& complexVector); 

}//end namespace
#endif
