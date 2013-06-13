#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <climits>
using namespace std;

int main () {
    int no_cases;
    cin >> no_cases;

    for (int c = 1; c <= no_cases; c++) {
        int n;
        cin >> n;
        
        vector<int> people(n);
        int total_weight = 0;

        for (int i = 0; i < n; i++) {
            cin >> people[i];
            total_weight += people[i];
        }

        sort(people.begin(), people.end());

        vector<set<int> > team_weights(total_weight+1);
        vector<bool> realizable(total_weight+1, false);
        team_weights[0].insert(0);
        realizable[0] = true;
        int last_realizable = 0;
        int best_diff = INT_MAX;
        
        int weight;
        for (int i = 0; i < n; i++) {
            weight = people[i];

            int start = min(last_realizable, total_weight - weight);
            int end = 0;

            for (int tw = start; tw >= end; tw--) {
                if (!realizable[tw]) continue;

                set<int>::iterator it;
                for (it = team_weights[tw].begin(); it != team_weights[tw].end(); it++) {
                    int new_tw = tw+weight;
                    team_weights[new_tw].insert(*it+1);
                    realizable[new_tw] = true;
                    last_realizable = max(last_realizable, new_tw);
                    int diff = total_weight - 2*(new_tw);
                    if (abs(diff) < abs(best_diff)) {
                        best_diff = diff;
                    }
                }
            }
            if (best_diff == 0) {
                break;
            }
        }

        if (n > 0) {
            cout << abs(best_diff) << endl;
        }
        else {
            cout << 0 << endl;
        }
    }

    return 0;
}

