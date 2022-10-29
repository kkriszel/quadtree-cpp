#include "quadtree.hpp"     // class declarations

#include <queue>            // std::queue

namespace qt {
    /*------------------------------------------------
            QuadTree template class implementation
    --------------------------------------------------*/

    // Constructs an empty QuadTree in the given bound.
    template <typename T>
    QuadTree<T>::QuadTree(const Bound &bound) {
        // Constructs the root of the tree structure
        rootNode = new QuadTreeNode(bound);
    }

    // Destructs the QuadTree.
    template <typename T>
    QuadTree<T>::~QuadTree() {
        // Delete the root node, which causes the deletion of the entire tree structure
        // No need for freeing other members, they have their own deallocators
        delete rootNode;                            
    }
    
    // Inserts an element into the QuadTree.
    template <typename T>
    void QuadTree<T>::insert(const T &itemWithBound) {
        // First, insert the item in the main container, then get an iterator to it,
        // using some iterator arithmetic. Then insert the iterator in the tree structure.
        items.push_back(itemWithBound);             
        rootNode->insert(std::prev(items.end()));
    }

    // Searches the QuadTree for elements that overlap with the given bound.
    template <typename T>
    std::vector<typename QuadTree<T>::l_Iter> QuadTree<T>::queryOverlap(const Bound &bound) const {
        std::vector<l_Iter> foundItems;
        // Call the generic query method of the node class, but with the overlapFn predicate.
        rootNode->query(bound, foundItems, overlapFn);
        return foundItems;                              
    }

    // Searches the QuadTree for elements that are fully contained within the given bound.
    template <typename T>
    std::vector<typename QuadTree<T>::l_Iter> QuadTree<T>::queryContain(const Bound &bound) const {
        std::vector<l_Iter> foundItems;
        // Call the generic query method of the node class, but with the containFn predicate.          
        rootNode->query(bound, foundItems, containFn);
        return foundItems;                              
    }

    // Removes all elements from the QuadTree that overlap with the given bound.
    template <typename T>
    void QuadTree<T>::removeOverlap(const Bound &bound) {
        // We call the generic remove with the overlap predicate,
        // but we also pass a pointer to the container of the items.
        rootNode->remove(bound, &items, overlapFn);
    }

    // Removes all elements from the QuadTree that are contained within the given bound.
    template <typename T>
    void QuadTree<T>::removeContain(const Bound &bound) {
        // We call the generic remove with the contain predicate,
        // but we also pass a pointer to the container of the items.
        rootNode->remove(bound, &items, containFn);
    }

    // Returns all the boundaries that make up the QuadTree.
    template <typename T>
    std::vector<Bound> QuadTree<T>::getBounds() const {
        std::vector<Bound> bounds;
        rootNode->getBounds(bounds);
        return bounds;
    }

    /*------------------------------------------------
      QuadTree<T>::QuadTreeNode class implementation
    --------------------------------------------------*/

    // Constructs a QuadTreeNode.
    template <typename T>
    QuadTree<T>::QuadTreeNode::QuadTreeNode(const Bound &bound, int depth) : depth(depth), bound(bound){
        // The node is a leaf node if it can't be divided more, or if it has reached the maximal depth level
        leafNode = !(bound.quadDivisible()) || depth + 1 == MAXDEPTH;

        // If it isn't a leaf, get the bounds of its children
        if(!leafNode) {
            childrenBounds = bound.getQuadDivision();
        }
        
        // No children yet
        children[0] = children[1] = children[2] = children[3] = nullptr;
    }

    // Destroys the tree structure recursively.
    template <typename T>
    QuadTree<T>::QuadTreeNode::~QuadTreeNode() {
        for(auto &child : children) {
            // Deleting nullptrs is safe
            delete child;
        }
    }

    // Inserts an iterator to an element in the tree.
    template <typename T>
    void QuadTree<T>::QuadTreeNode::insert(const l_Iter &item) {
        // Let's search the node in which the item should be inserted into,
        // and let's start with "this" node (usually the root)
        QuadTreeNode* currentNode = this;

        // Keep track that have we inserted it already, or should we continue the search, and
        // have we found the next node in the search path
        bool inserted = false, foundNext;
        while(!inserted) {
            // We haven't yet found a next node in the search path
            foundNext = false;

            // If the current node is not a leaf, we can check for its children
            if(!currentNode->leafNode) {
                // Iterate through its children
                for(int i = 0; i < 4 && !foundNext; i++) {
                    // If the current child should contain the bound of the item
                    if(currentNode->childrenBounds[i].contains(*item)) {
                        // Check if the child exists
                        if(currentNode->children[i]) {
                            // if it exists, we have found the next node in the search path
                            currentNode = currentNode->children[i];
                            foundNext = true;
                        }
                        // If it doesn't exist, but we can create it
                        else if(currentNode->depth < MAXDEPTH - 1) {
                            // Let's create it, and we also have our next node in the search path
                            currentNode->children[i] = new QuadTreeNode(currentNode->childrenBounds[i], currentNode->depth + 1);
                            currentNode = currentNode->children[i];
                            foundNext = true;
                        }
                    }
                }
            }
            
            // If we didn't succeed with any of the searches above, the item
            // should be inserted in the current node's container
            if(!foundNext) {
                currentNode->items.push_back(item);
                inserted = true;
            }
        }
    }

    // Searches the tree for elements that overlap with/are contained in the given bound, based on the binary predicate function.
    template <typename T>
    void QuadTree<T>::QuadTreeNode::query(const qt::Bound &bound, std::vector<l_Iter> &foundItems, const std::function<bool (const qt::Bound&, const qt::Bound&)> &predicateFn) const {
        // All the QuadTreeNodes that are to be inspected
        std::queue<const QuadTreeNode*> nodeSearchFIFO;

        // All the QuadTreeNodes whose all items should be added to the response items             
        std::queue<const QuadTreeNode*> allItemNodeFIFO;
        
        // Let's start the search with "this" node (usually the root)
        nodeSearchFIFO.push(this);
        QuadTreeNode const *currentNode;
        while(!nodeSearchFIFO.empty()) {
            // Extract the next node from the FIFO
            currentNode = nodeSearchFIFO.front();
            nodeSearchFIFO.pop();

            // If we encountered a node that is fully contained within the bounds of the query
            if(bound.contains(currentNode->bound)) {
                // We are done with this node, all of its and its children's items should be returned,
                // so add it to the other FIFO
                allItemNodeFIFO.push(currentNode);
            } else {
                // If it isn't fully contained, let's check its items against the bound
                for(const auto &item : currentNode->items) {
                    // If the item overlaps/is within the query bound, it should be returned
                    if(predicateFn(bound, *item)) {
                        foundItems.push_back(item);
                    }
                }

                // Let's check the children of the current node
                for(int i = 0; i < 4; i++) {
                    // If the child exists, and its bound overlaps the query bound
                    if(currentNode->children[i] && bound.overlaps(currentNode->childrenBounds[i])) {
                        // We have to search it too, so add it to the FIFO
                        nodeSearchFIFO.push(currentNode->children[i]);
                    }
                }
            }
        }

        // We have to return the items of the nodes that we know that
        // are contained fully within the query bound
        while(!allItemNodeFIFO.empty()) {
            // Extract the next node from the FIFO
            currentNode = allItemNodeFIFO.front();
            allItemNodeFIFO.pop();

            foundItems.insert(foundItems.begin(), currentNode->items.begin(), currentNode->items.end());

            // And also add all the existing children to this FIFO
            for(int i = 0; i < 4; i++) {
                if(currentNode->children[i]) {
                    allItemNodeFIFO.push(currentNode->children[i]);
                }
            }
        }
    }

    // Removes the elements from the tree that overlap with/are contained in the given bound, based on the binary predicate function.
    template <typename T>
    void QuadTree<T>::QuadTreeNode::remove(const qt::Bound bound, std::list<T>* itemContainer, const std::function<bool (const qt::Bound&, const qt::Bound&)> &predicateFn) {
        // All the QuadTreeNodes that are to be inspected for removal
        std::queue<QuadTreeNode*> nodeRemoveFIFO;

        // All the QuadTreeNodes whose all items should be removed
        std::queue<QuadTreeNode*> allItemNodeFIFO;
        
        // Let's start the search with "this" node (usually the root)
        nodeRemoveFIFO.push(this);
        QuadTreeNode *currentNode;
        while(!nodeRemoveFIFO.empty()) {
            // Extract the next node from the FIFO
            currentNode = nodeRemoveFIFO.front();
            nodeRemoveFIFO.pop();

            // If we encountered a node that is fully contained within the bounds of inerest
            if(bound.contains(currentNode->bound)) {
                // We are done with this node, all of its and its children's items should be removed,
                // so add it to the other FIFO
                allItemNodeFIFO.push(currentNode);
            } else {
                // If it isn't fully contained, let's check its items against the bound
                auto it = currentNode->items.begin();
                while(it != currentNode->items.end()) {
                    // If the item overlaps/is within the bound, it should be removed
                    if(predicateFn(bound, *(*it))) {
                        // First we have to erase it from the outer container, and only then from the list of iterators
                        itemContainer->erase(*it);
                        it = currentNode->items.erase(it);
                    } else {
                        ++it;
                    }
                }

                // Let's check the children of the current node
                for(int i = 0; i < 4; i++) {
                    // If the child exists, and its bound overlaps the query bound
                    if(currentNode->children[i] && bound.overlaps(currentNode->childrenBounds[i])) {
                        // We have to investigate it too, so add it to the FIFO
                        nodeRemoveFIFO.push(currentNode->children[i]);
                    }   
                }
            }
        }

        // We have to remove the items of the nodes that we know that
        // are contained fully within the query bound
        while(!allItemNodeFIFO.empty()) {
            // extract the next node from the FIFO
            currentNode = allItemNodeFIFO.front();
            allItemNodeFIFO.pop();

            // First remove all the items from the outer container
            for(const auto &item : currentNode->items) {
                itemContainer->erase(item);
            }
            // Then clear the whole iterator list
            currentNode->items.clear();

            // And also add all the existing children to this FIFO, so that their items can be removed
            for(int i = 0; i < 4; i++) {
                if(currentNode->children[i]) {
                    allItemNodeFIFO.push(currentNode->children[i]);
                }
            }
        }
    }

    // Returns all the boundaries that make up the tree.
    template <typename T>
    void QuadTree<T>::QuadTreeNode::getBounds(std::vector<Bound> &bounds) const {
        // Visit all the nodes in a BFS style
        std::queue<const QuadTreeNode*> nodeFIFO;

         // Start with "this" node (usually the root)
        nodeFIFO.push(this);

        QuadTreeNode const *currentNode;
        while(!nodeFIFO.empty()) {
            // Take the next node
            currentNode = nodeFIFO.front();
            nodeFIFO.pop();

            // Add its bound to the return container
            bounds.push_back(currentNode->bound);

            // aAd all its existing children to the FIFO
            for(int i = 0; i < 4; i++) {
                if(currentNode->children[i]) {
                    nodeFIFO.push(currentNode->children[i]);
                }
            }
        }
    }
}