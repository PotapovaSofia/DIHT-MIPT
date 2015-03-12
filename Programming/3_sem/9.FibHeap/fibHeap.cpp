#include <iostream>
#include <set>
#include <vector>
#include <memory>

using namespace std;

template <typename V>
bool operator == (weak_ptr<V> ptr, weak_ptr<V> other_ptr){
    return ptr.lock().get() == other_ptr.lock().get();
}

template <typename V>
bool operator == (weak_ptr<V> ptr, shared_ptr<V> other_ptr){
    return ptr.lock().get() == other_ptr.get();
}

template <typename V>
bool operator != (weak_ptr<V> ptr, weak_ptr<V> other_ptr){
    return !(ptr.lock().get() == other_ptr.lock().get());
}

template <typename V>
bool operator != (weak_ptr<V> ptr, shared_ptr<V> other_ptr){
    return !(ptr.lock().get() == other_ptr.get());
}

template < typename K, typename V, typename Compare = less<K> >
class FibHeap {
    struct Node {
        weak_ptr<Node> parent = FibHeap::NIL;
        weak_ptr<Node> child = FibHeap::NIL;
        weak_ptr<Node> left;
        weak_ptr<Node> right;
        int degree = 0;
        bool mark = false;
        K key;
        V value;
        Node(K key, V value): key(key), value(value){;}
        friend ostream& operator << (ostream& ostr, shared_ptr<Node> const & node)  {
            ostr << node->key << " (" << node->value << ") ";
            return ostr;
        }
    };

    set < shared_ptr<Node> > nodes;
    weak_ptr<Node> min;
    int number_of_nodes = 0;
    static shared_ptr<Node> NIL;
    Compare comp;

public:
    FibHeap() {
        min = NIL;
    }
    ~FibHeap() {}

private:
    weak_ptr<Node> createNode(K key, V value) {
        typename set<shared_ptr<Node> >::iterator itr;
        itr = nodes.insert(shared_ptr<Node>(new Node(key, value))).first;
        (*itr)->left = (*itr);
        (*itr)->right = (*itr);
        return (*itr);
    }

    void insertToFibHeap(weak_ptr<Node> node) {
        if (min == NIL || comp(node.lock()->key, min.lock()->key)) {
            min = node;
        }
        ++number_of_nodes;
    }

    void unionFibHeapToThis(FibHeap && other_heap) {
        if (this->min != NIL && other_heap.min != NIL) {
            weak_ptr<Node> left_of_this_min = this->min.lock()->left;
            weak_ptr<Node> right_of_other_min = other_heap.min.lock()->right;
            this->min.lock()->left = other_heap.min;
            other_heap.min.lock()->right = this->min;
            left_of_this_min.lock()->right = right_of_other_min;
            right_of_other_min.lock()->left = left_of_this_min;
        }
        if (this->min == NIL || (other_heap.min !=NIL && comp(other_heap.min.lock()->key,this->min.lock()->key))) {
            this->min = other_heap.min;
        }
        this->nodes.insert(other_heap.nodes.cbegin(), other_heap.nodes.cend());
        this->number_of_nodes += other_heap.number_of_nodes;
    }

    void consolidate() {
        vector< weak_ptr<Node> > vector_of_roots(number_of_nodes);
        for (int i = 0; i != this->number_of_nodes; ++ i) {
            vector_of_roots.push_back(NIL);
        }
        weak_ptr<Node> one_of_root = this->min;
        do {
            weak_ptr<Node> x = one_of_root;
            int d = x.lock()->degree;
            while (vector_of_roots[d] != NIL) {
                weak_ptr<Node> y = vector_of_roots[d];
                if (comp(x.lock()->key, y.lock()->key)) {
                    weak_ptr<Node> buf = x;
                    x = y;
                    y = buf;
                }
                link(y, x);
                vector_of_roots[d] = NIL;
                ++d;
            }
            vector_of_roots[d] = x;
            one_of_root = one_of_root.lock()->right;
        } while (one_of_root != this->min);
        this->min = NIL;
        for (int i = 0; i < this->number_of_nodes; ++i) {
            if (vector_of_roots[i] != NIL) {
                vector_of_roots[i].lock()->parent = NIL;
                this->min.lock()->left.lock()->right = vector_of_roots[i];
                vector_of_roots[i].lock()->left = this->min.lock()->left;
                this->min.lock()->left = vector_of_roots[i];
                vector_of_roots[i].lock()->right = this->min;
                if (this->min == NIL || comp(vector_of_roots[i].lock()->key, this->min.lock()->key)) {
                    this->min = vector_of_roots[i];
                }
            }
        }
    }

    void link(weak_ptr<Node> y, weak_ptr<Node> x) {
        y.lock()->left.lock()->right = y.lock()->right.lock();
        y.lock()->right.lock()->left = y.lock()->left;
        y.lock()->right = x.lock()->child;
        y.lock()->left = x.lock()->child.lock()->left;
        x.lock()->child.lock()->left.lock()->right = y;
        x.lock()->child.lock()->left = y;
        ++x.lock()->degree;
        y.lock()->parent = x;
        y.lock()->mark = false;
    }

public:
    bool isEmpty() {
    	if (min != NIL) {
    		return false;
        } else {
            return true;
        }
    }

    weak_ptr<Node> fibHeapInsert(K key, V value) {
        weak_ptr<Node> node = this->createNode(key, value);
        this->insertToFibHeap(node);
        return node;
    }

    weak_ptr<Node> fibHeapExtractMin() {
        weak_ptr<Node> z = this->min;
        if (z != NIL) {
            weak_ptr<Node> child = z.lock()->child;
            if (child != NIL) {
                weak_ptr<Node> next = child.lock()->right;
                do {
                    weak_ptr<Node> min_left = this->min.lock()->left;
                    next.lock()->right.lock()->left = next.lock()->left;
                    next.lock()->left.lock()->right = next.lock()->right;
                    next.lock()->parent = NIL;
                    this->min.lock()->left = next;
                    next.lock()->right = this->min;
                    min_left.lock()->right = next;
                    next.lock()->left = min_left;
                } while (next != child);
            }
            z.lock()->left.lock()->right = z.lock()->right;
            z.lock()->right.lock()->left = z.lock()->left;
            if (z == z.lock()->right) {
                this->min = NIL;
            } else {
                this->min = z.lock()->right;
                consolidate();
            }
            --number_of_nodes;
        }
        return z;
    }

    friend istream& operator >>  (istream& istr, FibHeap<K, V, Compare> & heap){
    	K key;
        V value;
        istr >> key >> value;
        while (!istr.eof()) {
            fibHeapInsert(key, value);
            istr >> key >> value;
        }
        return istr;
    }
};

template <typename K, typename V, typename Compare>
shared_ptr<typename FibHeap<K, V, Compare>::Node> FibHeap<K, V, Compare>::NIL = nullptr;

int main() {
    FibHeap<int, int> a;
    FibHeap<int, int> b;
    a.fibHeapInsert(1, 2);
    a.fibHeapInsert(2, 3);
    a.fibHeapInsert(-2, 3);
    b.fibHeapInsert(3, 4);
    cout << a.fibHeapExtractMin().lock()->key;
    /*
    ifstream fin("input.txt");
    FibHeap<int, int> fHeap;
    fin >> fHeap;
    cout << "\n";
    while (!fHeap.isEmpty()) {
        cout<< fHeap << endl;
        fHeap.fibHeapExtractMin();
        cout << "--------Extract min ---------------------------------------------------\n";
    }
    fin.close();
    */
    return 0;
}
