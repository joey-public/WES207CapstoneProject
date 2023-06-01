#include "ProcessingFuncs.h"

namespace proc {


dsp_struct process_data(std::vector<RX_DTYPE> &data_buff, double fs)
{
    std::cout << "Processing Data..." << std::endl;
    std::cout << "\tRaw Data Size "<< data_buff.size() << std::endl;
    dsp_struct result;
    std::vector<std::complex<float>> proc_data(data_buff.size()+sett::fir_coeffs.size());
    std::cout << "\tFiltering raw data..." << std::endl;
    float max_val = fir_complex(sett::fir_coeffs, data_buff, proc_data);
    std::cout << "\tFiltering done...max val = " << max_val << std::endl;
    std::cout << "\tNormalizing proc_data by " << max_val << std::endl;
    divide_vec_by_scalar(proc_data, max_val);
    std::cout << "\tTaking Magnitude" << std::endl;
    std::cout << "\tProc Data Size "<< proc_data.size() << std::endl;
    std::vector<float> mag_data = calc_mag(proc_data);
    util::save_complex_float_vec_to_file_bin(proc_data, "./proc_data.bin");
    proc_data.clear();
    std::cout << "\tDetecting Threshold (thr = "<< sett::proc_threshold <<
                 ")... "<< std::endl;
    uint64_t threshold_idx = detect_threshold(mag_data, sett::proc_threshold,
                                             sett::proc_offset_samples);
    mag_data.clear();

    if (threshold_idx == -1)
    {
        std::cout << "\tNO THRESHOLD DETECTED" << std::endl;
        result.start_idx = 0;
        result.end_idx = fs * 30e3;
    }
    else
    {
        double thresh_time = threshold_idx / fs;
        double start_time = thresh_time - 5e-3;
        double end_time = start_time + 30e-3;
        result.start_idx = uint64_t(start_time * fs);
        result.end_idx = uint64_t(end_time * fs);
        std::cout << "\tThreshold found at idx = " << threshold_idx << 
                     ", t = "<< thresh_time << std::endl;
        std::cout << "\tpulse starts at idx = " << result.start_idx << 
                     ", t = " << start_time << std::endl;
        std::cout << "\tpulse ends at idx = " << result.end_idx << 
                     ", t = " << end_time << std::endl;
        std::cout << "Done Processing Data..." << std::endl;
    }
    result.pulse_data = util::get_subvec(data_buff, result.start_idx, result.end_idx); 
    float pulse_mem = sizeof(float) * result.pulse_data.size();
    std::cout << "\tPulse Data takes: " << pulse_mem / 1e6 << " Mb of memory" << std::endl;
    return result;
}

std::vector<float> calc_mag(std::vector<std::complex<float>>& complexVector) 
{
    std::vector<float> magnitudes;
    magnitudes.reserve(complexVector.size());
    for (const auto& complexNumber : complexVector) {
        SAMP_DTYPE magnitude = std::abs(complexNumber);
        magnitudes.push_back(magnitude);
    }
    return magnitudes;
}

std::vector<float> calc_phase(std::vector<std::complex<float>>& complexVector) 
{
    std::vector<float> phases;
    phases.reserve(complexVector.size());
    for (const auto& complexNumber : complexVector) {
        SAMP_DTYPE phase = std::arg(complexNumber);
        phases.push_back(phase);
    }
    return phases;
}

uint64_t detect_threshold(std::vector<float>& values, float threshold, uint64_t offset) 
{
    for (std::size_t i = offset; i < values.size(); ++i) {
        if (values[i] > threshold) {
            return static_cast<uint64_t>(i);
        }
    }
    // Return -1 if no element is greater than the threshold
    return -1;
}

float fir_complex(std::vector<float>& coeffs, std::vector<RX_DTYPE>& input, 
                 std::vector<std::complex<float>>& output)
{
    size_t input_len = input.size();
    size_t filter_len = coeffs.size();
    float max_val = 0.0;
    for(uint64_t i=0; i < input_len - filter_len; i++){//for size of signal
        float real_result = 0.0;
        float imag_result = 0.0;
        for(uint64_t j=0; j<filter_len; j++){//for each filter coef 
            real_result += std::real(input[i+j]) * coeffs[j]; 
            imag_result += std::imag(input[i+j]) * coeffs[j]; 
        }
        output[i] = std::complex<float>(real_result, imag_result);
        if (std::abs(output[i]) > max_val)
        {
            max_val = std::abs(output[i]);
        }
    }
    return max_val;
}


void divide_vec_by_scalar(std::vector<std::complex<float>>& vec, float scalar) 
{
    std::transform(vec.begin(), vec.end(), vec.begin(),
                   [scalar](std::complex<float> element) { return element / scalar; });
}


}//end namespace 
