#pragma once

#include "../Utils/Utils.h"

#include <complex>
#include <cmath>

template < typename T >
T Cos2(const T& n1, const T& n2, const T& cos1) {
    return real(std::sqrt(std::complex<T>(1 - sqr(n1 / n2) * (1 - sqr(cos1)))));
}

template < typename T >
T FresnelR(const T& n1, const T& n2, const T& cos1) {
    T cos2 = Cos2(n1, n2, cos1);
 //   std::cout << cos2 << std::endl;
    T cached1 = (n2*std::abs(cos1) - n1*cos2) / (n2*std::abs(cos1) + n1*cos2);
    T cached2 = (n1*std::abs(cos1) - n2*cos2) / (n1*std::abs(cos1) + n2*cos2);
  //  std::cout <<cached1 << " " << cached2 << std::endl;
    return 0.5*sqr(cached1) + 0.5*sqr(cached2);
}

template < typename T >
T critCos(const T& n1, const T& n2) {
    if (n1 < n2)
        return std::cos(std::asin(n1/n2));
    else
        return std::cos(std::asin(n2/n1));
}
