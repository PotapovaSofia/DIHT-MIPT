#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

struct sccStruct {
	int pre, low;
    sccStruct(int _pre, int _low) {
    	pre = _pre;
    	low = _low;
    }
};

struct dfsNode {
	int n, pos;
    dfsNode(int _n, int _pos) {
    	n = _n;
    	pos = _pos;
    }
};

class Graph{
	vector< vector <int> > graph;
	vector<int> num;
public:
    Graph(int n = 0) : graph(2*n, vector<int>()) {}
    void addEdge(int a, int b) {
    	int vertexOut = getInternalNum(a);
    	int vertexIn = getInternalNum(b);
    	if (vertexOut > graph.size() || vertexIn > graph.size()) {
    		cout << "wrong nvalue\n";
		} else {
			graph[vertexOut].push_back(vertexIn);
		}
    }
    int getInternalNum(int i) {
    	vector<int>::iterator it = find(num.begin(), num.end(), i);
    	if (it == num.end()) {
    		num.push_back(i);
    		return num.size() -1;
    	}
    	return it - num.begin();
    }

    vector<int> Tarjan() {
        vector<sccStruct> scc_table(graph.size(), sccStruct(-1, -1));
        vector<dfsNode> dfs_stack;
        vector<int> scc_stack;
        vector<int> ret_vals(graph.size(), -1);
        int sccs_found = 0;

        int n = 0;
        bool completed = false;
        while (!completed) {
            if (dfs_stack.empty()) {
                for (int i = 0; i < graph.size(); ++i) {
                    if (scc_table[i].pre == -1) {
                        dfs_stack.push_back(dfsNode(i, 0));
                        scc_stack.push_back(i);
                        scc_table[i] = sccStruct(n, i);
                        ++n;
                        break;
                    }
                }
                if (dfs_stack.empty()) {
                    completed = true;
                }
            } else {
                int prevVertex = dfs_stack.back().n;
                vector<int> vertex = graph[prevVertex];
                if (dfs_stack.back().pos >= vertex.size()) {
                    dfs_stack.pop_back();
                    if (scc_table[prevVertex].low == prevVertex) {
                        int i = 1;
                        while (*(scc_stack.end() - i) != prevVertex) {
                            ++i;
                        }
                        for (int k = 0; k < i; ++k) {
                            ret_vals[scc_stack.back()] = sccs_found;
                            scc_stack.pop_back();
                        }
                        sccs_found++;
                    } else {
                        if (scc_table[scc_table[dfs_stack.back().n].low].pre > scc_table[scc_table[prevVertex].low].pre) {
                            scc_table[dfs_stack.back().n].low = scc_table[prevVertex].low;
                        }
                    }
                } else {
                    int currentVertex = vertex.at(dfs_stack.back().pos);
                    dfs_stack.back().pos += 1;
                    if (scc_table[currentVertex].pre != -1) {
                        if (scc_table[scc_table[prevVertex].low].pre > scc_table[currentVertex].pre &&
                            count(scc_stack.begin(), scc_stack.end(), currentVertex))
                            scc_table[prevVertex].low = currentVertex;
                    } else {
                        dfs_stack.push_back(dfsNode(currentVertex, 0));
                        scc_stack.push_back(currentVertex);
                        scc_table[currentVertex] = sccStruct(n, currentVertex);
                        ++n;
                    }
                }
            }
        }
        return ret_vals;
    }
};

int main() {
	int N, M, var1, var2;
	cin >> N >> M;

	Graph graph(N);
	vector<bool> isUsed(N, false);

	for (int i = 0; i < M; i++) {
		cin >> var1 >> var2;
		isUsed[abs(var1) - 1] = true;
		if (var2 != 0) {
			graph.addEdge(-var1, var2);
			graph.addEdge(-var2, var1);
			isUsed[abs(var2) - 1] = true;
			cin >> var1;
		} else {
			graph.addEdge(-var1, var1);
		}
	}

	vector<int> scc = graph.Tarjan();
	vector<int> result; // == 1

	for (int i = 1; i <= N; i++){
		int num = graph.getInternalNum(i);
		int minusNum = graph.getInternalNum(-i);
		if (scc[num] == scc[minusNum]) {
			cout << "No" << endl;
			return 0;
		}
		if (scc[num] < scc[minusNum]) {
			result.push_back(i);
		}
	}

	cout << "Yes" << endl;
	for (vector<int>::iterator it = result.begin(); it != result.end() ; it++){
		if (isUsed[*it -1]) {
			cout << *it << ' ';
		}
	}
	cout << 0;
}
