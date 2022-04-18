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
    Vector3D<T> coord;
    if (type == SourceType::Point)
        coord = this->centreCoord;
    else if (type == SourceType::Circle) {
        T RNDx, RNDy;
        do {
            RNDx = Math_NS::random<T>(-1, 1);
            RNDy = Math_NS::random<T>(-1, 1);
        } while ((Math_NS::sqr<T>(RNDx) + Math_NS::sqr<T>(RNDy)) > 1);
        coord = Vector3D<T>(sqrt(RNDx) * radius, sqrt(RNDy) * radius, 0);
    } else if (type == SourceType::Gaussian) {
        T RNDr, RNDa;
        RNDr = Math_NS::random<T>(0, 1);
        RNDa = Math_NS::random<T>(0,1) * 2 * M_PI;
        coord = Vector3D<T>(sqrt(-log(RNDr)) * radius * cos(RNDa), sqrt(-log(RNDr)) * radius * sin(RNDa), 0);
    }
    return coord;
}
