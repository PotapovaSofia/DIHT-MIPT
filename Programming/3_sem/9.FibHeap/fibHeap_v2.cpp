#include <iostream>
#include <set>
#include <vector>
#include <memory>

using namespace std;

template < typename K, typename V, typename Compare = less<K> >
class FibHeap {
    struct Node {
        Node* parent = FibHeap::NIL;
        Node* child = FibHeap::NIL;
        Node* left;
        Node* right;
        int degree = 0;
        bool mark = false;
        K key;
        V value;
        Node(K key, V value): key(key), value(value){;}
    };

    set < Node* > nodes;
    Node* min;
    int number_of_nodes = 0;
    static Node* NIL;
    Compare comp;

public:
    FibHeap() {
        min = NIL;
    }
    ~FibHeap() {}

private:
    Node* createNode(K key, V value) {
        typename set< Node* >::iterator itr;
        Node* n = new Node(key, value);
        itr = nodes.insert(n).first;
        (*itr)->left = (*itr);
        (*itr)->right = (*itr);
        return (*itr);
    }

    void insertToFibHeap(Node* node) {
        if (min == NIL || comp(node->key, min->key)) {
            min = node;
        }
        ++number_of_nodes;
    }

    void unionFibHeapToThis(FibHeap && other_heap) {
        if (this->min != NIL && other_heap.min != NIL) {
            Node* left_of_this_min = this->min->left;
            Node* right_of_other_min = other_heap.min->right;
            this->min->left = other_heap.min;
            other_heap.min->right = this->min;
            left_of_this_min->right = right_of_other_min;
            right_of_other_min->left = left_of_this_min;
        }
        if (this->min == NIL || (other_heap.min !=NIL && comp(other_heap.min->key,this->min->key))) {
            this->min = other_heap.min;
        }
        this->nodes.insert(other_heap.nodes.cbegin(), other_heap.nodes.cend());
        this->number_of_nodes += other_heap.number_of_nodes;
    }

    void consolidate() {
        vector< Node* > vector_of_roots(number_of_nodes);
        for (int i = 0; i != this->number_of_nodes; ++ i) {
            vector_of_roots.push_back(NIL);
        }
        Node* one_of_root = this->min;
        do {
            Node* x = one_of_root;
            int d = x->degree;
            while (vector_of_roots[d] != NIL) {
                Node* y = vector_of_roots[d];
                if (comp(x->key, y->key)) {
                    Node* buf = x;
                    x = y;
                    y = buf;
                }
                link(y, x);
                vector_of_roots[d] = NIL;
                ++d;
            }
            vector_of_roots[d] = x;
            one_of_root = one_of_root->right;
        } while (one_of_root != this->min);
        this->min = NIL;
        for (int i = 0; i < this->number_of_nodes; ++i) {
            if (vector_of_roots[i] != NIL) {
                vector_of_roots[i]->parent = NIL;
                this->min->left->right = vector_of_roots[i];
                vector_of_roots[i]->left = this->min->left;
                this->min->left = vector_of_roots[i];
                vector_of_roots[i]->right = this->min;
                if (this->min == NIL || comp(vector_of_roots[i]->key, this->min->key)) {
                    this->min = vector_of_roots[i];
                }
            }
        }
    }

    void link(Node* y, Node* x) {
        y->left->right = y->right;
        y->right->left = y->left;
        y->right = x->child;
        y->left = x->child->left;
        x->child->left->right = y;
        x->child->left = y;
        ++x->degree;
        y->parent = x;
        y->mark = false;
    }

public:
    bool isEmpty() {
    	if (min != NIL) {
    		return false;
        } else {
            return true;
        }
    }

    Node* fibHeapInsert(K key, V value) {
        Node* node = this->createNode(key, value);
        this->insertToFibHeap(node);
        return node;
    }

    Node* fibHeapExtractMin() {
        Node* z = this->min;
        if (z != NIL) {
            Node* child = z->child;
            if (child != NIL) {
                Node* next = child->right;
                do {
                    Node* min_left = this->min->left;
                    next->right->left = next->left;
                    next->left->right = next->right;
                    next->parent = NIL;
                    this->min->left = next;
                    next->right = this->min;
                    min_left->right = next;
                    next->left = min_left;
                } while (next != child);
            }
            z->left->right = z->right;
            z->right->left = z->left;
            if (z == z->right) {
                this->min = NIL;
            } else {
                this->min = z->right;
                consolidate();
            }
            --number_of_nodes;
        }
        return z;
    }
};

template <typename K, typename V, typename Compare>
typename FibHeap<K, V, Compare>::Node* FibHeap<K, V, Compare>::NIL = nullptr;

int main() {
    FibHeap<int, int> a;
    FibHeap<int, int> b;
    a.fibHeapInsert(1, 2);
    a.fibHeapInsert(2, 3);
    a.fibHeapInsert(-2, 3);
    b.fibHeapInsert(3, 4);
    cout << a.fibHeapExtractMin()->key;
    return 0;
}
