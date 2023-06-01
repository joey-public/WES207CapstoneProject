#ifndef UTIL_FUNCS
#define UTILFUNCS

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <complex>

#include "TypeDefs.h"

namespace util {

void save_complex_vec_to_file_bin(const std::vector<RX_DTYPE>& vec, 
                                       const std::string filename, 
                                       bool should_save);

void save_complex_float_vec_to_file_bin(const std::vector<std::complex<float>>& vec, 
                                       const std::string filename,
                                       bool should_save);

std::vector<RX_DTYPE> get_subvec(const std::vector<RX_DTYPE> &in_vec, 
                                           int start_idx, int k);

}//end namespace

#endif
