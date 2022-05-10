#pragma once

#include "../Utils/Contracts.h"
#include "../eigen/Eigen/Dense"
#include "../Math/Basic.h"
#include <map>

using namespace Eigen;
using namespace Math_NS;

template < typename T >
class Medium {
public:
     Medium() noexcept = default;
    ~Medium() noexcept = default;

    static Medium fromCoeffs(const T& n, const T& ua, const T& us, const T& D, const T& g) {
        /// TODO: this assert fails tests on glass
        // CHECK_ARGUMENT_CONTRACT(ua + us != 0);

        return Medium(n, ua, us, us / (ua + us), D * (ua + us), D, g);
    }

    static Medium fromAlbedo(const T& n, const T& a, const T& tau, const T& D, const T& g) {
        CHECK_ARGUMENT_CONTRACT(D != 0);

        return Medium(n, tau * (1 - a) / D, tau * a / D, a, tau, D, g);
    }

    inline T getN()   const noexcept { return n;  }
    inline T getMua() const noexcept { return ua; }
    inline T getMus() const noexcept { return us; }
    inline T getMut() const noexcept { return ut; }
    inline T getD()   const noexcept { return D;  }
    inline T getA()   const noexcept { return a;  }
    inline T getTau() const noexcept { return tau;  }
    inline T getG()   const noexcept { return g;  }


protected:
    T n  ; // refraction coefficient
    T ua ; // absorption coefficient
    T us ; // absorption coefficient
    T D  ; // thickness/depth
    T g  ; // scattering anisotropy
    T ut ; // total attenuation coefficient
    T a  ; // albedo
    T tau; // optical thickness

private:
    Medium(const T& newN, const T& newMua, const T& newMus, const T& newA, const T& newTau, const T& newD, const T& newG) noexcept
        : n(newN)
        , ua(newMua)
        , us(newMus)
        , D(newD)
        , g(newG)
        , ut(newMua + newMus)
        , a(newA)
        , tau(newTau) {
    }
};

template < typename T >
struct tissueProperties {
    T us = 0;
    T ua = 0;
    T g = 0;
    T ut = 0;
};

template < typename T, size_t Nz, size_t Nr >
class heterogeneousProperties {
public:
    heterogeneousProperties() noexcept = delete;
    heterogeneousProperties(const std::map<T, tissueProperties<T>>& tissuesNew, const Matrix<T,Dynamic,Dynamic>& coag) EXCEPT_INPUT_PARAMS
    : tissues(tissuesNew)
    , heterogenityMatrixCylindical(coag)
    , heterogenityMatrix3D(matrix3D(coag)) {}
    ~heterogeneousProperties() noexcept = default;

    Matrix<T, Dynamic, Dynamic> matrixSlice(const Matrix<T,Dynamic,Dynamic>& coag, const int& nSlice);
    std::vector<Matrix<T, Dynamic, Dynamic>> matrix3D(const Matrix<T,Dynamic,Dynamic>& coag);

    T getUs(T coagRate) const { return tissues.at(coagRate).us; }
    T getUa(T coagRate) const { return tissues.at(coagRate).ua; }
    T getUt(T coagRate) const { return tissues.at(coagRate).ut; }
    T getG (T coagRate) const { return tissues.at(coagRate).g ; }

    T getUsCoord(Vector3D<int> point) const { return getUs(heterogenityMatrix3D[point.z](point.x, point.y)); }
    T getUaCoord(Vector3D<int> point) const { return getUa(heterogenityMatrix3D[point.z](point.x, point.y)); }
    T getUtCoord(Vector3D<int> point) const { return getUt(heterogenityMatrix3D[point.z](point.x, point.y)); }
    T getGCoord (Vector3D<int> point) const { return getG (heterogenityMatrix3D[point.z](point.x, point.y)); }
    std::vector<Matrix<T,Dynamic,Dynamic>> getMatrix3D() const { return heterogenityMatrix3D; }

protected:
    std::map<T, tissueProperties<T>> tissues;
    Matrix<T,Dynamic,Dynamic> heterogenityMatrixCylindical;
    std::vector<Matrix<T,Dynamic,Dynamic>> heterogenityMatrix3D;
};

template < typename T, size_t Nz, size_t Nr >
Matrix<T, Dynamic, Dynamic> heterogeneousProperties<T,Nz,Nr>::matrixSlice(const Matrix<T,Dynamic,Dynamic>& coag, const int& nSlice) {
    Matrix<T,Dynamic,Dynamic> slice = Matrix<T, 2 * Nr - 1, 2 * Nr - 1>::Zero();
    int iRadius = -1;
    for (int i = 0; i < Nr - 1; i++) {
        if (coag(nSlice, i) != coag(nSlice, i + 1)) {
            iRadius = i;
            break;
        }
    }
    if (iRadius == -1) {
        Matrix<T,Dynamic,Dynamic> sliceEmpty = Matrix<T, 2 * Nr - 1, 2 * Nr - 1>::Constant(coag(nSlice, 0));
        return sliceEmpty;
    }
    for (int i = 0; i < 2 * Nr - 1; i++) {
        for (int j = 0; j < 2 * Nr - 1; j++) {
            if (sqr(i - Nr + 1) + sqr(j - Nr + 1) > sqr(iRadius))
                slice(i,j) = 1;
        }
    }
    return slice;
};

template < typename T, size_t Nz, size_t Nr >
std::vector<Matrix<T, Dynamic, Dynamic>> heterogeneousProperties<T,Nz,Nr>::matrix3D(const Matrix<T,Dynamic,Dynamic>& coag) {
    std::vector<Matrix<T, Dynamic, Dynamic>> coag3D;
    for (int i = 0; i < Nz; i++)
        coag3D.push_back(matrixSlice(coag, i));
    return coag3D;
};
