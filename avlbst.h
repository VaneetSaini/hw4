#ifndef RBBST_H
#define RBBST_H

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
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
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
    virtual void LEFT_ROTATE(AVLNode<Key, Value>* node);
    virtual void RIGHT_ROTATE(AVLNode<Key, Value>* node);
    virtual void INSERT_NODE(AVLNode<Key, Value>* p, AVLNode<Key, Value>* node);
    virtual void REMOVE_NODE(AVLNode<Key, Value>* node, int var);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::INSERT_NODE(AVLNode<Key, Value>* p, AVLNode<Key, Value>* node)
{
    if (p == nullptr) return;
    if (p->getParent() == nullptr) return;

    AVLNode<Key, Value>* grand_parent = p->getParent();
    if (grand_parent->getLeft() == p)
    {
        grand_parent->updateBalance(-1);
        if (grand_parent->getBalance() == 0)
        {
            return;
        }
        else if (grand_parent->getBalance() == -1)
        {
            INSERT_NODE(grand_parent, p);
        }
        else if (grand_parent->getBalance() == -2)
        {
            if (p->getLeft() == node)
            {
                RIGHT_ROTATE(grand_parent);
                p->setBalance(0);
                grand_parent->setBalance(0);
                return;
            }
            else if (p->getLeft() == nullptr && p->getRight() == node)
            {
                LEFT_ROTATE(p);
                RIGHT_ROTATE(grand_parent);
                if (node->getBalance() == -1)
                {
                    p->setBalance(0);
                    grand_parent->setBalance(1);
                    node->setBalance(0);
                }
                else if (node->getBalance() == 0)
                {
                    p->setBalance(0);
                    grand_parent->setBalance(0);
                    node->setBalance(0);
                }
                else if (node->getBalance() == 1)
                {
                    p->setBalance(-1);
                    grand_parent->setBalance(0);
                    node->setBalance(0);
                }
                return;
            }
        }
    }
    else if (grand_parent->getRight() == p)
    {
        grand_parent->updateBalance(1);
        if (grand_parent->getBalance() == 0)
        {
            return;
        }
        else if (grand_parent->getBalance() == 1)
        {
            INSERT_NODE(grand_parent, p);
        }
        else if (grand_parent->getBalance() == 2)
        {
            if (p->getRight() == nullptr && p->getLeft() == node)
            {
                RIGHT_ROTATE(p);
                LEFT_ROTATE(grand_parent);
                if (node->getBalance() == 1)
                {
                    p->setBalance(0);
                    grand_parent->setBalance(-1);
                    node->setBalance(0);
                }
                else if (node->getBalance() == 0)
                {
                   p->setBalance(0);
                    grand_parent->setBalance(0);
                    node->setBalance(0); 
                }
                else if (node->getBalance() == -1)
                {
                    p->setBalance(1);
                    grand_parent->setBalance(0);
                    node->setBalance(0);
                }
            }
        }
    }

}


template<class Key, class Value>
void AVLTree<Key, Value>::REMOVE_NODE(AVLNode<Key, Value>* node, int var)
{
    AVLNode<Key, Value>* p = node->getParent();
    if (node == nullptr) return;
    if (p == nullptr) return;

    int diff = 0;
    if (p->getLeft() == node)
    {
        diff++;
    }
    else if (p->getRight() == node)
    {
        diff--;
    }
    if (var == -1)
    {
        if (node->getBalance() + var == -2)
        {
            AVLNode<Key, Value>* left_child = node->getLeft();
            if (left_child->getBalance() == -1)
            {
                RIGHT_ROTATE(node);
                node->setBalance(0);
                left_child->setBalance(0);
            }
            else if (left_child->getBalance() == 0)
            {
                RIGHT_ROTATE(node);
                node->setBalance(-1);
                left_child->setBalance(1);
            }
            else if (left_child->getBalance() == 1)
            {
                AVLNode<Key, Value>* grand_child = left_child->getRight();
                LEFT_ROTATE(left_child);
                RIGHT_ROTATE(node);
                if (grand_child->getBalance() == 1)
                {
                    node->setBalance(0);
                    left_child->setBalance(-1);
                    grand_child->setBalance(0);
                }
                else if (grand_child->getBalance() == 0)
                {
                    node->setBalance(0);
                    left_child->setBalance(0);
                    grand_child->setBalance(0);
                }
                else if (grand_child->getBalance() == -1)
                {
                    node->setBalance(1);
                    left_child->setBalance(0);
                    grand_child->setBalance(0);
                }
                REMOVE_NODE(p, diff);
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::LEFT_ROTATE(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* p = node->getParent();
    AVLNode<Key, Value>* right_child = node->getRight();
    if (p != nullptr)
    {
        if (p->getRight() == node)
        {
            p->setRight(right_child);
        }
        else p->setLeft(right_child);
    }
    else this->root_ = right_child;
    right_child->setParent(p);
    node->setParent(right_child);
    AVLNode<Key, Value>* left_child = right_child->getLeft();
    if (left_child != nullptr) left_child->setParent(node);
    node->setRight(left_child);
    right_child->setLeft(node);
}

template<class Key, class Value>
void AVLTree<Key, Value>::RIGHT_ROTATE(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* p = node->getParent();
    AVLNode<Key, Value>* left_child = node->getLeft();
    if (p != nullptr)
    {
        if (p->getLeft() == node)
        {
            p->setLeft(left_child);
        }
        else p->setRight(left_child);
    }
    else this->root_ = left_child;
    left_child->setParent(p);
    node->setParent(left_child);
    AVLNode<Key, Value>* right_child = left_child->getRight();
    if (right_child != nullptr) right_child->setParent(node);
    node->setLeft(right_child);
    left_child->setRight(node);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* node = new AVLNode<Key, Value>*;
    if (node == nullptr)
    {
        node = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        this->root_ = node;
    }    
    else
    {
        while (node != nullptr)
        {
            if (node->getKey() < new_item.first)
            {
                if (node->getRight() == nullptr)
                {
                    AVLNode<Key, Value> *p = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
                    node->setRight(p);
                    if (node->getBalance() == -1)
                    {
                        node->setBalance(0);
                    }
                    else if (node->getBalance() == 0)
                    {
                        node->setBalance(1); INSERT_NODE(node, p);

                    }
                    break;
                }
                else{
                    node = node->getRight();
                }
            }
            else if (node->getKey() > new_item.first)
            {
                if (node->getLeft() == nullptr)
                {
                    AVLNode<Key, Value> *p = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
                    node->setLeft(p);
                    if (node->getBalance() == 1)
                    {
                        node->setBalance(0);
                    }
                    else if (node->getBalance() == 0)
                    {
                        node->setBalance(-1); INSERT_NODE(node, p);

                    }
                    break;
                }
                else{
                    node = node->getLeft();
                }
            }
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
