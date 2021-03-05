#ifndef ADDINGetDOUBLING_H
#define ADDINGetDOUBLING_H

#include <Eigen/LU>


template <class T, size_t M>
Matrix<T, M, M> newE(array<T, M> v, array<T, M> w) {
    Matrix<T, M, M> myE = E<T, M> ();
    int m = M;
    for (int i = 0; i < m; i++)
        myE(i, i)= 1/twoaw<T, M>(v, w)(i);
    return myE;
}

template <class T, size_t M>
Matrix<T, M, M> star(array<T, M> v, array<T, M> w) {
    Matrix<T, M, M> myStar = E<T, M> ();
    int m = M;
    for (int i = 0; i < m; i++)
        myStar(i, i)= twoaw<T, M>(v, w)(i);
    return myStar;
}

template <class T, size_t M>
void adding (Matrix<T, M, M> R01, Matrix<T, M, M> R12, Matrix<T, M, M>& R02, Matrix<T, M, M> T01, Matrix<T, M, M> T12, Matrix<T, M, M>& T02, array<T, M> v, array<T, M> w) {

    T02 = T12 * (newE<T, M>(v, w) - R01 * star<T, M>(v, w) * R12).inverse() * T01;
    R02 = T12 * (newE<T, M>(v, w) - R01 * star<T, M>(v, w) * R12).inverse() * R01 * star<T, M>(v, w) * T12 + R12;

}

template <class T, size_t M>
void addingBounds (Matrix<T, M, M> R01, Matrix<T, M, M> R12, Matrix<T, M, M>& R02, Matrix<T, M, M> T01, Matrix<T, M, M> T12, Matrix<T, M, M>& T02) {

    T02 = T12 * (E<T, M>() - R01 * R12).inverse() * T01;
    R02 = T12 * (E<T, M>() - R01 * R12).inverse() * R01 * T12 + R12;

}

template <class T, size_t M>
void doubling (T a, T tau, T g, T n_slab, array<T, M> v, array<T, M> w, Matrix<T, M, M>& Rs, Matrix<T, M, M>& Ts) {
    int n = n1<T, M>(a, tau, g, n_slab);
    Matrix<T, M, M> myR = Rd1<T, M>(a, tau, g, n_slab, v, w);
    Matrix<T, M, M> myT = Td1<T, M>(a, tau, g, n_slab, v, w);
    for (int i = 0; i < n; i ++) {
        Matrix<T, M, M> oldT = myT;
        Matrix<T, M, M> oldR = myR;
        adding<T, M>(oldR, oldR, myR, oldT, oldT, myT, v, w);
    }

    Rs = myR;
    Ts = myT;
}




#endif // ADDINGetDOUBLING_H
