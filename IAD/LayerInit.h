#ifndef LAYERINIT_H
#define LAYERINIT_H

#include <Eigen/LU>

#include "RedistributionFunction.h"


template <class T, size_t M>
Matrix<T, M, M> E() {
    return Matrix<T, M, M>::Identity();
}


int kd(int i, int j) {
    if (i == j)
        return 1;
    else
        return 0;
}

template <class T, size_t M>
Matrix<T, M, M> B(T a, T tau, T g, T n_slab, array<T, M> v, array<T, M> w) {
    int m = M;
    Matrix<T, M, M> myB;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            myB(i, j) = as<T, M>(a, g)*dtaus<T, M>(a, tau, g, n_slab)*w[j]*HPN<T, M>(v, g)(i,j)/(4*v[i]);
        }
    }
    return myB;
}

template <class T, size_t M>
Matrix<T, M, M> A(T a, T tau, T g, T n_slab, array<T, M> v, array<T, M> w) {
    int m = M;
    Matrix<T, M, M> myA;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            myA(i, j) = kd(i,j)*dtaus<T, M>(a, tau, g, n_slab)/(2*v[i]) - as<T, M>(a, g)*dtaus<T, M>(a, tau, g, n_slab)*w[j]*HPP<T, M>(v, g)(i,j)/(4*v[i]);
        }
    }
    return myA;
}

template <class T, size_t M>
Matrix<T, M, M> I(T a, T tau, T g, T n_slab, array<T, M> v, array<T, M> w) {
    Matrix<T, M, M> myI;
    myI = (E<T, M>() + A<T, M>(a, tau, g, n_slab, v, w)).inverse();
    return myI;
}

template <class T, size_t M>
Matrix<T, M, M> G(T a, T tau, T g, T n_slab, array<T, M> v, array<T, M> w) {
    Matrix<T, M, M> myG;
    myG = (E<T, M>() + A<T, M>(a, tau, g, n_slab, v, w) - B<T, M>(a, tau, g, n_slab, v, w)*I<T, M>(a, tau, g, n_slab, v, w)*B<T, M>(a, tau, g, n_slab, v, w)).inverse();
    return myG;
}

template <class T, size_t M>
Matrix<T, M, M> RR(T a, T tau, T g, T n_slab, array<T, M> v, array<T, M> w) {
    Matrix<T, M, M> myR;
    myR = 2 * G<T, M>(a, tau, g, n_slab, v, w) * B<T, M>(a, tau, g, n_slab, v, w) * I<T, M>(a, tau, g, n_slab, v, w);
    return myR;
}

template <class T, size_t M>
Matrix<T, M, M> TT(T a, T tau, T g, T n_slab, array<T, M> v, array<T, M> w) {
    Matrix<T, M, M> myT;
    myT = 2 * G<T, M>(a, tau, g, n_slab, v, w) - E<T, M>();
    return myT;
}

template <class T, size_t M>
Matrix<T, M, 1> twoaw(array<T, M> v, array<T, M> w) {
    Matrix<T, M, 1> t;
    int m = M;
    for (int i = 0; i < m; i++)
        t(i) = 2 * v[i] * w[i];
    return t;
}

template <class T, size_t M>
Matrix<T, M, M> Rd1(T a, T tau, T g, T n_slab, array<T, M> v, array<T, M> w) {
    int m = M;
    Matrix<T, M, M> myRd1;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            myRd1(i, j) = RR<T, M>(a, tau, g, n_slab, v, w)(i, j)/twoaw<T, M>(v, w)(j);
        }
    }
    return myRd1;
}

template <class T, size_t M>
Matrix<T, M, M> Td1(T a, T tau, T g, T Vc, array<T, M> v, array<T, M> w) {
    int m = M;
    Matrix<T, M, M> myTd1;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            myTd1(i, j) = TT<T, M>(a, tau, g, Vc, v, w)(i, j)/twoaw<T, M>(v, w)(j);
        }
    }
    return myTd1;
}

#endif // LAYERINIT_H
