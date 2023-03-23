#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    int8_t balanceFactor(AVLNode<Key, Value>* node);
    int8_t getHeight(AVLNode<Key,Value>*ptr) const;   
    void insert_fix(AVLNode<Key, Value>* p,AVLNode<Key, Value>* n); 

    void rotateRight(AVLNode<Key,Value>* p);   
    void rotateLeft(AVLNode<Key,Value>* p);
    void avlInsert(const std::pair<const Key, Value> &keyValuePair);                                                                                    
};

/*
 * Recall: If key is already in the tree, you should updat
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::avlInsert(const std::pair<const Key, Value> &keyValuePair)
{
    
    AVLNode<Key, Value> *newNode = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second,nullptr);
    if (this->root_ == nullptr){
        this->root_ = newNode;
    }
    else{
        Node<Key, Value> * curr = this->root_;
        while(curr != nullptr){
            if (keyValuePair.first < curr->getKey()){
                if (curr->getLeft() == nullptr){
                    curr->setLeft(newNode);
                    newNode->setParent(curr);
                    break;
                }
                else{
                    curr = curr->getLeft();
                }
            }
            else if (keyValuePair.first > curr->getKey()){
                if (curr->getRight() == nullptr){
                    curr->setRight(newNode);
                    newNode->setParent(curr);
                    break;
                }
                else{
                    curr = curr->getRight();
                }
            }
            else{
                curr->setValue(keyValuePair.second);
                break;
            }
        }
    }
}

template<class Key, class Value>
int8_t AVLTree<Key, Value>::balanceFactor(AVLNode<Key, Value>* node)
{
    if(node == nullptr)
        return 0;
    int8_t left = getHeight(node->getLeft());
    int8_t right = getHeight(node->getRight());
    // if(node->getLeft() == nullptr)
    //     left = -1;
    // if(node->getRight() == nullptr)
    //     right = -1;
    return right-left;
}

template<class Key, class Value>
int8_t AVLTree<Key, Value>::getHeight(AVLNode<Key,Value>*ptr) const
{
    if(ptr == nullptr)
        return 0;
    int8_t left = getHeight(ptr->getLeft());
    int8_t right = getHeight(ptr->getRight());
    if(left > right)
        return left + 1;
    else
        return right + 1;
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* x){
    AVLNode<Key,Value>* y = x->getRight();
    AVLNode<Key,Value>* z = y->getLeft();
    AVLNode<Key,Value>* p = x->getParent();
    if(p == nullptr){
        this->root_ = y;
        y->setParent(nullptr);
    }
    else{
        if(p->getLeft() == x){
            p->setLeft(y);
            y->setParent(p);
        }
        else{
            p->setRight(y);
            y->setParent(p);
        }
    }
    y->setLeft(x);
    x->setParent(y);
    x->setRight(z);
    if(z != nullptr)
        z->setParent(x);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* z){
    AVLNode<Key,Value>* y = z->getLeft();
    AVLNode<Key,Value>* x = y->getRight();
    AVLNode<Key,Value>* p = z->getParent();
    if(p == nullptr){
        this->root_ = y;
        y->setParent(nullptr);
    }
    else{
        if(p->getLeft() == z){
            p->setLeft(y);
            y->setParent(p);
        }
        else{
            p->setRight(y);
            y->setParent(p);
        }
    }
    y->setRight(z);
    z->setParent(y);
    z->setLeft(x);
    if(x != nullptr)
        x->setParent(z);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key,Value>*p,AVLNode<Key,Value>*n)
{
    if(p == nullptr || p->getParent() == nullptr)
        return;
    AVLNode<Key,Value>* g = p->getParent();

    //Assume p is left child of g
    if(p->getParent()->getLeft() == p){
        
        g->updateBalance(-1);
        //3 cases
        if(balanceFactor(g)==0)
            return;
        else if(balanceFactor(g)==-1){
            insert_fix(g, p);
        }
        else if(balanceFactor(g) == -2){
            //zigzig
            if(n == p->getLeft()){
                rotateRight(g);
                p->updateBalance(0);
                g->updateBalance(0);
                if(g == this->root_){
                    this->root_ = p;
                }
            }
            //zigzag
            else{
                rotateLeft(p);
                rotateRight(g);
                if(n->getBalance() == -1){
                    p->updateBalance(0);
                    g->updateBalance(1);
                    n->updateBalance(0);
                }
                else if(n->getBalance() == 0){
                    p->updateBalance(0);
                    g->updateBalance(0);
                    n->updateBalance(0);
                }
                else if(n->getBalance() == 1){
                    p->updateBalance(-1);
                    g->updateBalance(0);
                    n->updateBalance(0);
                }
                if(g == this->root_){
                    this->root_ = n;
                }
            }
        }
    }

    //Assume p is right child of g
   
    else if(p->getParent()->getRight() == p){
        g->updateBalance(1);
        //3 cases
        if(balanceFactor(g)==0)
            return;
        else if(balanceFactor(g)==1){
            insert_fix(g, p);
        }
        else if(balanceFactor(g) == 2){
            //zigzig
            if(n == p->getRight()){
                rotateLeft(g);
                p->updateBalance(0);
                g->updateBalance(0);
                if(g == this->root_){
                    this->root_ = p;
                }
            }
            else{
                rotateRight(p);
                rotateLeft(g);
                if(n->getBalance() == -1){
                    p->updateBalance(0);
                    g->updateBalance(1);
                    n->updateBalance(0);
                }
                else if(n->getBalance() == 0){
                    p->updateBalance(0);
                    g->updateBalance(0);
                    n->updateBalance(0);
                }
                else if(n->getBalance() == 1){
                    p->updateBalance(-1);
                    g->updateBalance(0);
                    n->updateBalance(0);
                }
                
                if(g == this->root_){
                    this->root_ = n;
                }
            }
        }
    }
}



template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    AVLTree<Key, Value>::avlInsert(new_item);
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(new_item.first));
    node->setBalance(0);
    if(!BinarySearchTree<Key, Value>::isBalanced())
    {
        int8_t balance = balanceFactor(node->getParent());
        
        node->getParent()->updateBalance(balance);
        if(balance !=0){
            insert_fix(node->getParent(),node);
        }

    }
    
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
