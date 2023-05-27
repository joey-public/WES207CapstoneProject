#include "ProcessingFuncs.h"

namespace proc {


void process_data(std::vector<RX_DTYPE> &data_buff)
{
    std::cout << "Processing Data..." << std::endl;
    std::vector<SAMP_DTYPE> data_mag = calc_mag(data_buff);
    SAMP_DTYPE thresh = 100;
    //int n = detect_threshold(data_mag, thresh);
    //optional perform xcorr here
    std::cout << "Done Processing Data..." << std::endl;
}

std::vector<SAMP_DTYPE> calc_mag(std::vector<RX_DTYPE>& complexVector) 
{
    std::vector<SAMP_DTYPE> magnitudes;
    magnitudes.reserve(complexVector.size());
    for (const auto& complexNumber : complexVector) {
        SAMP_DTYPE magnitude = std::abs(complexNumber);
        magnitudes.push_back(magnitude);
    }
    return magnitudes;
}

std::vector<SAMP_DTYPE> calc_phase(std::vector<RX_DTYPE>& complexVector) 
{
    std::vector<SAMP_DTYPE> phases;
    phases.reserve(complexVector.size());
    for (const auto& complexNumber : complexVector) {
        SAMP_DTYPE phase = std::arg(complexNumber);
        phases.push_back(phase);
    }
    return phases;
}

int detect_threshold(std::vector<SAMP_DTYPE>& values, SAMP_DTYPE threshold, int offset) 
{
    for (std::size_t i = offset; i < values.size(); ++i) {
        if (values[i] > threshold) {
            return static_cast<int>(i);
        }
    }
    // Return -1 if no element is greater than the threshold
    return -1;
}

std::vector<RX_DTYPE> xcorr_eigen(const std::vector<RX_DTYPE>& signalA, const std::vector<RX_DTYPE>& signalB) 
{
    std::size_t resultSize = signalA.size() + signalB.size() - 1;
    std::vector<RX_DTYPE> result(resultSize);

    Eigen::Map<const Eigen::Matrix<RX_DTYPE, Eigen::Dynamic, 1>> eigenSignalA(signalA.data(), signalA.size());
    Eigen::Map<const Eigen::Matrix<RX_DTYPE, Eigen::Dynamic, 1>> eigenSignalB(signalB.data(), signalB.size());
    Eigen::Map<Eigen::Matrix<RX_DTYPE, Eigen::Dynamic, 1>> eigenResult(result.data(), resultSize);

    eigenResult = eigenSignalA.reverse().asDiagonal() * eigenSignalB;

    return result;
}

void fir(int16_t *coeffs, RX_DTYPE *input, RX_DTYPE *output, int length, int filterLength)
// ----------------------------------------------
{
    std::cout << "Naive FIR\n";
    for(int i=0; i<length-filterLength; i++){//for size of signal
        RX_DTYPE result = 0.0;
        for(int j=0; j<filterLength; j++){//for each filter coef 
            result += input[i+j] * coeffs[j]; 
        }
        output[i] = result;
    }
}

void divide_vec_by_scalar(std::vector<SAMP_DTYPE>& vec, double scalar) 
{
    std::transform(vec.begin(), vec.end(), vec.begin(),
                   [scalar](SAMP_DTYPE element) { return element / scalar; });
}

std::vector<SAMP_DTYPE> calc_norm_mag(std::vector<RX_DTYPE>& complexVector) 
{
    std::vector<SAMP_DTYPE> magnitudes;
    double sum = 0;
    magnitudes.reserve(complexVector.size());
    for (const auto& complexNumber : complexVector) {
        SAMP_DTYPE magnitude = std::abs(complexNumber);
        magnitudes.push_back(magnitude);
        sum += magnitude;
    }
    divide_vec_by_scalar(magnitudes, sum);
    return magnitudes;
}

}//end namespace
