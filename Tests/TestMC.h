#pragma once

#include "../MC_TK/MonteCarlo.h"

#include <gtest/gtest.h>

template <typename T, size_t Nz, size_t Nr>
class testDataMC {
public:
    testDataMC(const Sample<T>& sample, const int& Np, const T& z, const T& r){
        setValues(sample, Np, z, r);
        mc.Calculate();
    }

    void setValues(const Sample<T>& new_sample, const int& new_Np, const T& new_z, const T& new_r):
        sample(new_sample),
        Np(new_Np),
        z(new_z),
        r(new_r) {
        }

    T getR() const noexcept {return mc.results.diffuseReflection;}
    T getRs() const noexcept { return mc.results.specularReflection;}
    T getT() const noexcept {return mc.results.diffuseTransmission;}
    T getA() const noexcept {return mc.results.absorbed;}

protected:
    Sample<T> sample;
    int Np;
    T z, r;
    MonteCarlo<T, Nz, Nr> mc(sample, Np, z, r);
};

TEST(MC, MCtest1) {
    using T = double;
    Medium<T> tissue(1.5, 200, 800, 4e-3, 0.5);
    Medium<T> glass(1.6, 0, 0, 1e-3, 0);
    std::vector<Medium<T>> layers = {glass,tissue,glass};
    Sample<T> mySample(layers, 1.0, 1.0);
    testDataMC<T, 1000, 10000> test(layers, 1e5, mySample.getTotalThickness(), 10e-2);
    EXPECT_NEAR(test.getR(), 0.065762, 1e-4);
    EXPECT_NEAR(test.getRs(), 0.0541872, 1e-4);
    EXPECT_NEAR(test.getT(), 0.0940703, 1e-4);
    EXPECT_NEAR(test.getA(), 0.78598, 1e-4);
}
