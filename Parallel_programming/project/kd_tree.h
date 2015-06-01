#include <string>
#include <algorithm>
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
		Node * cur = this;
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
	
	Node* root;
	Node* best;
	int sqr_best;
	int k;
	

public:
	const static int inf = 1e9;
	KD_Tree(std::vector<Point> points, int k_) {
		k = k_;
		std::sort(points.begin(), points.end());
		points.resize(std::unique(points.begin(), points.end()) - points.begin());
		root = build(points, 0, points.size() - 1, 0);
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
		result->set_left(build(points, left_bound, i - 2, depth + 1));
		result->set_right(build(points, i, right_bound, depth + 1));
		return result;
	}

	void find_nearest(const Point& x, Node* root) {
		if (root == nullptr)
			return;
		Node* current = root->descend_to_leaf(x);
		int fromLeft;
		while (current != root->parent) {
			int dist = x.distance(current->p);
			if (dist != 0 && dist < sqr_best) {
				sqr_best = dist;
				best = current;
			}
			if (sqr_best != inf) {
				int plane = current->p[current->axis];
				if (plane >= x[current->axis] - sqrt(sqr_best) && plane <= x[current->axis] + sqrt(sqr_best)) {
					if (fromLeft) {
						find_nearest(x, current->right);
					} else {
						find_nearest(x, current->left);
					}
				}
			}
			fromLeft = (current->parent != nullptr && current->parent->left == current);
			current = current->parent;
		}
	}

public:

	void print_tree() {
		print_tree(root, 0);
	}

	void print_tree(Node* node, int depth) {
		if (!node) return;
		node->print_node(depth);
		print_tree(node->left, depth + 1);
		print_tree(node->right, depth + 1);
	}

	Point find_nearest(const Point& x) {
		sqr_best = inf;
		find_nearest(x, root);
		return best->p;
	}
};