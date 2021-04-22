#pragma once

template <typename T>
class Medium
{
    public:
        Medium() = default;
        Medium(const T& new_n, const T& new_ua, const T& new_us, const T& new_D, const T& new_g);
        ~Medium() = default;

        T getN () const {return n;}
        T getMa () const {return ua;}
        T getMs () const {return us;}
        T getD () const {return D;}
        T getG () const {return g;}
        T getMt () const {return ut;}
        T getL () const {return l;}
        T getA () const {return a;}
        T getTau () const {return tau;}


    protected:
        T n = 1.0; // refraction coeff
        T ua = 0.0; // absorption coeff
        T us = 0.0; // absorption coeff
        T D = 0.0; // thickness/depth
        T g = 0.0; // scattering anisotropy
        T ut = ua + us; // total attenuation coeff
        T l = 1.0/ut; // mean free path length
        T a = us / ut; // albedo
        T tau = D * ut; // optical thickness
};

template < typename T>
Medium<T>::Medium(const T& new_n, const T& new_ua, const T& new_us, const T& new_D, const T& new_g) {
    n = new_n;
    ua = new_ua;
    us = new_us;
    D = new_D;
    g = new_g;
    ut = ua + us; // total attenuation coeff
    l = 1.0/ut; // mean free path length
    a = us / ut; // albedo
    tau = D * ut; // optical thickness
    // these declarations shouldn't be that way....
}
