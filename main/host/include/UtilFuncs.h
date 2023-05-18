#ifndef UTIL_FUNCS
#define UTILFUNCS

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <complex>

namespace util {

void save_complex_vec_to_file(const std::vector<std::complex<float>> &vec, 
                               const std::string filename);

void save_float_vec_to_file(const std::vector<float> &vec, 
                               const std::string filename);


std::vector<std::complex<float>> get_subvec(const std::vector<std::complex<float>> &in_vec, 
                                           int start_idx, int k);

}//end namespace

//void plot_with_python(std::string python_file, std::string data_path, std::string image_path);

//template<typename T>
//void save_vec_to_file(const std::vector<T> &data_buff, 
//                               const std::string& filepath);
//// Overload for std::complex
//template<typename T>
//std::ostream& operator<<(std::ostream& os, const std::complex<T>& complexNumber) 
//{
//    os << complexNumber.real() << " " << complexNumber.imag();
//    return os;
//}
#endif
