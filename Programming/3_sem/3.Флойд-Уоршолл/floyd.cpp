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
                vector< vector < double > > graph;
	public:
                Graph(int n){
                        graph.resize(n);
                        for (int i = 0; i < n; i++) {
				graph[i].resize(n);
//                                graph.push_back(vector < int > >());
			}
			for (int i = 0; i < n; i++){
				for (int j = 0; j < n; j++) {
/*					if (i == j) {
						graph[i][j] = 0;
					} else {
						graph[i][j] = INT_MAX;
					}
*/
					graph[i][j] = i == j ? 0 : INT_MAX; 
				}
			}
                }
                void addEdge(int vertexOut, int vertexIn, double weight) {
//		       	graph[vertexOut].push_back(make_pair(vertexIn, weight));
			graph[vertexOut][vertexIn] = weight;
//			cout << vertexOut << " " << vertexIn << " " << weight << endl; 
                }
		void printGraph(){
			cout << graph.size() << endl;
                        for(int i = 0; i < graph.size(); i++) {
				cout << i << " -> ";
                                for(int j = 0; j < graph[i].size(); j++) {
					if (graph[i][j] != 0 && graph[i][j] != INT_MAX) {
	                                        cout << j << " (" << graph[i][j] << ") ";
					}
				}
                                cout << endl;
                        }
                }
		void FloydBellman() {
			vector < vector < double > > dist = graph;
			int n = graph.size();
			for (int k = 0; k < n; ++k) {
				for (int i = 0; i < n; ++i) {
					for (int j = 0; j < n; ++j) {
						if (dist[i][k] < INT_MAX && dist[k][j] < INT_MAX) {
							dist[i][j] = min (dist[i][j], dist[i][k] + dist[k][j]);
						}
					}
				}
			}
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					if(dist[i][j] != INT_MAX) {
						cout << "Distance from " << i << " to " << j << " is " << dist[i][j] << endl;
					} else {
						cout << "No path from " << i << " to " << j << endl;
					}
				}
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
	myGraph.FloydBellman();

        return 0;
}
