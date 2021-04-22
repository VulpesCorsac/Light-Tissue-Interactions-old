#pragma once

#include <math.h>
#include <stdexcept>
#include <string>
#include <iostream>

namespace Geo {

template < typename T >
class Vector3D  {
public:
    using value_type = T;

public:
    Vector3D() noexcept;

    template < typename U = T, typename V = T, typename W = T >
    Vector3D(const U& new_x, const V& new_y, const W& new_z) noexcept;

    template < typename U = T >
    bool operator == (const Vector3D<U>& another) const noexcept;
    template < typename U = T >
    bool operator != (const Vector3D<U>& another) const noexcept;

    template < typename U = T >
    bool operator <  (const Vector3D<U>& another) const noexcept;
    template < typename U = T >
    bool operator <= (const Vector3D<U>& another) const noexcept;
    template < typename U = T >
    bool operator >  (const Vector3D<U>& another) const noexcept;
    template < typename U = T >
    bool operator >= (const Vector3D<U>& another) const noexcept;

    operator bool() const noexcept;

    T& operator [] (int idx);
    const T& operator [] (int idx) const;

    Vector3D<T> operator + () const noexcept;
    Vector3D<T> operator - () const noexcept;

    template < typename U = T >
    Vector3D<T>& operator += (const Vector3D<U>& another) noexcept;
    template < typename U = T >
    Vector3D<T>& operator -= (const Vector3D<U>& another) noexcept;
    template < typename U = T >
    Vector3D<T>& operator *= (const U& scale) noexcept;
    template < typename U = T >
    Vector3D<T>& operator /= (const U& scale);

    T norm2() const noexcept;
    T norm() const noexcept;

public:
    T x;
    T y;
    T z;
};

template < typename T, typename U = T >
Vector3D<T> operator + (const Vector3D<T>& lhs, const Vector3D<U>& rhs) noexcept;

template < typename T, typename U = T >
Vector3D<T> operator - (const Vector3D<T>& lhs, const Vector3D<U>& rhs) noexcept;

template < typename T, typename U = T >
T operator * (const Vector3D<T>& lhs, const Vector3D<U>& rhs) noexcept;

template < typename T, typename U = T >
Vector3D<T> operator ^ (const Vector3D<T>& vec1, const Vector3D<U>& vec2) noexcept;

template < typename T, typename U = T >
Vector3D<T> operator * (const U& scale, const Vector3D<T>& v) noexcept;

template < typename T, typename U = T >
Vector3D<T> operator * (const Vector3D<T>& v, const U& scale) noexcept;

template < typename T, typename U = T >
Vector3D<T> operator / (const Vector3D<T>& v, const U& scale);

template < typename T, typename U = T >
Vector3D<T> cross_product(const Vector3D<T>& vec1, const Vector3D<U>& vec2) noexcept;

template < typename T >
Vector3D<T> unit(const Vector3D<T>& v);

}

template < typename T >
std::ostream& operator << (std::ostream& os, const Geo::Vector3D<T>& v);

/******************
 * IMPLEMENTATION *
 ******************/

template < typename T >
Geo::Vector3D<T>::Vector3D() noexcept
    : x(0)
    , y(0)
    , z(0) {
}

template < typename T >
template < typename U, typename V, typename W >
Geo::Vector3D<T>::Vector3D(const U& new_x, const V& new_y, const W& new_z) noexcept
    : x(new_x)
    , y(new_y)
    , z(new_z) {
}

template < typename T >
template < typename U >
bool Geo::Vector3D<T>::operator == (const Geo::Vector3D<U>& another) const noexcept {
    return x == another.x && y == another.y && z == another.z;
}

template < typename T >
template < typename U >
bool Geo::Vector3D<T>::operator != (const Geo::Vector3D<U>& another) const noexcept {
    return !(*this == another);
}

template < typename T >
template < typename U >
bool Geo::Vector3D<T>::operator < (const Geo::Vector3D<U>& another) const noexcept {
    return (x < another.x) || (x == another.x && y < another.y) || (x == another.x && y == another.y && z < another.z);
}

template < typename T >
template < typename U >
bool Geo::Vector3D<T>::operator <= (const Geo::Vector3D<U>& another) const noexcept {
    return !(*this > another);
}

template < typename T >
template < typename U >
bool Geo::Vector3D<T>::operator > (const Geo::Vector3D<U>& another) const noexcept {
    return another < *this;
}

template < typename T >
template < typename U >
bool Geo::Vector3D<T>::operator >= (const Geo::Vector3D<U>& another) const noexcept {
    return !(*this < another);
}

template < typename T >
Geo::Vector3D<T>::operator bool() const noexcept {
    return x != 0 || y != 0 || z != 0;
}

template < typename T >
T& Geo::Vector3D<T>::operator [] (int idx) {
    if (idx == 0)
        return x;
    if (idx == 1)
        return y;
    if (idx == 2)
        return z;
    throw std::out_of_range("Vector3D[" + std::to_string(idx) + "] - idx out of range [0; 3)");
}

template < typename T >
const T& Geo::Vector3D<T>::operator [] (int idx) const {
    if (idx == 0)
        return x;
    if (idx == 1)
        return y;
    if (idx == 2)
        return z;
    throw std::out_of_range("Vector3D[" + std::to_string(idx) + "] - idx out of range [0; 3)");
}

template < typename T >
Geo::Vector3D<T> Geo::Vector3D<T>::operator + () const noexcept {
    return *this;
}

template < typename T >
Geo::Vector3D<T> Geo::Vector3D<T>::operator - () const noexcept {
    return Geo::Vector3D<T>(-x, -y, -z);
}

template < typename T >
template < typename U >
Geo::Vector3D<T>& Geo::Vector3D<T>::operator += (const Geo::Vector3D<U>& another) noexcept {
    x += another.x;
    y += another.y;
    z += another.z;
    return *this;
}

template < typename T >
template < typename U >
Geo::Vector3D<T>& Geo::Vector3D<T>::operator -= (const Geo::Vector3D<U>& another) noexcept {
    x -= another.x;
    y -= another.y;
    z -= another.z;
    return *this;
}

template < typename T >
template < typename U >
Geo::Vector3D<T>& Geo::Vector3D<T>::operator *= (const U& scale) noexcept {
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
}

template < typename T >
template < typename U >
Geo::Vector3D<T>& Geo::Vector3D<T>::operator /= (const U& scale) {
    if (!scale)
        throw std::invalid_argument("Cannot divide Vector3D by zero");

    x /= scale;
    y /= scale;
    z /= scale;
    return *this;
}

template < typename T >
T Geo::Vector3D<T>::norm2() const noexcept {
    return (*this) * (*this);
}

template < typename T >
T Geo::Vector3D<T>::norm() const noexcept {
    return static_cast<T>(sqrt(norm2()));
}

template < typename T, typename U >
Geo::Vector3D<T> Geo::operator + (const Geo::Vector3D<T>& lhs, const Geo::Vector3D<U>& rhs) noexcept {
    return Geo::Vector3D<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

template < typename T, typename U >
Geo::Vector3D<T> Geo::operator - (const Geo::Vector3D<T>& lhs, const Geo::Vector3D<U>& rhs) noexcept {
    return Geo::Vector3D<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

template < typename T, typename U >
T Geo::operator * (const Geo::Vector3D<T>& lhs, const Geo::Vector3D<U>& rhs) noexcept {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template < typename T, typename U >
Geo::Vector3D<T> Geo::operator ^ (const Geo::Vector3D<T>& lhs, const Geo::Vector3D<U>& rhs) noexcept {
    return cross_product(lhs, rhs);
}

template < typename T, typename U >
Geo::Vector3D<T> Geo::operator * (const U& scale, const Geo::Vector3D<T>& v) noexcept {
    return v * scale;
}

template < typename T, typename U >
Geo::Vector3D<T> Geo::operator * (const Geo::Vector3D<T>& v, const U& scale) noexcept {
    auto result = v;
    result *= scale;
    return result;
}

template < typename T, typename U >
Geo::Vector3D<T> Geo::operator / (const Geo::Vector3D<T>& v, const U& scale) {
    auto result = v;
    result /= scale;
    return result;
}

template < typename T, typename U >
Geo::Vector3D<T> Geo::cross_product(const Geo::Vector3D<T>& vec1, const Geo::Vector3D<U>& vec2) noexcept {
    return Geo::Vector3D<T>(vec1.y * vec2.z - vec1.z * vec2.y,
                            vec1.z * vec2.x - vec1.x * vec2.z,
                            vec1.x * vec2.y - vec1.y * vec2.x);
}

template < typename T >
Geo::Vector3D<T> Geo::unit(const Geo::Vector3D<T>& v) {
    return v / v.norm();
}

template < typename T >
std::ostream& operator << (std::ostream& os, const Geo::Vector3D<T>& v) {
    os << '(';
    os << v.x;
    os << ',';
    os << v.y;
    os << ',';
    os << v.z;
    os << ')';

    return os;
}
