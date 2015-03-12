#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <list>
#include <memory>
#include <stack>
#include <climits>
#include <fstream>

using namespace std;

class Graph{
		vector< shared_ptr< vector <int> > > graph;
		int sscCnt, myTime;
                stack<int> myStack;
                vector<int> component;
                vector<int> f;
                vector<int> timeIn;
	public:      
                Graph(int n = 0) {
                        graph.reserve(n);
                        for(int i = 0; i < n; i++)
                                graph.push_back(make_shared< vector <int> >());
                }
                void addVertex() {
                        graph.push_back(make_shared< vector <int> >());
                }
                void addEdge(int vertexOut, int vertexIn) {
                        if (vertexOut > graph.size() || vertexIn > graph.size()) {
                                cerr << "wrong nvalue\n";
			} else {
                                graph[vertexOut]->push_back(vertexIn);
			}
                }
                list<int> getList(int vertexOut) {
                        list<int> lst;
                        for (int j = 0; j < (*graph[vertexOut]).size(); ++j) {
                                lst.push_back( (*graph[vertexOut])[j] );
			}
                }
		void printGraph(){
			cout << "Graph:" << endl;
                        for(int i = 0; i < graph.size(); i++) {
				cout << i << " -> ";
                                for(int j = 0; j < (*graph[i]).size(); j++) {
                                        cout << (*graph[i])[j] << " ";
				}
                                cout << endl;
                        }
                }
		void dfs(int vertex) {
			if (timeIn[vertex] == 0) {
			myStack.push(vertex);
			f[vertex] = timeIn[vertex] = ++myTime;
			for (int i = 0; i < graph[vertex]->size(); ++i) {
				if (component[(*graph[vertex])[i]] == 0) {
					if (timeIn[(*graph[vertex])[i]] == 0) {
						dfs((*graph[vertex])[i]);
					}
					if (f[vertex] > f[(*graph[vertex])[i]]) {
						f[vertex] = f[(*graph[vertex])[i]];
					}
				}
			}
			if (f[vertex] == timeIn[vertex]) { //is root
				int x;
				++sscCnt;
				cout << sscCnt << ": ";
				do {
					x = myStack.top();
					component[x] = sscCnt;
					myStack.pop();
					cout << x << " ";
				} while (x != vertex);
				cout << "\n";
			}
		}
	}
	void Tarjan() {
		myTime = 0;
		sscCnt = 0;
		int n = graph.size();
		component.resize(n);
		f.resize(n);
		timeIn.resize(n);
		for (int i = 0; i < n; ++i) {
			component[i] = 0;
			f[i] = -1;
			timeIn[i] = 0;
		}
		for (int i = 0; i < n; i++) {
			if (component[i] == 0) {
				dfs(i);
			}
		}
	}
};
int main(){
        int vertexCount, edgeCount, vertexOut, vertexIn;
        Graph myGraph;
	ifstream fin("input.txt");
        fin >> vertexCount;
        for(int i = 0; i < vertexCount; i++)
                myGraph.addVertex();
        fin >> edgeCount;
        for (int i = 0; i < edgeCount; ++i){
                fin >> vertexOut >> vertexIn;
                myGraph.addEdge(vertexOut,vertexIn);
        }
        myGraph.printGraph();
	cout << endl << "Strongly connected components: " << endl;
	myGraph.Tarjan();
	
        return 0;
}

