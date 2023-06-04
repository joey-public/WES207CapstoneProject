#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <eigen3/Eigen/Dense>
#include <vector>
#include "TypeDefs.h"


// Calculates a location estimate for 4 receivers
//  @param      ToA1 Time of arrivaL for first receiver
//  @param      ToA2 Time of arrivaL for second receiver
//  @param      ToA3 Time of arrivaL for third receiver
//  @param      ToA4 Time of arrivaL for fourth receiver
//  @return          The estimated location
Eigen::Vector3d Localization_4Receivers_2D(double ToA1, double ToA2, double ToA3, double ToA4);

// Calculates a location estimate for 4 receivers
//  @param          data1-4 Raw pulse data from each receiver
//  @param      startIdx1-4 Starting index of the capture for the input data from each receiver
//  @param       sampleRate The sample rate the data was captured at
//  @return          	     Vector of TDoAs between the four receivers
std::vector<double> CalculateTDoAs(std::vector<RX_DTYPE>& data1, std::vector<RX_DTYPE>& data2, std::vector<RX_DTYPE>& data3, std::vector<RX_DTYPE>& data4,
	int startIdx1,int startIdx2, int startIdx3, int startIdx4, int sampleRate);

// Finds the index of the smallest element in an array
//  @param        *array The array to search
//  @param   numElements The number of elements to search
//  @return              The index of the minimum value in the array
int minimumIndex(double* array,int numElements);

// Finds the index of the smallest element in an array
//  @param        *array The array to search
//  @param   numElements The number of elements to search
//  @return              The index of the minimum value in the array
int minimumIndexInt(int* array,int numElements);

#endif
