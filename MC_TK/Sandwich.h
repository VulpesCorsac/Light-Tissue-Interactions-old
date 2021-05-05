#include <array>

#pragma once

template < typename T, size_t nLayers >
class Sandwich {
public:
    Sandwich() noexcept = default;
    Sandwich(const std::array<Medium<T>, nLayers>& mediums, const T& vacUpper, const T& vacLower);
    ~Sandwich() noexcept = default;

    inline T CurrentUpperBorderZ(const int& currentLayer) const noexcept { //layer numeration from 0!
        T upperBndZ = 0;
        for (int i = 0; i < currentLayer; i++) {
            upperBndZ += sandwich[i].getD();
        }
        return upperBndZ;
    }

    inline T CurrentLowerBorderZ(const int& currentLayer) const noexcept {
        return (CurrentUpperBorderZ(currentLayer) + sandwich[currentLayer].getD());
    }

    inline T getTotalThickness() const noexcept {
        T thickness = 0;
        for (int i = 0; i < nLayers; i++) {
            thickness += sandwich[i].getD();
        }
        return thickness;
    }

    inline Medium<T> getMedium(const int& currentLayer) const noexcept { return sandwich[currentLayer]; }
    inline T getNvacUpper() const noexcept { return nVacUpper; }
    inline T getNvacLower() const noexcept { return nVacLower; }
protected:
    std::array<Medium<T>, nLayers> sandwich;
    T nVacLower;
    T nVacUpper;

};
// number of layers, coordinates of boundaries, nVac

template < typename T, size_t nLayers >
Sandwich<T, nLayers>::Sandwich(const std::array<Medium<T>, nLayers>& mediums, const T& vacUpper, const T& vacLower) :
    sandwich(mediums),
    nVacLower(vacLower),
    nVacUpper(vacUpper) {
}
