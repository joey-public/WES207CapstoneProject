#include "ProcessingFuncs.h"

namespace proc {

void process_data(std::vector<std::complex<float>> &data_buff)
{
    std::cout << "Processing Data..." << std::endl;
    std::vector<float> data_mag = calc_mag(data_buff);
    float thresh = 1.0;
    //int n = detect_threshold(data_mag, thresh);
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

std::vector<float> calc_phase(std::vector<std::complex<float>>& complexVector) 
{
    std::vector<float> phases;
    phases.reserve(complexVector.size());
    for (const auto& complexNumber : complexVector) {
        float phase = std::arg(complexNumber);
        phases.push_back(phase);
    }
    return phases;
}

int detect_threshold(std::vector<float>& values, float threshold, int offset) 
{
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (values[i] > threshold) {
            return static_cast<int>(i);
        }
    }
    // Return -1 if no element is greater than the threshold
    return -1;
}

std::vector<std::complex<float>> xcorr_eigen(const std::vector<std::complex<float>>& signalA, const std::vector<std::complex<float>>& signalB) 
{
    std::size_t resultSize = signalA.size() + signalB.size() - 1;
    std::vector<std::complex<float>> result(resultSize);

    Eigen::Map<const Eigen::Matrix<std::complex<float>, Eigen::Dynamic, 1>> eigenSignalA(signalA.data(), signalA.size());
    Eigen::Map<const Eigen::Matrix<std::complex<float>, Eigen::Dynamic, 1>> eigenSignalB(signalB.data(), signalB.size());
    Eigen::Map<Eigen::Matrix<std::complex<float>, Eigen::Dynamic, 1>> eigenResult(result.data(), resultSize);

    eigenResult = eigenSignalA.reverse().asDiagonal() * eigenSignalB;

    return result;
}

}//end namespace
