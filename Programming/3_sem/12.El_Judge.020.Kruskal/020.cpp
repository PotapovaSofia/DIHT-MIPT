#include <iostream>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <set>
#include <exception>

using namespace std;

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

struct Point {
	int x;
	int y;
	Point(int x, int y): x(x), y(y) {}
};

int area(Point p1, Point p2, Point p3) {
	return (p3.x - p1.x)*(p2.y - p1.y) - (p2.x - p1.x)*(p3.y - p1.y);
}

bool onSegment(Point p1, Point p2, Point p3) {
	if (min(p1.x, p2.x) <= p3.x && p3.x <= max(p1.x, p2.x) && min(p1.y, p2.y) &&
		min(p1.y, p2.y) <= p3.y && p3.y <= max(p1.y, p2.y))
		return true;
	return false;
}
bool segmentsIntersect(Point p1, Point p2, Point p3, Point p4) {
	int a1 = area(p3, p4, p1);
	int a2 = area(p3, p4, p2);
	int a3 = area(p1, p2, p3);
	int a4 = area(p1, p2, p4);
	if (((a1 > 0 && a2 < 0) || (a1 < 0 && a2 > 0)) &&
		((a3 > 0 && a4 < 0) || (a3 < 0 && a4 > 0))) {
		return true;
	}
	else if (a1 == 0 && onSegment(p3, p4, p1)) {
		return true;
	}
	else if (a2 == 0 && onSegment(p3, p4, p2)) {
		return true;
	}
	else if (a3 == 0 && onSegment(p1, p2, p3)) {
		return true;
	}
	else if (a4 == 0 && onSegment(p1, p2, p4)) {
		return true;
	}
	return false;
}

bool isIntersectDitch(vector < pair <Point, Point> > &ditches, Point &pt1, Point &pt2) {
	for (int i = 0; i < ditches.size(); i++) {
		if (segmentsIntersect(ditches[i].first, ditches[i].second, pt1, pt2))
			return true;
	}
	return false;
}

struct Edge {
	int to;
	double length;
	Edge(int a, double b): to(a), length(b) {}
};

class Graph {
	vector <vector <Edge> >  graph;
	set < pair <double, pair <int, int> > > queue;
	vector <pair <int, int> > edges;
	vector <int> bufset;
	vector <double> dist;
	int edgeCount;
	int vertexCount;
public:
	Graph(int n) {
		for (int i = 0; i < n; i++) {
			graph.push_back(vector<Edge>());
		}
		dist.resize(n);
		edgeCount = 0;
		vertexCount = n;
	}

	void addEdge(int v1, int v2, double weight) {
		graph[v1].push_back(Edge(v2, weight));
		++edgeCount;
	}

	int findSet(int v1) {
		if (bufset[v1] == v1)
			return v1;
		bufset[v1] = findSet(bufset[v1]);
		return findSet(bufset[v1]);
	}

	bool isInOneTree(int v1, int v2) {
		int p1 = findSet(v1);
		int p2 = findSet(v2);
		return p1 == p2;
	}

	vector <pair <int, int> > &Kruskal() {
		for (int i = 0; i < vertexCount; i++) {
			bufset.push_back(i);
		}
		for (int i = 0; i < vertexCount; ++i) {
			for (int j = 0; j < graph[i].size(); j++) {
				queue.insert(make_pair(graph[i][j].length, make_pair(i, graph[i][j].to)));
			}
		}
		for (set <pair<double, pair <int, int> > >::iterator it = queue.begin(); it != queue.end(); ++it) {
			int vertexOne = it->second.first;
			int vertexTwo = it->second.second;
			if (!isInOneTree(vertexOne, vertexTwo)) {
				edges.push_back(make_pair(vertexOne, vertexTwo));
				bufset[bufset[vertexOne]] = vertexTwo;
			}
		}
		return edges;
	}
};

int main() {
	int townCount, ditchCount, x, y, x1, y1, x2, y2;
	cin >> townCount >> ditchCount;
	vector <Point>  vct;
	vector <string> towns;
	string townName;
	vector <pair <Point, Point> > ditches;
	Graph g(townCount);
	for (int i = 0; i < townCount; i++) {
		cin >> townName >> x >> y;
		vct.push_back(Point(x, y));
		towns.push_back(townName);
	}
	for (int i = 0; i < ditchCount; ++i) {
		cin >> x1 >> y1 >> x2 >> y2;
		ditches.push_back(make_pair(Point(x1, y1), Point(x2, y2)));
	}
	 for (int i = 0; i < townCount; i++) {
		for (int j = i; j < townCount; j++) {
			if (!isIntersectDitch(ditches, vct[i], vct[j]) && i != j) {
				double dist = sqrt(pow((vct[i].x - vct[j].x), 2) + pow((vct[i].y - vct[j].y), 2));
				g.addEdge(i, j, dist);
			}
		}
	}
	vector <pair<int, int> > result = g.Kruskal();

	if (result.size() < townCount - 1) {
		cout << "NO" << endl;
	} else {
		cout << "YES" << endl;
		cout << result.size() << endl;
		for (int i = 0; i < result.size(); i++) {
			cout << towns[result[i].first] << ' ' << towns[result[i].second] << endl;
		}
	}
	return 0;
}
