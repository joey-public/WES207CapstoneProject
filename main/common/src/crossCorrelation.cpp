#include <iostream>
#include <math.h>
#include <vector>
#include <crossCorrelation.h>
#include <complex>
#include <fftw3.h>
#include "TypeDefs.h"
#include "debug.h"


int CrossCorrelate(std::vector<RX_DTYPE>& s1, std::vector<RX_DTYPE>& s2, int length)
{
	int fftsize = 2*length-1;
	
	fftw_complex *in1, *in2, *res1, *res2, *product, *out;
	fftw_plan fft1, fft2, ifft;
	
	
	//allocate memory
	in1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*fftsize);
	if(NULL == in1)
	{
		std::cout << "Memory allocation error" << std::endl;
		return -1;
	}
	in2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*fftsize);
	if(NULL == in2)
	{
		std::cout << "Memory allocation error" << std::endl;
		return -1;
	}
	res1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*fftsize);
	if(NULL == res1)
	{
		std::cout << "Memory allocation error" << std::endl;
		return -1;
	}
	res2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*fftsize);
	if(NULL == res2)
	{
		std::cout << "Memory allocation error" << std::endl;
		return -1;
	}
	product = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*fftsize);
	if(NULL == product)
	{
		std::cout << "Memory allocation error" << std::endl;
		return -1;
	}
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*fftsize);
	if(NULL == out)
	{
		std::cout << "Memory allocation error" << std::endl;
		return -1;
	}

	//create plans
	fft1 = fftw_plan_dft_1d(fftsize,in1,res1,FFTW_FORWARD,FFTW_ESTIMATE);
	fft2 = fftw_plan_dft_1d(fftsize,in2,res2,FFTW_FORWARD,FFTW_ESTIMATE);
	ifft = fftw_plan_dft_1d(fftsize,product,out,FFTW_BACKWARD,FFTW_ESTIMATE);
	
	/*
	for(int k = 0; k < length; k++)
	{
		in1[k][0] = (double) s1.at(2*k);
		in1[k][1] = (double) s1.at(2*k+1);
		in2[k][0] = (double) s2.at(2*k);
		in2[k][1] = (double) s2.at(2*k+1);
	}*/
	for(int k = 0; k < length; k++)
	{
		in1[k][0] = std::real(s1.at(k));
		in1[k][1] = std::imag(s1.at(k));
		in2[k][0] = std::real(s2.at(k));
		in2[k][1] = std::imag(s2.at(k));
		
		//std::cout << in1[k][0] << " " << in1[k][1] << " " << in2[k][0] << " " << in2[k][1] << std::endl;
		
	}
	
	for(int k = length; k < fftsize; k++)
	{
		in1[k][0] = 0.0;
		in1[k][1] = 0.0;
		in2[k][0] = 0.0;
		in2[k][1] = 0.0;
	}
	
	fftw_execute(fft1);
	fftw_execute(fft2);
	
	/*
	//take complex conjugate of one of the vectors
	for(int k = 0; k < fftsize; k++)
	{
		res2[k][1] = -1*res2[k][1];
	}
	
	//point by point multiply
	for(int k = 0; k < fftsize; k++)
	{
		product[k][0] = res1[k][0]*res2[k][0] - res1[k][1]*res2[k][1];
		product[k][1] = res1[k][0]*res2[k][1] + res1[k][1]*res2[k][0];
	}
	*/
	//faster way to combine above steps
	for(int k = 0; k < fftsize; k++)
	{
		product[k][0] = res1[k][0]*res2[k][0] + res1[k][1]*res2[k][1];
		product[k][1] = res1[k][1]*res2[k][0] - res1[k][0]*res2[k][1];
	}
	fftw_execute(ifft);
	fftw_destroy_plan(fft1);
	fftw_destroy_plan(fft2);
	fftw_destroy_plan(ifft);
	fftw_free(in1);
	fftw_free(in2);
	fftw_free(res1);
	fftw_free(res2);
	fftw_free(product);
	//find maximum index
	std::vector<std::complex<double>> xcorrArray;
	xcorrArray.resize(fftsize);
	using namespace std::complex_literals;
	for(int k = 0; k < fftsize; k++)
	{
		xcorrArray[k] = out[k][0] + 1i*out[k][1];
		//std::cout << xcorrArray[k] << " " << out[k][0] << " " << out[k][1] << std::endl;
	}
	int lag = maximumIndex(xcorrArray.data(),fftsize);
	if(lag >= length)
	{
		lag = lag-fftsize;
	}
	fftw_free(out);
	
	return lag;
}

int maximumIndex(std::complex<double>* array,int numElements)
//int maximumIndex(fftw_complex* array,int numElements)
{
    double val = std::abs(array[0]);
    int idx = 0;
    for (int i = 1; i < numElements; i++)
    {
        if (std::abs(array[i]) > val)
        {
            val = std::abs(array[i]);
            idx = i;
        }
    }
    
    return idx;
}

