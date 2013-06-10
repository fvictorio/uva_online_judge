#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <climits>
#include <map>
using namespace std;

typedef unsigned long long ull;

set<ull> memoize;
const int MAX_N = 35;
vector<ull> one_bit;

int count_bits (ull a) {
    int cant = 0;
    
    while (a > 0) {
        if (a & 1 == 1) {
            cant++;
        }
        a >>= 1;
    }

    return cant;
}

void initialize() {
    one_bit.resize(MAX_N+1);
    ull aux = 1;
    for (int i = 1; i <= MAX_N; i++) {
        one_bit[i] = aux;
        aux <<= 1;
    }
}

set<int> ull_to_set(ull n) {
    set<int> result;

    int aux = 1;
    while (n > 0) {
        if (n & 1 == 1) {
            result.insert(aux);
        }
        n >>= 1;
        aux++;
    }

    return result;
}

class Graph {
public:
    Graph () {}
    void add_edge(int a, int b) {
        add_node(a);
        add_node(b);
        edges[a] |= one_bit[b];
        edges[b] |= one_bit[a];
    }
    void add_edges(int n, ull e);
    void add_edges(int n, set<int> &e);
    void add_node(int n);
    void remove_node(int i);
    void remove_max_node();
    void remove_node_zone(int i);
    void delete_dominants();
    bool empty() {return nodes.empty();}
    //void print() {
    //    set<int>::iterator it;
    //    for (it = nodes.begin(); it != nodes.end(); it++) {
    //        cout << "Node " << *it << endl;

    //        cout << "Neighbors: ";
    //        set<int> &neighbors = edges[*it];
    //        set<int>::iterator it2;
    //        for (it2 = neighbors.begin(); it2 != neighbors.end(); it2++) {
    //            cout << *it2 << " ";
    //        }
    //        cout << endl;
    //    }
    //}

    set<int> nodes;
    map<int,ull> edges;
};

void Graph::add_node(int n) {
    nodes.insert(n);
    edges[n] |= one_bit[n];
}

void Graph::add_edges(int n, set<int> &e) {
    set<int>::iterator it;
    for (it = e.begin(); it != e.end(); it++) {
        add_edge(n, *it);
    }
}

void Graph::add_edges(int n, ull e) {
    edges[n] |= e;
}

void Graph::remove_node(int i) {
    set<int> neighbors = ull_to_set(edges[i]);

    set<int>::iterator it;
    for (it = neighbors.begin(); it != neighbors.end(); it++) {
        if (*it == i) {
            continue;
        }
        edges[*it] &= (~one_bit[i]);
    }

    //set<int>::iterator it;
    //for (it = nodes.begin(); it != nodes.end(); it++) {
    //    edges[*it] &= (~one_bit[i]);
    //}

    edges.erase(i);
    nodes.erase(i);
}

void Graph::remove_node_zone(int i) {
    set<int> neighbors = ull_to_set(edges[i]);

    set<int>::iterator it;
    for (it = neighbors.begin(); it != neighbors.end(); it++) {
        if (*it == i) {
            continue;
        }
        remove_node(*it);
    }

    remove_node(i);
}

void Graph::remove_max_node() {
//    int maxx = -1;
//    int nmaxx;
//
//    set<int>::iterator it;
//    for (it = nodes.begin(); it != nodes.end(); it++) {
//        int no_adj = edges[*it].size();
//        if (maxx < no_adj) {
//            nmaxx = *it;
//            maxx = no_adj;
//        }
//    }
//
//    remove_node_zone(nmaxx);
}

bool is_subset(ull a, ull b) {
    return (a&b == a);
}

void Graph::delete_dominants() {
    set<int> removable;
    set<int> dominant;
    set<int>::iterator it1, it2;

    for (it1 = nodes.begin(); it1 != nodes.end(); it1++) {
        if (dominant.find(*it1) != dominant.end()) {
            continue;
        }

        if (removable.find(*it1) != removable.end()) {
            continue;
        }

        for (it2 = nodes.begin(); it2 != nodes.end(); it2++) {
            if (*it2 == *it1) {
                continue;
            }
            if (removable.find(*it2) != removable.end()) {
                continue;
            }
            
            if (is_subset(edges[*it1], edges[*it2])) {
                removable.insert(*it1);
                dominant.insert(*it2);
            }
        }
    }

    for (it1 = removable.begin(); it1 != removable.end(); it1++) {
        remove_node(*it1);
    }
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
            set<int> neighbors = ull_to_set(graph.edges[n]);
            g.add_edges(n, neighbors);

            s.erase(n);
            s.insert(neighbors.begin(), neighbors.end());
            graph.remove_node(n);
            if (s.empty()) {
                break;
            }
            n = *(s.begin());
        }

        result.push_back(g);
    }

    return result;
}

bool comp(pair<int, int> a, pair<int, int> b) {
    return a.second > b.second;
}

void construct_candidates(Graph &graph, vector<int> &candidates) {
    vector<pair<int, int> > aux;

    set<int>::iterator it;
    for (it = graph.nodes.begin(); it != graph.nodes.end(); it++) {
        aux.push_back(pair<int,int>(*it,count_bits(graph.edges[*it])));
    }

    sort(aux.begin(), aux.end(), comp);

    for (int i = 0; i < aux.size(); i++) {
        candidates.push_back(aux[i].first);
    }
}

int get_min_left(Graph &graph, vector<int> &candidates) {
    int no_nodes = graph.nodes.size();
    int min_left = 0;

    int i = 0;
    while (no_nodes > 0) {
        no_nodes -= count_bits(graph.edges[candidates[i]]);
        i += 1;
        min_left += 1;
    }

    return min_left;
}

void magia(Graph &graph, int elim, ull list_elim, int &maxx) {
    if (graph.empty()) {
        if (elim < maxx) {
            maxx = elim;
        }
        return;
    }

    if (elim + 1 >= maxx) {
        return;
    }

    vector<int> candidates;
    construct_candidates(graph, candidates);

    int min_left = get_min_left(graph, candidates);
    if (elim + min_left >= maxx) {
        return;
    }

    for (int i = 0; i < candidates.size(); i++)  {
        Graph subgraph = graph;
        int cand = candidates[i];
        subgraph.remove_node_zone(cand);
    
        ull new_list_elim = list_elim | one_bit[cand];
        if (memoize.find(new_list_elim) != memoize.end()) {
            continue;
        }
        memoize.insert(new_list_elim);

        magia(subgraph, elim+1, new_list_elim, maxx);
    }
}

int greedy(Graph graph) {
    int count = 0;

    while (!graph.empty()) {
        graph.remove_max_node();
        count++;
    }

    return count;
}

int main () {
    int n, m;

    initialize();

    while (cin >> n >> m && n != 0) {
        Graph graph;

        for (int i = 1; i <= n; i++) {
            graph.add_node(i);
        }

        for (int i = 1; i <= m; i++) {
            int a, b;
            cin >> a >> b;
            graph.add_edge(a, b);
        }

        vector<Graph> graphs = connected_graphs(graph);

        int accum = 0;
        for (int i = 0; i < graphs.size(); i++) {
            //int maxx = greedy(graphs[i]);
            int maxx = INT_MAX;
            memoize.clear();
            magia(graphs[i], 0, 0, maxx);
            accum += maxx;
        }
        cout << accum << endl;
    }

    return 0;
}
