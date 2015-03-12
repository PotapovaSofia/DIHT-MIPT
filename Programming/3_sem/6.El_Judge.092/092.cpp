#include <iostream>
#include <vector>
#include <iterator>
#include <cmath>

using namespace std;

class Graph {
	vector < vector < int > > graph;
	static vector < bool > inTree;
public:
	Graph(int n = 0) {
		graph.resize(n);
		inTree.resize(n, false);
	}
	void addEdge(int a, int b) {
		graph[a].push_back(b);
	}
/*
	int DFS(int vertex) {
	    int latest;
		inTree[vertex] = true;
		for (vector < int >::iterator it  = graph[vertex].begin(); it != graph[vertex].end(); ++it) {
			if (!inTree[*it]) {
				DFS(*it);
				latest = *it;
			}
		}
		return latest;
	}
*/
	int DFS(int vertex) {
	    vector<bool> visited(graph.size(), false);
	    int currentVertex, next, latestFound = 0;
	    vector <pair <int, int> > stack;
	    stack.push_back(make_pair(vertex, -1));
	    inTree[vertex] = true;
	    visited[vertex] = true;
	    while (stack.size() > 0) {
	        currentVertex = stack.back().first;
	        if (graph[currentVertex].size() <= stack.back().second + 1) {
	            stack.pop_back();
	        } else {
	            next = graph[currentVertex][stack.back().second + 1];
	            if (visited[next]) {
	                ++stack.back().second;
	            } else {
	                inTree[next] = true;
	                visited[next] = true;
	                ++stack.back().second;
	                stack.push_back(make_pair(next, -1));
	                latestFound = next;
	            }
	        }
	    }
	    return latestFound;
	}
	bool isInTree(int i) {
		return inTree[i];
	}

};

vector < bool > Graph::inTree;

int main() {
	int N, M, res = 0, tmp;
	scanf("%d %d", &N, &M);
	Graph myGraph(N);
	Graph tempGraph(N);
	vector < bool > haveAnc (N, false);
	for (int i = 0; i < M; ++i) {
		int a, b;
		scanf("%d %d", &a, &b);
		myGraph.addEdge(a - 1, b - 1);
		tempGraph.addEdge(b - 1, a - 1);
		haveAnc[b - 1] = true;
	}
	for (int i = 0; i < N; ++i) {
		if (!haveAnc[i]) {
			res++;
			myGraph.DFS(i);
		}
	}
	for (int i = 0; i < N; ++i) {
		if (!myGraph.isInTree(i)) {
			res++;
			tmp = myGraph.DFS(i);
			tempGraph.DFS(tmp);
		}
	}
	printf("%d", res);
	return 0;
}
