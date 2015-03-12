#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <list>

using namespace std;

template <typename K, typename V, typename Compare = less<K> >
class BinomialHeap {
public:
    class Node {
        K key;
        int degree;
        Node(){}
        Node (const K & _key): key(_key){}
        Node(const K & _key, const V & _value): key(_key), value(_value){}
    public:
        weak_ptr<Node> parent;
        shared_ptr<Node> sibling;
        shared_ptr<Node> child;
        V value;
        ~Node(){}

        static shared_ptr<Node> createNode(K _key, V _value) {
            shared_ptr<Node> newNode(new Node(_key, _value));
            return newNode;
        }

        K getKey() {
            return key;
        }

        friend ostream& operator << (ostream& ostr, shared_ptr<Node> const & node)  {
            ostr << node->key << " (" << node->value << ") ";
            return ostr;
        }

        friend class BinomialHeap;
    };

private:
    shared_ptr<Node> head;
    static shared_ptr<Node> NIL;
    K absMin;
    K absMax;
    Compare comp;

public:

    BinomialHeap(K infinity, K _absMin) {
    	if (NIL.get() == nullptr) {
            NIL = make_shared<Node>(*(new Node()));
            NIL -> parent = NIL;
            NIL -> child = NIL;
            NIL -> sibling = NIL;
        }
        head = NIL;
        absMax = infinity;
        absMin = _absMin;
    }

    ~BinomialHeap(){}

    shared_ptr<Node> binomialHeapMinimum() {
        shared_ptr<Node> y = NIL;
        shared_ptr<Node> x = head;
        K min = absMax;
        while (x != NIL) {
            if (comp(x->key, min)) {
                min = x->key;
                y = x;
            }
            x = x->sibling;
        }
        return y;
    }

    bool isEmpty() {
        if (head != NIL) {
            return false;
        } else {
            return true;
        }
    }

    void binomialLink(shared_ptr<Node> rootOfFirst, shared_ptr<Node> rootOfSecond) { //treeSecond will be root
        rootOfFirst->parent = rootOfSecond;
        rootOfFirst->sibling = rootOfSecond->child;
        rootOfSecond->child = rootOfFirst;
        rootOfSecond->degree += 1;
        return;
    }

    shared_ptr<Node> binomialHeapMerge(shared_ptr<Node> first, shared_ptr<Node> second){
        shared_ptr<Node> resultListHead;
        shared_ptr<Node> currentRoot;
        if (first->degree < second->degree) {
            resultListHead = first;
            first = first->sibling;
        } else {
            resultListHead = second;
            second = second->sibling;
        }
        currentRoot = resultListHead;
        while (first != NIL && second != NIL) {
            if (first->degree < second->degree) {
                currentRoot->sibling = first;
                first = first->sibling;
            } else {
                currentRoot->sibling = second;
                second = second->sibling;
            }
            currentRoot = currentRoot->sibling;
        }
        if (first != NIL) {
            currentRoot->sibling = first;
        }
        if (second != NIL) {
            currentRoot->sibling = second;
        }
        return resultListHead;
    }

    void binomialHeapUnion(BinomialHeap& otherHeap) {
        this->head = binomialHeapMerge(this->head, otherHeap.head);
        if (head != NIL) {
            shared_ptr<Node> prev_x = NIL;
            shared_ptr<Node> x = head;
            shared_ptr<Node> next_x = x->sibling;
            while (next_x != NIL) {
                if ((x->degree != next_x->degree)
                        || (next_x->sibling != NIL && next_x->sibling->degree == x->degree)) {
                    prev_x = x;
                    x = next_x;
                } else {
                    if (comp(x->key, next_x->key)) {
                        x->sibling = next_x->sibling;
                        binomialLink(next_x, x);
                    } else {
                        if (prev_x == NIL) {
                            head = next_x;
                        } else {
                            prev_x->sibling = next_x;
                        }
                        binomialLink(x, next_x);
                        x = next_x;
                    }
                }
                next_x = x->sibling;
            }
        }
    }


    shared_ptr<Node> binomialHeapExtractMin() {
        shared_ptr<Node> min = binomialHeapMinimum();
        shared_ptr<Node> headOfListOfChildren = min->child;
        shared_ptr<Node> buf;
        shared_ptr<Node> next = headOfListOfChildren->sibling;
        if (min == head) {
            head = min->sibling;
        } else {
            shared_ptr<Node> buf_node = head;
            shared_ptr<Node> prev_min;
            while (buf_node != min) {
                prev_min = buf_node;
                buf_node = buf_node->sibling;
            }
            prev_min->sibling = min->sibling;
        }
        if (min->child == NIL) {
            return min;
        }
        while (next != NIL) {
            headOfListOfChildren->parent = NIL;
            buf = headOfListOfChildren;
            headOfListOfChildren = next;
            next = headOfListOfChildren->sibling;
            headOfListOfChildren->sibling = buf;
        }
        min->child->sibling = NIL;
        BinomialHeap helper(absMax, absMin);
        helper.head = headOfListOfChildren;
        this->binomialHeapUnion(helper);
        return min;
    }

    void binomialHeapInsert(shared_ptr<Node> x) {
        x->parent = NIL;
        x->child = NIL;
        x->sibling = NIL;
        x->degree = 0;
        BinomialHeap helper(absMax, absMin);
        helper.head = x;
        this->binomialHeapUnion(helper);
    }

    K binomialHeapDecreaseKey(shared_ptr<Node> node, K newKey) {
        if (comp(node->key, newKey)) {
        	cerr << "New key is bigger then current";
        	exit(1);
        }
        K oldKey = node->key;
        node->key = newKey;
        shared_ptr<Node> y = node;
        shared_ptr<Node> z = y->parent.lock();
        while (z != NIL && comp(y->key, z->key)) {
            shared_ptr<Node> buf_parent_y = y->parent.lock();
            shared_ptr<Node> buf_parent_z = z->parent.lock();
            shared_ptr<Node> buf_child =  y->child;
            shared_ptr<Node> buf_sibling =  y->sibling;
            y->parent = z->parent;
            y->child = z->child;
            y->sibling = z->sibling;
            z->parent = buf_parent_y;
            z->child = buf_child;
            z->sibling = buf_sibling;
            shared_ptr<Node> buf_node = buf_parent_y->child;
            if (buf_node != y) {
                while (buf_node->sibling != y) {
                    buf_node = buf_node->sibling;
                }
                buf_node->sibling = z;
            }
            buf_node = buf_parent_z->child;
            if (buf_node != z) {
                while (buf_node->sibling != z) {
                    buf_node = buf_node->sibling;
                }
                buf_node->sibling = y;
            }
            y = z;
            z = y->parent.lock();
        }
        return oldKey;
    }

    shared_ptr<Node>binomialHeapDelete(shared_ptr<Node> node) {
        K key = binomialHeapDecreaseKey(node, absMin);
        shared_ptr<Node> result = binomialHeapExtractMin();
        result->key = key;
        return result;
    }

    shared_ptr<Node> put(K key, V value) {
        shared_ptr<Node> newNode = Node::createNode(key, value);
        this->binomialHeapInsert(newNode);
        return newNode;
    }

    list<shared_ptr<Node> > getNodesList() {
        list<shared_ptr<Node> > result;
        shared_ptr<Node> node = this->head;
        while (node != NIL) {
        	result.push_back(node);
            this->get_children(result, node);
            node = node->sibling;
        }
        return result;
    }

    friend istream& operator >>  (istream& istr, BinomialHeap<K, V, Compare> & heap){
        K key;
        V value;
        shared_ptr<Node> last;
        istr >> key >> value;
        while (!istr.eof()) {
            last = heap.put(key, value);
            istr >> key >> value;
        }
        return istr;
    }

    friend ostream& operator << (ostream& ostr, BinomialHeap<K, V, Compare> & heap)  {
        shared_ptr<Node> node = heap.head;
        int count_tab = 0;
        while (node != NIL) {
            ostr << node << "\n";
            ++count_tab;
            heap.print_children(ostr, node, count_tab);
            --count_tab;
            node = node->sibling;
            ostr << "\n\n";
        }
        return ostr;
    }

private:
    void print_children(ostream& ostr, shared_ptr<Node> node, int count_tab) {
        node = node->child;
        while (node != this->NIL) {
            for (int i = 0; i < count_tab; ++i) {
                ostr << "\t";
            }
            ostr << node << "\n";
            ++count_tab;
            print_children(ostr, node, count_tab);
            --count_tab;
            node = node->sibling;
        }
        return;
    }

    void get_children(list<shared_ptr<Node> >& answer, shared_ptr<Node> node) {
        node = node->child;
        while (node != this->NIL) {
            answer.push_back(node);
            get_children(answer, node);
            node = node->sibling;
        }
        return;
    }
};

template <typename K, typename V, typename Compare>
shared_ptr<typename BinomialHeap<K, V, Compare>::Node> BinomialHeap<K, V, Compare>::NIL = nullptr;

int main() {
    ifstream fin("input.txt");
    BinomialHeap<int, int> bHeap(10000,  -10000);
    fin >> bHeap;
    list < shared_ptr<BinomialHeap<int, int>::Node > > l = bHeap.getNodesList();
    for (auto itr : l) {
    	cout << itr;
    }
    cout << "\n";
    while (!bHeap.isEmpty()) {
        cout<< bHeap << endl;
        bHeap.binomialHeapExtractMin();
        cout << "--------Extract min ---------------------------------------------------\n";
    }
    fin.close();
    return 0;
}
