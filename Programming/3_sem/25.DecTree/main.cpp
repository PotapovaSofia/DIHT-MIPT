#include <iostream>
#include <vector>
#define NIL NULL

using namespace std;

//template <typename K = int, typename P = int, /*typename Compare = greater<P>, */typename V = int>
class Treap {
public:
    struct Node {
    public:
        Node* left_child;
        Node* right_child;
        Node* parent;
        int key;
        int priority;
        int value;
        Node(int k, int p, int v = 0): key(k), priority(p), value(v), left_child(NIL), right_child(NIL), parent(NIL) {}
    };

    //Compare comp;
    Node* root = NIL;
    //static Node* NIL;
public:
    Treap(vector<int> keys, vector<int> priorities) {
        root = new Node(keys[0], priorities[0]);
        Node* last_added = root;
        root->parent = root; ////////////////////////////////////////////////////////////////////
        Node* current;
        for (int i = 1; i < keys.size(); i++) {
            current = new Node(keys[i], priorities[i]);
            if (last_added->priority > priorities[i]) {
                current->parent = last_added;
                last_added->right_child = current;
            } else {
                Node* x = last_added->parent;
                while (x != root && x->priority < current->priority) {
                    x = x->parent;
                }
                if (x != root) {
                    current->left_child = x->right_child;
                    x->right_child->parent = current;
                    current->parent = x;
                    x->right_child = current;
                } else {
                    if (root->priority > priorities[i]) {
                        current->left_child = root->right_child;
                        root->right_child->parent = current;
                        current->parent = root;
                        root->right_child = current;
                    } else {
                        current->left_child = root;
                        root -> parent = current;
                        root = current;
                        root->parent = root;
                    }
                }
            }
            last_added = current;
        }
    }

    Node* insert(int key, int priority, int value = 0) {
        Node* new_node = new Node(key, priority, value);
        Node* t1, *t2;
        split(root, key, t1, t2);
        root = merge(t1, new_node);
        root = merge(root, t2);
        return new_node;
    }

    void remove(int key) {
        Node* t1, *t2, *t3;
        split(root, key, t1, t2);
        split(t2, key, t2, t3);
        root = merge(t1, t3);
    }

    void split(Node* start, int key, Node* &a, Node* &b) {
        if (start == NIL) {
            a = NIL;
            b = NIL;
        } else {
            if (key >= start->key) {
                split(start->right_child, key, start->right_child, b);
                a = start;
            } else {
                split(start->left_child, key, a, start->left_child);
                b = start;
            }
        }
    }

    Node* merge(Node* a, Node* b) {
        if (a == NIL || b == NIL) {
            return (a != NIL) ? a : b;
        }
        if (a->priority > b->priority) {
            a->right_child = merge(a->right_child, b);
            return a;
        } else {
            b->left_child = merge(a, b->left_child);
            return b;
        }
    }

    Node* find(int key) {
        Node* result = root;
        while (result != NIL && result->key != key) {
            if (result->key > key) {
                result = result->left_child;
            } else {
                result = result->right_child;
            }
        }
        return result;
    }

    void sort_by_key(Node* start) {
        if (start->left_child != NIL) {
            sort_by_key(start->left_child);
        }
        cout << start->key << ' ' << start->priority << ' ' << start->value << endl;
        if (start->right_child != NIL) {
            sort_by_key(start->right_child);
        }
    }
};

int main() {
    vector<int> keys = {1, 5, 8, -3, 9, 18, 4, 2, 10, -22};
    vector<int> priorities;
    for (int i = 0; i < 10; i++) {
        int a = rand()  % 100;
    }
    Treap treap(keys, priorities);
    treap.sort_by_key(treap.root);
    return 0;
}