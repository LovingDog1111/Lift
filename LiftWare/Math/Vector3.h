#pragma once
#include <algorithm>
#include <cmath>

template<typename T>
struct Vector3 {
    T x, y, z;

    Vector3() : x(T(0)), y(T(0)), z(T(0)) {}
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    Vector3<T> operator+(const Vector3<T>& other) const { return Vector3<T>(x + other.x, y + other.y, z + other.z); }
    Vector3<T> operator-(const Vector3<T>& other) const { return Vector3<T>(x - other.x, y - other.y, z - other.z); }
    Vector3<T> operator*(T scalar) const { return Vector3<T>(x * scalar, y * scalar, z * scalar); }
    Vector3<T> operator/(T scalar) const { return Vector3<T>(x / scalar, y / scalar, z / scalar); }

    Vector3<T>& operator+=(const Vector3<T>& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Vector3<T>& operator-=(const Vector3<T>& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    Vector3<T>& operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    Vector3<T>& operator/=(T scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
};

template<typename T>
Vector3<T> Add(const Vector3<T>& a, const Vector3<T>& b) { return a + b; }
template<typename T>
Vector3<T> Sub(const Vector3<T>& a, const Vector3<T>& b) { return a - b; }
template<typename T>
Vector3<T> Mul(const Vector3<T>& a, T scalar) { return a * scalar; }
template<typename T>
Vector3<T> Div(const Vector3<T>& a, T scalar) { return a / scalar; }

typedef Vector3<int> BlockPos;