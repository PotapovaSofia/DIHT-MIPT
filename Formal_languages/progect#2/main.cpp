#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>

using namespace std;

struct Rule {
	pair<string, vector<string>> r;
	friend bool operator< (const Rule& a, const Rule& b) {
		return a.r < b.r;
	}
	friend bool operator==(const Rule& a, const Rule& b) {
		return a.r == b.r;
	}
	Rule(){}
	Rule(const string& left, const vector<string>& right) {
		r = make_pair(left, right);
	}
};

class SLR {
	string S;
	string S_;

	string next = "!";
	vector<map<char, string>> action;
	vector<map<string, int>> go;
	vector<Rule> grammar_rules;

	set<string> non_terminals;
	set<char> terminals;
	set<string> all_symbols;

	map<string, set<char>> first;
	map<string, set<char>> follow;

	vector<set<Rule>> canonical_sets;

	stack<string> st;

public:
	SLR(const string& file_name);
	void run(const string& line);

private:
	void read_grammar(const string& file_name);
	bool match(const string& word);

	set<char> first_str(const vector<string>& line);										//First(X1X2...)
	void handle_line(int& index, const vector<string>& line, bool& end_line);
	void add_rules(const string& non_term, set<Rule>& result, set<string>& used_non_term);  //Recursively add Rules to closure
	
	set<Rule> closure();																	//Closure ({S' -> S}) - I0
	set<Rule> closure(const set<Rule>& Rules);												//General closure for goto
	set<Rule> go_to(const set<Rule>& Rules, const string& symbol);
	
	void build_canonical_sets();
	void build_action_table();
	void build_goto_table();

};

int main() {
	string file = "grammar.txt";
	SLR slr(file);
	string line;
	int m;
	cin >> m;
	for (int i = 0; i < m; i++) {
		cin >> line;
		slr.run(line);
	}
	return 0;
}

SLR::SLR(const string& file_name) {
	read_grammar(file_name);
	S = grammar_rules[1].r.first;
	S_ = S + '\'';

	for (int i = 1; i < grammar_rules.size(); i++) {
		non_terminals.insert(grammar_rules[i].r.first);
		for (auto it : grammar_rules[i].r.second) {
			all_symbols.insert(it);
		}
	}
	all_symbols.insert("$");
	for (auto it : all_symbols)
	if (non_terminals.find(it) == non_terminals.end())
		terminals.insert(it[0]);
	terminals.insert('$');

	//Build first sets.
	int size = grammar_rules.size();
	auto used = vector<bool>(size + 1);
	for (auto it : terminals) {
		char* str = new char[2];
		str[0] = it;
		str[1] = '\0';
		string k(str);
		first[k].insert(it);
		delete[] str;
	}
	bool flag = true;
	auto first_set_sizes = vector<int>(all_symbols.size() + 1);
	int i = 0;
	for (auto it : first) {
		first_set_sizes[i] = it.second.size();
		i++;
	}
	auto new_first_set_sizes = vector<int>(all_symbols.size() + 1);
	while (flag) {
		for (int i = 1; i < size; i++) {
			if (!used[i]) {
				auto right = grammar_rules[i].r.second;
				if (!right.empty()) {
					int k = 0;
					int eps_flag = true;
					if (find(terminals.begin(), terminals.end(), right[0][0]) != terminals.end())
						used[i] = true;
					while (k < right.size() && eps_flag) {
						first[grammar_rules[i].r.first].insert(first[right[k]].begin(), first[right[k]].end());
						if (first[right[k]].find('?') == first[right[k]].end())
							eps_flag = false;
						k += 1;
					}
				} else {
					first[grammar_rules[i].r.first].insert('?');
					used[i] = true;
				}
			}
		}
		int x = 0;
		for (auto it : first) {
			new_first_set_sizes[x] = it.second.size();
			x++;
		}
		if (new_first_set_sizes == first_set_sizes) {
			flag = false;
		} else {
			first_set_sizes = new_first_set_sizes;
			new_first_set_sizes = vector<int>(all_symbols.size() + 1);
		}
	}

	//Build follow sets.
	size = grammar_rules.size();
	auto follow_set_sizes = vector<int>(non_terminals.size() + 1);
	int k = 0;
	for (auto it : follow) {
		follow_set_sizes[k] = it.second.size();
		k++;
	}
	auto new_follow_set_sizes = vector<int>(non_terminals.size() + 1);

	follow[S].insert('$');
	for (int i = 1; i < size; i++) {
		string current_term = grammar_rules[i].r.first;
		auto right_part = grammar_rules[i].r.second;
		int j = 0;
		bool f = false;
		handle_line(j, right_part, f);
	}
	flag = true;
	while (flag) {
		flag = false;
		for (int i = 1; i < size; i++) {
			string current_term = grammar_rules[i].r.first;
			auto right_part = grammar_rules[i].r.second;
			int j = 0;
			while (j < right_part.size()) {
				while (j < right_part.size() && find(non_terminals.begin(), non_terminals.end(), right_part[j]) == non_terminals.end()) {
					j += 1;
				}
				vector<string> rest;
				if (j != right_part.size())
					rest = vector<string>(right_part.begin() + j + 1, right_part.end());
				else break;
				if (rest.empty()) {
					auto set = follow[current_term];
					follow[right_part[j]].insert(set.begin(), set.end());
					j += 1;
				}
				else {
					auto first_set = first_str(rest);
					if (first_set.find('?') != first_set.end()) {
						auto set = follow[current_term];
						follow[right_part[j]].insert(set.begin(), set.end());
					}
					j += 1;
				}
			}
		}
		int x = 0;
		for (auto it : follow) {
			new_follow_set_sizes[x] = it.second.size();
			x++;
		}

		if (new_follow_set_sizes == follow_set_sizes) {
			flag = false;
		}
		else {
			follow_set_sizes = new_follow_set_sizes;
			new_follow_set_sizes = vector<int>(non_terminals.size() + 1);
		}
	}

	//Build canonical sets.
	canonical_sets.push_back(closure());
	flag = true;
	i = 0;
	size = canonical_sets.size();
	int new_size = size;
	while (flag) {
		flag = false;
		for (int k = 0; k < size; k++) {
			size = new_size;
			for (auto it1 : all_symbols) {
				auto res = go_to(canonical_sets[k], it1);
				if (!res.empty()) {
					if (find(canonical_sets.begin(), canonical_sets.end(), res) == canonical_sets.end()) {
						canonical_sets.push_back(res);
						flag = true;
						new_size = size += 1;
					}
				}
			}
		}
	}

	action = vector<map<char, string>>(canonical_sets.size());
	go = vector<map<string, int>>(canonical_sets.size());
	
	//Build goto table.
	for (int i = 0; i < go.size(); i++) {
		for (auto it : non_terminals) {
			auto res = go_to(canonical_sets[i], it);
			int index = 0;
			auto iter = find(canonical_sets.begin(), canonical_sets.end(), res);
			if (iter != canonical_sets.end())
				go[i][it] = iter - canonical_sets.begin();
		}
	}	
	cout << "Goto: \n";
	for (int i = 0; i < go.size(); i++) {
		cout << i << ": \n";
		for (auto it : go[i]) {
			cout << it.first << " " << it.second << "\n";
		}
	}

	//Build action table.
	for (int i = 0; i < action.size(); i++) {
		for (auto it : canonical_sets[i]) {
			vector<string> right = it.r.second;
			auto iter = find(right.begin(), right.end(), next);
			if (iter == right.end() - 1) {
				if (it.r.first.compare(S_) != 0) {
					vector<string> right_part(right.begin(), iter);
					Rule current(it.r.first, right_part);
					int Rule_number = -1;
					bool flag = false;
					for (int j = 1; (j < grammar_rules.size()) && !flag; j++)
					if (grammar_rules[j] == current) {
						Rule_number = j;
						flag = true;
					}
					if (Rule_number != -1)
					for (auto it1 : follow[it.r.first])
						action[i][it1] = "r" + to_string(Rule_number);
				}
			} else {
				if (iter != right.end()) {
					string next_terminal = *(++iter);
					if (terminals.find(next_terminal[0]) != terminals.end()) {
						set<Rule> res = go_to(canonical_sets[i], next_terminal);
						vector<set<Rule>>::iterator iter = find(canonical_sets.begin(), canonical_sets.end(), res);
						if (iter != canonical_sets.end())
							action[i][next_terminal[0]] = "s" + to_string(iter - canonical_sets.begin());
					}
				}
			}
			vector<string> right_S = { S, next };
			Rule r(S_, right_S);
			if (it == r)
				action[i]['$'] = "acc";
		}
	}
	cout << "Action: \n";
	for (int i = 0; i < go.size(); i++) {
		cout << i << ": \n";
		for (auto it : action[i])
			cout << it.first << " " << it.second << "\n";
	}
}

void SLR::run(const string& line) {
	string word = line + "$";
	while (!st.empty())
		st.pop();
	if (match(word))
		cout << "YES\n";
	else
		cout << "NO\n";
}

void SLR::read_grammar(const string& file_name) {
	ifstream fin;
	fin.open(file_name, ifstream::in);
	char Rules_number;
	fin.get(Rules_number);
	fin.get();
	int Rule_num = Rules_number - 48;
	grammar_rules = vector<Rule>(Rule_num + 1);
	for (int i = 1; i <= Rule_num; i++) {
		string line;
		char* str_line = new char[20];
		fin.getline(str_line, 20);
		line = str_line;
		string left_part = "";
		int j = 0;
		while (j < line.length() && line[j] != '_') {
			left_part += line[j];
			j += 1;
		}
		j += 1;
		vector<string> right_part;
		string cur;
		while (j < line.length()) {
			while (line[j] != '_' && j < line.length()) {
				cur += line[j];
				j += 1;
			}
			j += 1;
			right_part.push_back(cur);
			cur = "";
		}
		Rule gr(left_part, right_part);
		grammar_rules[i] = gr;
		delete[] str_line;
	}

	fin.close();
}

bool SLR::match(const string& word) {
	st.push("0");
	int i = 0;
	while (true) {
		char current = word[i];
		int state = stoi(st.top());
		if (action[state].find(current) == action[state].end())
			return false;
		string now_action = action[state].at(current);
		//Shift
		if (now_action[0] == 's') {
			string state;
			int k = 1;
			for (; k < now_action.length(); k++)
				state[k - 1] = now_action[k];
			int new_state = stoi(state);
			char* str = new char[2];
			str[0] = current;
			str[1] = '\0';
			string string(str);
			delete[] str;
			st.push(string);
			st.push(to_string(new_state));
			i += 1;
		} else {
			// Reduce
			if (now_action[0] == 'r') {
				string state;
				int k = 1;
				for (; k < now_action.length(); k++) {
					state[k - 1] = now_action[k];
				}
				int Rule_num = stoi(state);
				int len = grammar_rules[Rule_num].r.second.size();
				string left_part = grammar_rules[Rule_num].r.first;
				for (int i = 1; i <= 2 * len; i++) {
					st.pop();
				}
				int now_state = stoi(st.top());
				st.push(left_part);
				if (go[now_state].find(left_part) == go[now_state].end()) {
					return false;
				}
				int new_state = go[now_state].at(left_part);
				if (new_state > 0) {
					st.push(to_string(new_state));
				}
			} else {
				// Access
				if (now_action == "acc") {
					return true;
				} else {
					return false;
				}
			}				
		}
	}
}

set<char> SLR::first_str(const vector<string>& line) {
	set<char> result;
	bool flag = false;
	for (auto it : line) {
		result.insert(first[it].begin(), first[it].end());
		if (result.find('?') == result.end()) {
			flag = true;
			break;
		}
		else
			result.erase('?');
	}
	if (!flag)
		result.insert('?');
	return result;
}

void SLR::handle_line(int& index, const vector<string>& line, bool& end_line) {
	int j = index;
	while (j < line.size() && find(non_terminals.begin(), non_terminals.end(), line[j]) == non_terminals.end()) {
		j += 1;
	}
	index = j;
	vector<string> rest;
	if (index != line.size())
		rest = vector<string>(line.begin() + j + 1, line.end());
	if (rest.empty())
		end_line = true;
	else
	if (!end_line) {
		set<char> current = first_str(rest);
		if (current.find('?') != current.end())
			current.erase('?');
		follow[line[index]].insert(current.begin(), current.end());
		index += 1;
		handle_line(index, line, end_line);
	}
}

void SLR::add_rules(const string& non_term, set<Rule>& result, set<string>& used_non_term) {
	int i = 1;
	used_non_term.insert(non_term);
	while (i < grammar_rules.size()) {
		string left_part = grammar_rules[i].r.first;
		if (grammar_rules[i].r.first.compare(non_term) == 0) {
			vector<string> right_part;
			right_part.push_back(next);
			auto right = grammar_rules[i].r.second;
			if (!right.empty()) {
				for (auto it : right)
					right_part.push_back(it);
				Rule new_Rule(left_part, right_part);
				result.insert(new_Rule);
				int j = 1;
				string new_non_term = right_part[1];
				if (used_non_term.find(new_non_term) == used_non_term.end()
					&& non_terminals.find(new_non_term) != non_terminals.end())
					add_rules(new_non_term, result, used_non_term);
			}
			else {
				Rule new_Rule(left_part, right_part);
				result.insert(new_Rule);
				i += 1;
			}
		}
		i += 1;
	}
}

set<Rule> SLR::closure() {
	set<Rule> result;
	set<string> used;
	int i = 1;
	vector<string> right_part = { next, S };
	Rule S_Rule(S_, right_part);
	result.insert(S_Rule);
	add_rules(S, result, used);
	return result;
}

set<Rule> SLR::closure(const set<Rule>& Rules) {
	set<Rule> result;
	result.insert(Rules.begin(), Rules.end());
	set<string> used;
	for (auto it : Rules) {
		auto right = it.r.second;
		auto iter = find(right.begin(), right.end(), next);
		if (iter != right.end() && iter != right.end() - 1) {
			string new_non_term = *(++iter);
			if (non_terminals.find(new_non_term) != non_terminals.end()) {
				add_rules(new_non_term, result, used);
			}
		}
	}
	return result;
}

set<Rule> SLR::go_to(const set<Rule>& Rules, const string& symbol) {
	set<Rule> result;
	set<Rule> items;
	for (auto it : Rules) {
		auto right = it.r.second;
		auto iter = find(right.begin(), right.end(), next);
		if (iter != right.end() && iter != right.end() - 1) {
			string new_symbol = *(++iter);
			iter--;
			if (new_symbol.compare(symbol) == 0) {
				vector<string> right_part;
				string left = "";
				for (auto i = right.begin(); i != iter; i++) {
					right_part.push_back(*i);
				}
				right_part.push_back(symbol);
				right_part.push_back(next);

				iter += 2;;
				while (iter != right.end()){
					right_part.push_back(*iter);
					iter++;
				}
				Rule new_item(it.r.first, right_part);
				items.insert(new_item);
			}
		}
	}
	return closure(items);
}
	