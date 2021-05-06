#pragma once

#include "../Utils/Utils.h"

#include <complex>
#include <cmath>

template < typename T >
T CosT(const T& ni, const T& nt, const T& cosi) {
      T cached = 1 - sqr(ni / nt) * (1 - sqr(cosi));
      if (cached < 0)
        return 0;
      else if (cosi > 0)
        return std::sqrt(cached);
      else
        return -std::sqrt(cached);

   // return real(std::sqrt(std::complex<T>(1 - sqr(ni / nt) * (1 - sqr(cosi)))));
}

template < typename T >
T FresnelR(const T& ni, const T& nt, const T& cosi) {
    T cost = CosT(ni, nt, cosi);
 //   std::cout << cost << std::endl;
    T cached1 = (nt*cosi - ni*cost) / (nt*cosi + ni*cost);
    T cached2 = (ni*cosi - nt*cost) / (ni*cosi + nt*cost);
  //  std::cout <<cached1 << " " << cached2 << std::endl;
    return 0.5*sqr(cached1) + 0.5*sqr(cached2);
}

template < typename T >
T critCos(const T& ni, const T& nt) {
    if (ni < nt)
        return std::cos(std::asin(ni/nt));
    else
        return std::cos(std::asin(nt/ni));
}
