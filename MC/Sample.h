#pragma once

#include <vector>
#include <stdexcept>
#include <utility>
#include <string>
#include <iostream>

template < typename T >
class Sample {
public:
    Sample() noexcept = default;
    Sample(const std::vector<Medium<T>>& mediums, const T& vacUpper, const T& vacLower);
    Sample(const std::vector<Medium<T>>& mediums);
    ~Sample() noexcept = default;

    inline T CurrentUpperBorderZ(const int& currentLayer) const noexcept { // layer numeration from 0!
        T upperBndZ = 0;
        for (int i = 0; i < currentLayer; i++)
            upperBndZ += sample[i].getD();
        return upperBndZ;
    }

    inline T CurrentLowerBorderZ(const int& currentLayer) const noexcept { return (CurrentUpperBorderZ(currentLayer) + sample[currentLayer].getD()); }
    inline Medium<T> getMedium(const int& currentLayer) const noexcept { return sample[currentLayer]; }
    inline T getNvacUpper() const noexcept { return nVacUpper; }
    inline T getNvacLower() const noexcept { return nVacLower; }
    inline T getTotalThickness() const noexcept { return totalThickness; }
    inline int getNlayers() const noexcept { return sample.size(); }
    inline T getNslab() const noexcept {
        CHECK_ARGUMENT_CONTRACT(sample.size() == 1 || sample.size() == 3);
        if (sample.size() == 1)
            return sample[0].getN();
        else if (sample.size() == 3)
            return sample[1].getN();
        else
            throw std::invalid_argument("Be a darling and feed this program either 1 or 3 layers getNslab");

    }
    inline T getNslideTop() const noexcept {
        CHECK_ARGUMENT_CONTRACT(sample.size() == 1 || sample.size() == 3);
        if (sample.size() == 1)
            return sample[0].getN();
        else if (sample.size() == 3)
            return sample[0].getN();
        else
            throw std::invalid_argument("Be a darling and feed this program either 1 or 3 layers getNslidetop");
    }
    inline T getNslideBottom() const noexcept {
        CHECK_ARGUMENT_CONTRACT(sample.size() == 1 || sample.size() == 3);
        if (sample.size() == 1)
            return sample[0].getN();
        else if (sample.size() == 3)
            return sample[2].getN();
        else
            throw std::invalid_argument("Be a darling and feed this program either 1 or 3 layers getNslidebottom");
    }
    inline Medium<T> getTurbidMedium() const noexcept {
        CHECK_ARGUMENT_CONTRACT(sample.size() == 1 || sample.size() == 3);
        if (sample.size() == 1)
            return sample[0];
        else if (sample.size() == 3)
            return sample[1];
        else
            throw std::invalid_argument("Be a darling and feed this program either 1 or 3 layers getTurbidMeduim");
    }

protected:
    std::vector<Medium<T>> sample;
    T nVacLower;
    T nVacUpper;
    T totalThickness;
};

template < typename T >
Sample<T>::Sample(const std::vector<Medium<T>>& mediums, const T& vacUpper, const T& vacLower)
    : sample(mediums)
    , nVacLower(vacLower)
    , nVacUpper(vacUpper) {
    CHECK_ARGUMENT_CONTRACT(mediums.size() == 1 || mediums.size() == 3);
    T thickness = 0;
    for (const auto& layer : sample)
        thickness += layer.getD();
    totalThickness = thickness;
}

template < typename T >
Sample<T>::Sample(const std::vector<Medium<T>>& mediums)
    : sample(mediums)
    , nVacLower(1.0)
    , nVacUpper(1.0) {
    CHECK_ARGUMENT_CONTRACT(mediums.size() == 1 || mediums.size() == 3);
    T thickness = 0;
    for (const auto& layer : sample)
        thickness += layer.getD();
    totalThickness = thickness;
}
