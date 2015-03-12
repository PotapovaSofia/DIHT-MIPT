#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <list>
#include <memory>

using namespace std;

template <typename T>
class Graph{
	public: 
		vector<shared_ptr<vector<T> > > graph;
		Graph(int n = 0){
			graph.reserve(n);
			for(int i = 0; i < n; i++)
				graph.push_back(make_shared<vector<T> >());
		}
		void addVertex(T vertex){
			vector<T> vct;
			vct.push_back(vertex);
			graph.push_back(make_shared<vector<T> >(vct));
		}
		void addEdge(T vertexOut, T vertexIn){
			if (vertexOut > graph.size() || vertexIn > graph.size())
				cout << "Error: addEdge\n";
			else
				graph[vertexOut]->push_back(vertexIn);
		}
		void printGraph(){
			for(int i = 0; i < graph.size(); i++){
				for(int j = 0; j < (*graph[i]).size(); j++)
					cout << (*graph[i])[j] << " ";
				cout << endl;
			}
		}
};

int main(){
	int vertexCount, edgeCount, vertexOut, vertexIn;
	Graph<int> myGraph;
	cin >> vertexCount;
	for(int i = 0; i < vertexCount; i++)
		myGraph.addVertex(i);
	cin >> edgeCount;
	for (int i = 0; i < edgeCount; ++i){
		cin >> vertexOut >> vertexIn;
		myGraph.addEdge(vertexOut,vertexIn);
	}
	myGraph.printGraph();
	return 0;
}
