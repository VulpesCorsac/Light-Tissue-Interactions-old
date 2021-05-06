#include <vector>

#pragma once

template < typename T>
class Sample {
public:
    Sample(const std::vector<Medium<T>>& mediums, const T& vacUpper, const T& vacLower);
    ~Sample() noexcept = default;

    inline T CurrentUpperBorderZ(const int& currentLayer) const noexcept { //layer numeration from 0!
        T upperBndZ = 0;
        for (int i = 0; i < currentLayer; i++) {
            upperBndZ += sample[i].D;
        }
        return upperBndZ;
    }

    inline T CurrentLowerBorderZ(const int& currentLayer) const noexcept {
        return (CurrentUpperBorderZ(currentLayer) + sample[currentLayer].D);
    }
    inline Medium<T> getMedium(const int& currentLayer) const noexcept { return sample[currentLayer]; }
    inline T getNvacUpper() const noexcept { return nVacUpper; }
    inline T getNvacLower() const noexcept { return nVacLower; }
    inline T getTotalThickness() const noexcept { return totalThickness; }
    inline int getNlayers() const noexcept { return sample.size(); }
protected:
    std::vector<Medium<T>> sample;
    T nVacLower;
    T nVacUpper;
    T totalThickness;
};

template < typename T >
Sample<T>::Sample(const std::vector<Medium<T>>& mediums, const T& vacUpper, const T& vacLower) :
    sample(mediums),
    nVacLower(vacLower),
    nVacUpper(vacUpper) {
        T thickness = 0;
        for (auto i : sample) {
            thickness += i.D;
        }
        totalThickness = thickness;
}

