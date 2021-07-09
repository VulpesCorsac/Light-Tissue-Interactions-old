#pragma once

template < typename T >
class Medium {
public:
    // Medium() noexcept = delete;
    // Medium(const T& new_n, const T& new_ua, const T& new_us, const T& new_D, const T& new_g);
    // ~Medium() noexcept = default;

    static Medium fromCoeffs(const T& new_n, const T& new_ua, const T& new_us, const T& new_D, const T& new_g) {
        return Medium(new_n, new_ua, new_us, new_us / (new_ua + new_us), new_D * (new_ua + new_us), new_D, new_g);
    }
    static Medium fromAlbedo(const T& new_n, const T& new_a, const T& new_tau, const T& new_D, const T& new_g) {
        return Medium(new_n, new_tau * (1 - new_a) / new_D, new_tau * new_a / new_D, new_a, new_tau, new_D, new_g);
    }

    T n;   // refraction coeff
    T ua;  // absorption coeff
    T us;  // absorption coeff
    T D;   // thickness/depth
    T g;   // scattering anisotropy
    T ut;  // total attenuation coeff
    T a;   // albedo
    T tau; // optical thickness

private:
    Medium(const T& new_n, const T& new_ua, const T& new_us, const T& new_a, const T& new_tau, const T& new_D, const T& new_g)
    : n(new_n)
    , ua(new_ua)
    , us(new_us)
    , D(new_D)
    , g(new_g)
    , ut(ua + us)
    , a(new_a)
    , tau(new_tau) {}
};

/*
inline Medium<T> Medium<T>::fromCoeffs(const T& new_n, const T& new_ua, const T& new_us, const T& new_D, const T& new_g) {
    return Medium<T>(new_n, new_ua, new_us, new_us / (new_ua + new_us), new_D * (new_ua + new_us), new_D, new_g);
}

template < typename T >
inline Medium<T> fromAlbedo(const T& new_n, const T& new_a, const T& new_tau, const T& new_D, const T& new_g) {
    return Medium(new_n, new_tau * (1 - new_a) / new_D, new_tau * new_a / new_D, new_a, new_tau, new_D, new_g);
}
//*/
