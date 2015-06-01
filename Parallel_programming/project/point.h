#include <vector>

class Point {

	std::vector<int> coord;
	ImageWrapper* image;

public:
	Point(): image(nullptr) {}
	Point(std::vector<int> coord_, ImageWrapper* image_): coord(coord_), image(image_) {}


	int distance(const Point& p) const {
		int dist = 0;
		for (int i = 0; i < p.coord.size(); i++) {
			dist += (coord[i] - p[i])*(coord[i] - p[i]);
		}
		return dist; 
	}

	int operator[](int i) const {
		return coord[i];
	}

	void read_point(int k) {
		coord.resize(k);
		for (int j = 0; j < k; j++) {
			std::cin >> coord[j];
		}
	}

	void read_random_point(int k) {
		coord.resize(k);
		for (int j = 0; j < k; j++)
				coord[j] = rand() % 128;
	}

	ImageWrapper get_image() {
		return *image;
	}

	void print_point() {
		std::cout << "(";
		for (int j = 0; j < coord.size(); j++) {
			std::cout << coord[j];
			if (j != coord.size() - 1)
				printf(", ");
		}
		std::cout << ")" << std::endl;
	}

	bool operator < (const Point & an) const {
		for (int j = 0; j < coord.size(); j++) {
			if (coord[j] != an[j]) {
				return coord[j] < an[j];
			}
		}
		return false;
	}

	bool operator == (Point an) {
		for (int j = 0; j < coord.size(); j++) {
			if (coord[j] != an[j]) {
				return false;
			}
		}
		return true;
	}
};