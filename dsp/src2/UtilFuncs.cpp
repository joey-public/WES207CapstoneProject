#include "include/UtilFuncs.h"

void save_raw_data_to_file(const std::vector<std::complex<float>> &data_buff, 
                               const std::string& filepath)
{
    std::ofstream outFile;
    outFile.open(filepath.c_str());
    
    if (outFile.is_open()) {
        for (const auto& item : data_buff) 
        {
            std::string realStr = std::to_string(item.real());
            std::string imagStr = std::to_string(item.imag());

            outFile << realStr << ", " << imagStr << "\n";
        }
    
    outFile.close();
    }
}

void plot_with_python(std::string python_file, std::string data_path, std::string image_path)
{
    std::string command = "python " + python_file + " " + data_path + " " + image_path;

    int result = system(command.c_str());

    if(result == 0)
    {
        std::cout << "Saved Plot to: " << image_path << std::endl;
    }
    else
    {
        std::cout << "Python Script Failed...." << std::endl;
    }
}
