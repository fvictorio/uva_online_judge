#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <climits>

#define DEBUG

using namespace std;

const int MAX_WORD_LENGTH = 16;

bool doublets(string &a, string &b) {
    int dif = 0;

    #ifdef DEBUG
        assert(a.size() == b.size());
    #endif

    for (int i = 0; i < a.size() && dif < 2; i++) {
        if (a[i] != b[i]) dif++;
    }

    return (dif == 1);
}

class Graph {
public:
    Graph() {}
    Graph(vector<string> w) {
        words = w;
        #ifdef DEBUG
            bool all_equal_size = true;
            for (int i = 1; i < words.size() && all_equal_size; i++) {
                if (words[i].size() != words[i-1].size()) {
                    all_equal_size = false;
                }
            }
            assert(all_equal_size);
        #endif
        n = words.size();
        construct_graph();
    }

    void construct_graph();
    void print_graph();
    vector<string> shortest_path(string a, string b);
    vector<int> shortest_path(int a, int b);

    vector<string> words;
    map<int,vector<int> > edges;
    int n;
};

void Graph::construct_graph() {
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (words[i] == "pnabmz" && words[j] == "puagmr") { //XXX
                int foo = 2;
            }
            if (doublets(words[i], words[j])) {
                edges[i].push_back(j);
                edges[j].push_back(i);
            }
        }
    }
}

void Graph::print_graph() {
    for (int i = 0; i < n; i++) {
        string &word = words[i];
        vector<int> &neighbours = edges[i];
        for (int j = 0; j < neighbours.size(); j++) {
            string &neighbour = words[neighbours[j]];
            cout << word << " -> " << neighbour << endl;
        }
    }
}

bool inside(set<int> &s, int x) {
    return (s.find(x) != s.end());
}

vector<int> Graph::shortest_path(int s, int d) {
    int v;
    set<int> seen;
    vector<int> distance(n, INT_MAX);
    vector<int> parent(n, -1);
    vector<int> path;

    distance[s] = 0;
    v = s;
    
    while (v != d) {
        seen.insert(v);

        vector<int> &neighbours = edges[v];
        for (int i = 0; i < neighbours.size(); i++) {
            int w = neighbours[i];
            if (distance[w] > (distance[v]+1)) {
                distance[w] = distance[v]+1;
                parent[w] = v;
            }
        }

        int dist = INT_MAX;
        bool all_seen = true;
        for (int i = 0; i < n; i++) {
            if (!inside(seen, i) && (dist > distance[i])) {
                dist = distance[i];
                v = i;
                all_seen = false;
            }
        }
        // Si ya vi todos y no me cruce a d,
        // entonces devuelvo un vector vacio
        // (que es mi forma de decir:
        //  "no hubo suerte, macho").
        if (all_seen) return vector<int>();
    }

    v = d;
    path.push_back(v);
    while (v != s) {
        v = parent[v];
        path.push_back(v);
    }

    return path;
}

vector<string> Graph::shortest_path(string a, string b) {
    vector<string>::iterator it_s, it_d;
    int s, d;

    it_s = find(words.begin(), words.end(), a);
    it_d = find(words.begin(), words.end(), b);

    #ifdef DEBUG
        assert(it_s != words.end());
        assert(it_d != words.end());
    #endif

    s = it_s - words.begin();
    d = it_d - words.begin();

    // Lo llamo al reves porque me devuelve
    // el camino desde el segundo argumento
    // al primero (por eficiencia al agregar
    // al vector un elemento)
    vector<int> result = shortest_path(d, s);
    vector<string> str_result(result.size());

    for (int i = 0; i < result.size(); i++) {
        str_result[i] = words[result[i]];
    }

    return str_result;
}

int main () {
    vector<vector<string> > dictionaries(MAX_WORD_LENGTH+1);
    vector<Graph> graphs(MAX_WORD_LENGTH+1);
    string line;
    string a, b;

    while (getline(cin, line) && line != "") {
        dictionaries[line.size()].push_back(line);
    }

    for (int i = 1; i <= MAX_WORD_LENGTH; i++) {
        sort(dictionaries[i].begin(), dictionaries[i].end());
        graphs[i] = Graph(dictionaries[i]);
        if (false && !dictionaries[i].empty()) { //XXX
            cout << "Size " << i << ":" << endl;
            graphs[i].print_graph();
            cout << endl;
        }
    }

    bool first = true;
    while (getline(cin, line)) {
        if (first) first = false;
        else cout << endl;

        stringstream ss;
        ss << line;
        ss >> a >> b;
        if (a.size() != b.size()) {
            cout << "No solution." << endl;
        }
        else {
            vector<string> path = graphs[a.size()].shortest_path(a, b);
            if (path.empty()) {
                cout << "No solution." << endl;
            }
            else {
                for (int i = 0; i < path.size(); i++) {
                    cout << path[i] << endl;
                }
            }
        }
    }
}
