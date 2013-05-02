#include <stdio.h>

#define MAX_M 16

typedef long long ll;

ll gcd(ll a, ll b) {
    ll aux;

    while (b > 0) {
        aux = b;
        b = a % aux;
        a = aux;
    }

    return a;
}

ll lcm(ll a, ll b) {
    return a / gcd(a,b) * b;
}

magia_aux(ll n, ll m, ll descartar[MAX_M], ll subconjunto, ll *subconjunto_lcm, ll *signo) {
    ll i;

    *signo = 1;
    *subconjunto_lcm = 1;
    for (i = 0; i < m; i++) {
        if (subconjunto & 1 == 1) {
            *signo = -(*signo);
            *subconjunto_lcm = lcm(*subconjunto_lcm, descartar[i]);
        }
        subconjunto = subconjunto >> 1;
    }

}

ll magia(ll n, ll m, ll descartar[MAX_M]) {
    ll n_final;
    ll i;
    ll subconjunto_lcm, signo;
    ll subconjunto, cant_subconjuntos;

    n_final = n;
    cant_subconjuntos = 1;
    for (i = 0; i < m; i++) 
        cant_subconjuntos = cant_subconjuntos << 1;

    for (subconjunto = 1; subconjunto < cant_subconjuntos; subconjunto++) {
        magia_aux(n, m, descartar, subconjunto, &subconjunto_lcm, &signo);
        n_final = n_final + signo*(n/subconjunto_lcm);
    }
    
    return n_final;
}

main () {
    ll i;
    ll n, m;
    ll descartar[MAX_M];

    while (scanf("%lld %lld", &n, &m) != EOF) {
        for (i = 0; i < m; i++) {
            scanf("%lld", &descartar[i]);
        }
        printf("%lld\n", magia(n, m, descartar));
    }

    return 0;
}
