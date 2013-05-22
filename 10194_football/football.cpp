#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
using namespace std;

bool comp(string s1, string s2) {
    char ch;
    for (int i = 0; i < s1.size(); i++) {
        ch = s1[i];
        ch = tolower(ch);
        s1[i] = ch;
    }
    for (int i = 0; i < s2.size(); i++) {
        ch = s2[i];
        ch = tolower(ch);
        s2[i] = ch;
    }

    return s1 < s2;
}

class Team {
public:
    string name;
    int points, games;
    int wins, ties, losses;
    int goals_scored, goals_against;
    int goals_dif;

    bool operator< (const Team t2) const {
        if (this->points != t2.points) 
            return this->points > t2.points;

        if (this->wins != t2.wins)
            return this->wins > t2.wins;

        if (this->goals_dif != t2.goals_dif)
            return this->goals_dif > t2.goals_dif;

        if (this->goals_scored != t2.goals_scored)
            return this->goals_scored > t2.goals_scored;

        if (this->games != t2.games)
            return this->games < t2.games;

        return comp(this->name, t2.name);
    }
};

int str_to_int (string sx) {
    int x;
    stringstream ss;

    ss << sx;
    ss >> x;

    return x;
}

void read_game(string game, vector<Team> &teams, map<string,int> &name_to_idx) {
    int a, b;
    string team1, team2;
    int goals1, goals2;

    a = 0;
    b = 0;
    while (game[b] != '#') b++;

    team1 = game.substr(a, b-a);

    a = b+1;
    b = b+1;
    while(game[b] != '@') b++;

    goals1 = str_to_int(game.substr(a, b-a));

    a = b+1;
    b = b+1;
    while(game[b] != '#') b++;

    goals2 = str_to_int(game.substr(a, b-a));
    team2 = game.substr(b+1, game.length() - b - 1);

    ////////////////////////
    int idx1, idx2;

    idx1 = name_to_idx[team1];
    idx2 = name_to_idx[team2];

    teams[idx1].games++;
    teams[idx2].games++;

    if (goals1 > goals2) {
        teams[idx1].points += 3;
        teams[idx1].wins += 1;
        teams[idx2].losses += 1;
    }
    else if (goals2 > goals1) {
        teams[idx2].points += 3;
        teams[idx2].wins += 1;
        teams[idx1].losses += 1;
    }
    else {
        teams[idx1].points += 1;
        teams[idx2].points += 1;
        teams[idx1].ties += 1;
        teams[idx2].ties += 1;
    }

    teams[idx1].goals_scored += goals1;
    teams[idx2].goals_scored += goals2;
    teams[idx1].goals_against += goals2;
    teams[idx2].goals_against += goals1;

    teams[idx1].goals_dif = teams[idx1].goals_scored - teams[idx1].goals_against;
    teams[idx2].goals_dif = teams[idx2].goals_scored - teams[idx2].goals_against;

}

int main () {
    int N;
    int no_teams, no_games;
    vector<Team> teams;
    map<string, int> name_to_idx;
    string tournament_name;
    string game;

    cin >> N;
    cin.ignore();

    for (int i = 1; i <= N; i++) {
        getline(cin, tournament_name);

        cin >> no_teams;
        cin.ignore();

        teams.clear();
        teams.resize(no_teams);

        name_to_idx.clear();

        for (int j = 0; j < no_teams; j++) {
            getline(cin, teams[j].name);
            name_to_idx[teams[j].name] = j;
        }

        cin >> no_games;
        cin.ignore();

        for (int j = 0; j < no_games; j++) {
            getline(cin, game);
            read_game(game, teams, name_to_idx);
        }

        sort(teams.begin(), teams.end());
        
        cout << tournament_name << endl;

        for (int j = 0; j < no_teams; j++) {
            cout << j+1 << ") ";
            cout << teams[j].name << " ";
            cout << teams[j].points << "p, ";
            cout << teams[j].games << "g ";
            cout << "(" << teams[j].wins << "-" << teams[j].ties << "-" << teams[j].losses << "), ";
            cout << teams[j].goals_dif << "gd ";
            cout << "(" << teams[j].goals_scored << "-" << teams[j].goals_against << ")" << endl;
        }

        if (i < N) cout << endl;

    }

    return 0;
}

