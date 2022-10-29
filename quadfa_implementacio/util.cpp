#include "util.hpp"         // class declarations

namespace qt {
    /*------------------------------------------------
            Vec2D template class implementation
    --------------------------------------------------*/

    // Constructs a 2D vector/point with given coordinates.
    template <typename T>
    Vec2D<T>::Vec2D(T x, T y) : x(x), y(y) {}

    // Calculates the vectorial sum of two vectors.
    template <typename T>
    Vec2D<T> Vec2D<T>::operator+(const Vec2D<T> &other) const {
        return Vec2D(x + other.x, y + other.y);
    }

    // Returns the vectorial difference of two vectors.
    template <typename T>
    Vec2D<T> Vec2D<T>::operator-(const Vec2D<T> &other) const {
        return Vec2D(x - other.x, y - other.y);
    }

    // Vec2D_i32 declaration
    template struct Vec2D<int>;
}