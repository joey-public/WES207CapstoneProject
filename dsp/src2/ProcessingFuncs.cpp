#include "include/ProcessingFuncs.h"

void process_data(std::vector<std::complex<float>> &data_buff)
{
    std::cout << "Processing Data..." << std::endl;
    std::vector<float> data_mag = calc_mag(data_buff);
    float thresh = 1.0;
    int n = detect_threshold(data_mag, thresh);
    //optional perform xcorr here
    std::cout << "Done Processing Data..." << std::endl;
}

std::vector<float> calc_mag(std::vector<std::complex<float>>& complexVector) 
{
    std::vector<float> magnitudes;
    magnitudes.reserve(complexVector.size());
    for (const auto& complexNumber : complexVector) {
        float magnitude = std::abs(complexNumber);
        magnitudes.push_back(magnitude);
    }

    return magnitudes;
}

int detect_threshold(std::vector<float>& values, float threshold) 
{
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (values[i] > threshold) {
            return static_cast<int>(i);
        }
    }
    // Return -1 if no element is greater than the threshold
    return -1;
}
