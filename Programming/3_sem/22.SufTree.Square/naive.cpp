#include <iostream>
#include <vector>
#include <map>
#include <string.h>

using namespace std;

struct vertex {
    map<char, int> edges;
};

int sz;

void add_string (const string & s, vector<vertex> &tree) {
    int v = 0;
    for (int i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (tree.at(v).edges.find(c) == tree.at(v).edges.end()) {
            tree.at(v).edges.insert(make_pair(c, ++sz));
            vertex vert;
            tree.push_back(vert);
        }
        v = tree.at(v).edges.at(c);
    }
}

void print_tree(int v, vector<vertex> tree, int k) {
    if (tree.at(v).edges.size() != 0) {
        for (map<char, int>::iterator it = tree.at(v).edges.begin();
             it != tree.at(v).edges.end(); it++) {
            for (int i = 0; i < k; i++) cout << "   ";
            cout << it->first << endl;
            print_tree(it->second, tree, k+1);
        }
    }

}

int main() {
    string s;
    cin >> s;
    s += '$';

    vector<vertex> tree;
    sz = 0;
    vertex root;
    tree.push_back(root);

    for (int i = 0; i < s.size(); i++) {
        string tmp(s.end() - i - 1, s.end());
        add_string(tmp, tree);
    }
    print_tree(0, tree, 0);

    return 0;
}