#pragma once
#include <algorithm>
#include <cmath>

template<typename T>
struct Vector2 {
    T x, y;

    Vector2() : x(T(0)), y(T(0)) {}
    Vector2(T x, T y) : x(x), y(y) {}

    Vector2<T> operator+(const Vector2<T>& other) const { return Vector2<T>(x + other.x, y + other.y); }
    Vector2<T> operator-(const Vector2<T>& other) const { return Vector2<T>(x - other.x, y - other.y); }
    Vector2<T> operator*(T scalar) const { return Vector2<T>(x * scalar, y * scalar); }
    Vector2<T> operator/(T scalar) const { return Vector2<T>(x / scalar, y / scalar); }

    Vector2<T>& operator+=(const Vector2<T>& other) { x += other.x; y += other.y; return *this; }
    Vector2<T>& operator-=(const Vector2<T>& other) { x -= other.x; y -= other.y; return *this; }
    Vector2<T>& operator*=(T scalar) { x *= scalar; y *= scalar; return *this; }
    Vector2<T>& operator/=(T scalar) { x /= scalar; y /= scalar; return *this; }
};

template<typename T>
Vector2<T> Add(const Vector2<T>& a, const Vector2<T>& b) { return a + b; }
template<typename T>
Vector2<T> Sub(const Vector2<T>& a, const Vector2<T>& b) { return a - b; }
template<typename T>
Vector2<T> Mul(const Vector2<T>& a, T scalar) { return a * scalar; }
template<typename T>
Vector2<T> Div(const Vector2<T>& a, T scalar) { return a / scalar; }
