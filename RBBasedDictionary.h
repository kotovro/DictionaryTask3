#pragma once

#include <iostream>
#include <utility>
#include <type_traits>
#include <vector>


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
class RBBasedDictionary
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
            if (parent == grandparent->left) 
            {
                Entry<T, V>* uncle = grandparent->right;
                if (getColor(uncle) == RED) 
                {
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
            else 
            {
                Entry<T, V>* uncle = grandparent->left;
                if (getColor(uncle) == RED) 
                {
                    setColor(uncle, BLACK);
                    setColor(parent, BLACK);
                    setColor(grandparent, RED);
                    ptr = grandparent;
                }
                else 
                {
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

    void fixDeleteRBTree(Entry<T, V>* node) 
    {
        if (node == nullptr || node == root) 
            return;

        Entry<T, V>* sibling = nullptr;
        Entry<T, V>* parent = node->parent;

        while (node != root && getColor(node) == DOUBLE_BLACK) 
        {
            if (node == parent->left) 
            {
                sibling = parent->right;

                // Case 1: Sibling is RED (convert to cases 2/3/4)
                if (getColor(sibling) == RED) 
                {
                    setColor(sibling, BLACK);
                    setColor(parent, RED);
                    rotateLeft(parent);
                    sibling = parent->right;  // New sibling after rotation
                }

                // Case 2: Sibling and both its children are BLACK
                if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) 
                {
                    setColor(sibling, RED);
                    setColor(node, BLACK);  // Resolve double-black
                    if (getColor(parent) == RED)
                        setColor(parent, BLACK);
                    else
                        setColor(parent, DOUBLE_BLACK);
                    node = parent;
                    parent = node->parent;
                }
                else 
                {
                    // Case 3: Sibling's left child is RED, right is BLACK
                    if (getColor(sibling->right) == BLACK) {
                        setColor(sibling->left, BLACK);
                        setColor(sibling, RED);
                        rotateRight(sibling);
                        sibling = parent->right;
                    }

                    // Case 4: Sibling's right child is RED
                    setColor(sibling, getColor(parent));
                    setColor(parent, BLACK);
                    setColor(sibling->right, BLACK);
                    rotateLeft(parent);
                    break;
                }
            }
            else 
            {  // Symmetric cases (node is right child)
                sibling = parent->left;

                // Case 1: Sibling is RED
                if (getColor(sibling) == RED) 
                {
                    setColor(sibling, BLACK);
                    setColor(parent, RED);
                    rotateRight(parent);
                    sibling = parent->left;
                }

                // Case 2: Sibling and both children are BLACK
                if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) 
                {
                    setColor(sibling, RED);
                    setColor(node, BLACK);
                    if (getColor(parent) == RED)
                        setColor(parent, BLACK);
                    else
                        setColor(parent, DOUBLE_BLACK);
                    node = parent;
                    parent = node->parent;
                }
                else 
                {
                    // Case 3: Sibling's right child is RED, left is BLACK
                    if (getColor(sibling->left) == BLACK) {
                        setColor(sibling->right, BLACK);
                        setColor(sibling, RED);
                        rotateLeft(sibling);
                        sibling = parent->left;
                    }

                    // Case 4: Sibling's left child is RED
                    setColor(sibling, getColor(parent));
                    setColor(parent, BLACK);
                    setColor(sibling->left, BLACK);
                    rotateRight(parent);
                    break;
                }
            }
        }

        // Ensure root is always BLACK
        setColor(root, BLACK);
    }

    void inorderBST(Entry<T, V>*& ptr, int level, std::vector<std::pair<T, V>>& keyValuePairs)
    {
        if (ptr == nullptr)
            return;

        // Traverse left subtree
        inorderBST(ptr->left, level + 1, keyValuePairs);

        // Store the key-value pair in the vector
        keyValuePairs.push_back(std::make_pair(ptr->key, ptr->value));

        // Output the current node details (for debugging or logging)
        std::cout << "Key: " << ptr->key << " Value: " << ptr->value << " color: " << ptr->color << " level:" << level << std::endl;

        // Traverse right subtree
        inorderBST(ptr->right, level + 1, keyValuePairs);
    }

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
        Entry<T, V>* ptr = node;

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
        else // if we want to insert entry with existing key, we rewrite value there
        {
            root->value = ptr->value;
            return ptr;
        }


        return root;
    };
    
    Entry<T, V>* deleteBST(Entry<T, V>*& root, T key) {
        if (root == nullptr) throw std::runtime_error("Key not found in tree");

        // 1. Standard BST deletion
        if (key < root->key) {
            return deleteBST(root->left, key);
        }
        else if (key > root->key) {
            return deleteBST(root->right, key);
        }
        else {
            // Node to delete found (root or otherwise)
            Entry<T, V>* nodeToDelete = root;

            // Case 1: Node has 0 or 1 child
            if (root->left == nullptr || root->right == nullptr) {
                Entry<T, V>* temp = (root->left != nullptr) ? root->left : root->right;

                // If root is being deleted, update the root pointer
                if (temp != nullptr) temp->parent = root->parent;

                // Handle root deletion case
                if (root->parent == nullptr) {
                    root = temp;  // New root
                }
                else {
                    if (root == root->parent->left) {
                        root->parent->left = temp;
                    }
                    else {
                        root->parent->right = temp;
                    }
                }

                // Rebalance if node was black
                if (nodeToDelete->color == BLACK) {
                    fixDeleteRBTree(temp);  // Rebalancing function for RBT
                }

                delete nodeToDelete;
                return temp;
            }
            // Case 2: Node has 2 children
            else {
                Entry<T, V>* successor = minValueNode(root->right);
                root->key = successor->key;
                root->value = successor->value;
                return deleteBST(root->right, successor->key);
            }
        }
    }
    
    V findElement(Entry<T, V>*& root, T key) {
        if (root == nullptr) {
           throw std::runtime_error("Key not found in tree");
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

    RBBasedDictionary()
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

    V find(T key)
    {
        return findElement(root, key);
    }

    std::vector<std::pair<T, V>>* inorder()
    {
        std::vector<std::pair<T, V>>* keyValuePairs = new std::vector<std::pair<T, V>>();
        inorderBST(root, 0, *keyValuePairs);
        return keyValuePairs;
    };

    void preorder()
    {
        preorderBST(root);
        std::cout << "-------" << std::endl;
    };
};