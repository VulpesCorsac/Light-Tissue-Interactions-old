#pragma once

#include "../Utils/Contracts.h"

template < typename T >
class Medium {
public:
    // Medium() noexcept = delete;
    ~Medium() noexcept = default;

    static Medium fromCoeffs(const T& n, const T& ua, const T& us, const T& D, const T& g) {
        /// TODO: this assert fails tests on glass
        // CHECK_ARGUMENT_CONTRACT(ua + us != 0);

        return Medium(n, ua, us, us / (ua + us), D * (ua + us), D, g);
    }

    static Medium fromAlbedo(const T& n, const T& a, const T& tau, const T& D, const T& g) {
        CHECK_ARGUMENT_CONTRACT(D != 0);

        return Medium(n, tau * (1 - a) / D, tau * a / D, a, tau, D, g);
    }

    T n  ; // refraction coefficient
    T ua ; // absorption coefficient
    T us ; // absorption coefficient
    T D  ; // thickness/depth
    T g  ; // scattering anisotropy
    T ut ; // total attenuation coefficient
    T a  ; // albedo
    T tau; // optical thickness

private:
    Medium(const T& n, const T& ua, const T& us, const T& a, const T& tau, const T& D, const T& g) noexcept
        : n(n)
        , ua(ua)
        , us(us)
        , D(D)
        , g(g)
        , ut(ua + us)
        , a(a)
        , tau(tau) {

    }
};
