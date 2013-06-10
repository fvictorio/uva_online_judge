#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <climits>
#include <map>
using namespace std;

typedef unsigned long long ull;

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

bool comp(pair<int,int> a, pair<int,int> b) {
    return a.second > b.second;
}

void construct_candidates(vector<ull> &adj, int n, vector<pair<int,int> > &aux, int &cand_max_adj, int last_elim, int &no_cands) {
    aux[0] = pair<int,int>(0,0);

    for (int i = 1; i <= n; i++) {
        int cant_bits = count_bits(adj[i]);

        if (cant_bits > 0) {
            no_cands++;
        }

        aux[i] = pair<int,int>(i,cant_bits);
    }

    sort(aux.begin(), aux.end(), comp);
    
    cand_max_adj = aux[0].second;
}

void backtrack(vector<ull> &adj, int n, int elim, int &maxx, ull accum, ull obj, int last_elim) {
    int cand_max_adj;
    vector<pair<int,int> > candidates(n+1);
    vector<ull> new_adj;

    if (elim >= maxx) {
        return;
    }

    if (accum == obj) {
        if (elim < maxx) {
            maxx = elim;
        }
        return;
    }

    int no_cands = 0;
    construct_candidates(adj,n,candidates,cand_max_adj,last_elim,no_cands);

    if (cand_max_adj == 1) {
        elim += no_cands;
        if (elim < maxx) {
            maxx = elim;
        }
        return;
    }

    int min_left = no_cands / (cand_max_adj+1);
    if (no_cands % (cand_max_adj+1) > 0) {
        min_left++;
    }

    if (elim + min_left >= maxx) {
        return;
    }


    for (int i = 0; i < no_cands; i++) {
        if (candidates[i].second == 1) return;
        new_adj = adj;
        int cand = candidates[i].first;

        ull new_accum = accum | adj[cand];

        for (int k = 1; k <= n; k++) {
            new_adj[k] = (~adj[cand]) & new_adj[k];
        }

        backtrack(new_adj, n, elim+1, maxx, new_accum, obj, cand);
    }
}

int main () {
    int n, m;
    int a, b;
    vector<ull> adj;
    int maxx;
    ull obj;

    initialize();
    
    while (cin >> n >> m && n != 0) {
        adj.clear();
        adj.resize(n+1, 0);
        
        obj = 0;
        for (int i = 1; i <= n; i++) {
            obj |= one_bit[i];
            adj[i] |= one_bit[i];
        }

        for (int i = 1; i <= m; i++) {
            cin >> a >> b;
            adj[a] |= one_bit[b];
            adj[b] |= one_bit[a];
        }

        maxx = INT_MAX;
        backtrack(adj, n, 0, maxx, 0, obj, 0);
        cout << maxx << endl;
    }

    return 0;
}
