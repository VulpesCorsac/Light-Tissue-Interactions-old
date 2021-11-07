#pragma once

#include "Medium/MediumInterface.h"

#include <memory>

namespace Physics_NS {
    template < typename T >
    class Layer {
    public:
        template < typename S >
        Layer(std::unique_ptr<MediumInterface<T>> medium, const S& width) noexcept;
        template < typename S, typename V >
        Layer(std::unique_ptr<MediumInterface<T>> medium, const S& begin, const V& end) noexcept;

    public:
        const std::unique_ptr<MediumInterface<T>> medium;

        T begin;
        T end;
        T width;
    };
}

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
template < typename S >
Physics_NS::Layer<T>::Layer(std::unique_ptr<MediumInterface<T>> medium, const S& width) noexcept
    : medium(std::move(medium))
    , begin(0)
    , end(width)
    , width(width) {
}

template < typename T >
template < typename S, typename V >
Physics_NS::Layer<T>::Layer(std::unique_ptr<MediumInterface<T>> medium, const S& begin, const V& end) noexcept
    : medium(std::move(medium))
    , begin(begin)
    , end(end)
    , width(end-begin) {
}
