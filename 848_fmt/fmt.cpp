#include <iostream>
#include <vector>
#include <string>
using namespace std;

const int MAX = 72;

int first_word_size(string &line) {
    int i = 0;
    int n = line.size();

    while (i < n && line[i] == ' ') i++;
    while (i < n && line[i] != ' ') i++;

    return i;
}

void lstrip(string &line) {
    int i;
    int n = line.size();

    i = 0;
    while (i < n && line[i] == ' ') i++;

    if (i > 0)
        line.erase(line.begin(), line.begin() + i);
}

void rstrip(string &line) {
    int i;
    int n = line.size();

    i = n-1;
    while (i >= 0 && line[i] == ' ') i--;

    if (i < n-1)
        line.erase(line.begin() + i + 1, line.end());
}

void magia(vector<string> &lines) {
    int i;

    // Primero borro todos los espacios al final de cada linea
    i = 0;
    while (i < lines.size()) {
        rstrip(lines[i]);
        i++;
    }

    i = 0;
    while (i < lines.size()) {
        string &line = lines[i];
        int n = line.size();

        if (n > MAX) {
            int fws = first_word_size(line);
            
            // Si la primera palabra ya me llena la linea
            if (fws >= MAX) {
                // Si no solo eso, sino que ademas
                // esta sola y me ocupa toda la linea
                // avanzo y no hago nada
                if (fws == n) {i++; continue;}

                // Si no, corto desde el espacio
                // que sigue a la palabra,
                // que me lo da fws
                
                lines.insert(lines.begin()+i+1, string(line.begin()+fws+1, line.end()));
                lines[i].erase(lines[i].begin()+fws, lines[i].end());
                rstrip(lines[i]);
                lstrip(lines[i+1]);
                i++; continue;
            }

            // Si no, bajo todo desde el primer espacio que puedo
            int k = MAX;
            while (k >= 0 && line[k] != ' ') k--;

            lines.insert(lines.begin()+i+1, string(line.begin()+k+1, line.end()));
            lines[i].erase(lines[i].begin()+k+1, lines[i].end());
            rstrip(lines[i]);
            lstrip(lines[i+1]);
        }
        // si la linea es menor al tamanio maximo
        else if (n < MAX) {
            // si estoy en una linea vacia, avanzo y no hago nada
            if (n == 0) {i++; continue;}

            // salgo si es la ultima linea
            if (i+1 == lines.size()) break;

            string &next_line = lines[i+1];

            // Si no puedo traerme palabras de la proxima linea
            // avanzo y no hago nada
            if (next_line.empty() || next_line[0] == ' ') {i++; continue;}

            // Si no puedo traerme ni la primera palabra
            // avanzo y no hago nada
            int nl_fws = first_word_size(next_line);
            if (n + 1 + nl_fws > MAX) {i++; continue;}

            // Veo cuanto puedo traerme de la proxima linea
            int chars_disp = MAX - n - 1;

            // Si puedo traerme todo, traigo
            // y repito manteniendome en la misma linea
            // (avanzo sin incrementar i)
            if (chars_disp >= next_line.size()) {
                line.insert(line.end(), ' ');
                line.insert(line.end(), next_line.begin(), next_line.end());
                lines.erase(lines.begin() + i + 1, lines.begin() + i + 2);
                continue;
            }
            else {
                int k = chars_disp;
                while (k >= 0 && next_line[k] != ' ') k--;
                while (k >= 0 && next_line[k] == ' ') k--;
                line.insert(line.end(), ' ');
                line.insert(line.end(), next_line.begin(), next_line.begin() + k + 1);
                next_line.erase(next_line.begin(), next_line.begin() + k + 1);
                lstrip(next_line);
                if (next_line.empty()) {
                    lines.erase(lines.begin() + i + 1, lines.begin() + i + 2);
                    continue;
                }
            }
        }
        i++;
    }
}

int main () {
    string line;
    vector<string> lines;

    while (getline(cin,line)) {
        lines.push_back(line);
    }
    
    magia(lines);

    int i;
    for (i = 0; i < lines.size(); i++) {
        cout << lines[i] << endl;
    }

    return 0;
}

