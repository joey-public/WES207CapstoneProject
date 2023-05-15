#include "include/Tests.h"

void run_all_tests()
{
    std::cout << "Running Tests..." << std::endl;
    if (!(test_calc_mag()))
    {
        std::cout << "\tcalc_mag() failed!" << std::endl;
    }
    else
    {
        std::cout << "\tcalc_mag() passed!" << std::endl;
    }

    if (!(test_calc_phase()))
    {
        std::cout << "\tcalc_phase() failed!" << std::endl;
    }
    else
    {
        std::cout << "\tcalc_phase() passed!" << std::endl;
    }
}

bool test_calc_mag()
{
    std::cout << "Running test_calc_mag()\n";
    //test 0
    std::cout << "\ttest 0...";
    std::vector<std::complex<float>> tvec_0 = {
        {1.2, 3.4}, {2.1, 4.3}, {3.4, 5.6}, {4.5, 6.7}, {5.6, 7.8},
        {6.7, 8.9}, {7.8, 9.0}, {8.9, 1.2}, {9.0, 2.3}, {1.2, 3.4}
    };
    std::vector<float> gout_0 = { 
        std::abs(tvec_0[0]), std::abs(tvec_0[1]), std::abs(tvec_0[2]), 
        std::abs(tvec_0[3]), std::abs(tvec_0[4]), std::abs(tvec_0[5]), 
        std::abs(tvec_0[6]), std::abs(tvec_0[7]), std::abs(tvec_0[8]), 
        std::abs(tvec_0[9]) };
    std::vector<float> tout_0 = calc_mag(tvec_0);
    if(tout_0 != gout_0){std::cout<< "failed\n";return false;}
    else{std::cout << "pass!\n";}

    //test 1
    std::cout << "\ttest 1...";
    std::vector<std::complex<float>> tvec_1 = {
        {0.5, 0.6}, {1.5, 2.6}, {2.5, 3.6}, {3.5, 4.6}, {4.5, 5.6},
        {5.5, 6.6}, {6.5, 7.6}, {7.5, 8.6}, {8.5, 9.6}, {9.5, 0.6}
    };
    std::vector<float> gout_1 = { 
        std::abs(tvec_1[0]), std::abs(tvec_1[1]), std::abs(tvec_1[2]), 
        std::abs(tvec_1[3]), std::abs(tvec_1[4]), std::abs(tvec_1[5]), 
        std::abs(tvec_1[6]), std::abs(tvec_1[7]), std::abs(tvec_1[8]), 
        std::abs(tvec_1[9]) };
    std::vector<float> tout_1 = calc_mag(tvec_1);
    if(tout_1 != gout_1){std::cout<< "failed\n";return false;}
    else{std::cout << "pass!\n";}

    //test 2
    std::cout << "\ttest 2...";
    std::vector<std::complex<float>> tvec_2 = {
        {0.1, 0.2}, {1.1, 2.2}, {2.1, 3.2}, {3.1, 4.2}, {4.1, 5.2},
        {5.1, 6.2}, {6.1, 7.2}, {7.1, 8.2}, {8.1, 9.2}, {9.1, 0.2}
    };
    std::vector<float> gout_2 = { 
        std::abs(tvec_2[0]), std::abs(tvec_2[1]), std::abs(tvec_2[2]), 
        std::abs(tvec_2[3]), std::abs(tvec_2[4]), std::abs(tvec_2[5]), 
        std::abs(tvec_2[6]), std::abs(tvec_2[7]), std::abs(tvec_2[8]), 
        std::abs(tvec_2[9]) };
    std::vector<float> tout_2 = calc_mag(tvec_2);
    if(tout_2 != gout_2){std::cout<< "failed\n"; return false;}
    else{std::cout << "pass!\n";}

    //test 3
    std::cout << "\ttest 3...";
    std::vector<std::complex<float>> tvec_3 = {
        {0.9, 1.8}, {1.9, 2.8}, {2.9, 3.8}, {3.9, 4.8}, {4.9, 5.8},
        {5.9, 6.8}, {6.9, 7.8}, {7.9, 8.8}, {8.9, 9.8}, {9.9, 0.8}
    };
    std::vector<float> gout_3 = { 
        std::abs(tvec_3[0]), std::abs(tvec_3[1]), std::abs(tvec_3[2]), 
        std::abs(tvec_3[3]), std::abs(tvec_3[4]), std::abs(tvec_3[5]), 
        std::abs(tvec_3[6]), std::abs(tvec_3[7]), std::abs(tvec_3[8]), 
        std::abs(tvec_3[9]) };
    std::vector<float> tout_3 = calc_mag(tvec_3);
    if(tout_3 != gout_3){std::cout<< "failed\n"; return false;}
    else{std::cout << "pass!\n";}

    //test 4
    std::cout << "\ttest 4...";
    std::vector<std::complex<float>> tvec_4 = {
        {1.5, 3.7}, {2.5, 4.7}, {3.5, 5.7}, {4.5, 6.7}, {5.5, 7.7},
        {6.5, 8.7}, {7.5, 9.7}, {8.5, 0.7}, {9.5, 1.7}, {1.5, 2.7}
    };
    std::vector<float> gout_4 = { 
        std::abs(tvec_4[0]), std::abs(tvec_4[1]), std::abs(tvec_4[2]), 
        std::abs(tvec_4[3]), std::abs(tvec_4[4]), std::abs(tvec_4[5]), 
        std::abs(tvec_4[6]), std::abs(tvec_4[7]), std::abs(tvec_4[8]), 
        std::abs(tvec_4[9]) };
    std::vector<float> tout_4 = calc_mag(tvec_4);
    if(tout_4 != gout_4){std::cout<< "failed\n"; return false;}
    else{std::cout << "pass!\n";}
    
    return true;
}


bool test_calc_phase()
{
    std::cout << "Running test_calc_phase()\n";
    //test 0
    std::cout << "\ttest 0...";
    std::vector<std::complex<float>> tvec_0 = {
        {1.2, 3.4}, {2.1, 4.3}, {3.4, 5.6}, {4.5, 6.7}, {5.6, 7.8},
        {6.7, 8.9}, {7.8, 9.0}, {8.9, 1.2}, {9.0, 2.3}, {1.2, 3.4}
    };
    std::vector<float> gout_0 = { 
        std::arg(tvec_0[0]), std::arg(tvec_0[1]), std::arg(tvec_0[2]), 
        std::arg(tvec_0[3]), std::arg(tvec_0[4]), std::arg(tvec_0[5]), 
        std::arg(tvec_0[6]), std::arg(tvec_0[7]), std::arg(tvec_0[8]), 
        std::arg(tvec_0[9]) };
    std::vector<float> tout_0 = calc_phase(tvec_0);
    if(tout_0 != gout_0){std::cout<< "failed\n";return false;}
    else{std::cout << "pass!\n";}

    //test 1
    std::cout << "\ttest 1...";
    std::vector<std::complex<float>> tvec_1 = {
        {0.5, 0.6}, {1.5, 2.6}, {2.5, 3.6}, {3.5, 4.6}, {4.5, 5.6},
        {5.5, 6.6}, {6.5, 7.6}, {7.5, 8.6}, {8.5, 9.6}, {9.5, 0.6}
    };
    std::vector<float> gout_1 = { 
        std::arg(tvec_1[0]), std::arg(tvec_1[1]), std::arg(tvec_1[2]), 
        std::arg(tvec_1[3]), std::arg(tvec_1[4]), std::arg(tvec_1[5]), 
        std::arg(tvec_1[6]), std::arg(tvec_1[7]), std::arg(tvec_1[8]), 
        std::arg(tvec_1[9]) };
    std::vector<float> tout_1 = calc_phase(tvec_1);
    if(tout_1 != gout_1){std::cout<< "failed\n";return false;}
    else{std::cout << "pass!\n";}

    //test 2
    std::cout << "\ttest 2...";
    std::vector<std::complex<float>> tvec_2 = {
        {0.1, 0.2}, {1.1, 2.2}, {2.1, 3.2}, {3.1, 4.2}, {4.1, 5.2},
        {5.1, 6.2}, {6.1, 7.2}, {7.1, 8.2}, {8.1, 9.2}, {9.1, 0.2}
    };
    std::vector<float> gout_2 = { 
        std::arg(tvec_2[0]), std::arg(tvec_2[1]), std::arg(tvec_2[2]), 
        std::arg(tvec_2[3]), std::arg(tvec_2[4]), std::arg(tvec_2[5]), 
        std::arg(tvec_2[6]), std::arg(tvec_2[7]), std::arg(tvec_2[8]), 
        std::arg(tvec_2[9]) };
    std::vector<float> tout_2 = calc_phase(tvec_2);
    if(tout_2 != gout_2){std::cout<< "failed\n"; return false;}
    else{std::cout << "pass!\n";}

    //test 3
    std::cout << "\ttest 3...";
    std::vector<std::complex<float>> tvec_3 = {
        {0.9, 1.8}, {1.9, 2.8}, {2.9, 3.8}, {3.9, 4.8}, {4.9, 5.8},
        {5.9, 6.8}, {6.9, 7.8}, {7.9, 8.8}, {8.9, 9.8}, {9.9, 0.8}
    };
    std::vector<float> gout_3 = { 
        std::arg(tvec_3[0]), std::arg(tvec_3[1]), std::arg(tvec_3[2]), 
        std::arg(tvec_3[3]), std::arg(tvec_3[4]), std::arg(tvec_3[5]), 
        std::arg(tvec_3[6]), std::arg(tvec_3[7]), std::arg(tvec_3[8]), 
        std::arg(tvec_3[9]) };
    std::vector<float> tout_3 = calc_phase(tvec_3);
    if(tout_3 != gout_3){std::cout<< "failed\n"; return false;}
    else{std::cout << "pass!\n";}

    //test 4
    std::cout << "\ttest 4...";
    std::vector<std::complex<float>> tvec_4 = {
        {1.5, 3.7}, {2.5, 4.7}, {3.5, 5.7}, {4.5, 6.7}, {5.5, 7.7},
        {6.5, 8.7}, {7.5, 9.7}, {8.5, 0.7}, {9.5, 1.7}, {1.5, 2.7}
    };
    std::vector<float> gout_4 = { 
        std::arg(tvec_4[0]), std::arg(tvec_4[1]), std::arg(tvec_4[2]), 
        std::arg(tvec_4[3]), std::arg(tvec_4[4]), std::arg(tvec_4[5]), 
        std::arg(tvec_4[6]), std::arg(tvec_4[7]), std::arg(tvec_4[8]), 
        std::arg(tvec_4[9]) };
    std::vector<float> tout_4 = calc_phase(tvec_4);
    if(tout_4 != gout_4){std::cout<< "failed\n"; return false;}
    else{std::cout << "pass!\n";}
    
    return true;
}
