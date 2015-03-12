#include <memory>
#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <cmath>
#include <climits>

using namespace std;

class Edge {
public:
    int to;
    double length;

    Edge(int a, double b) {
        to = a;
        length = b;
    }
};

class Graph {
	vector< vector < Edge > > graph;
public:
    bool addVertex(int n) {
    	if (n < 1)
    		return false;
    	for (int i = 0; i < n; ++i) {
    		graph.push_back(vector<Edge>());
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

    vector<double> Dijkstra(int from) {
    	if (graph.size() < 1) {
    	    return vector<double>();
    	}
    	if (from < 0 || from >= graph.size()) {
    	    return vector<double>();
    	}
    	vector<double> dist;
    	dist = vector<double>(graph.size(), -1);
    	dist[from] = 0;
    	set < pair <double, int> > wayset;
    	int currentVertex = from;
    	for (int i = 0; i < graph.size(); ++i) {
    		vector<Edge> v = graph[currentVertex];
    	    for (int j = 0; j < v.size(); ++j) {
    	        double way = v[j].length + dist[currentVertex];
    	        wayset.insert(make_pair(way, v[j].to));
    	    }
    	    if (wayset.empty())
    	    	break;
    	    set< pair <double, int> >::iterator it = wayset.begin();
    	    while (dist[it->second] != -1) {
    	    	++it;
    	        if (it == wayset.end())
    	        	break;
    	    }
    	    if (it == wayset.end())
    	    	break;
    	    currentVertex = it->second;
    	    dist[it->second] = it->first;
    	    ++it;
    	    wayset.erase(wayset.begin(), it);
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
