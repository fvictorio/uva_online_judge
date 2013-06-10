#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <climits>
#include <map>
using namespace std;

bool comp(pair<int,int> a, pair<int,int> b) {
    return a.second < b.second;
}

vector<int> construct_candidates(vector<vector<int> > &edges, int n, set<int> &disp) {
    vector<int> result(disp.size());
    vector<pair<int,int> > aux(disp.size());

    int i = 0;
    for (set<int>::iterator sit = disp.begin(); sit != disp.end(); sit++) {
        aux[i] = pair<int,int>(*sit,0);
        for (int k = 0; k < n; k++) {
            if (edges[*sit][k] == 1) {
                aux[i].second++;
            }
        }
        i++;
    }

    sort(aux.begin(), aux.end(), comp);

    for (i = 0; i < aux.size(); i++) {
        result[i] = aux[i].first;
    }
    return result;
}

void backtrack(vector<vector<int> > &edges, int n, int elim, int &maxx, set<int> &disp) {
    if (elim > maxx) {
        return;
    }

    if (disp.empty()) {
        if (elim < maxx) {
            maxx = elim;
        }
        return;
    }

    vector<int> candidates = construct_candidates(edges, n, disp);

    for (int c = 0; c < candidates.size(); c++) {
        int cand = candidates[c];
        disp.erase(cand);
        for (int i = 0; i < n; i++) {
            if (edges[cand][i] == 1 && disp.find(i) != disp.end()) {
                disp.erase(i);
            }
        }

        backtrack(edges, n, elim+1, maxx, disp);

        disp.insert(cand);
        for (int i = 0; i < n; i++) {
            if (edges[cand][i] == 1) {
                disp.insert(i);
            }
        }
    }
}

int main () {
    int n, m;
    int a, b;
    int maxx;
    vector<vector<int> > edges;
    set<int> disp;
    
    while (cin >> n >> m && n != 0) {
        edges.clear();
        edges.resize(n, vector<int>(n, 0));

        disp.clear();
        for (int i = 0; i < n; i++) {
            disp.insert(i);
        }

        for (int i = 1; i <= m; i++) {
            cin >> a >> b;
            a--; b--;
            edges[a][b] = 1;
            edges[b][a] = 1;
        }
        
        maxx = INT_MAX;
        backtrack(edges, n, 0, maxx, disp);

        cout << maxx << endl;
    }

    return 0;
}
