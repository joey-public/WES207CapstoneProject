#include <iostream>
#include <stdlib.h>

#include <eigen3/Eigen/Dense>
//#include <Eigen/Dense>

#include "localization.h"

using namespace std;

using Eigen::Vector3d;

int main() {
    
    //MATLAB generated values for testing
    double t1 = 0.000000346623060896099;
    double t2 = 0.000000272602470685208;
    double t3 = 0.000000245465060266658;
    double t4 = 0.000000127733196515759;
    
    Vector3d loc_est;
    loc_est = Localization_4Receivers_2D(t1,t2,t3,t4);
    
    cout << "Location estimate:" << endl << loc_est << endl;

    return 0;
}
