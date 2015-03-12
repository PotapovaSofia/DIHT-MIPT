#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <fstream>
#include <utility>
#include <climits>

#define min(a, b)  (a) < (b) ? (a) : (b)

using namespace std;

class Graph{
                vector< vector < pair < int, double > > > graph;
	public:
                Graph(int n = 0){
                        graph.reserve(n);
                        for (int i = 0; i < n; i++) {
                                graph.push_back(vector < pair <int, double> >());
			}
                }
                void addEdge(int vertexOut, int vertexIn, double weight) {
		       graph[vertexOut].push_back(make_pair(vertexIn, weight));
                }
		void printGraph(){
                        for(int i = 0; i < graph.size(); i++) {
				cout << i << " -> ";
                                for(int j = 0; j < graph[i].size(); j++) {
                                        cout << graph[i][j].first << " (" << graph[i][j].second << ") ";
				}
                                cout << endl;
                        }
                }
		void FordBellman(int vertex) {
			vector <double> dist (graph.size(), INT_MAX);
			bool fl = true;
			dist[vertex] = 0;
			for (int i = 0; i < graph.size() - 1; i++) {
				for (int j = 0; j < graph.size(); j++) {
					for (int k = 0; k < graph[j].size(); k++) {
						if (dist[j] < INT_MAX) {
							dist[graph[j][k].first] = min(dist[graph[j][k].first], dist[j] + graph[j][k].second);
						}
					}
				}
			}
			for (int j = 0; j < graph.size(); j++) {
                        	for (int k = 0; k < graph[j].size(); k++) {
                                	if (dist[graph[j][k].first] > dist[j] + graph[j][k].second) {
						fl = false;
                                	}
                        	}
                        }
			if (fl) {
				for (int i = 0; i < graph.size(); i++) {
                                        cout << "Distance from " << vertex << " to " << i << " is " << dist[i] << endl;
                                }
                                cout << endl;
			} else {
				cout << "This graph has negative cycle\n";
			}
		}
};

int main(){
        int vertexCount, edgeCount, vertexOut, vertexIn;
	double weight;
	ifstream fin("input.txt");
        fin >> vertexCount;
	Graph myGraph(vertexCount);
        fin >> edgeCount;
        for (int i = 0; i < edgeCount; ++i){
                fin >> vertexOut >> vertexIn >> weight;
                myGraph.addEdge(vertexOut, vertexIn, weight);
        }
        myGraph.printGraph();
	myGraph.FordBellman(0);
        return 0;
}
