#include <iostream>
#include <math.h>
#include <localization.h>

#include <eigen3/Eigen/Dense>

using namespace std;

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
    double Rs = real(-Rs_b+sqrt(pow(Rs_b,2)-4.0*Rs_a*Rs_c))/(2.0*Rs_a);
    
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

