#include "UtilFuncs.h"


namespace util{

void save_complex_vec_to_file_bin(const std::vector<RX_DTYPE>& vec, 
                                       const std::string filename, 
                                       bool should_save)
{
    if(!should_save) {return;}
    std::ofstream file;
    file.open(filename.c_str(), std::ofstream::binary);
    for (int i = 0; i < vec.size(); i++) 
    {
        file.write((const char*) &vec[i], 2 * sizeof(SAMP_DTYPE));
    }
    file.close();
    std::cout << "Vector saved to file: " << filename << std::endl;
}

void save_complex_float_vec_to_file_bin(const std::vector<std::complex<float>>& vec, 
                                       const std::string filename,
                                       bool should_save)
{
    if(!should_save) {return;}
    std::ofstream file;
    file.open(filename.c_str(), std::ofstream::binary);
    for (int i = 0; i < vec.size(); i++) 
    {
        file.write((const char*) &vec[i], 2 * sizeof(float));
    }
    file.close();
    std::cout << "Vector saved to file: " << filename << std::endl;
}

std::vector<RX_DTYPE> get_subvec(const std::vector<RX_DTYPE>&in_vec, 
                                           int start_idx, int k)
{
    if(start_idx + k > in_vec.size())
    {
        std::cout << "invalid range of start_idx+k is too large...\n";
        return in_vec;
    }
    std::vector<RX_DTYPE> out_vec(k);
    int j = 0;
    for(int i = start_idx; i < start_idx + k; i++)
    {
        out_vec[j] = in_vec[i]; 
        j += 1;
    }
    return out_vec;
}

}//end namespace
