#ifndef QUADRATURE_H
#define QUADRATURE_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

#define _USE_MATH_DEFINES
#define eps 1E-8
using namespace std;

template <class T>
class Quadrature
{
    public:
        Quadrature(int M, T Vc);
        vector<T> getV ();
        vector<T> getW ();

        void setValues (const int& M, const T& Vc);
        void printVector (vector<T>& vec);

    protected:
        int m;
        T vc;
        vector<T> v_g, v_r, w_g, w_r, v, w;

        void calculateQuadrature ();
        void gaussQuadrature ();
        void radauQuadrature ();
        void mergeQuadratures ();

        T d_legendre (int n, T x);

};

template <class T>
Quadrature<T>::Quadrature(int M, T Vc) {
    setValues (M, Vc);
    calculateQuadrature();
}

template <class T>
void Quadrature<T>::setValues (const int& new_m, const T& new_vc){
    m = new_m;
    vc = new_vc;
}

template <class T>
void Quadrature<T>::printVector (vector <T>& vec) {
    for (auto x : vec) {
        cout << x << ' ';
    }
    cout << endl;
}

template <class T>
vector<T> Quadrature<T>::getV (){
    return v;
}

template <class T>
vector<T> Quadrature<T>::getW (){
    return w;
}

template <class T>
void Quadrature<T>::calculateQuadrature (){
    gaussQuadrature();
    radauQuadrature();
    mergeQuadratures();
}

template <class T>
T Quadrature<T>::d_legendre (int n, T x) {
    return (n * (legendre(n - 1, x) - x * legendre(n, x)))/(1 - pow(x,2));
}

template <class T>
void Quadrature<T>::gaussQuadrature () {
    int n = m/2;
    for (int i = 1; i <= n; i++) {
        T x0 = cos((M_PI * (4*i - 1))/(4*n + 1));
        T xn = x0;
        T xn1 = xn - legendre(n, xn)/d_legendre(n,xn);
        while (fabs(xn1 - xn) > eps){
            xn = xn1;
            xn1 = xn - legendre(n, xn)/d_legendre(n,xn);
        }
        v_g.push_back (xn1);
//        w.push_back (2/((1 - pow(xn1, 2)) * pow((d_legendre(n, xn1)),2)));
    }
    for (int i = 0; i < n; i++) {
        w_g.push_back ( vc/((1 - pow(v_g[i],2)) * pow(d_legendre(n, v_g[i]), 2)) );
        v_g[i] = vc * (1 - v_g[i])/2;
    }

}
//n = M/2 must be 2, 4, 8 or 16

template <class T>
void Quadrature<T>::radauQuadrature () {
    int n = m/2;
    vector <T> roots;
    if (n == 2) {
        roots.push_back(0.3333333333333333);
        roots.push_back(-1.0);
    }
    else if (n == 4) {
        roots.push_back(0.822824080974592);
        roots.push_back(0.1810662711185306);
        roots.push_back(-0.5753189235216941);
        roots.push_back(-1.0);
    }
    else if (n == 8) {
        roots.push_back(0.955041227122575);
        roots.push_back(0.7706418936781912);
        roots.push_back(0.46842035443082103);
        roots.push_back(0.09430725266111077);
        roots.push_back(-0.29475056577366077);
        roots.push_back(-0.6395186165262154);
        roots.push_back(-0.887474878926156);
        roots.push_back(-1.0);
    }
    else if (n == 16) {
        roots.push_back(0.9887186220549766);
        roots.push_back(0.941035402704115);
        roots.push_back(0.8571740937696823);
        roots.push_back(0.7403379488928179);
        roots.push_back(0.5950144898997919);
        roots.push_back(0.42678782748494587);
        roots.push_back(0.24212262270604393);
        roots.push_back(0.04811538307353028);
        roots.push_back(-0.14777832181337175);
        roots.push_back(-0.33803039005991975);
        roots.push_back(-0.5153294780626855);
        roots.push_back(-0.6728619212112203);
        roots.push_back(-0.8045734013587561);
        roots.push_back(-0.905400819811667);
        roots.push_back(-0.9714610905263484);
        roots.push_back(-1.0);
    }
    sort(begin(roots), end(roots), greater<T>());
    for (auto x : roots) {
        w_r.push_back((1 - vc)/(2 * (1 - x) * pow(d_legendre(n-1, x), 2)));
        v_r.push_back((1 + vc)/2 - (1 - vc)*x/2);
    }
    w_r.erase(w_r.begin() + (n - 1));
    w_r.push_back((1 - vc)/pow(n,2));
}

template <class T>
void Quadrature<T>::mergeQuadratures () {
    v = v_g;
    w = w_g;
    v.insert(v.end(), v_r.begin(), v_r.end());
    w.insert(w.end(), w_r.begin(), w_r.end());
}

#endif // QUADRATURE_H
