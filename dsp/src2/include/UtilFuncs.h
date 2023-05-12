#ifndef UTIL_FUNCS
#define UTILFUNCS

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <complex>

void save_raw_data_to_file(const std::vector<std::complex<float>> &data_buff, 
                               const std::string &filepath);
void plot_with_python(std::string python_file, std::string data_path, std::string image_path);

#endif
