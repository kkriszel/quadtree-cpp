#ifndef RECTANGLE_CONTAINER_H
#define RECTANGLE_CONTAINER_H

#include "lib/quadtree.hpp"         // qt::QuadTree<Rectangle>
#include "lib/bound.hpp"            // qt::Bound
#include "lib/util.hpp"             // qt::Vec2D_i32
#include "shape.hpp"                // Shape
#include <list>                     // std::list

/**
 * @brief Shape container abstract class, the inheriting classes should implement its methods.
 * @note The base class has only a bound member, representing its maximal bound on the screen.
 */
class ShapeContainer {
    public:
        /**
         * @brief Construct a ShapeContainer with given bound.
         */
        ShapeContainer(const qt::Bound &bound);

        /**
         * @brief Insert a shape in the container.
         * @note Pure virtual (abstract) method, should be implemented.
         */
        virtual void insert(const Shape &shape) = 0;     

        /**
         * @brief Searches the container for elements that overlap with the given bound.
         * @param bound The search bound that all the found elements should overlap with.
         * @return A std::vector of std::list<Shape>::iterators, which point to the found elements. 
         * @note Pure virtual (abstract) method, should be implemented.
         */
        virtual std::vector<std::list<Shape>::iterator> queryOverlap(const qt::Bound &bound) = 0;

        /**
         * @brief Searches the container for elements that are contained within the given bound.
         * @param bound The search bound that all the found elements should be contained in.
         * @return A std::vector of std::list<Shape>::iterators, which point to the found elements. 
         * @note Pure virtual (abstract) method, should be implemented.
         */
        virtual std::vector<std::list<Shape>::iterator> queryContain(const qt::Bound &bound) = 0;

        /**
         * @brief Removes all elements from the container that overlap with the given bound.
         * @param bound The bound that all the removed elements should overlap with.
         * @note Pure virtual (abstract) method, should be implemented.
         */
        virtual void removeOverlap(const qt::Bound &bound) = 0;

         /**
         * @brief Removes all elements from the container that are fully contained within the given bound.
         * @param bound The bound that contains all the elements that should be removed. 
         * @note Pure virtual (abstract) method, should be implemented.
         */
        virtual void removeContain(const qt::Bound &bound) = 0;

        /**
         * @brief Returns all the boundaries that make up the inner structure of the container.
         * @note Pure virtual (abstract) method, should be implemented.
         */
        virtual std::vector<qt::Bound> getBounds() = 0;

        /**
         * @brief Destroy the container.
         * @note Needs to be virtual, so that the derived class calls its destructor too.
         */
        virtual ~ShapeContainer() {}

    protected:
        qt::Bound bound;
};

/**
 * @brief Shape container class extending abstract ShapeContainer class,
 *      with a qt::QuadTree<Shape> as its underlying mechanism.
 */
class QuadTreeContainer : public ShapeContainer {
    public:
        /**
         * @brief Construct a QuadTreeContainer with given bound.
         */
        QuadTreeContainer(const qt::Bound &bound);                                                  

        /**
         * @brief Insert a shape in the container.
         */
        void insert(const Shape &rectangle) override;                                           

        /**
         * @brief Searches the container for elements that overlap with the given bound.
         * @param bound The search bound that all the found elements should overlap with.
         * @return A std::vector of std::list<Shape>::iterators, which point to the found elements.
         */
        virtual std::vector<std::list<Shape>::iterator> queryOverlap(const qt::Bound &bound) override;

        /**
         * @brief Searches the container for elements that are contained within the given bound.
         * @param bound The search bound that all the found elements should be contained in.
         * @return A std::vector of std::list<Shape>::iterators, which point to the found elements. 
         */
        virtual std::vector<std::list<Shape>::iterator> queryContain(const qt::Bound &bound) override;

        /**
         * @brief Removes all elements from the container that overlap with the given bound.
         * @param bound The bound that all the removed elements should overlap with.
         */
        virtual void removeOverlap(const qt::Bound &bound) override;

         /**
         * @brief Removes all elements from the container that are fully contained within the given bound.
         * @param bound The bound that contains all the elements that should be removed.
         */
        virtual void removeContain(const qt::Bound &bound) override;

        /**
         * @brief Returns all the boundaries that make up the inner structure of the container.
         */
        virtual std::vector<qt::Bound> getBounds() override;

    protected:
        /**
         * @brief The qt::QuadTree<Shape>, as the underlying mechanism.
         */
        qt::QuadTree<Shape> itemContainer_qt;
};

/**
 * @brief Shape container class extending abstract ShapeContainer class,
 *      with a std::list<Shape> as its underlying mechanism.
 */
class LinearContainer : public ShapeContainer {
    public:
        /**
         * @brief Construct a LinearContainer with given bound.
         */
        LinearContainer(const qt::Bound &bound);                                                  

        /**
         * @brief Insert a shape in the container.
         */
        void insert(const Shape &rectangle) override;                                           

        /**
         * @brief Searches the container for elements that overlap with the given bound.
         * @param bound The search bound that all the found elements should overlap with.
         * @return A std::vector of std::list<Shape>::iterators, which point to the found elements.
         */
        virtual std::vector<std::list<Shape>::iterator> queryOverlap(const qt::Bound &bound) override;

        /**
         * @brief Searches the container for elements that are contained within the given bound.
         * @param bound The search bound that all the found elements should be contained in.
         * @return A std::vector of std::list<Shape>::iterators, which point to the found elements. 
         */
        virtual std::vector<std::list<Shape>::iterator> queryContain(const qt::Bound &bound) override;

        /**
         * @brief Removes all elements from the container that overlap with the given bound.
         * @param bound The bound that all the removed elements should overlap with.
         */
        virtual void removeOverlap(const qt::Bound &bound) override;

         /**
         * @brief Removes all elements from the container that are fully contained within the given bound.
         * @param bound The bound that contains all the elements that should be removed.
         */
        virtual void removeContain(const qt::Bound &bound) override;

        /**
         * @brief Returns all the boundaries that make up the inner structure of the container.
         * @note One bound only.
         */
        virtual std::vector<qt::Bound> getBounds() override;

    protected:
        /**
         * @brief The std::list<Shape>, as the underlying mechanism.
         */
        std::list<Shape> itemContainer_list;
};

#endif