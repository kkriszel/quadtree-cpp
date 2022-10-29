#ifndef BOUND_H
#define BOUND_H

#include "util.hpp"     // qt::Vec2D_i32
#include <array>        // std::array

namespace qt {
    /**
     * @brief Stores two coordinates by which the boundaries of a 2D rectangle can be described.
     * @note It uses integral signed fixed size (32 bit) type for storing the coordinates,
     *      as it uses the Vec2D_i32 struct from the utilities library (for which the + and - operations are also defined). 
     */
    class Bound {
        public:
            /**
             * @brief Public data memberts, representing the top left and bottom right coordinates
             *      of the boundary.
             */
            Vec2D_i32 topLeft;
            Vec2D_i32 bottomRight;

            /**
             * @brief Default constructor, initializes the boundary with {0, 0} coordinates on both points.
             */
            Bound();                                            

            /**
             * @brief Parameter constructor, constructs the bound with the two given points.
             */
            Bound(const Vec2D_i32 &topLeft, const Vec2D_i32 &bottomRight);

            /**
             * @brief Decides wether the boundary overlaps PARTIALLY of FULLY with another boundary.
             * @param other The boundary that it needs to be checked against.
             * @return true if the bound overlaps the other, false otherwise.
             * @note If the two bounds overlap only on a side, it is considered an overlap.
             */
            virtual bool overlaps(const Bound &other) const;

            /**
             * @brief Decides wether the boundary FULLY contains another boundary.
             * @param other The boundary that it needs to be checked against.
             * @return true if the bound fully covers the other, false otherwise.
             */
            virtual bool contains(const Bound &other) const;

            /**
             * @brief Splits the boundary in four equal boundaries (quadrons).
             * @return A std::array of qt::Bounds with 4 elements, the 4 quadrons of the bound,
             *      in the following order: NW, NE, SW, SE.
             */
            virtual std::array<Bound, 4> getQuadDivision() const;

            /**
             * @brief Decides wether the bound is big enough to be divided in four.
             * @return true if the bound can be divided in four, false otherwise.
             */
            virtual bool quadDivisible() const;                             
    };
}

#endif