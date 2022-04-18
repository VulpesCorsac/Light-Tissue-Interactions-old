#pragma once

#include "../Utils/Contracts.h"

template < typename T >
class Medium {
public:
     Medium() noexcept = default;
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

    inline T getN()   const noexcept { return n;  }
    inline T getMua() const noexcept { return ua; }
    inline T getMus() const noexcept { return us; }
    inline T getMut() const noexcept { return ut; }
    inline T getD()   const noexcept { return D;  }
    inline T getA()   const noexcept { return a;  }
    inline T getTau() const noexcept { return tau;  }
    inline T getG()   const noexcept { return g;  }


protected:
    T n  ; // refraction coefficient
    T ua ; // absorption coefficient
    T us ; // absorption coefficient
    T D  ; // thickness/depth
    T g  ; // scattering anisotropy
    T ut ; // total attenuation coefficient
    T a  ; // albedo
    T tau; // optical thickness

private:
    Medium(const T& newN, const T& newMua, const T& newMus, const T& newA, const T& newTau, const T& newD, const T& newG) noexcept
        : n(newN)
        , ua(newMua)
        , us(newMus)
        , D(newD)
        , g(newG)
        , ut(newMua + newMus)
        , a(newA)
        , tau(newTau) {
    }
};
