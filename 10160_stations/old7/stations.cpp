#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <climits>
#include <map>
using namespace std;

typedef unsigned long long ull;
#define one_bit(x) (((ull) 1) << x)

struct Aux {
    Aux() {}
    Aux(int n, int a) {
        node = n;
        no_adjs = a;
    }
    int node;
    int no_adjs;
};

set<ull> memoize;
const int MAX_N = 35;

class Graph {
public:
    void add_node(int n) {
        nodes.insert(n);
    }
    void add_nodes(set<int> &s) {
        nodes.insert(s.begin(), s.end());
    }
    void add_edge(int a, int b) {
        edges[a].insert(b);
        edges[b].insert(a);
    }
    void add_edges(int n, set<int> &e) {
        set<int>::iterator it;
        for (it = e.begin(); it != e.end(); it++) {
            add_edge(n, *it);
        }
    }
    void remove_node(int n) {
        nodes.erase(n);
    }
    void remove_node_and_edges(int n) {
        set<int> &neigh = edges[n];
        set<int>::iterator it;
        for (it = neigh.begin(); it != neigh.end(); it++) {
            if (n == *it) {
                continue;
            }
            edges[*it].erase(n);
        }

        nodes.erase(n);
        edges.erase(n);
    }
    void remove_node_zone(int n) {
        nodes.erase(n);
        set<int> &neigh = edges[n];
        set<int>::iterator it;
        for (it = neigh.begin(); it != neigh.end(); it++) {
            if (nodes.find(*it) != nodes.end()) {
                nodes.erase(*it);
            }
        }
    }
    void remove_node_zone(int n, set<int> &erased_nodes) {
        nodes.erase(n);
        set<int> &neigh = edges[n];
        set<int>::iterator it;
        for (it = neigh.begin(); it != neigh.end(); it++) {
            if (*it == n) continue;
            if (nodes.find(*it) != nodes.end()) {
                nodes.erase(*it);
                erased_nodes.insert(*it);
            }
        }
    }
    void remove_max_node() {
        set<int>::iterator it;
        int max_adj = -1;
        int max_node;
        for (it = nodes.begin(); it != nodes.end(); it++) {
            int no_adj = edges[*it].size();
            if (no_adj > max_adj) {
                max_adj = no_adj;
                max_node = *it;
            }
        }
        remove_node_zone(max_node);
    }
    int get_no_adj (int n) {
        int cant = 0;

        set<int> &neigh = edges[n];
        set<int>::iterator it;
        for (it = neigh.begin(); it != neigh.end(); it++) {
            if (nodes.find(*it) != nodes.end()) {
                cant++;
            }
        }

        return cant;
    }
    void print() {
        set<int>::iterator it;
        for (it = nodes.begin(); it != nodes.end(); it++) {
            cout << "Node " << *it << endl;

            cout << "Neighbors: ";
            set<int> &neighbors = edges[*it];
            set<int>::iterator it2;
            for (it2 = neighbors.begin(); it2 != neighbors.end(); it2++) {
                cout << *it2 << " ";
            }
            cout << endl;
        }
    }
    
    set<int> nodes;
    map<int, set<int> > edges;
};

bool is_subset(set<int> &sa, set<int> &sb) {
    set<int>::iterator it;

    for (it = sa.begin(); it != sa.end(); it++) {
        if (sb.find(*it) == sb.end()) {
            return false;
        }
    }

    return true;
}

vector<Graph> connected_graphs(Graph graph) {
    vector<Graph> result;

    while (!graph.nodes.empty()) {
        Graph g;
        set<int> s;

        int n = *(graph.nodes.begin());
        s.insert(n);
        while (true) {
            g.add_node(n);
            g.add_edges(n, graph.edges[n]);

            s.erase(n);
            s.insert(graph.edges[n].begin(), graph.edges[n].end());
            graph.remove_node_and_edges(n);
            if (s.empty()) {
                break;
            }
            n = *(s.begin());
        }

        result.push_back(g);
    }

    return result;
}

bool comp(Aux a, Aux b) {
    return a.no_adjs > b.no_adjs;
}

int greedy (Graph graph) {
    int cant = 0;
    while (!graph.nodes.empty()) {
        graph.remove_max_node();
        cant++;
    }
    return cant;
}

void construct_candidates(Graph &graph, vector<int> &cands, int elim, int &min_left) {
    int n = graph.nodes.size();
    vector<Aux> aux(n);
    cands.clear();
    cands.resize(n);
    min_left = 0;

    int i = 0;
    set<int>::iterator it;
    for (it = graph.nodes.begin(); it != graph.nodes.end(); it++) {
        //aux[i++] = Aux(*it, graph.edges[*it].size());
        aux[i++] = Aux(*it, graph.get_no_adj(*it));
    }

    sort(aux.begin(), aux.end(), comp);

    for (i = 0; i < cands.size(); i++) {
        if (n >= 0) {
            n -= aux[i].no_adjs;
            min_left++;
        }
        cands[i] = aux[i].node;
    }
}

void magia(Graph &graph, int elim, ull list_elim, int &maxx) {
    if (graph.nodes.empty()) {
        if (elim < maxx) {
            maxx = elim;
        }
        return;
    }

    if (elim + 1 >= maxx) {
        return;
    }

    vector<int> cands;
    int min_left;
    construct_candidates(graph, cands, elim, min_left);

    //if (elim + min_left >= maxx) {
    //    return;
    //}

    for (int i = 0; i < cands.size(); i++) {
        set<int> erased_nodes;

        int cand = cands[i];
        graph.remove_node_zone(cand, erased_nodes);

        ull new_list_elim = list_elim | one_bit(cand);
        if (memoize.find(new_list_elim) == memoize.end()) {
            memoize.insert(new_list_elim);
            magia(graph,elim+1,new_list_elim,maxx);
        }
        graph.add_node(cand);
        graph.add_nodes(erased_nodes);
    }
}

int main () {
    int n, m;

    while (cin >> n >> m && n != 0) {
        Graph graph;

        for (int i = 0; i < n; i++) {
            graph.add_node(i);
            graph.add_edge(i,i);
        }

        for (int i = 1; i <= m; i++) {
            int a, b;
            cin >> a >> b;
            a--; b--;
            graph.add_edge(a, b);
        }
        
        vector<Graph> graphs = connected_graphs(graph);

        int accum = 0;
        for (int i = 0; i < graphs.size(); i++) {
            //int maxx = INT_MAX;
            int maxx = greedy(graphs[i]);
            memoize.clear();
            magia(graphs[i], 0, 0, maxx);
            accum += maxx;
        }
        cout << accum << endl;
    }

    return 0;
}
