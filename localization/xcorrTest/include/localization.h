#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <eigen3/Eigen/Dense>

// Calculates a location estimate for 4 receivers
//  @param      ToA1 Time of arrivaL for first receiver
//  @param      ToA2 Time of arrivaL for second receiver
//  @param      ToA3 Time of arrivaL for third receiver
//  @param      ToA4 Time of arrivaL for fourth receiver
//  @return          The estimated location
Eigen::Vector3d Localization_4Receivers_2D(double ToA1, double ToA2, double ToA3, double ToA4);

// Finds the index of the smallest element in an array
//  @param        *array The array to search
//  @param   numElements The number of elements to search
//  @return              The index of the minimum value in the array
int minimumIndex(double* array,int numElements);

#endif
