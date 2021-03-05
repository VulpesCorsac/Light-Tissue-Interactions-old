#ifndef REDISTRIBUTIONFUNCTION_H
#define REDISTRIBUTIONFUNCTION_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <array>
#include <numeric>
#include <Eigen/Dense>

#include "DMmethod.h"

using namespace std;
using namespace Eigen;


template <class T, size_t M>
T chi(T& g, int& k) {
    return ((pow(g, k) - pow(g, M))/(1.0 - pow(g, M)));
}

template <class T, size_t M>
Matrix<T, M, M> PP(array<T, M>& a) {
    Matrix<T, M, M> pp;
    int m = M;
    for (int i = 0; i < m; i++) { //row
        for (int j = 0; j < m; j++) { //col
            pp(i, j) = legendre(i, a[j]);
        }
    }
    return pp;
}

template <class T, size_t M>
Matrix<T, M, M> PN(array<T, M>& a) {
    Matrix<T, M, M> pn;
    int m = M;
    for (int i = 0; i < m; i++) { //row
        for (int j = 0; j < m; j++) { //col
            pn(i, j) = PP<T, M>(a)(i, j) * pow(-1, i);
        }
    }
    return pn;
}

template <class T, size_t M>
Matrix<T, M, M> HPP(array<T, M>& a, T& g) {
    Matrix<T, M, M> hpp;
    int m = M;
    if (g == 0.0) {
        hpp = Matrix<T, M, M>::Constant(1.0);
    } else {
        for (int i = 0; i < m; i++) { //row
            for (int j = i; j < m; j++) { //col
                T val = 0;
                for (int k = 0; k < m; k++)
                    val += (2*k + 1)*chi<T, M>(g, k)*PP<T, M>(a)(k, i)*PP<T, M>(a)(k, j);
                hpp(i, j) = val;
            }
        }
        for (int i = 0; i < m; i++) {
            for (int j = i + 1; j < m; j++) {
                hpp (j, i) = hpp (i, j);
            }
        }
    }
    return hpp;
}

template <class T, size_t M>
Matrix<T, M, M> HPN(array<T, M>& a, T& g) {
    Matrix<T, M, M> hpn;
    int m = M;
    if (g == 0.0) {
        hpn = Matrix<T, M, M>::Constant(1.0);
    } else {
        for (int i = 0; i < m; i++) { //row
            for (int j = i; j < m; j++) { //col
                T val = 0;
                for (int k = 0; k < m; k++)
                    val += (2*k + 1)*chi<T, M>(g, k)*PP<T, M>(a)(k, i)*PN<T, M>(a)(k, j);
                hpn(i, j) = val;
            }
            for (int i = 0; i < m; i++) {
                for (int j = i + 1; j < m; j++) {
                    hpn (j, i) = hpn (i, j);
                }
            }
        }
    }
    return hpn;
}

#endif // REDISTRIBUTIONFUNCTION_H
