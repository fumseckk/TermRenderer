#include <stdlib.h>
#include <stdio.h>
#include "../../headers/main.h"


void bubble_sort(int* tab, int n) {
    if (n == 1) return;
    int buff;
    for (int j = 0; j < n-1; j++) {
        for (int i = 0; i < n-j-1; i++) {
            if (tab[i] > tab[i+1]) {
                buff = tab[i+1];
                tab[i+1] = tab[i];
                tab[i] = buff;
            }
            draw(tab, n, 0, i+1);
        }
    }
}