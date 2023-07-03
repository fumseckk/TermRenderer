#include <stdlib.h>
#include <stdio.h>
#include "../../headers/main.h"


void insertion_sort(int* tab, int n) {
    if (n == 1) return;
    int j; // 
    int key;
    for (int i=1; i<n; i++) {
        key = tab[i];
        j = i-1;
        draw(tab, n, 0, i);
        while (j >= 0 && tab[j] > key) {
            draw(tab, n, 0, j);
            tab[j+1] = tab[j];
            j = j - 1;
        }
        tab[j+1] = key;
        draw(tab, n, 0, j+1);
    }
}