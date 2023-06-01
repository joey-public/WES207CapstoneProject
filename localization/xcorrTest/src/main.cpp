#include <iostream>
#include <stdlib.h>
#include <vector>
#include <complex>
#include <eigen3/Eigen/Dense>
//#include <Eigen/Dense>

using Eigen::Vector3d;

#include "localization.h"
#include "crossCorrelation.h"

int main() {
    int fs = 250000000; //sample rate
    using namespace std::complex_literals;
    std::vector<RX_DTYPE> s1real = {235, 47,-87,987,1,5};
    std::vector<RX_DTYPE> s2real = {2,3,4,5,6,7};
    std::vector<RX_DTYPE> s1imag = {1,2,3,4,5,6};
    std::vector<RX_DTYPE> s2imag = {7,8,9,0,99,2};
    std::vector<RX_DTYPE> s1 = {0,0,0,0,0,0};
    std::vector<RX_DTYPE> s2 = {0,0,0,0,0,0};
    
    int length = 6;
    std::complex<short int> oneEye = std::complex<short int>(1i);
    for(int k = 0; k < length; k++)
    {
    	s1.at(k) = s1real.at(k)+oneEye*s1imag.at(k);
    	s2.at(k) = s2real.at(k)+oneEye*s2imag.at(k);
    }
    
    std::vector<double> TDoAs = CalculateTDoAs(s1,s2,s1,s2,847,856,847,856,fs);
    
    std::cout << "Starting localization. Inputs: " << std::endl << TDoAs.at(0) << std::endl << TDoAs.at(1) << std::endl << TDoAs.at(2) << std::endl << TDoAs.at(3) << std::endl;
    Vector3d loc_est;
    loc_est = Localization_4Receivers_2D(TDoAs.at(0),TDoAs.at(1),TDoAs.at(2),TDoAs.at(3));

    /*
    int thresholdSampleDifference; //index offset of data files

    int xCorrSampleDifference = CrossCorrelate(s1,s2,length);
    
    //Calculate TDoAs from offset and cross correlation
    
    int correctedSampleDifference = thresholdSampleDifference-xCorrSampleDifference;
    
    double TDoA1 = ((double)correctedSampleDifference)/fs;
    
    std::cout << "thresholdSampleDifference: " << thresholdSampleDifference << std::endl;
    std::cout << "xCorrSampleDifference: " << xCorrSampleDifference << std::endl;
    std::cout << "correctedSampleDifference: " << correctedSampleDifference << std::endl;
    std::cout << "TDoA: " << TDoA1 << std::endl;
    
    
    //MATLAB generated values for testing
    double t1 = 0.000000346623060896099;
    double t2 = 0.000000272602470685208;
    double t3 = 0.000000245465060266658;
    double t4 = 0.000000127733196515759;
    
    Vector3d loc_est;
    loc_est = Localization_4Receivers_2D(t1,t2,t3,t4);
    */
    
    std::cout << "Location estimate:" << std::endl << loc_est << std::endl;

    return 0;
}
