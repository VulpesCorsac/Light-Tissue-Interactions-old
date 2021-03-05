#ifndef QUADRATURE_H
#define QUADRATURE_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <array>

#define _USE_MATH_DEFINES
const int eps = 1E-8;
using namespace std;

template <class T, size_t M>
class Quadrature
{
    public:
        Quadrature(T Vc);
        array<T, M> getV ();
        array<T, M> getW ();

        void setValues (const T& Vc);
        void printQuadrature (array<T, M>& arr);

    protected:
        T vc;
        array<T, (M/2)> v_g, v_r, w_g, w_r;
        array<T, M> v, w;

        void calculateQuadrature ();
        void gaussQuadrature ();
        void radauQuadrature ();
        void mergeQuadratures ();

        T d_legendre (int n, T x);

};

template <class T, size_t M>
Quadrature<T, M>::Quadrature(T Vc) {
    setValues (Vc);
    calculateQuadrature();
}

template <class T, size_t M>
void Quadrature<T, M>::setValues (const T& new_vc){
    vc = new_vc;
}

template <class T, size_t M>
void Quadrature<T, M>::printQuadrature (array<T, M>& arr) {
    for (auto x : arr) {
        cout << x << ' ';
    }
    cout << endl;
}

template <class T, size_t M>
array<T, M> Quadrature<T, M>::getV (){
    return v;
}

template <class T, size_t M>
array<T, M> Quadrature<T, M>::getW (){
    return w;
}

template <class T, size_t M>
void Quadrature<T, M>::calculateQuadrature (){
    gaussQuadrature();
    radauQuadrature();
    mergeQuadratures();
}

template <class T, size_t M>
T Quadrature<T, M>::d_legendre (int n, T x) {
    return (n * (legendre(n - 1, x) - x * legendre(n, x)))/(1 - pow(x,2));
}

template <class T, size_t M>
void Quadrature<T, M>::gaussQuadrature () {
    int n = M/2;
    for (int i = 1; i <= n; i++) {
        T x0 = cos((M_PI * (4*i - 1))/(4*n + 1));
        T xn = x0;
        T xn1 = xn - legendre(n, xn)/d_legendre(n,xn);
        while (fabs(xn1 - xn) > eps){
            xn = xn1;
            xn1 = xn - legendre(n, xn)/d_legendre(n,xn);
        }
//        v_g.push_back (xn1);
        v_g[i-1] = xn1;
//        w.push_back (2/((1 - pow(xn1, 2)) * pow((d_legendre(n, xn1)),2)));
    }
    for (int i = 0; i < n; i++) {
//        w_g.push_back ( vc/((1 - pow(v_g[i],2)) * pow(d_legendre(n, v_g[i]), 2)) );
        w_g[i] = vc/((1 - pow(v_g[i],2)) * pow(d_legendre(n, v_g[i]), 2));
        v_g[i] = vc * (1 - v_g[i])/2;
    }


}
//n = M/2 must be 2, 4, 8 or 16

template <class T, size_t M>
void Quadrature<T, M>::radauQuadrature () {
    int n = M/2;
    array<T, (M/2)> roots;
    if (n == 2) {
        roots[0] = 0.3333333333333333;
        roots[1] = -1.0;
    }
    else if (n == 4) {
        roots[0] = 0.822824080974592;
        roots[1] = 0.1810662711185306;
        roots[2] = -0.5753189235216941;
        roots[3] = -1.0;
    }
    else if (n == 8) {
        roots[0] = 0.955041227122575;
        roots[1] = 0.7706418936781912;
        roots[2] = 0.46842035443082103;
        roots[3] = 0.09430725266111077;
        roots[4] = -0.29475056577366077;
        roots[5] = -0.6395186165262154;
        roots[6] = -0.887474878926156;
        roots[7] = -1.0;
    }
    else if (n == 16) {
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
    sort(begin(roots), end(roots), greater<T>());
 /*   for (auto x : roots) {
        w_r.push_back((1 - vc)/(2 * (1 - x) * pow(d_legendre(n-1, x), 2)));
        v_r.push_back((1 + vc)/2 - (1 - vc)*x/2);
    }*/
    for (int i = 0; i < n; i++) {
        w_r[i] = (1 - vc)/(2 * (1 - roots[i]) * pow(d_legendre(n-1, roots[i]), 2));
        v_r[i] = (1 + vc)/2 - (1 - vc)*roots[i]/2;
    }
//    w_r.erase(w_r.begin() + (n - 1));
//    w_r.push_back((1 - vc)/pow(n,2));
    w_r[n-1] = (1 - vc)/pow(n,2);

}

template <class T, size_t M>
void Quadrature<T, M>::mergeQuadratures () {
//    v = v_g;
//    w = w_g;
//    v.insert(v.end(), v_r.begin(), v_r.end());
//    w.insert(w.end(), w_r.begin(), w_r.end());
    int m = M;
    for (int i = 0; i < m/2; i++) {
        v[i] = v_g[i];
        w[i] = w_g[i];
    }
    for (int i = m/2; i < m; i++) {
        v[i] = v_r[i - m/2];
        w[i] = w_r[i - m/2];
    }
}

#endif // QUADRATURE_H
