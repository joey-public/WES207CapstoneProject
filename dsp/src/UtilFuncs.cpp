#include "include/UtilFuncs.h"

//template<typename T>
//void save_vec_to_file(const std::vector<T> &data_buff, 
//                               const std::string& filepath)
//{
//    std::ofstream outFile;
//    outFile.open(filepath.c_str());
//    
//    if (outFile.is_open()) {
//        for (const auto& item : data_buff) 
//        {
//            std::string val = std::to_string(item);
//            outFile << val << ", ";
//        }
//    
//    outFile.close();
//    }
//}


void save_complex_vec_to_file(const std::vector<std::complex<float>> &vec, 
                               const std::string filename)
{
     std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    for (const auto& element : vec) {
        file << element.real() << " " << element.imag() << " ";
    }

    file.close();
    std::cout << "Vector saved to file: " << filename << std::endl;
}

void save_float_vec_to_file(const std::vector<float> &vec, 
                               const std::string filename)
{
     std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    for (const auto& element : vec) {
        file << element << " ";
    }

    file.close();
    std::cout << "Vector saved to file: " << filename << std::endl;
}

void plot_with_python(std::string python_file, std::string data_path, std::string data_type)
{
    std::string command = "python " + python_file + " " + data_path + " " + data_type;

    int result = system(command.c_str());

    if(result == 0)
    {
        //std::cout << "Saved Plot to: " << image_path << std::endl;
    }
    else
    {
        std::cout << "Python Script Failed...." << std::endl;
    }
}

std::vector<std::complex<float>> get_subvec(const std::vector<std::complex<float>>&in_vec, 
                                           int start_idx, int k)
{
    if(start_idx + k > in_vec.size())
    {
        std::cout << "invalid range of start_idx+k is too large...\n";
        return in_vec;
    }
    std::vector<std::complex<float>> out_vec(k);
    int j = 0;
    for(int i = start_idx; i < start_idx + k; i++)
    {
        out_vec[j] = in_vec[i]; 
        j += 1;
    }
    return out_vec;
}
