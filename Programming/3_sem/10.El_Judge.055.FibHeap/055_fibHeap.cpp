#include <memory>
#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <cmath>
#include <climits>
#include <queue>
#include <vector>
#include <memory>

#define INF 65535

using namespace std;

template < typename K, typename V, typename Compare = less<K> >
class FibHeap {
    struct Node {
        Node* parent;
        Node* child;
        Node* left;
        Node* right;
        int degree;
        bool mark;
        K key;
        V value;
        Node(K key, V value): key(key), value(value), parent(FibHeap::NIL), child(FibHeap::NIL), degree(0), mark(false) {}
    };

    set < Node* > nodes;
    Node* min;
    int number_of_nodes;
    static Node* NIL;
    Compare comp;

public:
    FibHeap() {
        min = NIL;
        number_of_nodes = 0;
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
typename FibHeap<K, V, Compare>::Node* FibHeap<K, V, Compare>::NIL = NULL;

class Edge {
public:
    int to;
    double length;

    Edge(int a, double b) {
        to = a;
        length = b;
    }
};

class CompareEdges {
public:
	bool operator()(Edge& n1, Edge& n2){
		if (n1.length < n2.length)
			return true;
		return false;
    }
};

class Graph {
	vector< vector < Edge > > graph;
	//set < pair <double, int> > wayset;
	FibHeap<double, int> wayset;
	//priority_queue< pair <double, int> > wayset;
	vector <double> dist;
public:
    bool addVertex(int n) {
    	if (n < 1)
    		return false;
    	for (int i = 0; i < n; ++i) {
    		graph.push_back(vector<Edge>());
    	}
    	dist.resize(n);
    	for (int i = 0; i < n; i++) {
    		dist[i] = INF;
    	}
    	return true;
    }

    bool addEdge(int v1, int v2, double length) {
        if (v1 >= graph.size() || v2 >= graph.size())
            return false;
        graph[v1].push_back(Edge(v2, length));
        return true;
    }

    void print() {
        int v = 0, e = 0;
        v = graph.size();
        for (int i = 0; i < v; ++i) {
            e = graph[i].size();
            cout << i << ": ";
            for (int j = 0; j < e; ++j)
                cout << graph[i][j].to << " (" << graph[i][j].length << ") ";
            cout << "\n";
        }
    }

    int getSize() {
    	return graph.size();
    }

    vector<Edge> getListOfEdges(int n) {
		if (n > graph.size())
		    return vector<Edge>();
		return graph[n];
    }

    void relax(int vertex) {
    	double value;
    	vector < Edge >::iterator it = graph[vertex].begin();
    	for (; it != graph[vertex].end(); ++it) {
    		value = dist[vertex] + it->length;
    		if (dist[it->to] > value) {
    			dist[it->to] = value;
    			wayset.fibHeapInsert(value, it->to);
    			//wayset.push(make_pair(value, it->to));
    		}
    	}
    }

    vector <double> & Dijkstra(int vertex) {
    	dist[vertex] = 0;
    	for (int i = 0; i < graph.size(); i++) {
    		if (i != vertex)
    			dist[i] = INF;
    	}
    	relax(vertex);
    	/*wayset.push(make_pair(0, 0));
    	while (!wayset.empty()) {
    		set < pair <double, int> >::iterator it = wayset.begin();
    		relax((it->second));
    		wayset.erase(it);
    	}
    	*/
        wayset.fibHeapInsert(0, 0);
    	while (!wayset.isEmpty()) {
    		relax(wayset.fibHeapExtractMin()->value);
    	}
    	return dist;
    }
};

int main() {
    int N = 0;
    double R = 0, x, y;
    cin >> N >> R;
    Graph graph;
    vector< pair<double, double> > coordinates;
    graph.addVertex(N);
    for (int i = 0; i < N; ++i) {
        cin >> x >> y;
        coordinates.push_back(make_pair(x, y));
    }
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
        	double dist = (coordinates[i].first - coordinates[j].first) * (coordinates[i].first - coordinates[j].first)
                + (coordinates[i].second - coordinates[j].second) * (coordinates[i].second - coordinates[j].second);
            if (dist <= R*R) {
                graph.addEdge(i, j, sqrt(dist));
                graph.addEdge(j, i, sqrt(dist));
            }
        }
    }
    vector<double> wayFromA = graph.Dijkstra(0);
    vector<double> wayFromB = graph.Dijkstra(1);
    pair<int, double> result = make_pair(-1, -1);
    for (int i = 2; i < N; ++i) {
        if (coordinates[i].first < 0 && wayFromA[i] != -1 && wayFromB[i] != -1 &&
           (wayFromA[i] + wayFromB[i] < result.second || result.second == -1))
            result = make_pair(i, wayFromA[i] + wayFromB[i]);
    }
    cout << result.second;
}


