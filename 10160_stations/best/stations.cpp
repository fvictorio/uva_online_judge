#include <iostream>
#include <vector>
#include <set>
#include <climits>
#include <map>
#include <algorithm>
using namespace std;

typedef unsigned long long ull;

const int MAX_N = 35;
vector<ull> one_bit;
set<ull> memoize;

vector<int> no_adjs;
vector<int> cands;

void initialize() {
    one_bit.resize(MAX_N+1);
    ull aux = 1;
    for (int i = 0; i <= MAX_N; i++) {
        one_bit[i] = aux;
        aux <<= 1;
    }
}

inline int count_bits (ull a) {
    int cant = 0;
    
    while (a) {
        if (a & 1) {
            cant++;
        }
        a >>= 1;
    }

    return cant;
}

bool comp(pair<int,int> a, pair<int,int> b) {
    return a.second > b.second;
}

class Graph {
public:
    Graph () {}
    Graph (int n) {
        for (int i = 0; i < n; i++) {
            add_node(i);
        }
    }
    void add_node(int n) {
        nodes.insert(n);
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
        set<int> &neigh = edges[n];
        set<int>::iterator it;
        for (it = neigh.begin(); it != neigh.end(); it++) {
            if (*it == n) continue;
            remove_node_and_edges(*it);
            //if (nodes.find(*it) != nodes.end()) {
            //    nodes.erase(*it);
            //}
        }
        remove_node_and_edges(n);
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
    void eliminar_puntas(int &elim) {
        int a, b, c;
        set<int> to_erase;
        set<int>::iterator it, it2;

        bool flag;
        while (nodes.size() > 3) {
            flag = true;
            to_erase.clear();
            for (it = nodes.begin(); it != nodes.end(); it++) {
                a = *it;
                if (get_no_edges(a) != 2) continue;

                it2 = edges[a].begin();
                if (*it2 == a) it2++;
                b = *it2;
                if (get_no_edges(b) != 3) continue;

                it2 = edges[b].begin();
                if (*it2 == a || *it2 == b) it2++;
                if (*it2 == a || *it2 == b) it2++;
                c = *it2;
                if (get_no_edges(c) != 3) continue;

                to_erase.insert(b);
                flag = false;
            }

            if (flag) break;

            for (it = to_erase.begin(); it != to_erase.end(); it++) {
                remove_node_zone(*it);
            }

            elim += to_erase.size();
        }
    }


    int get_no_edges(int n) { return edges[n].size(); }
    unsigned int size() { return nodes.size(); }
    bool empty() { return nodes.empty(); }

    set<int> nodes;
    map<int,set<int> > edges;
};

int greedy (Graph graph) {
    int cant = 0;
    while (!graph.empty()) {
        graph.remove_max_node();
        cant++;
    }
    return cant;
}

Graph graph_sort(Graph graph) {
    Graph new_graph(graph.size());
    vector<pair<int, int> > aux(graph.size());
    map<int,int> node_trad;
    map<int,int> node_inv_trad;

    set<int>::iterator it;
    int i = 0;
    for (it = graph.nodes.begin(); it != graph.nodes.end(); it++, i++) {
        aux[i] = pair<int,int>(*it, graph.get_no_edges(*it));
    }
    sort(aux.begin(), aux.end(), comp);

    for (int i = 0; i < aux.size(); i++) {
        node_trad[i] = aux[i].first;
        node_inv_trad[aux[i].first] = i;
    }

    for (it = new_graph.nodes.begin(); it != new_graph.nodes.end(); it++) {
        set<int> &neigh = graph.edges[node_trad[*it]];
        set<int>::iterator it2;
        for (it2 = neigh.begin(); it2 != neigh.end(); it2++) {
            new_graph.add_edge(*it, node_inv_trad[*it2]);
        }
    }

    return new_graph;
}

vector<Graph> get_connected (Graph graph) {
    vector<Graph> result;

    while (!graph.empty()) {
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

int get_min_left(vector<ull> &adjacencies, int cant_elim, int n) {
    int min_left = 0;
    
    int nodes_left = n - cant_elim;

    for (int i = 0; i < cands.size() && nodes_left > 0; i++) {
        nodes_left -= count_bits(adjacencies[cands[i]]);
        min_left++;
    }

    return min_left;
}

void update(vector<ull> &adj, ull erased, int n) {
    for (int i = 0; i < n; i++) {
        if (erased & one_bit[i]) {
            adj[i] = 0;
        }
        else {
            adj[i] &= adj[i] ^ erased;
        }
    }
}

void pre_magia(Graph &graph, vector<ull> &adjacencies, int &elim, int &n) {
    set<int>::iterator it;

    graph.eliminar_puntas(elim);

    graph = graph_sort(graph);

    n = graph.size();
    adjacencies.clear();
    adjacencies.resize(n);
    
    for (int i = 0; i < n; i++) {
        for (it = graph.edges[i].begin(); it != graph.edges[i].end(); it++) {
            adjacencies[i] |= one_bit[*it];
        }
    }
}

void compute_no_adjs(vector<ull> &adj, vector<int> &no_adjs, ull erased) {
    for (int i = 0; i < adj.size(); i++) {
        if ((erased & one_bit[i]) != 0) continue;
        no_adjs[i] = count_bits(adj[i]);
    }
}

bool comp2(int a, int b) {
    return no_adjs[a] > no_adjs[b];
}

void construct_candidates(vector<ull> &adjacencies, ull erased, int n, int cant_elim)  {
    int k = 0;
    for (int i = 0; i < n; i++) {
        no_adjs[i] = count_bits(adjacencies[i]);
        cands[k++] = i;
    }

    sort(cands.begin(), cands.begin()+n-cant_elim, comp2);
}

void magia(vector<ull> &adjacencies, int n, ull list_elim, ull erased, int elim, int cant_elim, int &maxx) {
    if (cant_elim == n) {
        if (elim < maxx) {
            maxx = elim;
        }
        return;
    }

    if (elim + 1 >= maxx) {
        return;
    }

    construct_candidates(adjacencies, erased, n, cant_elim);

    int min_left = get_min_left(adjacencies, cant_elim, n);
    if (elim + min_left >= maxx) {
        return;
    }
    
    for (int i = 0; i < n-cant_elim; i++) {
        int cand = cands[i];

        ull new_list_elim = list_elim | one_bit[cand];
        if (memoize.find(new_list_elim) == memoize.end()) {
            ull new_erased = erased | adjacencies[cand];
            vector<ull> new_adjacencies = adjacencies;
            update(new_adjacencies, new_erased, n);
            magia(new_adjacencies, n, new_list_elim, new_erased, elim+1, count_bits(new_erased), maxx);
            memoize.insert(new_list_elim);
        }
    }
}

int main () {
    int n, m;
    int a, b;

    initialize();

    while (cin >> n >> m && n != 0) {
        Graph graph(n);
        vector<Graph> graphs;

        vector<ull> adjacencies(n);

        for (int i = 0; i < n; i++) {
            graph.add_edge(i, i);
        }

        for (int i = 1; i <= m; i++) {
            cin >> a >> b;
            a--; b--;
            graph.add_edge(a,b);
        }

        graphs = get_connected(graph);

        int accum = 0;
        for (int i = 0; i < graphs.size(); i++) {

            ull obj = 0;
            int elim = 0;

            memoize.clear();
            no_adjs.clear();
            no_adjs.resize(n);
            cands.clear();
            cands.resize(n);

            pre_magia(graphs[i], adjacencies, elim, n);
            int maxx = greedy(graphs[i]);

            magia(adjacencies, n, 0, 0, 0, 0, maxx);

            accum += maxx+elim;
        }
        cout << accum << endl;
    }

    return 0;
}
