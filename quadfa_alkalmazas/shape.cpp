#include "shape.hpp"        // class declarations

#include <stdlib.h>         // rand()

/*------------------------------------------------
            Shape class implementation
--------------------------------------------------*/

// Constructs a Shape with given top-left and bottom-right points, and color.
Shape::Shape(const qt::Vec2D_i32 &p1, const qt::Vec2D_i32 &p2, const Color &color) : Bound(p1, p2), color(color) {}

// Gives a random Shape within given bound, with given minimal and maximal
// width and height, and with a light grayscale color.
Shape Shape::getRandomGrayShape(const qt::Bound &bound, const qt::Vec2D_i32 &minSize, const qt::Vec2D_i32 &maxSize) {
    // Random lambda function, which returns an integral value in the interval [low..high].
    static auto random = [](int32_t low, int32_t high) {return rand() % (high - low + 1) + low;};

    qt::Vec2D_i32 boundSize = bound.bottomRight - bound.topLeft + qt::Vec2D_i32(1, 1);
    qt::Vec2D_i32 size = qt::Vec2D_i32(std::min(random(minSize.x, maxSize.x), boundSize.x), std::min(random(minSize.y, maxSize.y), boundSize.y));
    qt::Vec2D_i32 topLeft = qt::Vec2D_i32(random(bound.topLeft.x, bound.bottomRight.x - size.x), random(bound.topLeft.y, bound.bottomRight.y - size.y));
    qt::Vec2D_i32 bottomRight = topLeft + size;

    // If the r, g, b components of the color are the same, it becomes greyscale
    uint32_t randGrey = random(127, 255);
    Color color = Color(randGrey, randGrey, randGrey);

    return Shape(topLeft, bottomRight, color);
}