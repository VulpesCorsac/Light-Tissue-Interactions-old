#include "MCML/Photon.h"
#include "MCML/Vector3.h"
#include "MCML/Tissue.h"
#include "MCML/General.h"
#include "MCML/DataContainer.h"
#include "MCML/ThreadManagement.h"

#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <mutex>
#include <algorithm>

int main() {
	using T = double;
	
    //test script with Buger adsorbtion
    T mean_free_pathlength = 0.5;
    T attenuation = 0.1;
    T g = 1;
    T width = 5;
    T n = 1.5;

    Tissue<T> tissue(width, n, mean_free_pathlength, g, attenuation);
    T z0 = 0;
    T z1 = 5;
    T r1 = 10;
    int nR = 500;
    int nZ = 500;

    DataContainer<T> data(nR, nZ, z0, z1, r1);

    T num = 1000000;
    T treshold = 0.01;
    T chance = 0.1; //check to be greater than treshold

    bool debug = false;
    int numder_of_threads = 4;

    set_up_threads(numder_of_threads, tissue, data, ThreadParams<T>(num, chance, treshold, debug, 0), true);


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
