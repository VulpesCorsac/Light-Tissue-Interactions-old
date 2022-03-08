#pragma once

#include "Photon.h"

#include <tuple>

template < typename T >
struct DetectorDistance {
    T min  = 0;
    T max  = 0;
    T step = 0;
};

template < typename T >
class IntegratingSphere {
public:
    IntegratingSphere() noexcept = default;

    /// constructor with distance
    IntegratingSphere(const T& dSphere, const T& dPort1, const T& dPort2, const T& distance) noexcept;
    /// constructor without distance
    IntegratingSphere(const T& dSphere, const T& dPort1, const T& dPort2) noexcept;
    /// constructor from mother-sphere with distance
    IntegratingSphere(const IntegratingSphere<T>& sphere, const T& distance) noexcept;

    ~IntegratingSphere() noexcept = default;

    T getDistance() const noexcept;
    T getDSphere()  const noexcept;
    T getDPort1()   const noexcept;
    T getDPort2()   const noexcept;

    T totalLight = 0;

protected:
    T distance = 0;
    T dSphere  = 0;
    T dPort1   = 0;
    T dPort2   = 0;
};

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
IntegratingSphere<T>::IntegratingSphere(const T& dSphere, const T& dPort1, const T& dPort2, const T& distance) noexcept
    : distance(distance)
    , dSphere(dSphere)
    , dPort1(dPort1)
    , dPort2(dPort2) {
}

template < typename T >
IntegratingSphere<T>::IntegratingSphere(const T& dSphere, const T& dPort1, const T& dPort2) noexcept
    : dSphere(dSphere)
    , dPort1(dPort1)
    , dPort2(dPort2) {
}

template < typename T >
IntegratingSphere<T>::IntegratingSphere(const IntegratingSphere<T>& sphere, const T& distance) noexcept
    : distance(distance)
    , dSphere(sphere.dSphere)
    , dPort1(sphere.dPort1)
    , dPort2(sphere.dPort2) {
}

template < typename T >
T IntegratingSphere<T>::getDistance() const noexcept {
    return distance;
}

template < typename T >
T IntegratingSphere<T>::getDSphere() const noexcept {
    return dSphere;
}

template < typename T >
T IntegratingSphere<T>::getDPort1() const noexcept {
    return dPort1;
}

template < typename T >
T IntegratingSphere<T>::getDPort2() const noexcept {
    return dPort2;
}
