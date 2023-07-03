#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../../headers/main.h"


// trie l'array en utilisant uniquement _flip: comme un selection_sort mais on utilise _flip pour placer le minimum courant à gauche
void pancake_sort(int* tab, int n) {
    int max;
    for (int i = n-1; i>0; i--) {
        max = i;
        for (int j = 0; j <= i; j++) {
            draw(tab, n, 0, j);
            if (tab[j] > tab[max]) {
                max = j;
            }
        }
        _flip(tab, n, max);
        _flip(tab, n, i);
        draw(tab, n, 0, i);
    }
}

// flip l'array entre l'indice 0 et k inclus
void _flip(int* tab, int n, int k) {
    assert(k>=0);
    assert(k<n);
    int buff;
    for (int i = 0; i <= k/2; i++) {
        buff = tab[i];
        tab[i] =tab[k-i];
        tab[k-i] = buff;
        if (i%2 == 1) draw(tab, n, 0, i);
        else          draw(tab, n, 0, k-i);
   }
}