#include <iostream>
#include <math.h>
#include <localization.h>
#include <crossCorrelation.h>
#include <vector>
#include <eigen3/Eigen/Dense>
#include "TypeDefs.h"

using Eigen::Vector3d;
using Eigen::RowVector3d;
using Eigen::Matrix3d;
using Eigen::MatrixXd;

MatrixXd receivers(4,3);

Vector3d Localization_4Receivers_2D(double ToA1, double ToA2, double ToA3, double ToA4)
{
    double ToAs[4] = {ToA1, ToA2, ToA3, ToA4};
    int minidx = minimumIndex(&ToAs[0],4);
    double minval = ToAs[minidx];
    
    //receiver locations stored in rows- hardcoded for now - add capability to read from config file
    receivers <<   0.0,   0.0,  4.0,
                 100.0,   0.0,  8.0,
                   0.0, 100.0, 10.0,
                 100.0, 100.0,  6.0;
    
    Vector3d d; //range differences
    Matrix3d S; //receiver coordinates (reference receiver at origin)
    Vector3d delta;
    
    int k = 0;
    
    for(int i = 0; i < 4; i++) {
        if (i != minidx)
        {
            d(k) = (ToAs[i] - ToAs[minidx])*299792458.0;
            S(k,0) = receivers(i,0) - receivers(minidx,0);
            S(k,1) = receivers(i,1) - receivers(minidx,1);
            S(k,2) = receivers(i,2) - receivers(minidx,2);
            
            //((S(k,1)^2)+(S(k,2)^2)+(S(k,3)^2)-(d(k)^2));
            delta(k) = pow(S(k,0),2)+pow(S(k,1),2)+pow(S(k,2),2)-pow(d(k),2);
            k = k+1;
        }
    }
    
    //find S_star, the pseudoinverse
    Matrix3d S_star;
    Matrix3d W; //weight matrix (identity (equal weight) for now)
    
    W = Matrix3d::Identity();
                  
    S_star = ((S.transpose()*W*S).inverse())*(S.transpose())*W;
    
    //Calculate parameters for quadratic equation from Schau/Robinson paper
    RowVector3d d_T = d.transpose();
    Matrix3d S_star_T = S_star.transpose();
    RowVector3d delta_T = delta.transpose();
    
    double Rs_a = 4.0-4.0*(d_T*S_star_T*S_star*d).value();
    double Rs_b = 2.0*(d_T*S_star_T*S_star*delta).value()+2.0*(delta_T*S_star_T*S_star*d).value();
    double Rs_c = (-delta_T*S_star_T*S_star*delta).value();

    //Solve the aforementioned quadratic. Thus "plus" solution is the correct one
    double Rs = std::real(-Rs_b+sqrt(pow(Rs_b,2)-4.0*Rs_a*Rs_c))/(2.0*Rs_a);
    
    //Now use the range estimate of the source found above to solve the matrix equation
    Vector3d loc_est;
    loc_est = 0.5*S_star*(delta-2.0*Rs*d);
    
    //switch origin back (reference receiver (where the signal arrived first) was used as origin in calculation)
    loc_est = loc_est+receivers.row(minidx).transpose();
    
    /*
    cout << "Here is the matrix d\n" << d << endl;
    cout << "Here is the matrix delta\n" << delta << endl;
    cout << "Here is the matrix S\n" << S << endl;
    cout << "Here is the matrix W\n" << W << endl;
    cout << "Here is the matrix S_star\n" << S_star << endl;
    */
    
    return loc_est;
}

std::vector<double> CalculateTDoAs(std::vector<RX_DTYPE>& data1, std::vector<RX_DTYPE>& data2, std::vector<RX_DTYPE>& data3, std::vector<RX_DTYPE>& data4,
	int startIdx1,int startIdx2, int startIdx3, int startIdx4, int sampleRate)
{
	int startIndexes[4] = {startIdx1, startIdx2, startIdx3, startIdx4};
	int lags[4] = {0,0,0,0};
	int sampleOfArrival[4] = {startIdx1, startIdx2, startIdx3, startIdx4};
	double TDoA[4];
	
	int minidx = minimumIndexInt(&startIndexes[0],4);
	std::vector<RX_DTYPE>* signalData[4] = {&data1, &data2, &data3, &data4};
	int length = data1.size();
	
	//cross correlate each signal with refernece (first threshold detected) to get closer offset
	for(int i = 0; i < 4; i++)
	{
        	if (i != minidx)
        	{
        		lags[i] = CrossCorrelate(*signalData[minidx],*signalData[i],length);
        		sampleOfArrival[i] = startIndexes[i] - lags[i];
        	}
    	}
    	//std::cout << "Lags: " << std::endl << lags[0] << std::endl << lags[1]  << std::endl << lags[2]  << std::endl << lags[3]  << std::endl;
    	
    	minidx = minimumIndexInt(&sampleOfArrival[0],4);
    	int minval = sampleOfArrival[minidx];
    	for(int i = 0; i < 4; i++)
    	{
        	sampleOfArrival[i] = sampleOfArrival[i] - minval;
        	TDoA[i] = ((double)sampleOfArrival[i])/sampleRate;
    	}
    	
    	/*std::cout << "Sample Differences: " << std::endl << sampleOfArrival[0] << std::endl << sampleOfArrival[1]  << std::endl << sampleOfArrival[2]  << std::endl << sampleOfArrival[3]  << std::endl; */
    	
    	std::vector<double> TDoAs = {TDoA[0],TDoA[1],TDoA[2],TDoA[3]};
	return TDoAs;
	
}

int minimumIndex(double* array,int numElements)
{
    double val = array[0];
    int idx = 0;
    for (int i = 1; i < numElements; i++)
    {
        if (array[i] < val)
        {
            val = array[i];
            idx = i;
        }
    }
    return idx;
}

int minimumIndexInt(int* array,int numElements)
{
    double val = array[0];
    int idx = 0;
    for (int i = 1; i < numElements; i++)
    {
        if (array[i] < val)
        {
            val = array[i];
            idx = i;
        }
    }
    return idx;
}

