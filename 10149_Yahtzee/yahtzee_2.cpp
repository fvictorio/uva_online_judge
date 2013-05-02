#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#define n_tiradas 13
#define n_dados 5

using namespace std;

vector<map<int, pair<int, int> > > memoria;
vector<vector<int> > puntaje;

int sumar_n (vector<int> & tirada, int n) {
    int resultado = 0;

    for (int i = 0; i < n_dados; i++) {
        if (tirada[i] == n) resultado += n;
    }

    return resultado;
}

int chance (vector<int> & tirada) {
    int resultado = 0;

    for (int i = 0; i < n_dados; i++) {
        resultado += tirada[i];
    }

    return resultado;
}

int three_oak (vector<int> & tirada) {
    if (tirada[0] == tirada[2]) return chance(tirada);
    if (tirada[1] == tirada[3]) return chance(tirada);
    if (tirada[2] == tirada[4]) return chance(tirada);
    return 0;
}

int four_oak (vector<int> & tirada) {
    if (tirada[0] == tirada[3]) return chance(tirada);
    if (tirada[1] == tirada[4]) return chance(tirada);
    return 0;
}

int five_oak (vector<int> & tirada) {
    if (tirada[0] == tirada[4]) return 50;
    return 0;
}

int s_straight (vector<int> & tirada) {
    if ((tirada[0] + 1 == tirada[1]) && (tirada[1] + 1 == tirada[2]) && (tirada[2] + 1 == tirada[3])) return 25; 
    if ((tirada[1] + 1 == tirada[2]) && (tirada[2] + 1 == tirada[3]) && (tirada[3] + 1 == tirada[4])) return 25; 
    return 0;
}

int l_straight (vector<int> & tirada) {
    if ((tirada[0] + 1 == tirada[1]) && (tirada[1] + 1 == tirada[2]) && (tirada[2] + 1 == tirada[3]) && (tirada[3] + 1 == tirada[4])) return 35; 
    return 0;
}

int full (vector<int> & tirada) {
    if (tirada[0] == tirada[2] && tirada[3] == tirada[4]) return 40;
    if (tirada[0] == tirada[1] && tirada[2] == tirada[4]) return 40;
    return 0;
}

int calcular_puntaje (vector<int> & tirada, int categoria) {
    switch (categoria) {
        case 0:
            return sumar_n(tirada, 1);
            break;
        case 1:
            return sumar_n(tirada, 2);
            break;
        case 2:
            return sumar_n(tirada, 3);
            break;
        case 3:
            return sumar_n(tirada, 4);
            break;
        case 4:
            return sumar_n(tirada, 5);
            break;
        case 5:
            return sumar_n(tirada, 6);
            break;
        case 6:
            return chance(tirada);
            break;
        case 7:
            return three_oak(tirada);
            break;
        case 8:
            return four_oak(tirada);
            break;
        case 9:
            return five_oak(tirada);
            break;
        case 10:
            return s_straight(tirada);
            break;
        case 11:
            return l_straight(tirada);
            break;
        case 12:
            return full(tirada);
            break;
        default:
            cout << "NO DEBERIA ENTRAR ACA" << endl;
    }
}

void inicializar_puntaje (vector<vector<int> > & tiradas) {
    for (int i = 0; i < n_tiradas; i++) {
        for (int j = 0; j < n_tiradas; j++) {
            puntaje[i][j] = calcular_puntaje(tiradas[i], j);
        }
    }
}

int dp (vector<vector<int> > & tiradas, int i_categoria, int permitidas, int score=0) {
    if (i_categoria + 1 == n_tiradas) {
        if (memoria[permitidas].find(0) != memoria[permitidas].end()) {
            return memoria[permitidas][0].first;
        }
        for (int i = 0; i < n_tiradas; i++) {
            if (permitidas & (1 << i)) continue;
            memoria[permitidas][0].first = puntaje[i][i_categoria];
            memoria[permitidas][0].second = i;
            return memoria[permitidas][0].first;
        }
    }

    if (i_categoria > 5) {
        if (memoria[permitidas].find(0) != memoria[permitidas].end()) {
            return memoria[permitidas][0].first;
        }
    }
    else {
        if (memoria[permitidas].find(score) != memoria[permitidas].end()) {
            return memoria[permitidas][score].first;
        }
    }

    
    int puntaje_por_tirada, puntaje_temp, mejor_puntaje = -1, mejor_tirada, bonus = 0;
    for (int i = 0; i < n_tiradas; i++) {
        if (permitidas & (1 << i)) continue;
        
        puntaje_por_tirada = puntaje[i][i_categoria];
        if (i_categoria == 5 && (score + puntaje_por_tirada >= 63)) bonus = 35;
        else bonus = 0;
        puntaje_temp = puntaje_por_tirada + bonus + dp(tiradas, i_categoria+1, permitidas | (1 << i), score + puntaje_por_tirada + bonus);
        if (puntaje_temp > mejor_puntaje) {
            mejor_puntaje = puntaje_temp;
            mejor_tirada = i;
        }
    }
    if (i_categoria > 5 ) {
        memoria[permitidas][0].first = mejor_puntaje;
        memoria[permitidas][0].second = mejor_tirada;
    }
    else {
        memoria[permitidas][score].first = mejor_puntaje;
        memoria[permitidas][score].second = mejor_tirada;
    }
    return mejor_puntaje;
}


void magia (vector<vector<int> > & tiradas) {
    int total = dp(tiradas, 0, 0, 0);
    int mejor_tirada = memoria[0][0].second;
    int permitidas = 0, score = 0;
    bool bonus = false;

    for (int i = 0; i < 5; i++) {
        cout << puntaje[mejor_tirada][i] << " ";
        score += puntaje[mejor_tirada][i];
        permitidas = permitidas | (1 << mejor_tirada);
        mejor_tirada = memoria[permitidas][score].second;
    }

    cout << puntaje[mejor_tirada][5] << " ";
    score += puntaje[mejor_tirada][5];
    permitidas = permitidas | (1 << mejor_tirada);
    mejor_tirada = memoria[permitidas][0].second;

    if (score >= 63) bonus = true;
    
    for (int i = 6; i < n_tiradas; i++) {
        cout << puntaje[mejor_tirada][i] << " ";
        permitidas = permitidas | (1 << mejor_tirada);
        mejor_tirada = memoria[permitidas][0].second;
    }

    cout << (bonus ? 35 : 0) << " " << total << endl;
}

int main () {
    int first;
    vector<vector<int> > tiradas(n_tiradas, vector<int>(n_dados));

    while (cin >> tiradas[0][0]) {
        memoria.clear();
        memoria.resize(1<<(n_tiradas+1));
        puntaje.clear();
        puntaje.resize(n_tiradas, vector<int>(n_tiradas));
        for (int i = 0; i < n_tiradas; i++) {
            for (int j = 0; j < n_dados; j++) {
                if (i == 0 && j == 0) continue;
                cin >> tiradas[i][j];
            }
            sort(tiradas[i].begin(), tiradas[i].end());
        }
        inicializar_puntaje(tiradas);
        magia(tiradas);
    }

    return 0;
} 
