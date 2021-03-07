#pragma once

#include <vector>
#include <iostream>

template < typename T >
class DataContainer {
public:
    DataContainer(int NR, int NZ, T Z0, T Z1, T R1) noexcept
        : NR(NR)
        , NZ(NZ)
        , Z0(Z0)
        , Z1(Z1)
        , R1(R1)
        , Reflect(std::vector<T>(NR, 0))
        , Transit(std::vector<T>(NR, 0))
        , Medium(std::vector<std::vector<T>>(NZ, std::vector<T>(NR, 0))) {}

    void add_reflect(T r, T w) noexcept { if (r < R1) Reflect[static_cast<int>(NR*r/R1)] += w;}
    void add_transit(T r, T w) noexcept { if (r < R1) Transit[static_cast<int>(NR*r/R1)] += w;}
    void add_medium(T z, T r, T w) noexcept { if (r < R1 && z > Z0 && z < Z1) Medium[static_cast<int>(NZ*z/(Z1 - Z0))][static_cast<int>(NR*r/R1)] += w;}

    std::vector<T>& get_reflect() noexcept { return Reflect;}
    std::vector<T>& get_transit() noexcept { return Transit;}
    std::vector<std::vector<T>>& get_medium() noexcept { return Medium;}

    void print_to_output() noexcept {
        std::cerr << "\nWriting in output";
        T max_weight = 0;
        for (const auto& layer: Medium)
            for (const auto& cell: layer)
                max_weight = std::max(max_weight, cell);

        std::cout << "P3\n" << NR << ' ' << NZ << "\n255\n";
        for (int h = NZ - 1; h >= 0; --h)
            for (int w = 0; w < NR; ++w) {
                int grayvalue = static_cast<int>(255.999*sqrt(Medium[h][w]/max_weight)); // gamma corrected
                std::cout << grayvalue << ' ' << grayvalue << ' ' << grayvalue << '\n';
            }
    }

public:
    const int NR;
    const int NZ;
    const T Z0;
    const T Z1;
    const T R1;

private:
    std::vector<T> Reflect, Transit;
    std::vector<std::vector<T>> Medium;
};

