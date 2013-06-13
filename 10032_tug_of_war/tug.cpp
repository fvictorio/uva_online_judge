#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <climits>
using namespace std;

const bool DEBUG = false;

int main () {
    int no_cases;
    cin >> no_cases;

    for (int c = 1; c <= no_cases; c++) {
        int n;
        cin >> n;
        
        int max_team_size = n/2;

        vector<int> people(n);
        int total_weight = 0;

        for (int i = 0; i < n; i++) {
            cin >> people[i];
            total_weight += people[i];
        }

        sort(people.begin(), people.end());

        int max_weight = 0;
        for (int i = 1; i <= max_team_size; i++) {
            max_weight += people[n-i];
        }

        vector<set<int> > team_weights(max_weight+1);
        vector<bool> realizable(max_weight+1, false);
        team_weights[0].insert(0);
        realizable[0] = true;
        int last_realizable = 0;
        int best_diff = INT_MAX;
        int best_tw;
        
        int weight;
        for (int i = 0; i < n; i++) {
            weight = people[i];

            int start = min(last_realizable, max_weight - weight);
            if (best_diff != INT_MAX) {
                int cand_start = (total_weight - 2*weight + abs(best_diff) - 1)/2;
                start = min(start, cand_start);
            }

            int end = 0;

            for (int tw = start; tw >= end; tw--) {
                if (!realizable[tw]) continue;

                set<int>::iterator it;
                for (it = team_weights[tw].begin(); it != team_weights[tw].end(); it++) {
                    int new_tw = tw+weight;
                    team_weights[new_tw].insert(*it+1);
                    realizable[new_tw] = true;
                    last_realizable = max(last_realizable, new_tw);
                    if (*it +1 == max_team_size) {
                        int diff = total_weight - 2*(new_tw);
                        if (abs(diff) < abs(best_diff)) {
                            best_diff = diff;
                            best_tw = new_tw;
                        }
                        break;
                    }
                }
            }
            if ((total_weight%2==0 && best_diff==0) || (total_weight%2==1 && best_diff==1)) {
                break;
            }
        }

        if (DEBUG) {
            for (int i = 0; i <= max_weight; i++) {
                cout << i << ": ";
                if (realizable[i]) {
                    cout << "realizable with sizes ";
                    set<int>::iterator it;
                    for (it = team_weights[i].begin(); it != team_weights[i].end(); it++) {
                        cout << *it << " ";
                    }
                    cout << endl;
                }
                else {
                    cout << "not realizable." << endl;
                }
            }
        }

        int a = best_tw;
        int b = total_weight - best_tw;
        if (a > b) swap(a,b);

        cout << a << " " << b << endl;

        if (c < no_cases) cout << endl;
    }

    return 0;
}

