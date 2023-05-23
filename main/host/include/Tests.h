#ifndef TESTS_H
#define TESTS_H

#include <iostream>
#include <complex>
#include <vector>
#include "../include/ProcessingFuncs.h"
#include "TypeDefs.h"

void run_all_tests();
bool test_calc_mag();
bool test_calc_phase();
bool test_detect_threshold();
bool test_xcorr_eigen();

#endif
