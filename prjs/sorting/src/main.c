#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "../headers/main.h"


// Tous les paramètres (l'algorithme utilisé, SIZE (taille des données), les FPS, les couleurs d'affichage) sont à modifier dans headers/main.h
// Usage: ./sort (sorting_algorithm)
int main(int argc, char* argv[]) {
    srand(time(NULL));
    int n = SIZE;
    int* tab = create_array(n);
    HIDE_CUR;
    
    for (int i = 0; i<n/4+1; i++) {
        printf("\n");
    }

    if (argc > 1)
        launch_sort_alg_from_str(tab, n, argv[1]); // Si algorithme précisé, on lance l'algorithme en question, sinon on lance l'algorithme par défaut
    else {
        DEFAULT_ALG(tab, n);                   
    }

    check_if_sorted(tab, n);
    SHOW_CUR;
    return 0;
}


// Lance un algorithme à partir d'un string passé en argument
void launch_sort_alg_from_str(int* tab, int n, char* alg_str) {
    if(!strcmp(alg_str, "--help") || !strcmp(alg_str, "-h")) {
        SHOW_CUR;
        MOV_CUR_UP(n);
        printf("Options:\n");
        printf("bubble_sort, insertion_sort, selection_sort, lamp_sort, radix_sort, cocktail_sort, gnome_sort, pancake_sort\n");
        exit(EXIT_SUCCESS);
    }
    else if (!strcmp(alg_str, "bubble_sort"))    bubble_sort(tab, n);
    else if (!strcmp(alg_str, "insertion_sort")) insertion_sort(tab, n);
    else if (!strcmp(alg_str, "selection_sort")) selection_sort(tab, n);
    else if (!strcmp(alg_str, "lamp_sort"))      lamp_sort(tab, n);
    else if (!strcmp(alg_str, "radix_sort"))     radix_sort(tab, n);
    else if (!strcmp(alg_str, "cocktail_sort"))  cocktail_sort(tab, n);
    else if (!strcmp(alg_str, "gnome_sort"))     gnome_sort(tab, n);
    else if (!strcmp(alg_str, "pancake_sort"))   pancake_sort(tab, n);

    else  {
        SHOW_CUR;
        MOV_CUR_UP(n);
        printf("L'algorithme précisé ne fait pas partie de la liste d'algorithmes\n");
        exit(EXIT_FAILURE);
    }
}


// Crée un array mélangé, de taille n, avec des valeurs de 1 à n distinctes.
int* create_array(int n) {
    int* tab = malloc(sizeof(int) * n);
    for (int i=0; i<n; i++) {
        tab[i] = i+1;
    }
    shuffle(tab, n);
    return tab;
}


// Mélange un array avec l'algorithme de Fisher-Yates
void shuffle(int *tab, int n) {
    int j, tmp;
    for (int i = n-1; i > 0; i--) { 
        j = rand() % (i + 1);
        tmp = tab[j];
        tab[j] = tab[i];
        tab[i] = tmp;
    }
}


// Vérifie, à la fin du tri, que l'array est bien trié.
void check_if_sorted(int* tab, int n) {
    for (int i = 0; i < n-1; i++) {
        assert(tab[i] <= tab[i+1]);
        draw(tab, n, 1, i+1);
    }
}

// Print l'array en terminal sous forme d'histogramme.
// 3 cas: si mode = 0, print tout en blanc, sauf tab[selected] en bleu
//        si mode = 1, le sort est fini et on est en train de vérifier que tout est trié: print tout jusqu'à `selected` en vert, puis le reste en blanc
//        si mode = 2, print tout en blanc
void draw(int* tab, int n, int mode, int selected) {
    HIDE_CUR;
    
    for (int line = 0; line < n/4 + 1; line++) { // Compteur de lignes dans le terminal. `n` est utilisé ici en tant que valeur maximale de l'array; ne marche pas sinon.
        for (int i = 0; i < n; i++) {
            if (mode == 0 && i == selected) printf(SELECTED_COLOR);
            if (mode == 1 && i <= selected) printf(COMPLETED_COLOR);

            if      (tab[i] % 4 == 1 && tab[i] / 4 == line) printf("▂" RESET);
            else if (tab[i] % 4 == 2 && tab[i] / 4 == line) printf("▄" RESET);
            else if (tab[i] % 4 == 3 && tab[i] / 4 == line) printf("▆" RESET);
            else if (tab[i] / 4 > line)                     printf("█" RESET);
            else                                            printf(" " RESET);
        }
        MOV_CUR_UP(1);
        MOV_CUR_LEFT(n)
    }
    MOV_CUR_DOWN(n/4+1);
    SHOW_CUR;
    SLEEP;
}