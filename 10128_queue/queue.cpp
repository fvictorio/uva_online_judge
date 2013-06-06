#include <iostream>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

const int MAX_N = 8;

class Double {
public:
    Double() {}
    Double(int a_, int b_) : a(a_), b(b_) {}

    bool operator< (Double T) const {
        if (this->a != T.a) return this->a < T.a;
        return this->b < T.b;
    }
    
    int a, b;
};

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

map<Triple, int> LR_map;
map<Double, int> R_map;

int factorial (int n) {
    int result = 1;
    
    for (int i = 2; i <= n; i++) {
        result *= i;
    }

    return result;
}

int magia_aux(int n, int dr) {
    if (dr < 1 || dr > n) {
        return 0;
    }

    Double d(n, dr);
    if (R_map.find(d) == R_map.end()) {
        if (dr == 1) {
            R_map[d] = factorial(n-1);
        }
        else { //dr > 1
            R_map[d] = magia_aux(n-1,dr-1) + (n-1)*magia_aux(n-1,dr);
        }
    }

    return R_map[d];
}

int magia(int n, int dp, int dr) {
    // Impossible cases
    if ( (dp < 1) || (dp > n) || (dr < 1) || (dr > n) ) {
        return 0;
    }
    if (n > 1 && dp == 1 && dr == 1) {
        return 0;
    }

    //assume dr >= dp
    if (dp > dr) { 
        return magia(n, dr, dp);
    }

    // if the solution was not computed before, do it
    Triple t(n,dp,dr);
    if (LR_map.find(t) == LR_map.end()) {
        if (dp == 1) { 
            // put the bigger one on the left
            LR_map[t] = magia_aux(n-1, dr-1);
        }
        else {
            LR_map[t] = magia(n-1,dp-1,dr) + magia(n-1,dp,dr-1) + (n-2)*magia(n-1,dp,dr);
        }
    }

    return LR_map[t];
}

void initialize () {
    LR_map[Triple(1,1,1)] = 1;
}

int main () {
    int T;
    int n, dp, dr;

    initialize();

    cin >> T;
    for (int i = 1; i <= T; i++) {
        cin >> n >> dp >> dr;
        cout << magia(n,dp,dr) << endl;
    }

    return 0;
}
