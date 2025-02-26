#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../../headers/main.h"


// Bubble sort, mais des deux côtés à la fois pour plus de style
void cocktail_sort(int* tab, int n) {
    if (n == 1) return;
    int buff;
    bool comp = true; // Permet de sortir de la boucle dès que la boucle est triée
    for (int j = 0; comp == true; j++) {
        comp = false;
        
        for (int i = j; i < n-j-1; i++) {
            if (tab[i] > tab[i+1]) {
                buff = tab[i+1];
                tab[i+1] = tab[i];
                tab[i] = buff;
                comp = true;
            }
            draw(tab, n, 0, i+1);
        }

        for (int i = n-j-2; i >= j; i--) {
            if (tab[i] < tab[i-1]) {
                buff = tab[i-1];
                tab[i-1] = tab[i];
                tab[i] = buff;
                comp = true;
            }
            draw(tab, n, 0, i-1);
        }
    }
}