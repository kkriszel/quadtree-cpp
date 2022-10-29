#include "bound.hpp"    // class declarations

namespace qt {
     /*------------------------------------------------
            Bound template class implementation
    --------------------------------------------------*/

    // Default constructor, initializes the boundary with {0, 0} coordinates on both points.
    Bound::Bound() : Bound(Vec2D_i32(0, 0), Vec2D_i32(0, 0)) {}

    // Parameter constructor, constructs the bound with the two given points.
    Bound::Bound(const Vec2D_i32 &topLeft, const Vec2D_i32 &bottomRight) : topLeft(topLeft), bottomRight(bottomRight) {}

    // Decides wether the boundary overlaps PARTIALLY of FULLY with another boundary.
    bool Bound::overlaps(const Bound &other) const {
        return this->topLeft.x <= other.bottomRight.x
            && this->bottomRight.x >= other.topLeft.x
            && this->topLeft.y <= other.bottomRight.y
            && this->bottomRight.y >= other.topLeft.y;
    }

    // Decides wether the boundary FULLY contains another boundary.
    bool Bound::contains(const Bound &other) const {
        return this->topLeft.x <= other.topLeft.x
            && this->topLeft.y <= other.topLeft.y
            && this->bottomRight.x >= other.bottomRight.x
            && this->bottomRight.y >= other.bottomRight.y;
    }

    // Splits the boundary in four equal boundaries (quadrons).
    std::array<Bound, 4> Bound::getQuadDivision() const {
        Vec2D_i32 halfSize = Vec2D_i32((bottomRight.x - topLeft.x) / 2, (bottomRight.y - topLeft.y) / 2);

        std::array<Bound, 4> division;

        // Order: NW, NE, SW, SE
        division[0] = Bound(topLeft, topLeft + halfSize);
        division[1] = Bound(Vec2D_i32(topLeft.x + halfSize.x, topLeft.y), Vec2D_i32(bottomRight.x, topLeft.y + halfSize.y));
        division[2] = Bound(Vec2D_i32(topLeft.x, topLeft.y + halfSize.y), Vec2D_i32(topLeft.x + halfSize.x, bottomRight.y));
        division[3] = Bound(topLeft + halfSize, bottomRight);

        return division;
    }

    // Decides wether the bound is big enough to be divided in four.
    bool Bound::quadDivisible() const {
        // If the two endpoints lie on the same line, the bound can't be divided in four.
        return (topLeft.x < bottomRight.x) && (topLeft.y < bottomRight.y);
    }
}