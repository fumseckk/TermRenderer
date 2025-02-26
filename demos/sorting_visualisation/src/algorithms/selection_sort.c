#include <stdio.h>
#include <stdlib.h>
#include "../../headers/main.h"


void selection_sort(int* tab, int n) {
    int buff;
    int min;
    for (int i = 0; i<n; i++) {
        min = i;
        for (int j = i; j<n; j++) {
            draw(tab, n, 0, j);
            // On trouve le minimum de [i, n]
            if (tab[j] < tab[min]) {
                min = j;
            }
        }
        // On place ce minimum à tab[i]
        buff = tab[i];
        tab[i] = tab[min];
        tab[min] = buff;
        draw(tab, n, 0, i);
    }
}