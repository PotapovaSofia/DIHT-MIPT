#include <iostream>
#include <stack>
#include <climits>
#include <utility>

using namespace std;

int regularHandler(const string& alpha, char x) {
    stack< pair <int, int> > myStack;
    for (int i = 0; i < alpha.length(); i++) {
        switch (alpha[i]) {
            case '1':
            	myStack.push(make_pair(0, 0));
                break;
            case '+': {
            	if (myStack.size() < 2) {
                	throw exception();
                }
                pair<int, int> second = myStack.top();
                myStack.pop();
                pair<int, int> first = myStack.top();
                myStack.pop();
                int maxWordLength = max(first.first, second.first);
                int maxSuffixLength = max(first.second, second.second);
                myStack.push(make_pair(maxWordLength, maxSuffixLength));
            }
                break;
            case '.': {
                if (myStack.size() < 2) {
                	throw exception();
                }
                pair<int, int> second = myStack.top();
                myStack.pop();
                pair<int, int> first = myStack.top();
                myStack.pop();
                int maxWordLength, maxSuffixLength;
                if (first.first >= 0 && second.first >= 0) {
                    if (first.first == INT_MAX || second.first == INT_MAX) {
                        maxWordLength = INT_MAX;
                    } else {
                        maxWordLength = first.first + second.first;
                    }
                } else {
                    maxWordLength = INT_MIN;
                }
                if (second.first >= 0) {
                	int tmp;
                	if (second.first == INT_MAX) {
                		tmp = INT_MAX;
                	} else {
                		tmp = first.second + second.first;
                	}
                    maxSuffixLength = max(tmp, second.second);
                }
                else {
                    maxSuffixLength = second.second;
                }
                myStack.push(make_pair(maxWordLength, maxSuffixLength));
            }
                break;
            case '*': {
                if (myStack.size() < 1) {
                	throw exception();
                }
                pair<int, int> first = myStack.top();
                myStack.pop();
                int maxWordLength;
                if (first.first > 0) {
                    maxWordLength= INT_MAX;
                } else {
                    maxWordLength = 0;
                }
                int maxSuffixLength = first.second;
                myStack.push(make_pair(maxWordLength, maxSuffixLength));
            }
                break;
            default:
                if (alpha[i] != 'a' && alpha[i] != 'b' && alpha[i] != 'c') {
                	throw exception();
                }
            	if (alpha[i] == x) {
                	myStack.push(make_pair(1, 1));
                } else {
                	myStack.push(make_pair(INT_MIN, 0));
                }
                break;
        }
    }
    if (myStack.size() != 1) {
        throw exception();
    }
    pair<int, int> result = myStack.top();
    return max(result.first, result.second);
}

int main() {
    string alpha;
    char x;
    int k;
    cin >> alpha >> x >> k;
    int result;
    try {
    	result = regularHandler(alpha, x);
    } catch (exception e) {
    	cout << alpha << ": Not a regular expression" << endl;
    	exit(1);
    }
	if (result >= k) {
		cout << "YES" << endl;
	}
	else {
		cout << "NO" << endl;
	}
    return 0;
}
