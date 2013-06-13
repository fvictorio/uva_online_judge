#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int sum_vec(vector<int> &v) {
    int accum = 0;
    for (int i = 0; i < v.size(); i++) {
        accum += v[i];
    }
    return accum;
}

int greedy(vector<int> &people, int n) {
    return INT_MAX;
}

void magia(vector<int> &people, int n, int idx, int cant, int cant_max, int diff, int &minn, int sum_so_far, int &best_sum) {
    if (cant == cant_max) {
        if (abs(diff) < abs(minn)) {
            minn = diff;
            best_sum = sum_so_far;
        }
        return;
    }

    if (minn < 0 && diff < minn) {
        return;
    }

    int max_rest = 0;
    int min_rest = 0;

    for (int i = 0; i < cant_max-cant; i++) {
        max_rest += people[idx+i];
        min_rest += people[n-1-i];
    }
    int max_possible = diff-2*min_rest;
    int min_possible = diff-2*max_rest;

    if (minn > 0 && min_possible > minn) {
        return;
    }
    if (minn < 0 && max_possible < minn) {
        return;
    }

    int lim = n-cant_max+cant;
    for (int i = idx; i <= lim; i++) {
        magia(people, n, i+1, cant+1, cant_max, diff-2*people[i], minn, sum_so_far+people[i], best_sum);
    }
}

int main () {
    int no_cases;
    cin >> no_cases;

    for (int c = 1; c <= no_cases; c++) {
        int n;
        cin >> n;

        vector<int> people(n);
        for (int i = 0; i < n; i++) {
            cin >> people[i];
        }

        sort(people.rbegin(), people.rend());

        int minn = greedy(people, n);
        int sum = sum_vec(people);
        int best_sum;
        magia(people, n, 0, 0, n/2, sum, minn, 0, best_sum);

        int a, b;
        a = best_sum;
        b = sum-best_sum;
        if (a > b) swap(a,b);
        cout << a << " " << b << endl;
        if (c < no_cases) cout << endl;
    }

    return 0;
}
