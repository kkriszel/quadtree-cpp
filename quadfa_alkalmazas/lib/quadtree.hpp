#ifndef QUADTREE_H
#define QUADTREE_H

#include "bound.hpp"        /// qt::Bound
#include "util.hpp"         /// qt::Vec2D_i32

#include <list>             /// std::list
#include <vector>           /// std::vector
#include <type_traits>      /// std::is_convertible
#include <functional>

/**
 * @brief Wrapper namespace for the QuadTree and related classes.
 */
namespace qt {
    /** 
     * @brief A container which can store any object with boundary based 2D spatial information,
     *      and which also offers fast (logarithmic) insertion/query/removal operations.
     * @tparam T The type of elements in the QuadTree. Because the operations are based on
     *      spatial information, T has to be convertible to qt::Bound type, for which the
     *      easiest solution is for T to inherit from qt::Bound.
     */
    template <typename T>
    class QuadTree {
        /**
         * Make sure that T is convertible to qt::Bound; if not,
         * the compiler gives the corresponding error message.
         */
        static_assert(std::is_convertible<T, qt::Bound>::value, "Type T must be convertible to Bound.");

        public:
            /**
             * @brief The type of the elements stored in each QuadTreeNode,
             *      and also the type of the container that is the result of a query.
             */
            typedef typename std::list<T>::iterator l_Iter;

            /**     
             * @brief Constructs an empty QuadTree in the given bound.
             * @param[in] bound The bound that contains all the future elements of the QuadTree.
             * @note No default constructor exists for the QuadTree, the bound has to be known upon construction.
             */
            QuadTree(const Bound &bound);

            /**
             * @brief No copy constructor (yet).
             * @todo Implement copy constructor.
             */
            QuadTree(const QuadTree<T> &other) = delete;

            /**
             * @brief No move constructor (yet).
             * @todo Implement move constructor.
             */
            QuadTree(QuadTree<T> &&other) = delete;                                     

            /**
             * @brief Destructs the QuadTree.
             */
            virtual ~QuadTree();     

            /**
             * @brief Inserts an element into the QuadTree.
             * @param[in] itemWithBound An element with type T, which needs to be inserted into the QuadTree.
             * @note The element will be inserted iff. the bound of the QuadTree fully contains it.
             */                    
            virtual void insert(const T &itemWithBound);

            /**
             * @brief Searches the QuadTree for elements that overlap with the given bound.
             * @param[in] bound The search bound that all the found elements should overlap with.
             * @return A std::vector of l_Iter (std::list<T>::iterator) elements, which are valid iterators in the inner
             *      container of the QuadTree, and which point to the found elements.
             */
            virtual std::vector<l_Iter> queryOverlap(const qt::Bound &bound) const;

            /**
             * @brief Searches the QuadTree for elements that are fully contained within the given bound.
             * @param[in] bound The search bound that should contain all the found elements.
             * @return A std::vector of l_Iter (std::list<T>::iterator) elements, which are valid iterators in the inner
             *      container of the QuadTree, and which point to the found elements.
             */
            virtual std::vector<l_Iter> queryContain(const qt::Bound &bound) const;

            /**
             * @brief Removes all elements from the QuadTree that overlap with the given bound.
             * @param[in] bound The bound that all the removed elements should overlap with.
             */
            virtual void removeOverlap(const Bound &bound);

            /**
             * @brief Removes all elements from the QuadTree that are contained within the given bound.
             * @param[in] bound The bound that contains all the elements that should be removed.
             */
            virtual void removeContain(const Bound &bound);

            /**
             * @brief Returns all the boundaries that make up the QuadTree.
             * @return A std::vector of qt::Bounds, all the bound objects that make up the inner structure of the QuadTree
             */
            virtual std::vector<qt::Bound> getBounds() const;

            /**
             * @brief Lambda function, which returns a logical value based on whether Bound "a" overlaps with Bound "b".
             *      It is used as a two operand predicate function for generalizing the query and remove operations
             *      in the QuadTreeNode class.
             */
            static constexpr auto overlapFn = [](const qt::Bound &a, const qt::Bound &b) {return a.overlaps(b);};

            /**
             * @brief Lambda function, which returns a logical value based on whether Bound "a" contains Bound "b".
             *      It is used as a two operand predicate function for generalizing the query and remove operations
             *      in the QuadTreeNode class.
             */
            static constexpr auto containFn = [](const qt::Bound &a, const qt::Bound &b) {return a.contains(b);};
            
        protected:
            /**
             * @brief Forward declaration of the inner QuadTreeNode class.
             */
            class QuadTreeNode;

            /**
             * @brief The container in which the inserted items are stored.
             * @note Using an std::vector container is wrong, because upon insertion
             *      the iterators of its elements can be invalidated. On the other hand,
             *      std::list guarantees that its iterators will not be invalidated.
             */
            std::list<T> items;

            /**
             * @brief The entry point to the inner structure of the tree. It is initialised upon constructing the QuadTree object.
             */
            QuadTreeNode* rootNode;
    };

    /**
     * @brief Inner class of QuadTree, with recursive structure. It makes up the concrete data structure that
     *      implements the required operations on the QuadTree.
     * @tparam T The type of elements in the QuadTree. It is needed because a QuadTreeNode
     *      can only be interpreted in the context of a QuadTree wrapper.
     * @note Each node stores iterators to the contained items, and doesn't store the item itself. This
     *      makes all operations more efficient.
     */
    template <typename T>
    class QuadTree<T>::QuadTreeNode {
        public:
            /**
             * @brief Constructs a QuadTreeNode.
             * @param[in] bound The bound that the node fully covers.
             * @param[in] depth The depth of the node in the tree structure.
             */
            QuadTreeNode(const Bound &bound, int depth = 0);

            /**
             * @brief No copy constructor.
             * @note Unlike the copy constructor of the QuadTree class, this doesn't
             *      have a purpose to be implemented.
             */
            QuadTreeNode(const QuadTreeNode &other) = delete;

            /**
             * @brief No move constructor.
             * @note Unlike the move constructor of the QuadTree class, this doesn't
             *      have a purpose to be implemented.
             */
            QuadTreeNode(QuadTreeNode &&other) = delete;

            /**
             * @brief Destroys the tree structure recursively.
             */
            virtual ~QuadTreeNode();                                    

            /**
             * @brief Inserts an iterator to an element in the tree.
             * @param[in] item An iterator to the element which needs to be inserted in the tree.
             *          From it it can be deduced the bound of the element.
             * @note Instead of the intuitive recursive method, it is implemented in a faster, iterative way.
             * @see QuadTree<T>::insert
             */
            virtual void insert(const l_Iter &item);

             /**
             * @brief Searches the tree for elements that overlap with/are contained in the given bound, based on the binary predicate function.
             * @param[in] bound The search bound that all the found elements should overlap with/be contained in.
             * @param[out] foundItems The std::vector of std::list<T>::iterators, which point
             *          to the found elements, and which are valid in the context of the QuadTree classes container.
             * @param[in] predicateFn Binary predicate function, taking two Bound arguments and returning whether they are in a certain relation or not.
             * @note Instead of the intuitive recursive method, it is implemented in a faster, iterative way.
             * @see QuadTree<T>::queryOverlap
             * @see QuadTree<T>::queryContain
             */
            virtual void query(const qt::Bound &bound, std::vector<l_Iter> &foundItems, const std::function<bool (const qt::Bound&, const qt::Bound&)> &predicateFn) const;

            /**
             * @brief Removes the elements from the tree that overlap with/are contained in the given bound, based on the binary predicate function.
             * @param[in] bound The search bound that all the found elements should overlap with/be contained in.
             * @param[out] itemContainer itemContainer A pointer to the outter container in which the elements' iterators have context.
             * @param[in] predicateFn Binary predicate function, taking two Bound arguments and returning whether they are in a certain relation or not.
             * @note Instead of the intuitive recursive method, it is implemented in a faster, iterative way.
             * @see QuadTree<T>::removeOverlap
             * @see QuadTree<T>::removeContain
             */
            virtual void remove(const qt::Bound bound, std::list<T>* itemContainer, const std::function<bool (const qt::Bound&, const qt::Bound&)> &predicateFn);

             /**
             * @brief Returns all the boundaries that make up the tree.
             * @param[out] bounds A std::vector of qt::Bounds, all the bound objects that make up the tree structure.
             * @note Instead of the intuitive recursive method, it is implemented in a faster, iterative way.
             * @see QuadTree<T>::getBounds
             */
            virtual void getBounds(std::vector<Bound> &bounds) const;

        protected:
            /**
             * @brief Static constant in all QuadTreeNode instances. It represents the maximal depth (plus 1)
             * that a node can reach.
             * @note 0 <= depth < MAXDEPTH
             * @note Should be choosen carefully.
             */
            static const int MAXDEPTH = 6;

            /**
             * @brief Tells wether the node is a leaf or node.
             */
            bool leafNode;    

            /**
             * @brief Stores the depth level that the node resides in.
             * @note 0 <= depth < MAXDEPTH.
             */                                                 
            int depth;

            /**
             * @brief The bound that the node covers.
             */                                                        
            qt::Bound bound;                                                     
            
            /**
             * @brief A list of iterators that point to the elements stored in the node.
             */
            std::vector<l_Iter> items;
            
            /**
             * @brief Pointers to the children nodes of the node.
             */
            QuadTreeNode* children[4];

            /**
             * @brief The four subdivisions (quadrons) of the node.
             */                                
            std::array<qt::Bound, 4> childrenBounds;
    };
}

#endif