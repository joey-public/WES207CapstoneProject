#include <iostream>
#include <matio.h>
#include <complex>
#include <vector>

// Function to perform FIR filtering
void fir(double* coeff, std::vector<std::complex<double>>& input, std::vector<std::complex<double>>& output, uint64_t input_length, uint64_t filter_length)
{
    for(uint64_t i = 0; i< (input_length-filter_length); i++)
    {
        std::complex<double> acc = 0;
        for(uint64_t j = 0; j < filter_length; j++)
        {
            acc += input[i+j]*coeff[j];
        }
        output[i] = acc;
    }

}

void write_to_matlab_format(std::vector<std::complex<double>>& output)
{
    // Extract real and imaginary parts into separate vectors
    std::vector<double> real_data;
    std::vector<double> imag_data;

    for (const auto& complex_value : output) 
    {
        real_data.push_back(complex_value.real());
        imag_data.push_back(complex_value.imag());
    }

    // Create MAT file
    mat_t* matfp = Mat_Create("filtered_data.mat", NULL);
    if (matfp == NULL) {
        std::cout << "Could not create MAT file" << std::endl;
        return;
    }

    // Write real data to MAT file
    size_t real_dims[2] = {real_data.size(), 1};
    matvar_t* real_matvar = Mat_VarCreate("real_data", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, real_dims, real_data.data(), 0);
    if (real_matvar == NULL) 
    {
        std::cout << "Could not create matrix for real data" << std::endl;
        Mat_Close(matfp);
        return;
    }
    int write_real_status = Mat_VarWrite(matfp, real_matvar, MAT_COMPRESSION_NONE);
    if (write_real_status != 0) 
    {
        std::cout << "Error writing real data to MAT file" << std::endl;
    }
    Mat_VarFree(real_matvar);

    // Write imaginary data to MAT file
    size_t imag_dims[2] = {imag_data.size(), 1};
    matvar_t* imag_matvar = Mat_VarCreate("imag_data", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, imag_dims, imag_data.data(), 0);
    if (imag_matvar == NULL) 
    {
        std::cout << "Could not create matrix for imaginary data" << std::endl;
        Mat_Close(matfp);
        return;
    }
    int write_imag_status = Mat_VarWrite(matfp, imag_matvar, MAT_COMPRESSION_NONE);
    if (write_imag_status != 0) 
    {
        std::cout << "Error writing imaginary data to MAT file" << std::endl;
    }
    Mat_VarFree(imag_matvar);

    // Close MAT file
    Mat_Close(matfp);

    std::cout << "MAT file created successfully" << std::endl;
    
}

int main()
{
    // Load the complex signal from the "complex_data_pulse.mat" file
    mat_t* matfp = NULL;
    matvar_t* matvar = NULL;
    matfp = Mat_Open("real_data.mat", MAT_ACC_RDONLY);
    if(NULL == matfp)
    {
        std::cout << "Could not open file" << std::endl;
        return 0;
    }

    matvar = Mat_VarRead(matfp, "real_data");
    if(NULL == matvar)
    {
        std::cout << "Mat var is null" << std::endl;
        return 0;
    }
    double* real_signal = (double*)matvar->data;
    uint64_t data_size_real = matvar->nbytes / sizeof(double);
    std::cout << "Data size real: " << data_size_real << std::endl;
    enum matio_types data_type = matvar->data_type;
    std::cout << "Data type = " << data_type <<std::endl;

    matfp = Mat_Open("imag_data.mat", MAT_ACC_RDONLY);
    if(NULL == matfp)
    {
        std::cout << "Could not open file" << std::endl;
        return 0;
    }

    matvar = Mat_VarRead(matfp, "imag_data");
    if(NULL == matvar)
    {
        std::cout << "Mat var is null" << std::endl;
        return 0;
    }
    
    double* imag_signal = (double*)matvar->data;
    uint64_t data_size_imag = matvar->nbytes / sizeof(double);
    std::cout << "Data size imag: " << data_size_imag << std::endl;
    data_type = matvar->data_type;
    std::cout << "Data type = " << data_type <<std::endl;

    /**Combine to complex signal*/
     // Combine real and imaginary data into a complex vector
    std::vector<std::complex<double>> complex_signal;
    for (size_t i = 0; i < data_size_real; ++i) 
    {
        std::complex<double> complex_value(real_signal[i], imag_signal[i]);
        complex_signal.push_back(complex_value);
    }

    // Load the FIR filter coefficients from the "filter_taps_sinc_kaiser.mat" file
    matfp = Mat_Open("firpm_coefficient.mat", MAT_ACC_RDONLY);
    if(NULL == matfp)
    {
        std::cout << "Could not open file filter taps" << std::endl;
        return 0;
    }
    matvar = Mat_VarRead(matfp, "h");
    if(NULL == matfp)
    {
        std::cout << "Could not open file filter taps" << std::endl;
        return 0;
    }
    double* filter_coefficients = (double*)matvar->data;
    int filter_size = matvar->nbytes / sizeof(double);
    std::cout << "Filter size: " << filter_size << std::endl;

    // Define the buffer size and allocate memory
    const int buffer_size = filter_size; // Choose an appropriate buffer size based on the filter size
    const int PADDED_SIZE = data_size_imag + 2* filter_size;
    const int OUTPUT_SIZE = data_size_imag + filter_size;

    std::vector<std::complex<double>> paddedInput;
    paddedInput.resize(PADDED_SIZE);
    std::vector<std::complex<double>> output;
    output.resize(OUTPUT_SIZE);
    try
    {
        /* code */
        for(uint64_t i = 0; i < PADDED_SIZE; i++)
        {
            if(i < filter_size || i >= (data_size_imag+filter_size))
            {
                paddedInput.at(i) = 0;
            }
            else
            {
                paddedInput.at(i) = complex_signal.at(i-filter_size);
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';

    }
    
    fir(filter_coefficients, paddedInput, output, PADDED_SIZE, filter_size);
    write_to_matlab_format(output);    

    // Clean up and close the MATLAB file
    if(matvar != NULL)
    {    
        Mat_VarFree(matvar);
    }
    if(matfp != NULL)
    {
        Mat_Close(matfp);
    }

    return 0;
}
