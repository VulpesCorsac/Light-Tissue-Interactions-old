#include "src/Photon.hpp"
#include "src/Vector3.hpp"
#include "src/Tissue.hpp"
#include "src/General.hpp"
#include "src/DataContainer.hpp"
#include "src/ThreadManagement.hpp"

#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <mutex>
#include <algorithm>


int main() {
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
    int numder_of_threads = 4;

    set_up_threads(numder_of_threads, tissue, data, ThreadParams<double>(num, chance, treshold, debug, 0));


    double transited = std::accumulate(data.get_transit().begin(), data.get_transit().end(), 0);
    double reflected = std::accumulate(data.get_reflect().begin(), data.get_reflect().end(), 0);
    double adsorbed = 0;

    for(int i = 0; i < data.NZ; i++)
        adsorbed += std::accumulate(data.get_medium()[i].begin(), data.get_medium()[i].end(), 0);

    std::cout << std::endl << "------------------------------------------------" << std::endl;
    std::cout << "Coefficient of transition: " << sqrt(transited/num) << std::endl;
    //std::cout << "Coefficient of reflectance: " << reflected/num << std::endl;
    //std::cout << "Coefficient of adsorbtion: " << adsorbed/num << std::endl;
    std::cout << "Buger lambert says: " << exp(-attenuation*width) << std::endl;

    return 0;
}
