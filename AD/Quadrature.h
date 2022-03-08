#pragma once

#include "../Math/Basic.h"
#include "../Physics/Angles.h"
#include "../Utils/Contracts.h"
#include "../Utils/Utils.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <math.h>

#define _USE_MATH_DEFINES

namespace AddingDoubling_NS {
    template < typename T, size_t M >
    class Quadrature {
    public:
        Quadrature(T nSlab);
        std::array<T,M> getV() const noexcept;
        std::array<T,M> getW() const noexcept;

        void setValues(const T& nSlab) noexcept;
        void printTTT(const std::array<T,M>& arr) const noexcept;

    protected:
        void calculateQuadrature();
        void gaussQuadrature();
        void radauQuadrature();
        void mergeQuadratures();

        T vc;
        std::array<T,M/2> vG, vR, wG, wR;
        std::array<T,M> v, w;
    };
}

/// TODO
template < typename T, size_t M >
std::ostream& operator << (std::ostream& os, const AddingDoubling_NS::Quadrature<T,M>& data) noexcept;

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T, size_t M >
AddingDoubling_NS::Quadrature<T,M>::Quadrature(T nSlab) {
    setValues(nSlab);
    calculateQuadrature();
}

template < typename T, size_t M >
void AddingDoubling_NS::Quadrature<T,M>::setValues(const T& nSlab) noexcept {
    using namespace Physics_NS;

    vc = CriticalCos(nSlab);
}

template < typename T, size_t M >
void AddingDoubling_NS::Quadrature<T,M>::printTTT(const std::array<T,M>& arr) const noexcept {
    using namespace std;

    for (const auto& x : arr)
        cerr << x << ' ';
    cerr << endl;
}

template < typename T, size_t M >
std::array<T,M> AddingDoubling_NS::Quadrature<T,M>::getV() const noexcept {
    return v;
}

template < typename T, size_t M >
std::array<T,M> AddingDoubling_NS::Quadrature<T,M>::getW() const noexcept {
    return w;
}

template < typename T, size_t M >
void AddingDoubling_NS::Quadrature<T,M>::calculateQuadrature() {
    gaussQuadrature();
    radauQuadrature();
    mergeQuadratures();
}

template < typename T, size_t M >
void AddingDoubling_NS::Quadrature<T,M>::gaussQuadrature() {
    using namespace Math_NS;
    using namespace std;

    const int n = M / 2;
    for (int i = 1; i <= n; i++) {
        CHECK_RUNTIME_CONTRACT(4 * n + 1 != 0);

        T x0 = cos((M_PI * (4 * i - 1)) / (4 * n + 1));
        T xn = x0;

        CHECK_RUNTIME_CONTRACT(legendreDerivative(n, xn) != 0);

        T xn1 = xn - legendre(n, xn) / legendreDerivative(n, xn);
        while (abs(xn1 - xn) > 1E-7) {
            xn = xn1;
            xn1 = xn - legendre(n, xn) / legendreDerivative(n, xn);
        }

        // vG.push_back(xn1);
        vG[i-1] = xn1;
        // w.push_back(2 / ((1 - sqr(xn1)) * sqr(legendreDerivative(n, xn1))));
    }
    for (int i = 0; i < n; i++) {
        CHECK_RUNTIME_CONTRACT((1 - sqr(vG[i])) * sqr(legendreDerivative(n, vG[i])) != 0);

        // wG.push_back(vc / ((1 - sqr(vG[i])) * sqr(legendreDerivative(n, vG[i]))));
        wG[i] = vc / ((1 - sqr(vG[i])) * sqr(legendreDerivative(n, vG[i])));
        vG[i] = vc * (1 - vG[i]) / 2;
    }
}

template < typename T, size_t M >
void AddingDoubling_NS::Quadrature<T,M>::radauQuadrature() {
    using namespace Math_NS;
    using namespace std;

    CHECK_ARGUMENT_CONTRACT(M % 2 == 0);
    CHECK_ARGUMENT_CONTRACT(M >=  4);
    CHECK_ARGUMENT_CONTRACT(M <= 32);

    const int n = M / 2;
    array<T,n> roots;
    if (n == 2) {
        roots[0] = +0.3333333333333333;
        roots[1] = -1.0;
    } else if (n == 4) {
        roots[0] = +0.822824080974592;
        roots[1] = +0.1810662711185306;
        roots[2] = -0.5753189235216941;
        roots[3] = -1.0;
    } else if (n == 8) {
        roots[0] = +0.955041227122575;
        roots[1] = +0.7706418936781912;
        roots[2] = +0.46842035443082103;
        roots[3] = +0.09430725266111077;
        roots[4] = -0.29475056577366077;
        roots[5] = -0.6395186165262154;
        roots[6] = -0.887474878926156;
        roots[7] = -1.0;
    } else if (n == 16) {
        roots[0]  = +0.9887186220549766;
        roots[1]  = +0.941035402704115;
        roots[2]  = +0.8571740937696823;
        roots[3]  = +0.7403379488928179;
        roots[4]  = +0.5950144898997919;
        roots[5]  = +0.42678782748494587;
        roots[6]  = +0.24212262270604393;
        roots[7]  = +0.04811538307353028;
        roots[8]  = -0.14777832181337175;
        roots[9]  = -0.33803039005991975;
        roots[10] = -0.5153294780626855;
        roots[11] = -0.6728619212112203;
        roots[12] = -0.8045734013587561;
        roots[13] = -0.905400819811667;
        roots[14] = -0.9714610905263484;
        roots[15] = -1.0;
    }

    sort(ALL_CONTAINER(roots), greater<T>());

    /*
    for (const auto& x : roots) {
        wR.push_back((1 - vc) / (2 * (1 - x) * sqr(legendreDerivative(n-1, x))));
        vR.push_back((1 + vc) / 2 - (1 - vc) * x / 2);
    }
    //*/
    for (int i = 0; i < n; i++) {
        CHECK_RUNTIME_CONTRACT(2 * (1 - roots[i]) * sqr(legendreDerivative(n-1, roots[i])) != 0);

        wR[i] = (1 - vc) / (2 * (1 - roots[i]) * sqr(legendreDerivative(n-1, roots[i])));
        vR[i] = (1 + vc) / 2 - (1 - vc) * roots[i] / 2;
    }
    // wR.erase(wR.begin() + (n - 1));
    // wR.push_back((1 - vc) / sqr(n));
    wR[n-1] = (1 - vc) / sqr(n);
}

template < typename T, size_t M >
void AddingDoubling_NS::Quadrature<T,M>::mergeQuadratures() {
    // v = vG;
    // w = wG;
    // v.insert(v.end(), vR.begin(), vR.end());
    // w.insert(w.end(), wR.begin(), wR.end());
    const int m = M;
    for (int i = 0; i < m / 2; i++) {
        v[i] = vG[i];
        w[i] = wG[i];
    }
    for (int i = m / 2; i < m; i++) {
        v[i] = vR[i-m/2];
        w[i] = wR[i-m/2];
    }
}

template < typename T, size_t M >
std::ostream& operator << (std::ostream& os, const AddingDoubling_NS::Quadrature<T,M>& data) noexcept {
    /*
    for (const auto& x : arr)
        os << x << ' ';
    os << endl;
    //*/
    return os;
}
