#include <iostream>
#include <vector>

using namespace std;

struct Node {
public:
    Node* left_child;
    Node* right_child;
    Node* parent;
    int key;
    int priority;
    Node(int k, int p): key(k), priority(p), left_child(NULL), right_child(NULL), parent(NULL) {}
};

void split(Node* root, int key, Node* &a, Node* &b) {
    if (root == NULL) {
        a = NULL;
        b = NULL;
    } else {
        if (key >= root->key) {
            split(root->right_child, key, root->right_child, b);
            a = root;
        } else {
            split(root->left_child, key, a, root->left_child);
            b = root;
        }
    }
}

Node* merge(Node* a, Node* b) {
    if (a == NULL || b == NULL) {
        return (a != NULL) ? a : b;
    }
    if (a->priority > b->priority) {
        a->right_child = merge(a->right_child, b);
        return a;
    } else {
        b->left_child = merge(a, b->left_child);
        return b;
    }
}

void insert(Node* &root, int key, int priority) {
    Node* new_node = new Node(key, priority);
    Node* t1, *t2;
    split(root, key, t1, t2);
    root = merge(t1, new_node);
    root = merge(root, t2);
}

void sort_by_key(Node* start) {
    if (start == NULL) {
        return;
    }
    if (start->left_child != NULL) {
        sort_by_key(start->left_child);
    }
    cout << start->key << ' ';
    if (start->right_child != NULL) {
        sort_by_key(start->right_child);
    }
}

int main() {
    vector<int> keys = {1, 5, 8, 3, 9, 18, 4, 2, 10, 22};
    vector<int> priorities;
    for (int i = 0; i < 10; i++) {
        priorities.push_back(rand()  % 100);
    }
    Node* treap = NULL;
    for (int i = 0; i < 10; i++) {
        insert(treap, keys[i], priorities[i]);
    }
    sort_by_key(treap);
    cout << endl;
    return 0;
}