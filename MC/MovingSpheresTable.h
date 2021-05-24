#pragma once

#include "Detector.h"

template <typename T>
class MovingSpheresTable {
    public:
        MovingSpheresTable() noexcept = default;
        MovingSpheresTable(const T& new_minDist, const T& new_maxDist, const T& new_step) :
           minDist(new_minDist),
           maxDist(new_maxDist),
           step(new_step),
           length((maxDist - minDist) / step + 1) {
                for (int i = 0; i < length; i++)
                    table.push_back(std::make_pair(minDist + i*step, 0.0));
           };
        ~MovingSpheresTable() noexcept = default;

        std::vector<std::pair<T, T>> getTableOnePort(std::vector<Photon<T>>& photons, const int& Nphotons, const T& dSphere, const T& dPort) {
            fillTableOnePort(photons, Nphotons, dSphere, dPort);
            return table;
        };

        std::vector<std::pair<T, T>> getTableTwoPorts(std::vector<Photon<T>>& photons, const int& Nphotons, const T& dSphere, const T& dPort1, const T& dPort2) {
            fillTableTwoPorts(photons, Nphotons, dSphere, dPort1, dPort2);
            return table;
        };

    protected:
        T minDist = 0.0;
        T maxDist = 0.0;
        T step = 0.0;
        int length = 1;
        std::vector<std::pair<T, T>> table;

        void fillTableOnePort (std::vector<Photon<T>>& photons, const int& Nphotons, const T& dSphere, const T& dPort);
        void fillTableTwoPorts (std::vector<Photon<T>>& photons, const int& Nphotons, const T& dSphere, const T& dPort1, const T& dPort2);
};

template <typename T>
void MovingSpheresTable<T>::fillTableOnePort (std::vector<Photon<T>>& photons, const int& Nphotons, const T& dSphere, const T& dPort) {
    using namespace std;
    for (int i = 0; i < length; i++) {
        SphereOnePort<T> newSphere = SphereOnePort<T>(dSphere, dPort, table[i].first);
        table[i].second = newSphere.dataOnePort(photons, Nphotons);
    }
}

template <typename T>
void MovingSpheresTable<T>::fillTableTwoPorts (std::vector<Photon<T>>& photons, const int& Nphotons, const T& dSphere, const T& dPort1, const T& dPort2) {
    using namespace std;
    for (int i = 0; i < length; i++) {
        SphereTwoPorts<T> newSphere = SphereTwoPorts<T>(dSphere, dPort1, dPort2, table[i].first);
        table[i].second = newSphere.dataTwoPorts(photons, Nphotons);
    }
}

