#pragma once

#include "../AD/RT.h"
#include "../AD/Quadrature.h"
#include "../AD/NelderMead.h"
#include "../AD/IAD.h"

#include <gtest/gtest.h>

template <typename T, size_t M, size_t N, bool fix>
class testDataIAD {
public:
    testDataIAD(T fixedParam, T n_slab, T n_slide_top, T n_slide_bottom, T Rsmeas, T Tsmeas, T astart, T tstart, T gstart) {
        setValues(fixedParam, n_slab, n_slide_top, n_slide_bottom, Rsmeas, Tsmeas, astart, tstart, gstart);
        calc();
    }

    void setValues(const T& fixedParam, const T& n_slab, const T& n_slide_top, const T& n_slide_bottom, const T& Rsmeas, const T& Tsmeas, const T& astart, const T& tstart, const T& gstart) {
        myFixedParam = fixedParam;
        myN_slab = n_slab;
        myN_slide_top = n_slide_top;
        myN_slide_bottom = n_slide_bottom;
        myRsmeas = Rsmeas;
        myTsmeas = Tsmeas;
        myAstart = astart;
        myTstart = tstart;
        myGstart = gstart;
    }

    void calc() {
        func<T, M, N, fix> toMinimize(myFixedParam, myN_slab, myN_slide_top, myN_slide_bottom, myRsmeas, myTsmeas);
        NelderMeadMin<T, M, N, fix>(toMinimize, maxIter, myAstart, myTstart, myGstart, myVecMin, myFmin);
    }

    Matrix<T, 1, N> getVecMin() const noexcept {
        return myVecMin;
    }

    T getMin() const noexcept {
        return myFmin;
    }

protected:
    T myFixedParam, myN_slab, myN_slide_top, myN_slide_bottom, myRsmeas, myTsmeas, myFmin, myAstart, myTstart, myGstart;
    Matrix<T, 1, N> myVecMin;
    const int maxIter = 100;
};

TEST(IAD, testT1) {
    using T = double;
    T rsmeas = 0.0862;
    T tsmeas = 0.764;
    T tcmeas = 0.338;
    T n_slab = 1.4;
    T n_slide_top = 1.5;
    T n_slide_bottom = 1.5;
    T fixedParam = tauCalc<T, 32>(n_slab, n_slide_top, n_slide_bottom, tcmeas);
    testDataIAD<T, 32, 2, 1> test(fixedParam, n_slab, n_slide_top, n_slide_bottom, rsmeas, tsmeas, 0.9, 0.95, 0.9);
    EXPECT_NEAR((test.getVecMin())(0), 0.9, 1e-4);
    EXPECT_NEAR((test.getVecMin())(1), 0.9, 1e-4);
}
