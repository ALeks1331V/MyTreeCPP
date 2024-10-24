#include <iostream>
#include <thread>
#include <mutex>
#pragma once
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    Node* root;
    mutex mtx;

public:
    BinarySearchTree() : root(nullptr) {}

    void insert(int key) {
        lock_guard<mutex> lock(mtx);
        if (root == nullptr) {
            root = new Node(key);
        }
        else {
            insertRecursive(root, key);
        }
    }

    void printTree() {
        lock_guard<mutex> lock(mtx);
        printTreeRecursive(root);
    }

    void remove(int key) {
        lock_guard<mutex> lock(mtx);
        root = removeRecursive(root, key);
    }

    void removeSubtree(Node* subtreeRoot) {
        lock_guard<mutex> lock(mtx);
        removeSubtreeRecursive(root, subtreeRoot);
    }

    void clearMethod() {
        lock_guard<mutex> lock(mtx);
        clearRecursive(root);
        root = nullptr;
    }

    void search(int value) {
        lock_guard<mutex> lock(mtx);
        Node* temp = SearchRecursive(root, value);
        cout << temp->key << "\n";
    }

private:
    void insertRecursive(Node* currentNode, int key) {
        if (key < currentNode->key) {
            if (currentNode->left == nullptr) {
                currentNode->left = new Node(key);
            }
            else {
                insertRecursive(currentNode->left, key);
            }
        }
        else if (key > currentNode->key) {
            if (currentNode->right == nullptr) {
                currentNode->right = new Node(key);
            }
            else {
                insertRecursive(currentNode->right, key);
            }
        }
    }

    Node* removeRecursive(Node* currentNode, int key) {
        if (currentNode == nullptr) {
            return currentNode;
        }

        if (key < currentNode->key) {
            currentNode->left = removeRecursive(currentNode->left, key);
        }
        else if (key > currentNode->key) {
            currentNode->right = removeRecursive(currentNode->right, key);
        }
        else {
            if (currentNode->left == nullptr) {
                Node* temp = currentNode->right;
                delete currentNode;
                return temp;
            }
            else if (currentNode->right == nullptr) {
                Node* temp = currentNode->left;
                delete currentNode;
                return temp;
            }

            Node* temp = minValueNode(currentNode->right);
            currentNode->key = temp->key;
            currentNode->right = removeRecursive(currentNode->right, temp->key);
        }
        return currentNode;
    }

    Node* SearchRecursive(Node* current, int value) {

        if (root == nullptr || root->key == value) {
            return current;
        }

        if (value < current->key) {
            return SearchRecursive(current->left, value); //рекурсивный вызов ф-ии
        }

        return SearchRecursive(current->right, value);
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    void printTreeRecursive(Node* currentNode) {
        if (currentNode != nullptr) {
            printTreeRecursive(currentNode->left);
            cout << currentNode->key << " ";
            printTreeRecursive(currentNode->right);
        }
    }

    void removeSubtreeRecursive(Node* currentNode, Node* subtreeRoot) {
        if (subtreeRoot != nullptr) {
            remove(subtreeRoot->key);
            removeSubtreeRecursive(currentNode, subtreeRoot->left);
            removeSubtreeRecursive(currentNode, subtreeRoot->right);
        }
    }

    void clearRecursive(Node* node) {
        if (node != nullptr) {
            clearRecursive(node->left);
            clearRecursive(node->right);
            delete node;
        }
    }
};

int main() {
    BinarySearchTree bst;
    setlocale(LC_ALL, "Russian");

    thread t1([&bst]() {
        bst.insert(5);
        bst.insert(3);
        bst.insert(7);
        });

    thread t2([&bst]() {
        bst.insert(4);
        bst.insert(6);
        bst.insert(8);
        });

    t1.join();
    t2.join();

    cout << "Изначальное дерево\n";
    bst.printTree();

    bst.remove(3);
    cout << "\nДерево после удаления элемпента\n";
    bst.printTree();

    cout << "\nРеализация метода поиска элемента по ключу: ";
    bst.search(4);

    bst.clearMethod();
    cout << "\nДерево после удаления:\n";
    bst.printTree();
};