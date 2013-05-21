#include <iostream>
#include <string>
using namespace std;

bool isdigit(char c) {
    return ((c >= '0') && (c <= '9'));
}

int magia(string &correct, string &answer) {
    if (answer == correct) return 0;

    int i;
    int cs = correct.size(),
        as = answer.size();
    
    string digits_correct = "",
           digits_answer = "";

    i = 0;
    while (i < cs) {
        while (i < cs && !isdigit(correct[i])) i++;
        digits_correct += correct[i];
        i++;
    }

    i = 0;
    while (i < as) {
        while (i < as && !isdigit(answer[i])) i++;
        digits_answer += answer[i];
        i++;
    }
    
    if (digits_correct == digits_answer) return 1;
    else {
        if (digits_answer.size() > digits_correct.size()) {
            digits_answer.erase(digits_answer.begin() + digits_correct.size(), digits_answer.end());
            if (digits_answer == digits_correct) {
                return 1;
            }
            else {return 2;}
        }
        else {return 2;}
    }
}

int main () {
    int n, m;
    string line;
    string answer, correct_answer;
    int resolucion;

    int run = 1;
    while (cin >> n && n != 0) {
        correct_answer = "";
        answer = "";

        cin.ignore();
        for (int i = 0; i < n; i++) {
            getline(cin, line);
            correct_answer += line;
            if (i+1 < n) correct_answer += '\n';
        }
        
        cin >> m;
        cin.ignore();
        for (int i = 0; i < m; i++) {
            getline(cin, line);
            answer += line;
            if (i+1 < m) answer += '\n';
        }

        resolucion = magia(correct_answer, answer);
        if (resolucion == 0) {
            cout << "Run #" << run << ": Accepted" << endl;
        }
        else if (resolucion == 1) {
            cout << "Run #" << run << ": Presentation Error" << endl;
        }
        else {
            cout << "Run #" << run << ": Wrong Answer" << endl;
        }

        run++;
    }

    return 0;
}

