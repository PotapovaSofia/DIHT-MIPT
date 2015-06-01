#include <future>
#include <algorithm>
#include <string>
#include <functional>
#include "thread_pool.h"
#include "point.h"

struct Node {
	Point p;
	Node* left;
	Node* right;
	Node* parent;
	int axis;

	Node(Point p_, int axis_): p(p_), axis(axis_), left(nullptr), right(nullptr), parent(nullptr) {}

	void set_left(Node* left_) {
		left = left_;
		if(left) {
			left->parent = this; 
		}
	}

	void set_right(Node* right_) {
		right = right_;
		if(right) {
			right->parent = this;
		}
	}

	inline Node* descend_to_leaf(const Point& x) {
		Node* cur = this;
		while(true) {
			if (!cur->left && !cur->right) {
				return cur;
			}
			if(x[cur->axis] <= cur->p[cur->axis]) {
				if(!cur->left) {
					cur = cur->right;
				} else {
					cur = cur->left;
				}
			} else {
				if(!cur->right) {
					cur = cur->left;
				} else {
					cur = cur->right;
				}
			}	
		}
		
	}

	void print_node(int depth = 0) {
		std::string s = "";
		for (int j = 0; j < depth * 4; j++)
			s += " ";
		std::cout << s << "point: ";
		p.print_point();
		std::cout << s  << "left:  ";
		if (left) 
			left->p.print_point();
		else 
			std::cout << "NULL" << std::endl;
		std::cout << s << "right: ";
		if (right) 
			right->p.print_point();
		else
			std::cout << "NULL" << std::endl;
	}
};

struct cmp {
	int param;
	cmp(int param_): param(param_) {}
	bool operator()(const Point & a, const Point & b) {
		return a[param] < b[param];
	} 
};

class KD_Tree {
    thread_pool<Node*> task_queue_for_build;
    thread_pool<void> task_queue_for_find;
	Node* root;
	std::atomic<Node*> best;
	std::atomic<int> sqr_best;
	int k;
	
public:
	
	const static int inf = 1e9;
	
	KD_Tree(std::vector<Point> points, int k_) {
		k = k_;
		std::sort(points.begin(), points.end());
		points.resize(std::unique(points.begin(), points.end()) - points.begin());
        auto build_future = task_queue_for_build.submit(std::bind(&KD_Tree::build, this, std::ref(points), 0, points.size() - 1, 0));
        root = build_future.get();
	}

	~KD_Tree() {
		delete_nodes(root);
	}


private:

	Node* build(std::vector<Point>& points, int left_bound, int right_bound, int depth) {
		if (left_bound > right_bound) {
			return nullptr;
		}

		int axis = depth % k;
		std::sort(points.begin() + left_bound, points.begin() + right_bound + 1, cmp(axis));

		int medium_index = (left_bound + right_bound) / 2;
		Point medium = points[medium_index];
		Node* result = new Node(medium, axis);
		
		int i;
		for (i = medium_index; i <= right_bound; i++) {
			if (points[i][axis] > medium[axis])
				break;
		}
		std::swap(points[medium_index], points[i - 1]);

        auto left_part_built = task_queue_for_build.submit(std::bind(&KD_Tree::build, 
        	this, std::ref(points), left_bound, i - 2, depth + 1));
        
		result->set_right(build(points, i, right_bound, depth + 1));

		task_queue_for_build.active_wait(left_part_built);
		result->set_left(left_part_built.get());	
		
		return result;
	}

	void find_nearest(const Point& x, Node* root) {
		
		if (root == nullptr) {
			return;
		}
		Node* current = root->descend_to_leaf(std::cref(x));
		bool from_left;
		while (current != root->parent) {
			int dist = x.distance(current->p);
			if (dist != 0 && dist < sqr_best.load()) {
				sqr_best.store(dist);
				best.store(current);
			}
			if (sqr_best.load() != inf) {
				int plane = current->p[current->axis];
				double sqrtb = sqrt(sqr_best.load());
				std::future<void> found_part;
				if (plane >= x[current->axis] - sqrtb && plane <= x[current->axis] + sqrtb) {
					if (from_left) {
						found_part = task_queue_for_find.submit(std::bind(&KD_Tree::find_nearest, this, std::cref(x), current->right));
					} else {
						found_part = task_queue_for_find.submit(std::bind(&KD_Tree::find_nearest, this, std::cref(x), current->left));
					}
					task_queue_for_find.active_wait(found_part);
				}
			}
			from_left = (current->parent != nullptr && current->parent->left == current);
			current = current->parent;
		}
	}

	void delete_nodes(Node* node) {
		if (!node) {
			return;
		}
		delete_nodes(node->left);
		delete_nodes(node->right);
		delete node;
	}

	void print_tree(Node* node, int depth) {
		if (!node) return;
		node->print_node(depth);
		print_tree(node->left, depth + 1);
		print_tree(node->right, depth + 1);
	}


public:

	void print_tree() {
		print_tree(root, 0);
	}

	Point find_nearest_neighbor(const Point& x) {
		sqr_best.store(inf);
		best.store(nullptr);
		auto res = task_queue_for_find.submit(std::bind(&KD_Tree::find_nearest, this, std::cref(x), root));
		res.get();
		return best.load()->p;
	}
};