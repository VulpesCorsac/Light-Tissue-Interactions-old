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

/*
template <typename T>
void Detector<T>::movePhotons (std::vector<Photon<T>>& photons, const T& dist) {
    using namespace std;
    int s = photons.size();
    for (int i = 0; i < s; i++) {
        T stepSize = abs(dist / photons[i].direction.z);
        photons[i].coordinate += stepSize * photons[i].direction;
    }
}*/
/*
template <typename T>
T SphereOnePort<T>::dataOnePort (std::vector<Photon<T>>& photons, const int& Nphotons) {
    using namespace std;
    vector<Photon<T>> photonsInsideSphere;
    this->movePhotons(photons, this->distance);
    int s = photons.size();
    T totalWeight = 0;
    for (int i = 0; i < s; i++) {
        if ((sqr(photons[i].coordinate.x) + sqr(photons[i].coordinate.y)) < sqr(dPort/2)) {
            photonsInsideSphere.push_back(photons[i]);
            totalWeight += photons[i].weight;
            }
    }
    return totalWeight / Nphotons;
}

template <typename T>

T SphereTwoPorts<T>::dataTwoPorts (std::vector<Photon<T>>& photons, const int& Nphotons) {
    using namespace std;

    vector<Photon<T>> photonsThroughPort1;
    this->movePhotons(photons, this->distance);
    int s = photons.size();

    for (int i = 0; i < s; i++) {
        if ((sqr(photons[i].coordinate.x) + sqr(photons[i].coordinate.y)) < sqr(dPort1/2)) {
            photonsThroughPort1.push_back(photons[i]);
            }
    }

    vector<Photon<T>> photonsInsideSphere;
    this->movePhotons(photonsThroughPort1, dSphere);
    T totalWeight = 0;
    int s1 = photonsThroughPort1.size();

    for (int i = 0; i < s1; i++) {
        if ((sqr(photonsThroughPort1[i].coordinate.x) + sqr(photonsThroughPort1[i].coordinate.y)) >= sqr(dPort2/2)) {
            photonsInsideSphere.push_back(photonsThroughPort1[i]);
            totalWeight += photonsThroughPort1[i].weight;
            }
    }

    return totalWeight / Nphotons;
}
*/
