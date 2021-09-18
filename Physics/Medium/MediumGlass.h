#pragma once

#include "MediumInterface.h"

namespace Physics_NS {
    /// \brief class for glass Medium
    template < typename T >
    class MediumGlass : public MediumInterface<T> {
    public:
        /// \param[in] n glass refraction index
        explicit MediumGlass(const T& n = 1) noexcept;

        T refraction(const T& t = 0, const T& d = 0) const override;

    protected:
        T n; ///< Glass refraction index
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Physics_NS::MediumGlass<T>::MediumGlass(const T& n) noexcept
    : n(n) {
    this->type = MediumType::Glass;
}

template < typename T >
T Physics_NS::MediumGlass<T>::refraction(const T& t, const T& d) const {
    return n;
}
