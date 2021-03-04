#pragma once

#include "Photon.hpp"
#include "Tissue.hpp"

#include <vector>
#include <iostream>

template<class T>
class DataContainer {
private:
    std::vector<T> Reflect, Transit;
    std::vector<std::vector<T>> Medium;
public:
    int NR, NZ;
    T Z0, Z1, R1;

    DataContainer() {}
    DataContainer(int NR, int NZ, T Z0, T Z1, T R1) : NR(NR), NZ(NZ), Z0(Z0), Z1(Z1), R1(R1) {
        Reflect = std::vector<T>(NR, 0);
        Transit = std::vector<T>(NR, 0);
        Medium = std::vector<std::vector<T>>(NZ, std::vector<T>(NR, 0));
    }

    void add_reflect(T r, T w) { if(r < R1) Reflect[static_cast<int>(NR*r/R1)] += w;}
    void add_transit(T r, T w) { if(r < R1) Transit[static_cast<int>(NR*r/R1)] += w;}
    void add_medium(T z, T r, T w) { if(r < R1 && z > Z0 && z < Z1) Medium[static_cast<int>(NZ*z/(Z1 - Z0))][static_cast<int>(NR*r/R1)] += w;}

    std::vector<T>& get_reflect() { return Reflect;}
    std::vector<T>& get_transit() { return Transit;}
    std::vector<std::vector<T>>& get_medium() { return Medium;}


    void print_to_output() {
        std::cerr << "\nWriting in output";

        T max_weight = 0;
        for(int i = 0; i < NR; i++) {
            for(int j = 0; j < NZ; j++){
                if(max_weight < Medium[j][i])
                    max_weight = Medium[j][i];
            }
        }

        std::cout << "P3\n" << NR << ' ' << NZ << "\n255\n";
        for(int h = NZ - 1; h >= 0; --h) {
            for(int w = 0; w < NR; ++w){
                int grayvalue = static_cast<int>(255.999*sqrt(Medium[h][w]/max_weight)); // gamma corrected
                std::cout << grayvalue << ' ' << grayvalue << ' ' << grayvalue << '\n';
            }
        }
    }
};

