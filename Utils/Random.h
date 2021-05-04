#pragma once

#include <random>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>



template < typename T >
T random(T min, T max) {
    static std::random_device rd;
    static thread_local std::default_random_engine generator(rd());
    static std::uniform_real_distribution < T > distribution(min, max);
    return distribution(generator);
}

#define MBIG 1000000000
 #define MSEED 161803398
 #define MZ 0
 #define FAC 1.0E-9

 float ran3(int *idum)
 {
 static int inext,inextp;
 static long ma[56];
 static int iff=0;
 long mj,mk;
 int i,ii,k;

 if (*idum < 0 || iff == 0) {
 iff=1;
 mj=MSEED-(*idum < 0 ? -*idum : *idum);
 mj %= MBIG;
 ma[55]=mj;
 mk=1;
 for (i=1;i<=54;i++) {
 ii=(21*i) % 55;
 ma[ii]=mk;
 mk=mj-mk;
 if (mk < MZ) mk += MBIG;
 mj=ma[ii];
 }
 for (k=1;k<=4;k++)
 for (i=1;i<=55;i++) {
 ma[i] -= ma[1+(i+30) % 55];
 if (ma[i] < MZ) ma[i] += MBIG;
 }
 inext=0;
 inextp=31;
 *idum=1;
 }
 if (++inext == 56) inext=1;
 if (++inextp == 56) inextp=1;
  mj=ma[inext]-ma[inextp];
  if (mj < MZ) mj += MBIG;
  ma[inext]=mj;
  return mj*FAC;
  }

  #undef MBIG
  #undef MSEED
  #undef MZ
  #undef FAC

 double RandomNum(void)
  {
  static bool first_time=1;
  static int idum; /* seed for ran3. */

  if(first_time) {
  #if STANDARDTEST /* Use fixed seed to test the program. */
  idum = - 1;
  #else
  idum = -(int)time(NULL)%(1<<15);
  /* use 16-bit integer as the seed. */
  #endif
  ran3(&idum);
  first_time = 0;
  idum = 1;
  }

  return( (double)ran3(&idum) );
  }

template < typename T >
T fast_random() {
    return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
}

int random_int(int min, int max) {
    return static_cast<int>(random<double>(min, max + 1));
}
