#include "shape.hpp"
#include "lib/quadtree.cpp"

// Instantiation of the QuadTree<T> template class, with Shape class as type.
template class qt::QuadTree<Shape>;