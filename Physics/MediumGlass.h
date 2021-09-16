#pragma once

#include "MediumInterface.h"

template < typename T >
class MediumGlass : public MediumInterface<T> {
public:
    explicit MediumGlass(const T& refractionIndex = 1) noexcept;

    T refraction(const T& temperature = 0, const T& degradation = 0) const override;
    bool isGlass() const override;

protected:
    T refractionIndex; ///< Glass refraction index
};

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
MediumGlass<T>::MediumGlass(const T& refractionIndex) noexcept
    : refractionIndex(refractionIndex) {
}

template < typename T >
T MediumGlass<T>::refraction(const T& temperature, const T& degradation) const {
    return refractionIndex;
}

template < typename T >
bool MediumGlass<T>::isGlass() const {
    return true;
}
