#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../../headers/main.h"
#include "../../headers/algorithms/radix_sort.h"


// Modifier la valeur de BLOCK_SIZE dans headers/algorithms/radis_sort.h fait des résultats visuels très différents et très parlants!
// A partir de 8, tout se fait en une seule itération -> plus énormément d'intérêt visuel


void radix_sort(int* tab, int n) {
    int* temp = malloc(sizeof(int) * n);
    memcpy(temp, tab, n); // Cette copie sert pour la représentation visuelle car je print le tableau out pendant sa modification
                          // car je ne peux représenter qu'un seul tableau à la fois. Inutile sinon.
    int max = _get_max(tab, n);
    int digits = _get_digits(max);

    int** hists = histograms(tab, n, digits);

    for (int k = 0; k<digits; k++) {
        for (int i = 0; i<n; i++) {
            draw(tab, n, 0, i); // Ne sert à rien maintenant que les hists se font en une seule itération, mais sert à montrer l'apparence d'un radix sort classique
                                // qui re-scanne l'array entre chaque sous-tri
        }
        radix_pass(temp, tab, n, k, hists[k]);
        memcpy(tab, temp, n * sizeof(int));
        free(hists[k]);                     
    }

    free(hists);
    free(temp);
}

void radix_pass(int* out, int* tab, int n, int k, int* hist) {
    int* sum  = effectifs_cumules(hist, RADIX);

    for (int i = 0; i<n; i++) {
        out[sum[_extract_digit(tab[i], k)]] = tab[i];

        draw(out, n, 0, sum[_extract_digit(tab[i], k)]); // La copie de tab dans out sert uniquement à ce que ce print soit joli

        sum[_extract_digit(tab[i], k)]++;
    }

    free(sum);
}


int** histograms(int* tab, int n, int digits) {
    int** hists = malloc(sizeof(int*) * digits);

    for (int i=0; i<digits; i++) {
        hists[i] = malloc(sizeof(int) * RADIX);
        memset(hists[i], 0, sizeof(int) * RADIX);
    }

    for (int i = 0; i<n; i++) {
        for (int k = 0; k<digits; k++) {
            hists[k][_extract_digit(tab[i], k)]++;
        }
    }
    return hists;
}


int* effectifs_cumules(int* hist, int n) { // Ici, n représente la taille de hist, donc RADIX (et pas la taille de in)
    int* sum = malloc(sizeof(int) * n);
    memset(sum, 0, sizeof(int) * n);

    for (int i = 1; i<n; i++) {
        sum[i] = hist[i-1] + sum[i-1];
    }
    return sum;
}

int _extract_digit(int n, int k) {
    return (n >> (k*BLOCK_SIZE)) & MASK;
}


// Renvoie le maximum d'un array
int _get_max(int* arr, int n) {
    int max = 0;
    for (int i = 0; i<n; i++) {
        if (arr[i] > arr[max]) max = i;
    }
    return arr[max];
}


// Renvoie le nombre de fois que l'on doit appeler radix_pass
int _get_digits(int n) {
    int digits;
    for (digits = 1; (n >>= BLOCK_SIZE); digits++);
    return digits;
}