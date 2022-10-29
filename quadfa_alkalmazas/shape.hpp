#ifndef SHAPE_H
#define SHAPE_H

#include "lib/util.hpp"         // qt::uint8_t, qt::Vec2D
#include "lib/bound.hpp"        // qt::Bound

/**
 * @brief Shape class, with qt::Bound as parent class. 
 * @note It has all the members and functions of qt::Bound, with an additional
 *      Color inner class and member, and also a static getRandomGrayShape member function.
 */
class Shape : public qt::Bound {
    public:
        /**
         * @brief Color type, storing the red, green and blue components of a color.
         * @note The componenst are stored on 1 unsigned byte each.
         */
        struct Color {
            qt::uint8_t r;
            qt::uint8_t g;
            qt::uint8_t b;

            /**
             * @brief Constructs a Color, with given values, or {0, 0, 0} by default.
             * @param r,g,b the red, green and blue components of the color.
             */
            Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) : r(r), g(g), b(b) {}
        };

        /**
         * @brief Public member of the Shape class.
         */
        Color color;

        /**
         * @brief Constructs a Shape with given top-left and bottom-right points, and color.
         * @param p1 Top left coordinate of the bound.
         * @param p2 Bottom right coordinate of the bound.
         * @param color The color of the shape.
         */
        Shape(const qt::Vec2D_i32 &p1 = 0, const qt::Vec2D_i32 &p2 = 0, const Color &color = 0);    

        /**
         * @brief Gives a random Shape within given bound, with given minimal and maximal
         *      width and height, and with a light grayscale color.
         * @param bound The bounds in between the randomized shape should lie.
         * @param minSize The minimum width and height of the randomized shape.
         * @param maxSize The maximum width and height of the randomized shape.
         * @return A randomized Shape object.
         */
        static Shape getRandomGrayShape(const qt::Bound &bound, const qt::Vec2D_i32 &minSize, const qt::Vec2D_i32 &maxSize);
};

#endif