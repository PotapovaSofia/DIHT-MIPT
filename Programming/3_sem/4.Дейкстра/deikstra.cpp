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
			if (weight < 0) {
				cerr << "wrong value\n"; 
			}
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
		void Dijkstra (int vertex) {	
			vector<double> dist (graph.size(), INT_MAX);
			dist[vertex] = 0;
			vector<bool> flags (graph.size());
			for (int i = 0; i < graph.size(); ++i) {
				int v = -1;
				for (int j = 0; j < graph.size(); ++j) {
					if (!flags[j] && (v == -1 || dist[j] < dist[v])) {
						v = j;
					}
				}
				if (dist[v] == INT_MAX) {
					break;
				}
				flags[v] = true;
 				for (int j = 0; j < graph[v].size(); ++j) {
					dist[graph[v][j].first] = min(dist[graph[v][j].first], dist[v] + graph[v][j].second);
				}	
			}
			for (int i = 0; i < dist.size(); i++) {
	                	cout << "Distance from " << vertex << " to " << i << " is " << dist[i] << endl;
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
	myGraph.Dijkstra(0);

        return 0;
}
