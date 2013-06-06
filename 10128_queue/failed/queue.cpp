#include <iostream>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

const int MAX_N = 8;

class Triple {
public:
    Triple() {}
    Triple(int a_, int b_, int c_) : a(a_), b(b_), c(c_) {}

    bool operator< (Triple T) const {
        if (this->a != T.a) return this->a < T.a;
        if (this->b != T.b) return this->b < T.b;
        return this->c < T.c;
    }
    
    int a, b, c;
};

set<int> range(int n) {
    set<int> result;

    for (int i = 1; i <= n; i++) {
        result.insert(i);
    }

    return result;
}

int set_max(set<int> &s) {
    int curr_max = 0;

    for (set<int>::iterator it = s.begin(); it != s.end(); it++) {
        if (*it > curr_max) curr_max = *it;
    }

    return curr_max;
}

bool valid(int v, int n, int dp, int dr, int p, int r, int curr_max, set<int> &disp) {
    int cant_mayores = 0;

    for (set<int>::iterator it = disp.begin(); it != disp.end(); it++) {
        if (*it > curr_max) {
            cant_mayores++;
        }
    }

    if (cant_mayores == 0) {
        if (p != dp) {
            return false;
        }
    }
    else {
        int min_p = p+1;
        int max_p = p+cant_mayores;
        if ((dp < min_p) || (dp > max_p)) {
            return false;
        }
    }

    if (disp.empty()) {
        if (r != dr) {
            return false;
        }
    }
    else {
        int min_r = r + 1;
        int max_r = r + disp.size();
        if ((dr < min_r) || (dr > max_r)) {
            return false;
        }
    }

    return true;
}

int backtrack(int v, int n, int dp, int dr, int p, int r, int curr_max, set<int> &disp) {
    if (v > curr_max) {
        curr_max = v;
        p++;
    }
    if (v > set_max(disp)) {
        r++;
    }

    if (p > dp || r > dr) {
        return 0;
    }

    if (disp.empty()) {
        if ((p == dp) && (r == dr)) {
            return 1;
        }
        else {
            return 0;
        }
    }

    if (!valid(v,n,dp,dr,p,r,curr_max,disp)) {
        return 0;
    }

    int accum = 0;
    for (set<int>::iterator it = disp.begin(); it != disp.end(); it++) {
        disp.erase(*it);
        accum += backtrack(*it,n,dp,dr,p,r,curr_max,disp);
        disp.insert(*it);
    }
    
    return accum;
}

void initialize (map<Triple, int> &mapa) {
    for (int n = 1; n <= MAX_N; n++) {
        for (int p = 1; p <= n; p++) {
            for (int r = 1; r <= p; r++) {
                set<int> disp = range(n);
                int res = backtrack(0,n,p,r,0,0,0,disp);
                mapa[Triple(n,p,r)] = res;
                mapa[Triple(n,r,p)] = res;
            }
        }
    }
}

int main () {
    int T;
    int n, dp, dr;
    map<Triple, int> mapa;

    initialize(mapa);
    cout << "Done." << endl; //XXX

    cin >> T;
    for (int i = 1; i <= T; i++) {
        cin >> n >> dp >> dr;
        cout << mapa[Triple(n,dp,dr)] << endl;
        //set<int> disp = range(n); //XXX
        //cout << backtrack(0,n,dp,dr,0,0,0,disp) << endl;
    }

    return 0;
}
