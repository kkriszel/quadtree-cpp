#ifndef UTIL_H
#define UTIL_H

#include <cstdint>          // fixed size types

namespace qt {
    /**
     * @brief Typedefs of fixed size integral types.
     */
    typedef std::int8_t int8_t;
    typedef std::uint8_t uint8_t;
    typedef std::int32_t int32_t;

    /** 
     * @brief Generic type, representing a 2D vector/point.
     * @tparam T The type of the components of the coordinate.
     */
    template <typename T>
    class Vec2D {
        public:
            /** 
             * @brief Data members.
             */
            T x, y;

            /**
             * @brief Constructs a 2D vector/point with given coordinates.
             */
            Vec2D(T x = 0, T y = 0);

            /**
             * @brief Calculates the vectorial sum of two vectors.
             * @param other The RHS operand.
             * @return A new Vec2D<T>, the sum of the two vectors.
             */
            Vec2D<T> operator+(const Vec2D<T> &other) const;

            /**
             * @brief Calculates the vectorial difference of two vectors.
             * @param other The RHS operand.
             * @return A new Vec2D<T>, the difference of the two vectors.
             */
            Vec2D<T> operator-(const Vec2D<T> &other) const; 
        
            /**
             * @todo Implement and overload all operations.
             */
    };

    /** 
     * @brief 32 bit signed integral type, has to be declared in the implementation
     */
    typedef Vec2D<int32_t> Vec2D_i32;
}

#endif