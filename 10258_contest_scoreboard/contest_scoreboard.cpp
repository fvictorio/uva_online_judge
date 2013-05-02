#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
using namespace std;

const int NO_CONTESTANTS = 100;
const int NO_PROBLEMS = 10;

class Contestant {
public:
    Contestant() {
        active = false; 
        penalty = 0;
        no_solved_problems = 0;
        solved_problems.resize(NO_PROBLEMS, false);
        no_tries_problem.resize(NO_PROBLEMS, 0);
    }
    void set_active() {active = true;}
    void set_id(int _id) {id = _id;}
    bool is_active() {return active;}
    void add_submission(int no_problem, int submission_time, char submission_type) {
        if (submission_type == 'C') {
            if (!solved_problems[no_problem]) {
                solved_problems[no_problem] = true;
                penalty += submission_time + 20*no_tries_problem[no_problem];
                no_solved_problems++;
            }
        }
        else if (submission_type == 'I') {
            if (!solved_problems[no_problem]) {
                no_tries_problem[no_problem]++;
            }
        }
    }
    void print() {
        cout << id << " " << no_solved_problems << " " << penalty << endl;
    }
    const bool operator< (const Contestant &c2) const {
        if (!active && !c2.active) return false;
        if (active && !c2.active) return true;
        if (!active && c2.active) return false;

        if (no_solved_problems > c2.no_solved_problems) return true;
        if (no_solved_problems < c2.no_solved_problems) return false;
        
        if (penalty < c2.penalty) return true;
        if (penalty > c2.penalty) return false;

        return (id < c2.id);
    }
private:
    bool active;
    int penalty;
    int no_solved_problems;
    int id;
    vector<bool> solved_problems;
    vector<int> no_tries_problem;
};

int main () {
    int no_cases;
    string line;
    vector<Contestant> contestants;
    int no_contestant, no_problem, submission_time;
    char submission_type;

    cin >> no_cases;
    cin.ignore();
    cin.ignore();
    for (int current_case = 0; current_case < no_cases; current_case++) {
        contestants.clear();
        contestants.resize(NO_CONTESTANTS);
        for (int i = 0; i < NO_CONTESTANTS; i++) {
            contestants[i].set_id(i+1);
        }
        while (getline(cin, line) && line != "") {
            stringstream ss;
            ss << line;
            ss >> no_contestant; no_contestant--;
            ss >> no_problem; no_problem--;
            ss >> submission_time;
            ss >> submission_type;
            contestants[no_contestant].set_active();
            contestants[no_contestant].add_submission(no_problem, submission_time, submission_type);
        }
        sort(contestants.begin(), contestants.end());
        for (int i = 0; i < NO_CONTESTANTS; i++) {
            if (!contestants[i].is_active()) break;
            contestants[i].print();
        }
        if (current_case + 1 < no_cases) cout << endl;
    }

    return 0;
}
