#pragma once

#include "../Math/Random.h"
#include "../Math/Vector3.h"
#include <math.h>

enum class SourceType {
    Point = 0,
    Circle = 1,
    Gaussian = 2
};

template < typename T >
class LightSource {
public:
    LightSource() noexcept = default;
    LightSource(const Vector3D<T>& newCentreCoord, const T& newRadius, SourceType newType) noexcept;
    ~LightSource() noexcept = default;

    Vector3D<T> getPhotonCoord() const noexcept;
protected:
    SourceType type;
    Vector3D<T> centreCoord;
    T radius;
};

template < typename T >
LightSource<T>::LightSource(const Vector3D<T>& newCentreCoord, const T& newRadius, SourceType newType) noexcept
    : type(newType)
    , centreCoord(newCentreCoord)
    , radius(newRadius){
}

template < typename T >
Vector3D<T> LightSource<T>::getPhotonCoord() const noexcept {
    using namespace Math_NS;

    Vector3D<T> coord;
    if (type == SourceType::Point)
        coord = this->centreCoord;
    else if (type == SourceType::Circle) {
        T RNDx, RNDy;
        do {
            RNDx = random<T>(-1, 1);
            RNDy = random<T>(-1, 1);
        } while ((sqr(RNDx) + sqr(RNDy)) > 1);
        coord = Vector3D<T>(sqrt(RNDx) * radius, sqrt(RNDy) * radius, 0);
    } else if (type == SourceType::Gaussian) {
        T RNDr, RNDa;
        RNDr = random<T>(0, 1);
        RNDa = random<T>(0,1) * 2 * M_PI;
        coord = Vector3D<T>(sqrt(-log(RNDr)) * radius * cos(RNDa), sqrt(-log(RNDr)) * radius * sin(RNDa), 0);
    }
    return coord;
}
