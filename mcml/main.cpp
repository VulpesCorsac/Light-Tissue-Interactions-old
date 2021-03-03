#include "src/Photon.hpp"
#include "src/Vector3.hpp"
#include "src/Tissue.hpp"
#include "src/General.hpp"
#include "src/DataContainer.hpp"
#include "src/ThreadManagement.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>


int main() {
    //test script with image

                        //w  n    l    g    att
    Tissue<double> tissue(5, 1.5, 0.5, 0.9, 0.1);
    double z0 = 0;
    double z1 = 5;
    double r1 = 5;
    int nR = 500;
    int nZ = 500;

    DataContainer<double> data(nR, nZ, z0, z1, r1);

    double num = 1000000;
    double treshold = 0.01;
    double chance = 0.1; //check to be greater than treshold

    bool debug = false;

    ProgressBar counter(num);
    int number_of_threads = 4;
    std::vector<std::thread> threads;
    std::mutex m;

    for(int i = 0; i < number_of_threads; ++i) {
        ThreadParams<double> tmp(num/number_of_threads, chance, treshold, debug, i);
        threads.emplace_back(std::thread(compute<double>, std::ref(tissue), std::ref(data), tmp, std::ref(counter), std::ref(m)));
    }

    while(counter.current() < counter.Total) {
        std::cerr << "\rSampling photons, done " << counter.get_percentage() << "\%";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    for(auto& t : threads)
        t.join();

    data.print_to_output();


    return 0;
}
