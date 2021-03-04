#include "../src/ThreadManagement.hpp"

#include <gtest/gtest.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>


using namespace testing;

TEST(TEST_INIT_UNDEF, BUGER)
{
    //test script with Buger adsorbtion
    double mean_free_pathlength = 0.5;
    double attenuation = 0.1;
    double g = 1;
    double width = 5;
    double n = 1.5;

    Tissue<double> tissue(width, n, mean_free_pathlength, g, attenuation);
    double z0 = 0;
    double z1 = 5;
    double r1 = 10;
    int nR = 500;
    int nZ = 500;

    DataContainer<double> data(nR, nZ, z0, z1, r1);

    double num = 1000000;
    double treshold = 0.01;
    double chance = 0.1; //check to be greater than treshold

    bool debug = false;
    int numder_of_threads = 1;

    set_up_threads(numder_of_threads, tissue, data, ThreadParams<double>(num, chance, treshold, debug, 0), false);


    double transited = std::accumulate(data.get_transit().begin(), data.get_transit().end(), 0);

    EXPECT_EQ(static_cast<int>(sqrt(transited/num)*100), static_cast<int>(exp(-width*attenuation)*100));
}

