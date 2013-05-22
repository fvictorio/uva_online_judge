#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

int magia(vector<int> &original) {
    int n = original.size();
    int res = n;

    for (int i = 0; i < n; i++) {
        if (original[i] == res) res--;
    }
    
    return res;
}

int main () {
    int k;
    int n;
    vector<string> name_original, name_sorted;
    vector<int> int_original;
    int shell_sort;
    map<string, int> name_to_int;
    map<int, string> int_to_name;

    cin >> k;

    for (int i = 0; i < k; i++) {
        cin >> n;

        name_original.clear();
        name_sorted.clear();
        int_original.clear();
        name_to_int.clear();
        int_to_name.clear();

        name_original.resize(n);
        name_sorted.resize(n);
        int_original.resize(n);

        cin.ignore();
        for (int j = 0; j < n; j++) {
            getline(cin, name_original[j]);
        }

        for (int j = 0; j < n; j++) {
            getline(cin, name_sorted[j]);
            name_to_int[name_sorted[j]] = j;
            int_to_name[j] = name_sorted[j];
        }

        for (int j = 0; j < n; j++) {
            int_original[j] = 1+name_to_int[name_original[j]];
        }

        reverse(int_original.begin(), int_original.end());
        shell_sort = magia(int_original);

        for (int j = shell_sort-1; j >= 0; j--) {
            cout << int_to_name[j] << endl;
        }

        cout << endl;
    }


    return 0;
}

