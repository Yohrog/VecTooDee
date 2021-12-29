/* Copyright (c) 2021 Yohrog

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <cmath>
#include <ostream>
#include <type_traits>

#ifdef USE_DOUBLE_PRECISION
using vFloat = double;
#define vSin sin
#define vCos cos
#define vAcos acos
#define vSqrt sqrt
#define vAtan2 atan2
#define vAbs abs
#else
using vFloat = float;
#define vSin sinf
#define vCos cosf
#define vAcos acosf
#define vSqrt sqrtf
#define vAtan2 atan2f
#define vAbs fabs
#endif

template <typename ArithmeticType>
struct IndependentVector2
{
    ArithmeticType x;
    ArithmeticType y;
};

#ifdef WITHOUT_VECTOR_INHERITANCE
template <class Vector>
struct InheritingVector2
{
    using Vector::x;
    using Vector::y;
};

#else
template <class Vector>
struct InheritingVector2 : Vector
{
    using Vector::x;
    using Vector::y;
};

#endif

template <class T=vFloat>
struct vec2 : public std::conditional<std::is_arithmetic<T>::value, IndependentVector2<T>, InheritingVector2<T>>::type {

    vec2() {this->x = 0; this->y = 0;};
    vec2(const vec2& vec) {this->x = std::move(vec.x); this->y = std::move(vec.y);};
    template<class Value>
    vec2(Value x, Value y) {this->x = std::move(x); this->y = std::move(y);};

    template<class Vector>
    explicit vec2(const Vector& vec) {this->x = std::move(vec.x); this->y = std::move(vec.y);};
    vec2(const vec2&& vec) noexcept {this->x = std::move(vec.x); this->y = std::move(vec.y);};

#ifdef ALLOW_IMPLICIT_CONVERSION
    template<class Vector>
    operator Vector() const {
        return Vector {static_cast<typeof(Vector::x)>(this->x), static_cast<typeof(Vector::y)>(this->y)};
    };
#else
    template<class Vector>
    explicit operator Vector() const {return Vector {static_cast<typeof(Vector::x)>(this->x),
                                                     static_cast<typeof(Vector::y)>(this->y)};};
#endif

#ifndef WITHOUT_OPERATORS
    template<class Vector>
    vec2& operator=(const Vector& vec) {this->x = std::move(vec.x); this->y = std::move(vec.y); return *this;};
    vec2& operator=(const vec2& vec) {this->x = std::move(vec.x); this->y = std::move(vec.y); return *this;};

    template <class Vector>
    vec2 operator+(const Vector& vec) const {return vec2{this->x + vec.x, this->y + vec.y};};
    template <class Vector>
    vec2 operator-(const Vector& other) const {return vec2{this->x - other.x, this->y - other.y};};
    template<typename Number>
    vec2 operator*(const Number multiplier) const {return vec2(this->x * multiplier, this->y * multiplier);};
    template<typename Number>
    vec2 operator/(const Number divisor) const {return vec2(this->x / divisor, this->y / divisor);};

    template <class Vector>
    vec2 operator+=(const Vector& vec) {this->x += vec.x; this->y += vec.y; return *this;};
    template <class Vector>
    vec2 operator-=(const Vector& vec) {this->x -= vec.x; this->y -= vec.y; return *this;};
    template<typename Number>
    vec2 operator*=(const Number multiplier) {this->x *= multiplier; this->y *= multiplier; return *this;};
    template<typename Number>
    vec2 operator/=(const Number divisor) {this->x /= divisor; this->y /= divisor; return *this;};

    template <class Vector>
    bool operator==(const Vector& vector) const {return this->x == vector.x && this->y == vector.y;};
    template <class Vector>
    bool operator!=(const Vector& vector) const {return this->x != vector.x || this->y != vector.y;};
#endif

#ifndef WITHOUT_MEMBER_FUNCTIONS

    inline vFloat norm() const {
        return vSqrt(this->x * this->x + this->y * this->y);
    }

    inline vec2 unitVector() const {
        return *this / norm();
    }

    inline vFloat rectangleArea() const {
        return vAbs(this->x * this->y);
    }

    //Angle in radians
    inline vFloat angle() const {
        return vAtan2(this->y, this->x);
    }

    void scale(const vFloat factor) const {
        this->x *= factor;
        this->y *= factor;
    }

    void rotateRadians(const vFloat angle) {
        const auto oldX = this->x;
        this->x = vCos(angle) * this->x - vSin(angle) * this->y;
        this->y = vSin(angle) * oldX + vCos(angle) * this->y;
    }

    void multiplyComponents(const vec2& other) const {
        this->x *= other.x;
        this->y *= other.y;
    }
#endif
};
using vec2f = vec2<vFloat>;
using vec2i = vec2<int>;
using vec2u = vec2<unsigned int>;

template <class T>
std::ostream &operator<<(std::ostream &os, const vec2<T>& vector) {
    return os << "(" << std::to_string(vector.x) << ", " << std::to_string(vector.y) << ")";
}

#ifndef WITHOUT_VECTOR_FUNCTIONS
// Static Functions
namespace vectoo {

    // Convert any vector struct into any other
    template<class VectorA, class VectorB>
    static inline VectorA convert(const VectorB &vector) {
        return {vector.x, vector.y};
    }

    // Norm of any vector struct that has an x and y component
    template<class Vector>
    static inline vFloat norm(const Vector &vector) {
        return vSqrt(vector.x * vector.x + vector.y * vector.y);
    }

    template<class Vector>
    static inline Vector unitVector(const Vector &vector) {
        return vector / norm(vector);
    }

    template<class VectorA, class VectorB>
    static inline VectorA add(const VectorA &vector1, const VectorB &vector2) {
        return {vector1.x + vector2.x, vector1.y + vector2.y};
    }

    template<class VectorA, class VectorB>
    static inline VectorA subtract(const VectorA &vector1, const VectorB &vector2) {
        return {vector1.x - vector2.x, vector1.y - vector2.y};
    }

    template<class VectorA, class VectorB>
    static inline vFloat dotProduct(const VectorA &vector1, const VectorB &vector2) {
        return vector1.x * vector2.x + vector1.y * vector2.y;
    }

    // Component-wise multiplication
    template<class VectorA, class VectorB>
    static inline VectorA multiplyComponents(const VectorA &vector1, const VectorB &vector2) {
        return {vector1.x * vector2.x, vector1.y * vector2.x};
    }

    // Area of the rectangle formed by the vector
    template<class Vector>
    static inline vFloat rectangleArea(const Vector &vector) {
        return vAbs(vector.x * vector.y);
    }

    template<class VectorA, class VectorB>
    static inline vFloat rhombusArea(const VectorA &vector1, const VectorB &vector2) {
        return vAbs(vector1.x * vector2.y - vector1.y * vector2.x);
    }

    // Area of the triangle between the two vectors
    template<class VectorA, class VectorB>
    static inline vFloat triangleArea(const VectorA &vector1, const VectorB &vector2) {
        return rhombusArea(vector1, vector2) / 2;
    }

    // Signed area used in centroid calculations that use positive and negative areas
    template<class VectorA, class VectorB>
    static inline vFloat signedArea(const VectorA &vector1, const VectorB &vector2) {
        return vector1.x * vector2.y - vector1.y * vector2.x;
    }

    template<class Vector>
    static inline vFloat vectorAngleRadians(const Vector &vector) {
        return vAtan2(vector.y, vector.x);
    }

    template<class Vector>
    static inline Vector rotateVectorRadians(const Vector &vector, vFloat angle) {
        return {vCos(angle) * vector.x - vSin(angle) * vector.y, vSin(angle) * vector.x + vCos(angle) * vector.y};
    }

    //Angle is unsigned in radians
    template<class VectorA, class VectorB>
    static inline vFloat angleBetweenVectors(const VectorA &vector1, const VectorB &vector2) {
        return vAcos(dotProduct(vector1, vector2) / (norm(vector1) * norm(vector2)));
    }

    //linear interpolation
    template<class VectorA, class VectorB>
    static inline VectorA lerp(const VectorA &startPoint, const VectorB &endPoint, const vFloat alpha) {
        return {startPoint.x * (1 - alpha) + endPoint.x * alpha, startPoint.y * (1 - alpha) + endPoint.y * alpha};
    }

    //faster than reflect() but it expects the normal to be a unit vector
    template<class VectorA, class VectorB>
    static inline VectorA reflectUnit(const VectorA &vector, const VectorB &normal) {
        return vector - normal * 2 * dotProduct(vector, normal);
    }

    //reflects the vector off the normal vector
    template<class VectorA, class VectorB>
    static inline VectorA reflect(const VectorA &vector, const VectorB &normal) {
        const auto unitNormal = unitVector(normal);
        return reflectUnit(vector, unitNormal);
    }
}
#endif
