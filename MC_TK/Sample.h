#include <array>

#pragma once

template < typename T, size_t nLayers >
class Sample {
public:
    Sample() noexcept = default;
    Sample(const std::array<Medium<T>, nLayers>& mediums, const T& vacUpper, const T& vacLower);
    ~Sample() noexcept = default;

    inline T CurrentUpperBorderZ(const int& currentLayer) const noexcept { //layer numeration from 0!
        T upperBndZ = 0;
        for (int i = 0; i < currentLayer; i++) {
            upperBndZ += sample[i].getD();
        }
        return upperBndZ;
    }

    inline T CurrentLowerBorderZ(const int& currentLayer) const noexcept {
        return (CurrentUpperBorderZ(currentLayer) + sample[currentLayer].getD());
    }

    inline T getTotalThickness() const noexcept {
        T thickness = 0;
        for (int i = 0; i < nLayers; i++) {
            thickness += sample[i].getD();
        }
        return thickness;
    }

    inline Medium<T> getMedium(const int& currentLayer) const noexcept { return sample[currentLayer]; }
    inline T getNvacUpper() const noexcept { return nVacUpper; }
    inline T getNvacLower() const noexcept { return nVacLower; }
protected:
    std::array<Medium<T>, nLayers> sample;
    T nVacLower;
    T nVacUpper;

};
// number of layers, coordinates of boundaries, nVac

template < typename T, size_t nLayers >
Sample<T, nLayers>::Sample(const std::array<Medium<T>, nLayers>& mediums, const T& vacUpper, const T& vacLower) :
    sample(mediums),
    nVacLower(vacLower),
    nVacUpper(vacUpper) {
}

