#pragma once
#include <algorithm>
#include <cmath>

template<typename T>
struct Vector4 {
    T x, y, z, w;

    Vector4() : x(T(0)), y(T(0)), z(T(0)), w(T(0)) {}
    Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    Vector4<T> operator+(const Vector4<T>& other) const { return Vector4<T>(x + other.x, y + other.y, z + other.z, w + other.w); }
    Vector4<T> operator-(const Vector4<T>& other) const { return Vector4<T>(x - other.x, y - other.y, z - other.z, w - other.w); }
    Vector4<T> operator*(T scalar) const { return Vector4<T>(x * scalar, y * scalar, z * scalar, w * scalar); }
    Vector4<T> operator/(T scalar) const { return Vector4<T>(x / scalar, y / scalar, z / scalar, w / scalar); }

    Vector4<T>& operator+=(const Vector4<T>& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
    Vector4<T>& operator-=(const Vector4<T>& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
    Vector4<T>& operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this; }
    Vector4<T>& operator/=(T scalar) { x /= scalar; y /= scalar; z /= scalar; w /= scalar; return *this; }

    template<typename V>
    bool contains(const V& p) const {
        return p.x >= x && p.x <= z && p.y >= y && p.y <= w;
    }
};

template<typename T>
Vector4<T> Add(const Vector4<T>& a, const Vector4<T>& b) { return a + b; }
template<typename T>
Vector4<T> Sub(const Vector4<T>& a, const Vector4<T>& b) { return a - b; }
template<typename T>
Vector4<T> Mul(const Vector4<T>& a, T scalar) { return a * scalar; }
template<typename T>
Vector4<T> Div(const Vector4<T>& a, T scalar) { return a / scalar; }
