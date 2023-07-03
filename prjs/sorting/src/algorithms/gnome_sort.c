#include <stdlib.h>
#include <stdio.h>
#include "../../headers/main.h"


void gnome_sort(int* tab, int n) {
    if (n == 1) return;
    int buff;
    int i = 0;
    while (i <= n-1) {
        if (i == 0) i++;
        else if (tab[i-1] > tab[i]) {
            buff = tab[i];
            tab[i] = tab[i-1];
            tab[i-1] = buff;
            i--;
        }
        else {
            i++;
        }
        draw(tab, n, 0, i);
    }
}