#pragma once

#include <cmath>
#include <ostream>
#include <type_traits>


using vFloat = float;


template <typename ArithmeticType>
struct IndependentVector2
{
    ArithmeticType x;
    ArithmeticType y;
};


template <class Vector>
struct InheritingVector2 : Vector
{
    using Vector::x;
    using Vector::y;
};


template <class T>
struct vec2 : public std::conditional<std::is_arithmetic<T>::value, IndependentVector2<T>, InheritingVector2<T>>::type
{
    vec2() {this->x = 0; this->y = 0;};
    vec2(const vec2& vec) {this->x = std::move(vec.x); this->y = std::move(vec.y);};
    template<class Value>
    vec2(Value x, Value y) {this->x = std::move(x); this->y = std::move(y);};

    template<class Vector>
    explicit vec2(const Vector& vec) {this->x = std::move(vec.x); this->y = std::move(vec.y);};
    vec2(const vec2&& vec)  noexcept {this->x = std::move(vec.x); this->y = std::move(vec.y);};

    template<class Vector>
    explicit operator Vector() const {return Vector {static_cast<typeof(Vector::x)>(this->x),
                                                     static_cast<typeof(Vector::y)>(this->y)};};

    template<class Vector>
    vec2& operator=(const Vector& vec) {this->x = std::move(vec.x); this->y = std::move(vec.y); return *this;};
    vec2& operator=(const vec2& vec) {this->x = std::move(vec.x); this->y = std::move(vec.y); return *this;};

    template <class Vector>
    vec2 operator+(const Vector& vec) const {return vec2{this->x + vec.x, this->y + vec.y};};
    template <class Vector>
    vec2 operator-(const Vector& other) const {return vec2{this->x - other.x, this->y - other.y};};
    template<typename Number>
    vec2 operator/(const Number divisor) const {return vec2(this->x / divisor, this->y / divisor);};
    template<typename Number>
    vec2 operator*(const Number multiplier) const {return vec2(this->x * multiplier, this->y * multiplier);};


    inline vFloat norm() const {
        return sqrtf(this->x * this->x + this->y * this->y);
    }

    inline vec2 unitVector() const {
        return *this / norm();
    }

    inline vFloat rectangleArea() const {
        return fabs(this->x * this->y);
    }

    //Angle in radians
    inline vFloat angle() const {
        return atan2f(this->y, this->x);
    }

    void scale(const vFloat factor) const {
        this->x *= factor;
        this->y *= factor;
    }

    void rotateRadians(const vFloat angle) {
        const auto oldX = this->x;
        this->x = cosf(angle) * this->x - sinf(angle) * this->y;
        this->y = sinf(angle) * oldX + cosf(angle) * this->y;
    }

    void multiplyComponents(const vec2& other) const {
        this->x *= other.x;
        this->y *= other.y;
    }

};
using vec2f = vec2<vFloat>;
using vec2i = vec2<int>;
using vec2u = vec2<unsigned int>;

template <class T>
std::ostream &operator<<(std::ostream &os, const vec2<T>& vector) {
    return os << "(" << std::to_string(vector.x) << ", " << std::to_string(vector.y) << ")";
}

namespace Vector{

    template <class Vector>
    static inline vFloat norm(const Vector& vector) {
        return sqrtf(vector.x * vector.x + vector.y * vector.y);
    }

    template <class Vector>
    static inline Vector unitVector(const Vector& vector) {
        return vector / norm(vector);
    }

    template <class VectorA, class VectorB>
    static inline vFloat dotProduct(const VectorA& vector1, const VectorB& vector2) {
        return vector1.x * vector2.x + vector1.y * vector2.y;
    }

    // Component-wise multiplication
    template <class VectorA, class VectorB>
    static inline VectorA multiplyComponents(const VectorA& vector1, const VectorB& vector2) {
        return {vector1.x * vector2.x, vector1.y * vector2.x};
    }

    // Area of the rectangle formed by the vector
    template <class Vector>
    static inline vFloat rectangleArea(const Vector& vector) {
        return fabs(vector.x * vector.y);
    }

    template <class VectorA, class VectorB>
    static inline vFloat rhombusArea(const VectorA& vector1, const VectorB& vector2) {
        return fabs(vector1.x * vector2.y - vector1.y * vector2.x);
    }

    // Area of the triangle between the two vectors
    template <class VectorA, class VectorB>
    static inline vFloat triangleArea(const VectorA& vector1, const VectorB& vector2) {
        return rhombusArea(vector1, vector2) / 2;
    }

    // Signed area used in centroid calculations that use positive and negative areas
    template <class VectorA, class VectorB>
    static inline vFloat signedArea(const VectorA& vector1, const VectorB& vector2) {
        return vector1.x * vector2.y - vector1.y * vector2.x;
    }

    template <class Vector>
    static inline vFloat vectorAngleRadians(const Vector& vector) {
        return atan2f(vector.y, vector.x);
    }

    template <class Vector>
    static inline Vector rotateVectorRadians(const Vector& vector, vFloat angle) {
        return {cosf(angle) * vector.x - sinf(angle) * vector.y, sinf(angle) * vector.x + cosf(angle) * vector.y};
    }

    //Angle is unsigned in radians
    template <class VectorA, class VectorB>
    static inline vFloat angleBetweenVectors(const VectorA& vector1, const VectorB& vector2) {
        return acosf(dotProduct(vector1, vector2) / (norm(vector1) * norm(vector2)));
    }

    //linear interpolation
    template <class VectorA, class VectorB>
    static inline VectorA lerp(const VectorA& startPoint, const VectorB& endPoint, const vFloat alpha) {
        return {startPoint.x * (1 - alpha) + endPoint.x * alpha, startPoint.y * (1 - alpha) + endPoint.y * alpha};
    }

    //faster than reflect() but it expects the normal to be a unit vector
    template <class VectorA, class VectorB>
    static inline VectorA reflectUnit(const VectorA& vector, const VectorB& normal) {
        return vector - normal * 2 * dotProduct(vector, normal);
    }

    //reflects the vector off the normal vector
    template <class VectorA, class VectorB>
    static inline VectorA reflect(const VectorA& vector, const VectorB& normal) {
        const auto unitNormal = unitVector(normal);
        return reflectUnit(vector, unitNormal);
    }
}
