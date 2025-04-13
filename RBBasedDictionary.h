#pragma once

#include <iostream>
#include <utility>

class ElementNotFoundException
{
public:
    ElementNotFoundException() {};
private:
    std::string message;
};

enum Color { RED, BLACK, DOUBLE_BLACK };

template <typename TKey, typename TValue>
struct Entry
{
    TKey key;
    TValue value;
    int color;
    Entry* left, * right, * parent;

    explicit Entry(TKey key, TValue value)
    {
        this->key = key;
        this->value = value;
        color = RED;
        left = right = parent = nullptr;
    };
};

template <typename T, typename V>
class RBBasedDicitonary
{
private:
    Entry<T, V>* root;
protected:

    void rotateLeft(Entry<T, V> *&ptr)
    {
        Entry<T, V>* right_child = ptr->right;
        ptr->right = right_child->left;

        if (ptr->right != nullptr)
            ptr->right->parent = ptr;

        right_child->parent = ptr->parent;

        if (ptr->parent == nullptr)
            root = right_child;
        else if (ptr == ptr->parent->left)
            ptr->parent->left = right_child;
        else
            ptr->parent->right = right_child;

        right_child->left = ptr;
        ptr->parent = right_child;
    };

    void rotateRight(Entry<T, V> *&ptr)
    {
        Entry<T, V>* left_child = ptr->left;
        ptr->left = left_child->right;

        if (ptr->left != nullptr)
            ptr->left->parent = ptr;

        left_child->parent = ptr->parent;

        if (ptr->parent == nullptr)
            root = left_child;
        else if (ptr == ptr->parent->left)
            ptr->parent->left = left_child;
        else
            ptr->parent->right = left_child;

        left_child->right = ptr;
        ptr->parent = left_child;
    };

    void fixInsertRBTree(Entry<T, V> *&ptr)
    {
        Entry<T, V>* parent = nullptr;
        Entry<T, V>* grandparent = nullptr;
        while (ptr != root && getColor(ptr) == RED && getColor(ptr->parent) == RED) {
            parent = ptr->parent;
            grandparent = parent->parent;
            if (parent == grandparent->left) {
                Entry<T>* uncle = grandparent->right;
                if (getColor(uncle) == RED) {
                    setColor(uncle, BLACK);
                    setColor(parent, BLACK);
                    setColor(grandparent, RED);
                    ptr = grandparent;
                }
                else {
                    if (ptr == parent->right) {
                        rotateLeft(parent);
                        ptr = parent;
                        parent = ptr->parent;
                    }
                    rotateRight(grandparent);
                    std::swap(parent->color, grandparent->color);
                    ptr = parent;
                }
            }
            else {
                Entry<T>* uncle = grandparent->left;
                if (getColor(uncle) == RED) {
                    setColor(uncle, BLACK);
                    setColor(parent, BLACK);
                    setColor(grandparent, RED);
                    ptr = grandparent;
                }
                else {
                    if (ptr == parent->left) {
                        rotateRight(parent);
                        ptr = parent;
                        parent = ptr->parent;
                    }
                    rotateLeft(grandparent);
                    std::swap(parent->color, grandparent->color);
                    ptr = parent;
                }
            }
        }
        setColor(root, BLACK);
    };
    void fixDeleteRBTree(Entry<T, V>*& node)
    {
        if (node == nullptr)
            return;

        if (node == root) {
            root = nullptr;
            return;
        }

        if (getColor(node) == RED || getColor(node->left) == RED || getColor(node->right) == RED) {
            Entry<T, V>* child = node->left != nullptr ? node->left : node->right;

            if (node == node->parent->left) {
                node->parent->left = child;
                if (child != nullptr)
                    child->parent = node->parent;
                setColor(child, BLACK);
                delete (node);
            }
            else {
                node->parent->right = child;
                if (child != nullptr)
                    child->parent = node->parent;
                setColor(child, BLACK);
                delete (node);
            }
        }
        else {
            Entry<T, V>* sibling = nullptr;
            Entry<T, V>* parent = nullptr;
            Entry<T, V>* ptr = node;
            setColor(ptr, DOUBLE_BLACK);
            while (ptr != root && getColor(ptr) == DOUBLE_BLACK) {
                parent = ptr->parent;
                if (ptr == parent->left) {
                    sibling = parent->right;
                    if (getColor(sibling) == RED) {
                        setColor(sibling, BLACK);
                        setColor(parent, RED);
                        rotateLeft(parent);
                    }
                    else {
                        if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {
                            setColor(sibling, RED);
                            if (getColor(parent) == RED)
                                setColor(parent, BLACK);
                            else
                                setColor(parent, DOUBLE_BLACK);
                            ptr = parent;
                        }
                        else {
                            if (getColor(sibling->right) == BLACK) {
                                setColor(sibling->left, BLACK);
                                setColor(sibling, RED);
                                rotateRight(sibling);
                                sibling = parent->right;
                            }
                            setColor(sibling, parent->color);
                            setColor(parent, BLACK);
                            setColor(sibling->right, BLACK);
                            rotateLeft(parent);
                            break;
                        }
                    }
                }
                else {
                    sibling = parent->left;
                    if (getColor(sibling) == RED) {
                        setColor(sibling, BLACK);
                        setColor(parent, RED);
                        rotateRight(parent);
                    }
                    else {
                        if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {
                            setColor(sibling, RED);
                            if (getColor(parent) == RED)
                                setColor(parent, BLACK);
                            else
                                setColor(parent, DOUBLE_BLACK);
                            ptr = parent;
                        }
                        else {
                            if (getColor(sibling->left) == BLACK) {
                                setColor(sibling->right, BLACK);
                                setColor(sibling, RED);
                                rotateLeft(sibling);
                                sibling = parent->left;
                            }
                            setColor(sibling, parent->color);
                            setColor(parent, BLACK);
                            setColor(sibling->left, BLACK);
                            rotateRight(parent);
                            break;
                        }
                    }
                }
            }
            if (node == node->parent->left)
                node->parent->left = nullptr;
            else
                node->parent->right = nullptr;
            delete(node);
            setColor(root, BLACK);
        }
    };

    void inorderBST(Entry<T, V>*& ptr, int level)
    {
        if (ptr == nullptr)
            return;

        inorderBST(ptr->left, level+1);
        std::cout << ptr->key << " " << ptr->color << " level:" << level << std::endl;
        inorderBST(ptr->right, level+1);
    };

    void preorderBST(Entry<T, V>*& ptr)
    {
        if (ptr == nullptr)
            return;

        std::cout << "Key: " << ptr->key << " value: " << ptr->value << " " << ptr->color << std::endl;
        preorderBST(ptr->left);
        preorderBST(ptr->right);
    };

    int getColor(Entry<T, V>*& node)
    {
        if (node == nullptr)
            return BLACK;

        return node->color;
    };

    void setColor(Entry<T, V>*& node, int color)
    {
        if (node == nullptr)
            return;

        node->color = color;
    };

    Entry<T, V>* minValueNode(Entry<T, V>*& node)
    {
        Entry<T>* ptr = node;

        while (ptr->left != nullptr)
            ptr = ptr->left;

        return ptr;
    };

    Entry<T, V>* maxValueNode(Entry<T, V>*& node)
    {
        Entry<T, V>* ptr = node;

        while (ptr->right != nullptr)
            ptr = ptr->right;

        return ptr;
    };

    Entry<T, V>* insertBST(Entry<T, V>*& root, Entry<T, V>*& ptr)
    {
        if (root == nullptr)
            return ptr;

        if (ptr->key < root->key) {
            root->left = insertBST(root->left, ptr);
            root->left->parent = root;
        }
        else if (ptr->key > root->key) {
            root->right = insertBST(root->right, ptr);
            root->right->parent = root;
        }
        else
        {
            return ptr;
        }


        return root;
    };
    
    Entry<T, V>* deleteBST(Entry<T, V>*& root, T key)
    {
        if(root == nullptr)
            return root;

        if (key < root->key)
            return deleteBST(root->left, key);

        if (key > root->key)
            return deleteBST(root->right, key);

        if (root->left == nullptr || root->right == nullptr)
            return root;

        Entry<T, V>* temp = minValueNode(root->right);
        root->key = temp->key;
        return deleteBST(root->right, temp->key);
    };
    
    V findElement(Entry<T, V>*& root, T key) {
        if (root == nullptr) {
           
            return V();
        }

        if (key < root->key) 
        {
            return findElement(root->left, key);
        }
        else if (key > root->key) 
        {
            return findElement(root->right, key);
        }
        else 
        {
            return root->value;
        }
    }


    int getBlackHeight(Entry<T, V>* entry)
    {
        int blackheight = 0;
        while (entry != nullptr) {
            if (getColor(entry) == BLACK)
                blackheight++;
            entry = entry->left;
        }
        return blackheight;
    };

public:

    RedBlackTree()
    {
        root = nullptr;
    };
    
    void insertValue(T key, V value)
    {
        Entry<T, V>* node = new Entry<T, V>(key, value);
        root = insertBST(root, node);
        fixInsertRBTree(node);
    };

    void deleteValue(T key)
    {
        Entry<T, V>* node = deleteBST(root, key);
        fixDeleteRBTree(node);
    };

    void inorder()
    {
        inorderBST(root, 0);
    };

    void preorder()
    {
        preorderBST(root);
        std::cout << "-------" << std::endl;
    };
};