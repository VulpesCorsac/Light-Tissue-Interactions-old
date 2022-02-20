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

template < typename T, size_t M >
class Quadrature {
public:
    Quadrature(T nSlab);
    std::array<T,M> getV() const noexcept;
    std::array<T,M> getW() const noexcept;

    void setValues(const T& nSlab) noexcept;
    void printQuadrature(const std::array<T,M>& arr) const noexcept;

protected:
    void calculateQuadrature();
    void gaussQuadrature();
    void radauQuadrature();
    void mergeQuadratures();

    T vc;
    std::array<T,M/2> v_g, v_r, w_g, w_r;
    std::array<T,M> v, w;
};

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T, size_t M >
Quadrature<T,M>::Quadrature(T nSlab) {
    setValues(nSlab);
    calculateQuadrature();
}

template < typename T, size_t M >
void Quadrature<T,M>::setValues(const T& nSlab) noexcept {
    vc = Physics_NS::CriticalCos(nSlab);
}

template < typename T, size_t M >
void Quadrature<T,M>::printQuadrature(const std::array<T,M>& arr) const noexcept {
    using namespace std;

    for (const auto& x : arr)
        cout << x << ' ';
    cout << endl;
}

template < typename T, size_t M >
std::array<T,M> Quadrature<T,M>::getV() const noexcept {
    return v;
}

template < typename T, size_t M >
std::array<T,M> Quadrature<T,M>::getW() const noexcept {
    return w;
}

template < typename T, size_t M >
void Quadrature<T,M>::calculateQuadrature() {
    gaussQuadrature();
    radauQuadrature();
    mergeQuadratures();
}

template < typename T, size_t M >
void Quadrature<T,M>::gaussQuadrature() {
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

        // v_g.push_back(xn1);
        v_g[i-1] = xn1;
        // w.push_back(2 / ((1 - sqr(xn1)) * sqr(legendreDerivative(n, xn1))));
    }
    for (int i = 0; i < n; i++) {
        CHECK_RUNTIME_CONTRACT((1 - sqr(v_g[i])) * sqr(legendreDerivative(n, v_g[i])) != 0);

        // w_g.push_back(vc / ((1 - sqr(v_g[i])) * sqr(legendreDerivative(n, v_g[i]))));
        w_g[i] = vc / ((1 - sqr(v_g[i])) * sqr(legendreDerivative(n, v_g[i])));
        v_g[i] = vc * (1 - v_g[i]) / 2;
    }
}

template < typename T, size_t M >
void Quadrature<T,M>::radauQuadrature() {
    using namespace Math_NS;
    using namespace std;

    CHECK_ARGUMENT_CONTRACT(M % 2 == 0);
    CHECK_ARGUMENT_CONTRACT(M >=  4);
    CHECK_ARGUMENT_CONTRACT(M <= 32);

    const int n = M / 2;
    array<T,n> roots;
    if (n == 2) {
        roots[0] = 0.3333333333333333;
        roots[1] = -1.0;
    } else if (n == 4) {
        roots[0] = 0.822824080974592;
        roots[1] = 0.1810662711185306;
        roots[2] = -0.5753189235216941;
        roots[3] = -1.0;
    } else if (n == 8) {
        roots[0] = 0.955041227122575;
        roots[1] = 0.7706418936781912;
        roots[2] = 0.46842035443082103;
        roots[3] = 0.09430725266111077;
        roots[4] = -0.29475056577366077;
        roots[5] = -0.6395186165262154;
        roots[6] = -0.887474878926156;
        roots[7] = -1.0;
    } else if (n == 16) {
        roots[0] = 0.9887186220549766;
        roots[1] = 0.941035402704115;
        roots[2] = 0.8571740937696823;
        roots[3] = 0.7403379488928179;
        roots[4] = 0.5950144898997919;
        roots[5] = 0.42678782748494587;
        roots[6] = 0.24212262270604393;
        roots[7] = 0.04811538307353028;
        roots[8] = -0.14777832181337175;
        roots[9] = -0.33803039005991975;
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
        w_r.push_back((1 - vc) / (2 * (1 - x) * sqr(legendreDerivative(n-1, x))));
        v_r.push_back((1 + vc) / 2 - (1 - vc) * x / 2);
    }
    //*/
    for (int i = 0; i < n; i++) {
        CHECK_RUNTIME_CONTRACT(2 * (1 - roots[i]) * sqr(legendreDerivative(n-1, roots[i])) != 0);

        w_r[i] = (1 - vc) / (2 * (1 - roots[i]) * sqr(legendreDerivative(n-1, roots[i])));
        v_r[i] = (1 + vc) / 2 - (1 - vc) * roots[i] / 2;
    }
    // w_r.erase(w_r.begin() + (n - 1));
    // w_r.push_back((1 - vc) / sqr(n));
    w_r[n-1] = (1 - vc) / sqr(n);
}

template < typename T, size_t M >
void Quadrature<T,M>::mergeQuadratures() {
    // v = v_g;
    // w = w_g;
    // v.insert(v.end(), v_r.begin(), v_r.end());
    // w.insert(w.end(), w_r.begin(), w_r.end());
    const int m = M;
    for (int i = 0; i < m / 2; i++) {
        v[i] = v_g[i];
        w[i] = w_g[i];
    }
    for (int i = m / 2; i < m; i++) {
        v[i] = v_r[i-m/2];
        w[i] = w_r[i-m/2];
    }
}
