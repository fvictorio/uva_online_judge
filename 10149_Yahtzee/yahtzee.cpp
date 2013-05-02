#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int minimum(vector<int> &v, int n) {
    int minn = v[0];
    for (int i = 1; i < n; i++) {
        minn = min(minn, v[i]);
    }
    return minn;
}

int hungarian_algorithm (vector<vector<int> > m) {
    int row_mean;
    int n = m.size();
    vector<vector<int> > matrix = m;

    for (int i = 0; i < n; i++) {
        row_mean = minimum(matrix[i], n);
        for (int j = 0; j < n; j++) {
            matrix[i][j] -= row_mean;
        }
    }

    vector<int> assigned(n, -1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 0) {
                bool already_assigned = false;
                for (int k = 0; k < i; k++) {
                    if (assigned[k] == j) {
                        already_assigned = true;
                        break;
                    }
                }
                if (!already_assigned) {
                    assigned[i] = j;
                }
            }
        }
    }

    bool all_assigned = true;
    for (int i = 0; i < n; i++) {
        if (assigned[i] == -1) {
            all_assigned = false;
            break;
        }
    }
    if (all_assigned) {
        int total = 0;
        for (int i = 0; i < n; i++) {
            total += m[i][assigned[i]];
        }
        return total;
    }
}

int main () {
    int n;
    vector<vector<int> > matrix;

    while (cin >> n && n != -1) {
        matrix.clear();
        matrix.resize(n, vector<int>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cin >> matrix[i][j];
            }
        }

        cout << hungarian_algorithm(matrix) << endl;
    }

    return 0;
}

