#pragma once

#include "Photon.h"

template < typename T >
struct DetectorDistance {
    T min  = 0;
    T max  = 0;
    T step = 0;
};

template < typename T >
class DetectorInterface {
public:
    DetectorInterface() noexcept = default;

    virtual ~DetectorInterface() noexcept = default;

    /// TODO: detector writes all the data it needs in this function
    virtual void detectPhoton(const Photon<T>& photon) noexcept = 0;
};

template < typename T >
class IntegratingSphere {
public:
    IntegratingSphere() noexcept = default;

    /// constructor with distance
    IntegratingSphere(const T& new_dSphere, const T& new_dPort1, const T& new_dPort2, const T& new_dist) noexcept;
    /// constructor without distance
    IntegratingSphere(const T& new_dSphere, const T& new_dPort1, const T& new_dPort2) noexcept;
    /// constructor from mother-sphere with distance
    IntegratingSphere(const IntegratingSphere<T>& new_sphere, const T& new_dist) noexcept;

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

/// TODO: implement
template < typename T >
class OpticalFiber : public DetectorInterface<T> {
public:
    /// just not to be pure virtual
    void detectPhoton(const Photon<T>& photon) noexcept override {
        return;
    }
};

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
IntegratingSphere<T>::IntegratingSphere(const T& new_dSphere, const T& new_dPort1, const T& new_dPort2, const T& new_dist) noexcept
    : distance(new_dist)
    , dSphere(new_dSphere)
    , dPort1(new_dPort1)
    , dPort2(new_dPort2) {
}

template < typename T >
IntegratingSphere<T>::IntegratingSphere(const T& new_dSphere, const T& new_dPort1, const T& new_dPort2) noexcept
    : dSphere(new_dSphere)
    , dPort1(new_dPort1)
    , dPort2(new_dPort2) {
}

template < typename T >
IntegratingSphere<T>::IntegratingSphere(const IntegratingSphere<T>& new_sphere, const T& new_dist) noexcept
    : distance(new_dist)
    , dSphere(new_sphere.dSphere)
    , dPort1(new_sphere.dPort1)
    , dPort2(new_sphere.dPort2) {
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
