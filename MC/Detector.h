#pragma once

#include "Photon.h"

template <typename T>
struct DetectorDistances {
    T minDist = 0.0;
    T maxDist = 0.0;
    T stepSize = 0.0;
};

template <typename T>
class IntegratingSphere {
public:
    IntegratingSphere() noexcept = default;
    IntegratingSphere(const T& new_dSphere, const T& new_dPort1, const T& new_dPort2, const T& new_dist) :
        distance(new_dist),
        dSphere(new_dSphere),
        dPort1(new_dPort1),
        dPort2(new_dPort2){}; // constructor with distance
    IntegratingSphere(const T& new_dSphere, const T& new_dPort1, const T& new_dPort2) :
        dSphere(new_dSphere),
        dPort1(new_dPort1),
        dPort2(new_dPort2){}; // constructor without distance
    IntegratingSphere(const IntegratingSphere<T>& new_sphere, const T& new_dist) :
        distance(new_dist),
        dSphere(new_sphere.dSphere),
        dPort1(new_sphere.dPort1),
        dPort2(new_sphere.dPort2) {}; // constructor from mother-sphere with distance
    ~IntegratingSphere() noexcept = default;

    T getDistance() {return distance;};
    T getDSphere() {return dSphere;};
    T getDPort1() {return dPort1;};
    T getDPort2() {return dPort2;};

    T totalLight = 0.0;

protected:
    T distance = 0.0;
    T dSphere = 0.0;
    T dPort1 = 0.0;
    T dPort2 = 0.0;

};

template <typename T>
class OpticalFiber { // to be implemented...

};
