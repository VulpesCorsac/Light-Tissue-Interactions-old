#pragma once

#include "../AD/Quadrature.h"
#include "../AD/RT.h"
#include "../Inverse/FixedParam.h"
#include "../Utils/Contracts.h"

#include "../eigen/Eigen/Dense"

namespace Inverse_NS {
    template < typename T, size_t N >
    bool SortSimplex(const std::pair<Matrix<T,1,N>,T> &a,
                     const std::pair<Matrix<T,1,N>,T> &b) noexcept;

    template < typename T, size_t N >
    int CheckConvergence(const Matrix<T,1,N>& current,
                         const Matrix<T,1,N>& previous,
                         const T& EPS) noexcept;

    template < typename T >
    T a2aComp(T a);

    template < typename T >
    T tau2tauComp(T tau);

    template < typename T >
    T g2gComp(T g);

    template < typename T >
    T aComp2a(T aC);

    template < typename T >
    T tauComp2tau(T tauC);

    template < typename T >
    T gComp2g(T gC);

    template < typename T, size_t N, Inverse_NS::FixedParameter fix >
    Matrix<T,1,N> v2vComp(const Matrix<T,1,N>& v);

    template < typename T, size_t N, Inverse_NS::FixedParameter fix >
    Matrix<T,1,N> vComp2v(const Matrix<T,1,N>& vComp);
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T, size_t N >
bool Inverse_NS::SortSimplex(const std::pair<Matrix<T,1,N>,T> &a,
                             const std::pair<Matrix<T,1,N>,T> &b) noexcept{
    return a.second < b.second;
}

template < typename T, size_t N >
int Inverse_NS::CheckConvergence(const Matrix<T,1,N>& current,
                                 const Matrix<T,1,N>& previous,
                                 const T& EPS) noexcept {
    using namespace std;

    if (current == previous)
        return 0;

    int checksum = 0;
    for (size_t m = 0; m < N; m++)
        if (abs(current(m) - previous(m)) < EPS)
             checksum += 1;
    return checksum;
}

template < typename T >
T Inverse_NS::a2aComp(T a) {
    CHECK_ARGUMENT_CONTRACT(a != 0 && a != 1);

    return (2 * a - 1) / (a * (1 - a));
}

template < typename T >
T Inverse_NS::tau2tauComp(T tau) {
    using namespace std;

    CHECK_ARGUMENT_CONTRACT(tau > 0);

    return log(tau);
}

template < typename T >
T Inverse_NS::g2gComp(T g) {
    using namespace std;

    CHECK_ARGUMENT_CONTRACT(abs(g) != 1);

    return g / (1 - abs(g));
    // return (2 * g - 1) / (g * (1 - g));
}

template < typename T >
T Inverse_NS::aComp2a(T aC) {
    using namespace Math_NS;

    if (aC == 0)
        aC += 1e-5;
    CHECK_ARGUMENT_CONTRACT(aC != 0);

    return (sqrt(sqr(aC) + 4) + aC - 2) / (2 * aC);
}

template < typename T >
T Inverse_NS::tauComp2tau(T tauC) {
    using namespace std;

    return exp(tauC);
}

template < typename T >
T Inverse_NS::gComp2g(T gC) {
    using namespace std;

    return gC / (1 + abs(gC));
    // return (sqrt(sqr(gC) + 4) + gC - 2) / (2 * gC);
}

template < typename T, size_t N, Inverse_NS::FixedParameter fix >
Matrix<T,1,N> Inverse_NS::v2vComp(const Matrix<T,1,N>& v) {
    using namespace Inverse_NS;

    CHECK_ARGUMENT_CONTRACT(2 <= N && N <= 3);

    Matrix<T,1,N> vComp;
    if (N == 3)
        vComp << a2aComp<T>(v(0)), tau2tauComp<T>(v(1)), g2gComp<T>(v(2));
    else if (N == 2) {
        CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);

        if (fix == FixedParameter::Tau)
            vComp << a2aComp<T>(v(0)), g2gComp<T>(v(1));
        else if (fix == FixedParameter::G)
            vComp << a2aComp<T>(v(0)), tau2tauComp<T>(v(1));
    }
    return vComp;
}

template < typename T, size_t N, Inverse_NS::FixedParameter fix >
Matrix<T,1,N> Inverse_NS::vComp2v(const Matrix<T,1,N>& vComp) {
    using namespace Inverse_NS;

    CHECK_ARGUMENT_CONTRACT(2 <= N && N <= 3);

    Matrix<T,1,N> v;
    if (N == 3)
        v << aComp2a<T>(vComp(0)), tauComp2tau<T>(vComp(1)), gComp2g<T>(vComp(2));
    else if (N == 2) {
        CHECK_ARGUMENT_CONTRACT(fix == FixedParameter::Tau || fix == FixedParameter::G);

        if (fix == FixedParameter::Tau)
            v << aComp2a<T>(vComp(0)), gComp2g<T>(vComp(1));
        else if (fix == FixedParameter::G)
            v << aComp2a<T>(vComp(0)), tauComp2tau<T>(vComp(1));
    }
    return v;
}

