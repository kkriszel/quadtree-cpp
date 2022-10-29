#include "shape_container.hpp"      // class declarations

/*------------------------------------------------
        ShapeContainer class definitions
--------------------------------------------------*/
ShapeContainer::ShapeContainer(const qt::Bound &bound) : bound(bound) {}

/*------------------------------------------------
        QuadTreeContainer class definitions
--------------------------------------------------*/

// Construct a ShapeContainer with given bound..
QuadTreeContainer::QuadTreeContainer(const qt::Bound &bound) : ShapeContainer(bound), itemContainer_qt(bound) {}

// Insert a shape in the container.
void QuadTreeContainer::insert(const Shape &itemWithBound) {
    itemContainer_qt.insert(itemWithBound);
}

// Searches the container for elements that overlap with the given bound.
std::vector<std::list<Shape>::iterator> QuadTreeContainer::queryOverlap(const qt::Bound &bound) {
    return itemContainer_qt.queryOverlap(bound);
}

// Searches the container for elements that are contained within the given bound.
std::vector<std::list<Shape>::iterator> QuadTreeContainer::queryContain(const qt::Bound &bound) {
    return itemContainer_qt.queryContain(bound);
}

// Removes all elements from the container that overlap with the given bound.
void QuadTreeContainer::removeOverlap(const qt::Bound &bound) {
    itemContainer_qt.removeOverlap(bound);
}

// Removes all elements from the container that are fully contained within the given bound.
void QuadTreeContainer::removeContain(const qt::Bound &bound) {
    itemContainer_qt.removeContain(bound);
}

// Returns all the boundaries that make up the inner structure of the container.
std::vector<qt::Bound> QuadTreeContainer::getBounds() {
    return itemContainer_qt.getBounds();
}

/*------------------------------------------------
        LinearContainer class definitions
--------------------------------------------------*/

// Construct a LinearContainer with given bound.
LinearContainer::LinearContainer(const qt::Bound &bound) : ShapeContainer(bound) {}

// Insert a shape in the container.
void LinearContainer::insert(const Shape &itemWithBound) {
    if(this->bound.contains(bound))
        itemContainer_list.push_back(itemWithBound);
}

// Searches the container for elements that overlap with the given bound.
std::vector<std::list<Shape>::iterator> LinearContainer::queryOverlap(const qt::Bound &bound) {
    std::vector<std::list<Shape>::iterator> returnItems;
    for(auto it = itemContainer_list.begin(); it != itemContainer_list.end(); ++it) {
        if(bound.overlaps(*it))
            returnItems.push_back(it);
    }
    return returnItems;
}

// Searches the container for elements that are contained within the given bound.
std::vector<std::list<Shape>::iterator> LinearContainer::queryContain(const qt::Bound &bound) {
    std::vector<std::list<Shape>::iterator> returnItems;
    for(auto it = itemContainer_list.begin(); it != itemContainer_list.end(); ++it) {
        if(bound.contains(*it))
            returnItems.push_back(it);
    }
    return returnItems;
}

// Removes all elements from the container that overlap with the given bound.
void LinearContainer::removeOverlap(const qt::Bound &bound) {
    auto it = itemContainer_list.begin();
    while(it != itemContainer_list.end()) {
        if(bound.overlaps(*it)) {
            it = itemContainer_list.erase(it);
        } else {
            ++it;
        }
    }
}

// Removes all elements from the container that are fully contained within the given bound.
void LinearContainer::removeContain(const qt::Bound &bound) {
    auto it = itemContainer_list.begin();
    while(it != itemContainer_list.end()) {
        if(bound.contains(*it)) {
            it = itemContainer_list.erase(it);
        } else {
            ++it;
        }
    }
}

// Returns all the boundaries that make up the inner structure of the container.
std::vector<qt::Bound> LinearContainer::getBounds() {
    std::vector<qt::Bound> bounds;
    bounds.push_back(bound);
    return bounds;
}