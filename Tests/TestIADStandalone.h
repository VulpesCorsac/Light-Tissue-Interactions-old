#include "../AD/NelderMead.h"
#include "../AD/IAD.h"

using namespace std;

class TestsIAD {
public:
    static void Test1();
    static void Test2();
    static void Test3();
    static void Test4();
    static void Test5();
    static void Test6();
    static void RunAllTests();
};

constexpr float TOTAL_TOLERANCE = 1e-4;

void TestsIAD::Test1() {
    using T = float;
    const int M = 4;

    const int N = 2; // minimize 2 parameters
    const bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    T n_slab = 1.4; // refraction index of sample
    T n_slide_top = 1.5; // refraction index of slide
    T n_slide_bottom = 1.5;
    T rsmeas = 0.08624;
    T tsmeas = 0.76446;
    T tcmeas = 0.338341;
    T aOut, tauOut, gOut;

    cout << "Test 1 start" << endl;

    IAD<T,M,N,fix>(rsmeas, tsmeas, tcmeas, n_slab, n_slide_top, n_slide_bottom, aOut, tauOut, gOut);

    assert(abs(aOut - 0.9) < TOTAL_TOLERANCE);
    assert(abs(tauOut - 1.0) < TOTAL_TOLERANCE);
    assert(abs(gOut - 0.9) < TOTAL_TOLERANCE);
    cout << "Test 1 passed" << endl;
}

void TestsIAD::Test2() {
    using T = float;
    const int M = 32;

    const int N = 2; // minimize 2 parameters
    const bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    T n_slab = 1.4; // refraction index of sample
    T n_slide_top = 1.5; // refraction index of slide
    T n_slide_bottom = 1.5;
    T rsmeas = 0.08531;
    T tsmeas = 0.77350;
    T tcmeas = 0.338341;
    T aOut, tauOut, gOut;

    cout << "Test 2 start" << endl;

    IAD<T,M,N,fix>(rsmeas, tsmeas, tcmeas, n_slab, n_slide_top, n_slide_bottom, aOut, tauOut, gOut);

    assert(abs(aOut - 0.9) < TOTAL_TOLERANCE);
    assert(abs(tauOut - 1.0) < TOTAL_TOLERANCE);
    assert(abs(gOut - 0.9) < TOTAL_TOLERANCE);
    cout << "Test 2 passed" << endl;
}

void TestsIAD::Test3() {
    using T = float;
    const int M = 16;

    const int N = 2; // minimize 2 parameters
    const bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    T n_slab = 1.5; // refraction index of sample
    T n_slide_top = 1.5; // refraction index of slide
    T n_slide_bottom = 1.5;
    T rsmeas = 0.06548;
    T tsmeas = 0.74409;
    T tcmeas = 0.124729;
    T aOut, tauOut, gOut;

    cout << "Test 3 start" << endl;

    IAD<T,M,N,fix>(rsmeas, tsmeas, tcmeas, n_slab, n_slide_top, n_slide_bottom, aOut, tauOut, gOut);

    assert(abs(aOut - 0.9) < TOTAL_TOLERANCE);
    assert(abs(tauOut - 2.0) < TOTAL_TOLERANCE);
    assert(abs(gOut - 0.99) < TOTAL_TOLERANCE);
    cout << "Test 3 passed" << endl;
}

void TestsIAD::Test4() {
    using T = float;
    const int M = 4;

    const int N = 2; // minimize 2 parameters
    const bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    T n_slab = 1.4; // refraction index of sample
    T n_slide_top = 1.4; // refraction index of slide
    T n_slide_bottom = 1.4;
    T rsmeas = 0.38911;
    T tsmeas = 0.11869;
    T tcmeas = 0.006369;
    T aOut, tauOut, gOut;

    cout << "Test 4 start" << endl;

    IAD<T,M,N,fix>(rsmeas, tsmeas, tcmeas, n_slab, n_slide_top, n_slide_bottom, aOut, tauOut, gOut);

    assert(abs(aOut - 0.95) < TOTAL_TOLERANCE);
    assert(abs(tauOut - 5.0) < TOTAL_TOLERANCE);
    assert(abs(gOut - 0.0) < TOTAL_TOLERANCE);
    cout << "Test 4 passed" << endl;
}

void TestsIAD::Test5() {
    using T = float;
    const int M = 8;

    const int N = 2; // minimize 2 parameters
    const bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    T n_slab = 1.5; // refraction index of sample
    T n_slide_top = 1.6; // refraction index of slide
    T n_slide_bottom = 1.6;
    T rsmeas = 0.07204;
    T tsmeas = 0.54314;
    T tcmeas = 0.543166;
    T aOut, tauOut, gOut;

    cout << "Test 5 start" << endl;

    IAD<T,M,N,fix>(rsmeas, tsmeas, tcmeas, n_slab, n_slide_top, n_slide_bottom, aOut, tauOut, gOut);

    assert(abs(aOut - 0.0) < TOTAL_TOLERANCE);
    assert(abs(tauOut - 0.5) < TOTAL_TOLERANCE);
    assert(abs(gOut - 0.0) < 10*TOTAL_TOLERANCE);
    cout << "Test 5 passed" << endl;
}

void TestsIAD::Test6() {
    using T = float;
    const int M = 32;

    const int N = 2; // minimize 2 parameters
    const bool fix = 1; // 0 -- fix g, 1 -- fix tau (N = 2)

    T n_slab = 1.3; // refraction index of sample
    T n_slide_top = 1.4; // refraction index of slide
    T n_slide_bottom = 1.4;
    T rsmeas = 0.03278;
    T tsmeas = 0.34684;
    T tcmeas = 0.346838;
    T aOut, tauOut, gOut;

    cout << "Test 6 start" << endl;

    IAD<T,M,N,fix>(rsmeas, tsmeas, tcmeas, n_slab, n_slide_top, n_slide_bottom, aOut, tauOut, gOut);

    assert(abs(aOut - 0.0) < TOTAL_TOLERANCE);
    assert(abs(tauOut - 1.0) < TOTAL_TOLERANCE);
    assert(abs(gOut - 0.0) < 5*TOTAL_TOLERANCE);
    cout << "Test 6 passed" << endl;
}

void TestsIAD::RunAllTests() {
    Test1();
    Test2();
    Test3();
    Test4();   
    Test5();
    Test6();
}
