#include "UtilFuncs.h"


namespace util{

void save_complex_vec_to_file(const std::vector<RX_DTYPE> &vec, 
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

void save_complex_vec_to_file_bin(const std::vector<RX_DTYPE>& vec, 
                                       const std::string filename)
{
    std::ofstream file;
    file.open(filename.c_str(), std::ofstream::binary);
    for (int i = 0; i < vec.size(); i++) 
    {
        file.write((const char*) &vec[i], 2 * sizeof(int16_t));
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

void plot_with_python(std::string data_path, int fs)
{
    std::string python_file = "../../scripts/plot_complex_bin_data.py";
    std::string command = "python " + python_file + 
                          " " + data_path + 
                          " " + std::to_string(fs) + " &";
    int result = system(command.c_str());
    if(result == 0)
    {
        //std::cout << "Saved Plot to: " << image_path << std::endl;
        std::cout << "\t\tPython Script Ran Sucessfully..." << std::endl;
    }
    else
    {
        std::cout << "\t\tPython Script Failed..." << std::endl;
    }
}

}//end namespace
 

//
//
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
