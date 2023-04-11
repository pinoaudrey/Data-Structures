#ifndef __MYBST_H__
#define __MYBST_H__

#include <iostream>
#include <fstream>
#include <cstdlib>

enum TreeTravOrderType
{
    PreOrder,
    InOrder,
    PostOrder
};

enum NodePosType
{
    Left,
    Right,
    Root
};

template <typename ComparableType>
class MyBST
{
  private:
    struct BinaryNode
    {
        ComparableType element;
        size_t height;
        BinaryNode *left;
        BinaryNode *right;
        
        BinaryNode(const ComparableType & x, const size_t h, BinaryNode *l, BinaryNode *r) : 
            element{x}, 
            height{h},
            left{l}, 
            right{r} 
        { 
            ;
        }

        BinaryNode(ComparableType && x, const size_t h, BinaryNode *l, BinaryNode *r) : 
            element{std::move(x)}, 
            height{h},
            left{l},
            right{r}
        {
            ;
        }
    };

    BinaryNode *root;       // the root node
    size_t theSize;         // the number of data elements in the tree

    // finds the minimum data element from the tree rooted at t
    BinaryNode* findMin(BinaryNode *t) const
    {
        // code begins
        if(t == nullptr)
            return nullptr;
        if(t->left == nullptr)
            return t;
        return findMin(t->left);
        // code ends
    }

    // finds the maximum data element from the tree rooted at t
    BinaryNode* findMax(BinaryNode *t) const
    {
        // code begins
        if(t != nullptr){
            while(t->right != nullptr)
                t = t->right;
        }
        return t;
        // code ends
    }

    // checks if x is contained in the tree rooted at t
    bool contains(const ComparableType& x, BinaryNode *t) const
    {
        // code begins
        if(t == nullptr)
            return false;
        else if(x < t->element)
            return contains(x, t->left); // recursive call
        else if(t->element < x)
            return contains(x, t->right); // recursive call
        else
            return true; // match
        // code ends
    }

    // deletes the tree rooted at t
    void clear(BinaryNode*& t)
    {
        // code begins
        if(t != nullptr)
        {
            clear(t->left);
            clear(t->right);
            delete t;
        }
        t = nullptr;
        // code ends
    }

    // returns the height of the node
    // leave nodes have a height of 1
    size_t height(BinaryNode* t) const
    {
        return (t == nullptr ? 0 : t->height);
    }

    static const int ALLOWED_IMBALANCE = 1;
    // balances tree node t
    void balance(BinaryNode*& t)
    {
        // code begins
        //std::cout << "Balance" << std::endl;

        if(t == nullptr){
            return;
        }
        
        int leftG = height(t->left) - height(t->right);
        int rightG = height(t->right) - height(t->left);
        if(leftG > ALLOWED_IMBALANCE){
            int leftL = height(t->left->left);
            int leftR = height(t->left->right);
            if(leftL >= leftR)
                rotateLeft(t);
            else
                doubleRotateLeft(t);
        }
        
        else {
            if(rightG > ALLOWED_IMBALANCE){
                int rightL = height(t->right->left);
                int rightR = height(t->right->right);
                if(rightR >= rightL){
                    rotateRight(t);
                }
                else{
                    doubleRotateRight(t);
                }
            }
        }
        
        t->height = std::max(height(t->left), height(t->right)) + 1;
        // code ends
    }
    /*
        void balance(BinaryNode*& t)
    {
        // code begins
        if(t == nullptr){
            return;
        }
        int leftGreater = height(t->left) - height(t->right);
        int rightGreater = height(t->right) - height(t->left);
        if(leftGreater > ALLOWED_IMBALANCE){ // if AVL is violated
            int leftLeft = height(t->left->left);
            int leftRight = height(t->left->right);
            if ( leftLeft >= leftRight) { // case 1
                rotateLeft(t);
            }
            else { // case 2
                doubleRotateLeft(t);
            }
        }
        else if (rightGreater > ALLOWED_IMBALANCE){
            int rightRight = height(t->right->right);
            int rightLeft = height(t->right->left);
                if ( rightRight >= rightLeft) { //case 4
                    rotateRight(t);
                } 
                else { // case 3
                    doubleRotateRight(t);
                }
        }
        t->height = std::max(height(t->left), height(t->right))+1;
        // code ends
    }*/
    
    // single rotation that reduces left branch depth
    void rotateLeft(BinaryNode*& t)
    {
        // code begins
        BinaryNode *t1 = t -> left;
        t -> left = t1 -> right;
        t1 -> right = t;
        t -> height = std::max(height(t -> left), height(t -> right)) + 1;
        t1 -> height = std::max(height(t1 -> left), t -> height) + 1;
        t = t1;
        // code ends
    }

    // single rotation that reduces right branch depth
    void rotateRight(BinaryNode*& t)
    {
        // code begins
        BinaryNode *t1 = t -> right;
        t -> right = t1 -> left;
        t1 -> left = t;
        t -> height = std::max(height(t -> left), height(t -> right)) + 1; 
        t1 -> height = std::max(t -> height, height(t1 -> right)) + 1; 
        t = t1;
        // code ends
    }

    // double rotation that reduces left branch depth
    void doubleRotateLeft(BinaryNode*& t)
    {
        // code begins
        rotateRight(t -> left);
        rotateLeft(t);
        // code ends
    }

    // double rotation that reduces right branch depth
    void doubleRotateRight(BinaryNode*& t)
    {
        // code begins
        rotateLeft(t -> right);
        rotateRight(t);
        // code ends
    }

    // inserts x to the tree rooted at t (copy)
    // if x exists, the do nothing
    void insert(const ComparableType& x, BinaryNode*& t)
    {
        // code begins
        if(t == nullptr){
            t = new BinaryNode{x, 1, nullptr, nullptr};
            theSize++;
        }
        else if(x < t->element){
            insert(x, t->left);
        }
        else if(t->element < x){
            insert(x, t->right);
        }
        else{
            ;
        }
        balance(t); // re-balance the tree if necessary
        // code ends
    }

    // inserts x to the tree rooted at t (move)
    void insert(ComparableType && x, BinaryNode*& t)
    {
        // code begins
        if(t == nullptr){
            t = new BinaryNode{std::move(x), 1, nullptr, nullptr};
            theSize++;
        }
        else if(x < t->element){
            insert(std::move(x), t->left);
        }
        else if(t->element < x){
            insert(std::move(x), t->right);
        }
        else{
            ;
        }
        balance(t); // re-balance the tree if necessary
        // code ends
    }

    // removes x from the tree rooted at t
    // when deleting a node with two children, replace it with the smallest child of the right subtree
    void remove(const ComparableType& x, BinaryNode*& t)
    {
        //std::cout << "remove called" << std::endl;
        if (t==nullptr)
            return;
        if ( x < t->element)
            remove(x, t->left);
        else if ( x > t->element)
            remove(x, t->right);
        else if ( t->left != nullptr && t->right != nullptr ) {
            t->element = findMin(t->right)->element;
            remove(t->element,t->right);
        } else {
            BinaryNode *oldNode = t;
            if(t->left != nullptr){
                t = t->left;
            }
            else{
                t = t->right;
            }
            delete oldNode;
            theSize--;
        }

        balance(t);
        // code ends
    }

    // clones the node t and returns the clone
    BinaryNode* clone(BinaryNode* t) const
    {
        if( t == nullptr ) return nullptr;
        else
            return new BinaryNode{t->element, clone(t->left), clone(t->right)};
    }

    // prints all data elements rooted at t pre-order
    void printPreOrder(BinaryNode* t, std::ostream& out) const
    {
        const char delim = ' ';
        if(t != nullptr)
        {
            out << t->element<< delim;
            printPreOrder(t->left, out);
            printPreOrder(t->right, out);
        }
        return;
    }

    // prints all data elements rooted at t in-order
    void printInOrder(BinaryNode* t, std::ostream& out) const    
    {
        const char delim = ' ';
        if(t != nullptr)
        {
            printInOrder(t->left, out);
            out << t->element<< delim;
            printInOrder(t->right, out);
        }
        return;
    }

    // prints all data elements rooted at t post-order
    void printPostOrder(BinaryNode* t, std::ostream& out) const
    {
        const char delim = ' ';
        if(t != nullptr)
        {
            printPostOrder(t->left, out);
            printPostOrder(t->right, out);
            out << t->element << delim;
        }
        return;
    }

    // prints the subtree rooted at t
    // lv is the level of t (the root corresponds to level 0)
    // p is the relative topological position of t (left/right/root) 
    void printTree(BinaryNode* t, const int lv, const NodePosType p)
    {
        if(t == nullptr)
            return;

        char pos;
        switch(p)
        {
            case Left:
                pos = 'L';
                break;
            case Right:
                pos = 'R';
                break;
            case Root:
                pos = 'T';
                break;
            default:
                std::cout << "Error: MyBST::printTree: unrecognized position type." << std::endl;
        }        
        std::cout << t->element << "|" << lv << "|" << pos << std::endl;
        printTree(t->left, lv + 1, Left);
        printTree(t->right, lv + 1, Right);
        return;
    }

  public:

    // default constructor
    MyBST() : 
        root{nullptr},
        theSize{0}
    {
        ;
    }

    // copy constructor
    MyBST(const MyBST& rhs) : 
        root{nullptr},
        theSize{rhs.theSize}
    {
        root = clone(rhs.root);
    }

    // move constructor
    MyBST(MyBST && rhs) : 
        root{rhs.root},
        theSize{rhs.theSize}
    {
        rhs.root = nullptr;
    }
      
    // destructor
    ~MyBST()
    {
        clear();
    }

    // finds the minimum data element in the tree
    const ComparableType& findMin() const
    {
        // code begins
        return findMin(root)->element;
        // code ends
    }

    // finds the maximum data element in the tree
    const ComparableType& findMax( ) const
    {
        // code begins
        return findMax(root)->element;
        // code ends
    }

    // checks whether x is contained in the tree
    bool contains(const ComparableType& x) const
    {
        // code begins
        return contains(x, root);
        // code ends
    }
    
    // returns the number of data elements in the tree
    size_t size(void) const
    {
        // code begins
        return theSize;
        // code ends
    }

    // returns the depth of the tree
    // depth defined as the longest path length from the root to any leaf
    // e.g. an empty tree has a depth of 0, a tree with a single node has a depth of 1
    size_t depth(void) const
    {
        // code begins
        //return height(root);

        if(!empty()){
            return height(root);
        }
        else{
            return 0;
        }
        // code ends
    }

    // checks whether the tree is empty
    bool empty() const
    {
        // code begins
        return root == nullptr;
        // code ends
    }

    // delete all data elements in the tree
    void clear()
    {
        // code begins
        clear(root);
        // code ends
    }

    // insert x into the tree (copy)
    void insert(const ComparableType& x)
    {
        // code begins
        insert(x, root);
        // code ends
    }

    // insert x into the tree (move)
    void insert(ComparableType && x)
    {
        // code begins
        insert(std::move(x), root);
        // code ends
    }

    // removes x from the tree
    void remove(const ComparableType& x)
    {
        // code begins
        remove(x, root);
        // code ends
    }

    // copy assignment
    MyBST& operator=(const MyBST& rhs)
    {
        // code begins
        theSize = rhs.theSize;
        root = clone(rhs.root);
        // code ends
    }

    // move assignment
    MyBST& operator=(MyBST && rhs)
    {
        // code begins
        theSize = std::move(rhs.theSize);
        root = clone(std::move(rhs.root));
        // code ends
    }

    // finds the lowest common ancestor (LCA) of x and y
    // if x and y are both in the tree, return true and store their LCA in lca
    // otherwise, return false
    bool lowestCommonAncestor(const ComparableType& x, const ComparableType& y, ComparableType& lca)
    {
        // code begins
        BinaryNode* node = root;
        if(contains(x, root) && contains(y, root)){
            while(contains(x, node) && contains(y, node)){
                if(contains(x, node -> left) && contains(y, node -> left)){
                    node = node -> left;
                }
                else if(contains(x, node -> right) && contains(y, node -> right)){
                    node = node -> right;
                }
                else{
                    lca = node -> element;
                    return true;
                }
            }
        }
        else{
            return false;
        }
        return false;
        // code ends
    }

    // print all data elements in the tree
    void print(TreeTravOrderType order, std::ostream& out = std::cout) const
    {
        switch (order)
        {
            case PreOrder:
                printPreOrder(root, out);
                out << std::endl;
                break;
            case InOrder:
                printInOrder(root, out);
                out << std::endl;
                break;
            case PostOrder:
                printPostOrder(root, out);
                out << std::endl;
                break;
            default:
                out << "Error: MyBST::print: Unsupported print order." << std::endl;
                break;
        }
        return;
    }

    // print all data elements in the tree
    // including partial topological information (layer and relative position among siblings)
    void printTree(void)
    {
        printTree(root, 0, Root);
        return;
    }
      
};

#endif // __MYBST_H__