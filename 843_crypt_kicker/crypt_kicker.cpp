#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <cstring>
using namespace std;

typedef vector<int> WordClass;


WordClass get_class(string &s) {
    int j;
    map<char, int> mapa;
    WordClass clase;
    
    j = 1;
    clase.resize(s.size());
    for (int i = 0; i < s.size(); i++) {
        if (mapa.find(s[i]) == mapa.end()) {
            mapa[s[i]] = j;
            j++;
        }
        clase[i] = mapa[s[i]];
    }
    return clase;
}

class StringWithClass {
public:
    StringWithClass() {
    }
    StringWithClass(string s_) {
        s = s_;
        c = get_class(s);
    }
    void set_string(string s_) {
        s = s_;
        c = get_class(s);
    }
    string s;
    WordClass c;
    vector<string> candidatos;
};

vector<StringWithClass> dictionary;

bool comp_wc(WordClass a, WordClass b) {
    int min_size = min(a.size(), b.size());

    for (int i = 0; i < min_size; i++) {
        if (a[i] != b[i]) return (a[i] < b[i]);
    }

    return (a.size() < b.size());
}

bool comp_swc(StringWithClass a, StringWithClass b) {
    return comp_wc(a.c, b.c);
}

bool equal_wc(WordClass a, WordClass b) {
    if (a.size() != b.size()) return false;

    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) return false;
    }

    return true;
}

bool esta_en_el_diccionario(string &word, vector<string> &dictionary_strings) {
    return binary_search(dictionary_strings.begin(), dictionary_strings.end(), word); 
}

bool traducible(string &palabra, map<char,char> &mapa, vector<string> &dictionary_strings) {
    for (int i = 0; i < palabra.size(); i++) {
        if (mapa.find(palabra[i]) == mapa.end()) return false;
    }

    string palabra_traducida = palabra;
    for (int i = 0; i < palabra.size(); i++) {
        palabra_traducida[i] = mapa[palabra[i]];
    }

    bool res = esta_en_el_diccionario(palabra_traducida, dictionary_strings);

    return res;
}

bool candidato_posible(string &candidato, string &palabra, map<char, char> &mapa, map<char, char> &mapa_inverso) {
    for (int i = 0; i < candidato.size(); i++) {
        if (mapa_inverso.find(candidato[i]) != mapa_inverso.end()) {
            if (mapa_inverso[candidato[i]] != palabra[i]) return false;
        }
        if (mapa.find(palabra[i]) != mapa.end()) {
            if (mapa[palabra[i]] != candidato[i]) return false;
        }
    }
    return true;
}

bool backtracking(vector<StringWithClass> &words, map<char, char> &mapa, map<char,char> &mapa_inverso, int curr, vector<string> &dictionary_strings) {
    string &current_word = words[curr].s;

    // Si ya termine, devuelvo verdadero
    if (curr == words.size()) return true;

    // Si se como traducir la palabra, paso a lo siguiente
    if (traducible(current_word, mapa, dictionary_strings)) {
        return backtracking(words, mapa, mapa_inverso, curr+1, dictionary_strings);
    }

    // Si no se como traducirla, me fijo para cada palabra si puedo usarla (uso el mapa y el mapa inverso)
    

    for (int i = 0; i < words[curr].candidatos.size(); i++) {
        string &candidato = words[curr].candidatos[i];

        if (candidato_posible(candidato, current_word, mapa, mapa_inverso)) {
            // Si puedo usar la palabra, actualizo el mapa y paso a la siguiente palabra.
            // Si yendo por ese camino logro la solucion, devuelvo verdadero con ese mapa
            // Si no, deshago lo que le hice al mapa
            map<char, char> mapa_backup = mapa;
            map<char, char> mapa_inverso_backup = mapa_inverso;

            for (int j = 0; j < candidato.size(); j++) {
                mapa[current_word[j]] = candidato[j];
                mapa_inverso[candidato[j]] = current_word[j];
            }

            bool resultado = backtracking(words, mapa, mapa_inverso, curr+1, dictionary_strings);
            if (resultado) {
                return true;
            }
            mapa = mapa_backup;
            mapa_inverso = mapa_inverso_backup;
        }
    }

    return false;
}

bool comp_cand (StringWithClass a, StringWithClass b) {
    return a.candidatos.size() < b.candidatos.size();
}

map<char, char> magia(vector<StringWithClass> &words, vector<string> &dictionary_strings) {
    map<char, char> mapa, mapa_inverso;
    
    for (int i = 0; i < words.size(); i++) {
        vector<StringWithClass>::iterator it = lower_bound(dictionary.begin(), dictionary.end(), words[i], comp_swc);

        if (it != dictionary.end() && equal_wc(it->c, words[i].c)) {
            words[i].candidatos.push_back(it->s);
            it++;
            while ( it != dictionary.end() && equal_wc(it->c, (it-1)->c) ) {
                words[i].candidatos.push_back(it->s);
                it++;
            }
        }
    }

    sort(words.begin(), words.end(), comp_cand);

    if (!backtracking(words, mapa, mapa_inverso, 0, dictionary_strings)) {
        for (char i = 'a'; i <= 'z'; i++) mapa[i] = '*';
    }
    return mapa;
}

int main() {
    int n;
    string line, word;
    vector<StringWithClass> words;
    vector<string> dictionary_strings;
    map<char, char> traductor;

    cin >> n;
    dictionary.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> word;
        dictionary[i].set_string(word);
        dictionary_strings.push_back(word);
    }
    sort(dictionary.begin(), dictionary.end(), comp_swc);
    sort(dictionary_strings.begin(), dictionary_strings.end());

    cin.ignore();
    while (getline(cin,line)) {
        words.clear();
        traductor.clear();
        stringstream ss;
        ss << line;
        while (ss >> word) {
            words.push_back(StringWithClass(word));
        }
        traductor = magia(words, dictionary_strings);

        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ' ') 
                cout << ' ';
            else 
                cout << traductor[line[i]];
        }
        cout << endl;
    }

    return 0;
}
