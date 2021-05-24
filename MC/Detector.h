#pragma once

template <typename T>
class Detector {
public:
    Detector() noexcept = default;
    Detector(const T& new_dist) : distance(new_dist) {};
    ~Detector() noexcept = default;

protected:
    T distance;
    void movePhotons (std::vector<Photon<T>>& photons, const T& dist);
};

template <typename T>
class SphereOnePort : public Detector<T> {
public:
    SphereOnePort() noexcept = default;
    SphereOnePort(const T& new_dSphere, const T& new_dPort, const T& new_dist) : Detector<T> (new_dist),
        dSphere(new_dSphere),
        dPort(new_dPort){};
    ~SphereOnePort() noexcept = default;
    T dataOnePort (std::vector<Photon<T>>& photons, const int& Nphotons);

protected:
    T dSphere;
    T dPort;
};

template <typename T>
class SphereTwoPorts : public Detector<T> {
public:
    SphereTwoPorts() noexcept = default;
    SphereTwoPorts(const T& new_dSphere, const T& new_dPort1, const T& new_dPort2, const T& new_dist) : Detector<T> (new_dist),
        dSphere(new_dSphere),
        dPort1(new_dPort1),
        dPort2(new_dPort2){};
    ~SphereTwoPorts() noexcept = default;

    T dataTwoPorts (std::vector<Photon<T>>& photons, const int& Nphotons);

protected:
    T dSphere;
    T dPort1;
    T dPort2;
};

template <typename T>
void Detector<T>::movePhotons (std::vector<Photon<T>>& photons, const T& dist) {
    using namespace std;
    int s = photons.size();
    for (int i = 0; i < s; i++) {
        T stepSize = abs(dist / photons[i].direction.z);
        photons[i].coordinate += stepSize * photons[i].direction;
    }
}

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
