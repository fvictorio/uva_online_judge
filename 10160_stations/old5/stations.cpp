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

void initialize() {
    one_bit.resize(MAX_N+1);
    ull aux = 1;
    for (int i = 1; i <= MAX_N; i++) {
        one_bit[i] = aux;
        aux <<= 1;
    }
}

class Graph {
public:
    Graph () {}
    void add_edge(int a, int b) {
        add_node(a);
        add_node(b);
        edges[a].insert(b);
        edges[b].insert(a);
    }
    void add_edges(int n, set<int> &e);
    void add_node(int n);
    void remove_node(int i);
    void remove_max_node();
    vector<int> remove_node_zone(int i);
    void delete_dominants();
    bool empty() {return nodes.empty();}
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
    map<int,set<int> > edges;
};

void Graph::add_node(int n) {
    nodes.insert(n);
    edges[n].insert(n);
}

void Graph::add_edges(int n, set<int> &e) {
    set<int>::iterator it;
    for (it = e.begin(); it != e.end(); it++) {
        add_edge(n, *it);
    }
}

void Graph::remove_node(int i) {
    //set<int> &neighbors = edges[i];

    //set<int>::iterator it;
    //for (it = neighbors.begin(); it != neighbors.end(); it++) {
    //    if (*it == i) {
    //        continue;
    //    }
    //    edges[*it].erase(i);
    //}

    edges.erase(i);
    nodes.erase(i);
}

vector<int> Graph::remove_node_zone(int i) {
    set<int> &neighbors = edges[i];
    vector<int> erased(neighbors.size() + 1);

    set<int>::iterator it;
    int k = 0;
    for (it = neighbors.begin(); it != neighbors.end(); it++) {
        if (*it == i) {
            continue;
        }
        remove_node(*it);
        erased[k] = *it;
        k++;
    }

    remove_node(i);
    erased[k] = i;
    return erased;
}

void Graph::remove_max_node() {
    int maxx = -1;
    int nmaxx;

    set<int>::iterator it;
    for (it = nodes.begin(); it != nodes.end(); it++) {
        int no_adj = edges[*it].size();
        if (maxx < no_adj) {
            nmaxx = *it;
            maxx = no_adj;
        }
    }

    remove_node_zone(nmaxx);
}

bool is_subset(set<int> &sa, set<int> &sb) {
    set<int>::iterator it;

    for (it = sa.begin(); it != sa.end(); it++) {
        if (sb.find(*it) == sb.end()) {
            return false;
        }
    }

    return true;
}

void Graph::delete_dominants() {
    set<int> removable;
    set<int> dominant;
    set<int>::iterator it1, it2;
    int to_remove;

    for (it1 = nodes.begin(); to_remove == -1 && it1 != nodes.end(); it1++) {
        if (dominant.find(*it1) != dominant.end()) {
            continue;
        }

        if (removable.find(*it1) != removable.end()) {
            continue;
        }

        for (it2 = nodes.begin(); to_remove == -1 && it2 != nodes.end(); it2++) {
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
            g.add_edges(n, graph.edges[n]);

            s.erase(n);
            s.insert(graph.edges[n].begin(), graph.edges[n].end());
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

void construct_candidates(Graph &graph, vector<int> &candidates, set<int> &erased_nodes) {
    vector<pair<int, int> > aux;

    set<int>::iterator it;
    for (it = graph.nodes.begin(); it != graph.nodes.end(); it++) {
        if (erased_nodes.find(*it) == erased_nodes.end()) {
            aux.push_back(pair<int,int>(*it,graph.edges[*it].size()));
        }
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
        no_nodes -= graph.edges[candidates[i]].size();
        i += 1;
        min_left += 1;
    }

    return min_left;
}

void magia(Graph &graph, int elim, ull list_elim, int &maxx, set<int> &erased_nodes, int n) {
    if (erased_nodes.size() == n) {
        if (elim < maxx) {
            maxx = elim;
        }
        return;
    }

    if (elim + 1 >= maxx) {
        return;
    }

    vector<int> candidates;
    construct_candidates(graph, candidates, erased_nodes);

    //int min_left = get_min_left(graph, candidates);
    //if (elim + min_left >= maxx) {
    //    return;
    //}

    for (int i = 0; i < candidates.size(); i++)  {
        int cand = candidates[i];

        erased_nodes.insert(cand);
        graph.nodes.erase(cand);
        set<int> &neighbors = graph.edges[cand];
        for (set<int>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
            erased_nodes.insert(*it);
            graph.nodes.erase(*it);
        }
    
        ull new_list_elim = list_elim | one_bit[cand];
        if (memoize.find(new_list_elim) != memoize.end()) {
            continue;
        }
        memoize.insert(new_list_elim);

        magia(graph, elim+1, new_list_elim, maxx, erased_nodes, n);

        erased_nodes.erase(cand);
        graph.nodes.insert(cand);
        for (set<int>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
            erased_nodes.erase(*it);
            graph.nodes.insert(*it);
        }
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
            //graphs[i].delete_dominants();
            memoize.clear();
            set<int> erased_nodes;
            magia(graphs[i], 0, 0, maxx, erased_nodes,graphs[i].nodes.size());
            accum += maxx;
        }
        cout << accum << endl;
    }

    return 0;
}
