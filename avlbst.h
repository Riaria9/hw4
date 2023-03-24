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
    void removeFix(AVLNode<Key,Value>*p,int8_t diff);
    void rotateRight(AVLNode<Key,Value>* p);   
    void rotateLeft(AVLNode<Key,Value>* p);
    void avlInsert(const std::pair<const Key, Value> &keyValuePair);   

    void avlRemove(const Key& key);                                                                                 
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
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    
     
    
    if(this->root_ == nullptr){
        AVLTree<Key, Value>::avlInsert(new_item);
        AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(new_item.first));
        this->root_ = node;
        node->setBalance(0);
        return;
    }
    
    else 
    {
        AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(new_item.first));
        if(node != nullptr){
            AVLTree<Key, Value>::avlInsert(new_item);
            return;
        }
        AVLTree<Key, Value>::avlInsert(new_item);
        node = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(new_item.first));
        node->setBalance(0);
                
        // if node is parent's left child
        // parent updatebalance(-1)
        // else
        // updatebalance(1)

        // if parent balance not 0
        // insert fix
        AVLNode<Key, Value>* p = node->getParent();
        int balance = node->getParent()->getBalance();
        if(balance == 0){
            //node->getParent()->setBalance(balanceFactor(node->getParent()));
            if(node == p->getLeft())
                p->setBalance(-1);
            if(node == p->getRight())
                p->setBalance(1);
            insert_fix(node->getParent(),node);
        }
        else if(balance == -1)
        {
            node->getParent()->setBalance(0);
            return;
        }
        else if(balance ==1)
        {
            node->getParent()->setBalance(0);
            return;
        }

    }
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key,Value>*p,AVLNode<Key,Value>*n)
{
    if(p == nullptr || p->getParent() == nullptr)
        return;
    AVLNode<Key,Value>* g = p->getParent();

    //p is left child of g
    if(p->getParent()->getLeft() == p){
        
        g->updateBalance(-1);
        //3 cases
        if(g->getBalance()==0)
            return;
        else if(g->getBalance()==-1){
            insert_fix(g, p);
        }
        else if(g->getBalance() == -2){
            //zigzig
            if(n == p->getLeft()){
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
                if(g == this->root_){
                    this->root_ = p;
                }
            }
            //zigzag
            else{
                rotateLeft(p);
                rotateRight(g);
                if(n->getBalance() == -1){
                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0);
                }
                else if(n->getBalance() == 0){
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if(n->getBalance() == 1){
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);
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
        if(g->getBalance()==0)
            return;
        else if(g->getBalance()==1){
            insert_fix(g, p);
        }
        else if(g->getBalance() == 2){
            //zigzig
            if(n == p->getRight()){
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
                if(g == this->root_){
                    this->root_ = p;
                }
            }
            else{
                rotateRight(p);
                rotateLeft(g);
                if(n->getBalance() == -1){
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if(n->getBalance() == 0){
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if(n->getBalance() == 1){
                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0);
                }
                
                if(g == this->root_){
                    this->root_ = n;
                }
            }
        }
    }
}

// template<typename Key, typename Value>
// void AVLTree<Key, Value>::avlRemove(const Key& key)
// {    
//     AVLNode<Key, Value> *curr = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
//     if(curr == nullptr) return;
//     else{
//         //if it has 2 children, swap with its predecessor
//         if(curr->getLeft()!=nullptr && curr->getRight()!=nullptr){
//             AVLNode<Key, Value> *pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(curr));
//             //since right most child, could possibly only have left child
//             if(pred->getLeft() == nullptr){//if no more left child
//                 nodeSwap(pred,curr);
//                 if(curr->getParent()->getLeft()==curr)curr->getParent()->setLeft(nullptr);
//                 else if(curr->getParent()->getRight()==curr)curr->getParent()->setRight(nullptr);
//                 delete curr;
//             }
    
//             else{//if left child exists on the predecessor
//                 nodeSwap(pred,curr);
//                 if(curr->getParent()->getLeft()==curr){
//                     if(curr->getLeft()!=nullptr){//updating pointer
//                         curr->getParent()->setLeft(curr->getLeft());
//                         curr->getLeft()->setParent(curr->getParent());
//                     } 
//                     else curr->getParent()->setLeft(nullptr);
                    
//                 }
//                 else if(curr->getParent()->getRight()==curr){
//                     if(curr->getLeft()!=nullptr)//updating pointer
//                     {
//                         curr->getParent()->setRight(curr->getLeft());
//                         curr->getLeft()->setParent(curr->getParent());
//                     } 
//                     else {//updating
//                         curr->getParent()->setRight(nullptr);
                        
//                     }
//                 }
//                 delete curr;
//             }
//         }
//         //following 2 cases are if it has 1 child
//         else if(curr->getLeft()!=nullptr){//child on current's left
//             curr->getLeft()->setParent(curr->getParent());
//             if(curr->getParent()==nullptr){
//                 this->root_ = curr->getLeft();
//             }
//             else{
//                 if(curr->getParent()->getRight()==curr) curr->getParent()->setRight(curr->getLeft());
//             else if(curr->getParent()->getLeft()==curr) curr->getParent()->setLeft(curr->getLeft());
//             }
            
//             delete curr;
//         }
//         else if(curr->getRight()!= nullptr){//child on current's right
//             curr->getRight()->setParent(curr->getParent());
//             if(curr->getParent()==nullptr){
//                 this->root_ = curr->getRight();
//             }
//             else{
//                 if(curr->getParent()->getRight()==curr) curr->getParent()->setRight(curr->getRight());
//                 else if (curr->getParent()->getLeft()==curr) curr->getParent()->setLeft(curr->getRight());
//             }
//             delete curr;
            
//         }
//         else{
//             if(curr->getParent() == nullptr){
//                 BinarySearchTree<Key,Value>::clear();
//             }
//             else{
//                 if(curr->getParent()->getLeft()==curr) curr->getParent()->setLeft(nullptr);
//                 else if(curr->getParent()->getRight()==curr) curr->getParent()->setRight(nullptr);
//                 delete curr;
//             }
//         }
//     }
// }



/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    int8_t diff;
    //if empty tree
    if(this->root_ == nullptr)
        return;
    
    
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
    AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(curr));
    if(curr==nullptr)
        return;

    //if n has 2 children, swap with predecessor
    if(curr->getLeft()!=nullptr && curr->getRight()!=nullptr)
        nodeSwap(curr,pred);
    AVLNode<Key, Value>* p = curr->getParent();
    if(p != nullptr)
    {
        if(p->getLeft() == curr){
            diff = 1;
        }
        else if(p->getRight() == curr){
            diff = -1;
        }
    }

    
    //now setting pointer
    if(curr->getLeft()!=nullptr && curr->getRight()!=nullptr){
            //Node<Key, Value> *pred = predecessor(curr);
            //since right most child, could possibly only have left child
            if(pred->getLeft() == nullptr){//if no more left child
                // nodeSwap(pred,curr);
                if(curr->getParent()->getLeft()==curr)curr->getParent()->setLeft(nullptr);
                else if(curr->getParent()->getRight()==curr)curr->getParent()->setRight(nullptr);
                // delete curr;
            }
            else{//if left child exists on the predecessor
                // nodeSwap(pred,curr);
                if(curr->getParent()->getLeft()==curr){
                    if(curr->getLeft()!=nullptr){
                        curr->getParent()->setLeft(curr->getLeft());
                        curr->getLeft()->setParent(curr->getParent());
                    } 
                    else curr->getParent()->setLeft(nullptr);
                    
                }
                else if(curr->getParent()->getRight()==curr){
                    if(curr->getLeft()!=nullptr)
                    {
                        curr->getParent()->setRight(curr->getLeft());
                        curr->getLeft()->setParent(curr->getParent());
                    } 
                    else {
                        curr->getParent()->setRight(nullptr);
                        
                    }
                }
                delete curr;
            }
        }
        //following 2 cases are if it has 1 child
        else if(curr->getLeft()!=nullptr){//child on current's left
            curr->getLeft()->setParent(curr->getParent());
            if(curr->getParent()==nullptr){
                this->root_ = curr->getLeft();
            }
            else{
                if(curr->getParent()->getRight()==curr) curr->getParent()->setRight(curr->getLeft());
            else if(curr->getParent()->getLeft()==curr) curr->getParent()->setLeft(curr->getLeft());
            }
            
            delete curr;
        }
        else if(curr->getRight()!= nullptr){//child on current's right
            curr->getRight()->setParent(curr->getParent());
            if(curr->getParent()==nullptr){
                this->root_ = curr->getRight();
            }
            else{
                if(curr->getParent()->getRight()==curr) curr->getParent()->setRight(curr->getRight());
                else if (curr->getParent()->getLeft()==curr) curr->getParent()->setLeft(curr->getRight());
            }
            delete curr;
            
        }
        else{
            if(curr->getParent() == nullptr){
                BinarySearchTree<Key,Value>::clear();
            }
            else{
                if(curr->getParent()->getLeft()==curr) curr->getParent()->setLeft(nullptr);
                else if(curr->getParent()->getRight()==curr) curr->getParent()->setRight(nullptr);
                delete curr;
            }
        }

    removeFix(p, diff);
   
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>*n,int8_t diff)
{
    if(n == nullptr)
        return;
    AVLNode<Key, Value>* p = n->getParent();
    int8_t nDiff;
    if(p != nullptr){
        if(p->getLeft() == n){
            nDiff = 1;
        }
        else if(p->getRight() == n){
            nDiff = -1;
        }
    }

    if(diff == -1){//left case
        if(n->getBalance() + diff == -2){
            AVLNode<Key, Value>* c = n->getLeft();
            if(c->getBalance()==-1){
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, nDiff);
            }
            else if (c->getBalance() == 0){
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
                return;
            }
            else if(c->getBalance() == 1){
                AVLNode<Key, Value>* g = c->getRight();
                rotateLeft(c);
                rotateRight(n);
                if(g->getBalance() == -1){
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if(g->getBalance() == 0){
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if(g->getBalance() == 1){
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                removeFix(p, nDiff);
            }
        }
        else if(n->getBalance() + diff == -1){
            n->setBalance(-1);
            return;
        }
        else if(n->getBalance() + diff == 0){
            n->setBalance(0);
            removeFix(p, nDiff);
        }

    }
    else if(diff == 1)//right case
    {
        if(n->getBalance() + diff == 2){
            AVLNode<Key, Value>* c = n->getRight();
            if(c->getBalance()==1){
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, nDiff);
            }
            else if (c->getBalance() == 0){
                rotateLeft(n);
                n->setBalance(1);
                c->setBalance(-1);
                return;
            }
            else if(c->getBalance() == -1){
                AVLNode<Key, Value>* g = c->getLeft();
                rotateRight(c);
                rotateLeft(n);
                if(g->getBalance() == -1){
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                }
                else if(g->getBalance() == 0){
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if(g->getBalance() == 1){
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, nDiff);
            }
        }
        else if(n->getBalance() + diff == 1){
            n->setBalance(1);
            return;
        }
        else if(n->getBalance() + diff == 0){
            n->setBalance(0);
            removeFix(p, nDiff);
        }
    }
    
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
