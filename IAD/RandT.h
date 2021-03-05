#ifndef RANDT_H
#define RANDT_H

#include "Fresnel.h"


template <class T, size_t M>
Matrix<T, M, M> Rslab(T& a, T& tau, T& g, T& Vc, array<T, M>& v, array<T, M>& w) {
    Matrix<T, M, M> myRslab;
    Matrix<T, M, M> myTslab;
    doubling<T, M>(a, tau, g, Vc, v, w, myRslab, myTslab);
    return myRslab;
}

template <class T, size_t M>
Matrix<T, M, M> Tslab(T& a, T& tau, T& g, T& Vc, array<T, M>& v, array<T, M>& w) {
    Matrix<T, M, M> myRslab;
    Matrix<T, M, M> myTslab;
    doubling<T, M>(a, tau, g, Vc, v, w, myRslab, myTslab);
    return myTslab;
}

template <class T, size_t M>
Matrix<T, M, M> Rbound(T a, T tau, T g, T Vc, T n_slab, T n_slide, array<T, M> v, array<T, M> w) {
    Matrix<T, M, M> myRb = E<T, M> ();
    int m = M;
    for (int i = 0; i < m; i++)
        myRb(i, i)= real(twoaw<T, M>(v, w)(i) * (FresnelR(n_slab, n_slide, v[i]) + FresnelR(n_slide, 1.0, Vt(n_slab, n_slide, v[i])) - 2*FresnelR(n_slab, n_slide, v[i])*FresnelR(n_slide, 1.0, Vt(n_slab, n_slide, v[i]))) / (1 - FresnelR(n_slab, n_slide, v[i])*FresnelR(n_slide, 1.0, Vt(n_slab, n_slide, v[i]))));
    return myRb;
}

template <class T, size_t M>
Matrix<T, M, M> Tbound(T a, T tau, T g, T Vc, T n_slab, T n_slide, array<T, M> v, array<T, M> w) {
    Matrix<T, M, M> myTb = E<T, M> ();
    int m = M;
    for (int i = 0; i < m; i++)
        myTb(i, i)= real(1 - (FresnelR(n_slab, n_slide, v[i]) + FresnelR(n_slide, 1.0, Vt(n_slab, n_slide, v[i])) - 2*FresnelR(n_slab, n_slide, v[i])*FresnelR(n_slide, 1.0, Vt(n_slab, n_slide, v[i]))) / (1 - FresnelR(n_slab, n_slide, v[i])*FresnelR(n_slide, 1.0, Vt(n_slab, n_slide, v[i]))));
    return myTb;
}

template <class T, size_t M>
Matrix<T, M, M> Ttotal(T a, T tau, T g, T Vc, T n_slab, T n_slide_top, T n_slide_bottom, array<T, M> v, array<T, M> w) {
    Matrix<T, M, M> T02t, R20t, T03t, R30t;
    addingBounds<T, M>(Rbound<T, M>(a, tau, g, Vc, n_slab, n_slide_top, v, w), Rslab<T, M>(a, tau, g, Vc, v, w), R20t, Tbound<T, M>(a, tau, g, Vc, n_slab, n_slide_top, v, w), Tslab<T, M>(a, tau, g, Vc, v, w), T02t);
    addingBounds<T, M>(R20t, Rbound<T, M>(a, tau, g, Vc, n_slab, n_slide_bottom, v, w), R30t, T02t, Tbound<T, M>(a, tau, g, Vc, n_slab, n_slide_bottom, v, w), T03t);
    return T03t;
}

template <class T, size_t M>
Matrix<T, M, M> Rtotal(T a, T tau, T g, T Vc, T n_slab, T n_slide_top, T n_slide_bottom, array<T, M> v, array<T, M> w) {
    int m = M;
    Matrix<T, M, M> T02r, R20r, T03r, R30r, R30mod;
    addingBounds<T, M>(Rbound<T, M>(a, tau, g, Vc, n_slab, n_slide_top, v, w), Rslab<T, M>(a, tau, g, Vc, v, w), R20r, Tbound<T, M>(a, tau, g, Vc, n_slab, n_slide_top, v, w), Tslab<T, M>(a, tau, g, Vc, v, w), T02r);
    addingBounds<T, M>(R20r, Rbound<T, M>(a, tau, g, Vc, n_slab, n_slide_bottom, v, w), R30r, T02r, Tbound<T, M>(a, tau, g, Vc, n_slab, n_slide_bottom, v, w), T03r);
    R30mod = R30r - Rbound<T, M>(a, tau, g, Vc, n_slab, n_slide_bottom, v, w);
    for (int i = 0; i < m; i++)
        R30mod(i, i) += Rbound<T, M>(a, tau, g, Vc, n_slab, n_slide_bottom, v, w)(i, i)/pow(twoaw<T, M>(v, w)(i),2);
    return R30mod;
}

template <class T, size_t M>
T Ts (T a, T tau, T g, T Vc, T n_slab, T n_slide_top, T n_slide_bottom, array<T, M> v, array<T, M> w) {
    T myTs = 0.0;
    int m = M;
    Matrix<T, M, M> Ttot = Ttotal<T, M> (a, tau, g, Vc, n_slab, n_slide_top, n_slide_bottom, v, w);
    for (int i = 0; i < m; i++)
        myTs += 2 * v[i] * w[i] * Ttot(i, m - 1);
    return myTs;
}

template <class T, size_t M>
T Rs (T a, T tau, T g, T Vc, T n_slab, T n_slide_top, T n_slide_bottom, array<T, M> v, array<T, M> w) {
    T myRs = 0.0;
    int m = M;
    Matrix<T, M, M> Rtot = Rtotal<T, M> (a, tau, g, Vc, n_slab, n_slide_top, n_slide_bottom, v, w);
    for (int i = 0; i < m; i++)
        myRs += 2 * v[i] * w[i] * Rtot(i, m - 1);
    return myRs;
}

template <class T, size_t M>
T Rborder (T n_slab, T n_slide) {
    return (FresnelR(n_slab, n_slide, 1.0) + FresnelR(n_slide, 1.0, 1.0) - 2*FresnelR(n_slab, n_slide, 1.0)*FresnelR(n_slide, 1.0, 1.0))/(1.0 - FresnelR(n_slab, n_slide, 1.0)*FresnelR(n_slide, 1.0, 1.0));
}

template <class T, size_t M>
T Tc (T tau, T n_slab, T n_slide_top, T n_slide_bottom) {
    T Rbtop = Rborder<T, M> (n_slab, n_slide_top);
    T Rbbottom = Rborder<T, M> (n_slab, n_slide_bottom);
    return ((1 - Rbtop)*(1 - Rbbottom)*exp(-tau))/(1 - Rbtop*Rbbottom*exp(-2*tau));
}



#endif // RANDT_H
