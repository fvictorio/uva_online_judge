#include <iostream>
#include <vector>
#include <string>
using namespace std;

void get_rules(int id, vector<int> &rules) {
    //for (int i = 7; i >= 0; i--) {
    for (int i = 0; i < 8; i++) {
        rules[i] = (id % 2 == 0 ? 0 : 1);
        id /= 2;
    }
}

void get_state(string &str_state, vector<int> &state, int n) {
    for (int i = 0; i < n; i++) {
        state[i] = str_state[i] - '0';
    }
}

bool check(int p1, int p2, int p3, int a, vector<int> &rules) {
    int rule = 4*p1+2*p2+p3;
    return rules[rule] == a;
}

bool backtrack(vector<int> &rules, vector<int> &state, int n, vector<int> &prev_state, int i) {
    if (!check(prev_state[i-3], prev_state[i-2], prev_state[i-1], state[i-2], rules)) {
        return false;
    }

    if (i == n) {
        if (!check(prev_state[n-2], prev_state[n-1], prev_state[0], state[n-1], rules)) {
            return false;
        }
        if (!check(prev_state[n-1], prev_state[0], prev_state[1], state[0], rules)) {
            return false;
        }
        return true;
    }

    bool reachable;
    prev_state[i] = 0;
    reachable = backtrack(rules, state, n, prev_state, i+1);
    if (reachable) return true;
    prev_state[i] = 1;
    reachable = backtrack(rules, state, n, prev_state, i+1);
    if (reachable) return true;

    return false;
}

bool magia(vector<int> &rules, vector<int> &state, int n) {
    bool reachable;
    vector<int> prev_state(n);

    for (int i = 0; i < 8; i++) {
        prev_state[0] = i%2;
        prev_state[1] = (i/2)%2;
        prev_state[2] = (i/4)%2;
        reachable = backtrack(rules, state, n, prev_state, 3);
        if (reachable) return true;
    }

    return false;
}

int main () {
    int identifier;
    int n;
    string str_state;

    while (cin >> identifier >> n >> str_state) {
        vector<int> rules(8);
        get_rules(identifier, rules);
        vector<int> state(n);
        get_state(str_state, state, n);

        bool reachable = magia(rules, state, n);
        if (reachable) {
            cout << "REACHABLE" << endl;
        }
        else {
            cout << "GARDEN OF EDEN" << endl;
        }
    }
}
