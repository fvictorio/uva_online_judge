#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define one_bit(i) (((unsigned long long) 1) << i)

typedef unsigned long long ull;

int count_bits(ull x) {
    int cant = 0;
    while (x > 0) {
        if ((x & 1) != 0) cant++;
        x >>= 1;
    }

    return cant;
}

bool comp(pair<int,int> a, pair<int,int> b) {
    return a.second > b.second;
}

void magia(vector<int> &vertices, vector<ull> &adjacencies, vector<ull> &coverable,
            int idx, int cant_elim, int &minn, ull erased, ull obj) {
    
    if (erased == obj) {
        if (cant_elim < minn) {
            minn = cant_elim;
        }
        return;
    }
    
    if (cant_elim + 1 >= minn) {
        return;
    }

    if ( (erased | coverable[idx]) != obj ) {
        return;
    }

    int v = vertices[idx];
    magia(vertices, adjacencies, coverable, idx+1, cant_elim+1, 
          minn, erased | adjacencies[v], obj);
    magia(vertices, adjacencies, coverable, idx+1, cant_elim, 
          minn, erased, obj);
}


int main () {
    int n, m;
    int a, b;
    
    while (cin >> n >> m && n != 0) {
        // Adyacencias
        vector<ull> adjacencies(n, 0);

        for (int i = 0; i < n; i++) {
            adjacencies[i] |= one_bit(i);
        }

        for (int i = 0; i < m; i++) {
            cin >> a >> b;
            a--; b--;
            adjacencies[a] |= one_bit(b);
            adjacencies[b] |= one_bit(a);
        }

        // Vertices en orden decreciente de grado
        vector<int> vertices(n);
        vector<pair<int, int> > aux(n);
        for (int i = 0; i < n; i++) {
            aux[i].first = i;
            aux[i].second = count_bits(adjacencies[i]);
        }
        sort(aux.begin(), aux.end(), comp);
        for (int i = 0; i < n; i++) {
            vertices[i] = aux[i].first;
        }

        // Vertices que se cubren desde i hasta el ultimo
        vector<ull> coverable(n, 0);
        coverable[n-1] = adjacencies[vertices[n-1]];
        for (int i = n-2; i >= 0; i--) {
            coverable[i] = coverable[i+1];
            coverable[i] |= adjacencies[vertices[i]];
        }

        // Objetivo
        ull obj = 0;
        for (int i = 0; i < n; i++) {
            obj |= one_bit(i);
        }

        int minn = n;
        magia(vertices, adjacencies, coverable, 0, 0, minn, 0, obj);
        cout << minn << endl;
    }

    return 0;
}

