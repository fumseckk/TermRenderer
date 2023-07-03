#ifndef MAIN_H
#define MAIN_H

#include <unistd.h>

#include "../headers/algorithms/lamp_sort.h"
#include "../headers/algorithms/bubble_sort.h"
#include "../headers/algorithms/insertion_sort.h"
#include "../headers/algorithms/selection_sort.h"
#include "../headers/algorithms/radix_sort.h"
#include "../headers/algorithms/cocktail_sort.h"
#include "../headers/algorithms/gnome_sort.h"
#include "../headers/algorithms/pancake_sort.h"

#define GRY   "\x1B[90m"
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define BOLD  "\x1B[1m"
#define RESET "\x1B[0m"

// Pour lancer un algorithme; 2 options: 
//   -> soit vous changez la valeur ci-dessous de DEFAULT_ALG et vous recompilez le programme
//   -> soit vous le précisez en argument en lançant le programme, e.g: `./sort lamp_sort`, et donc pas de recompilation.


// -------- PARAMETRES --------
#define DEFAULT_ALG radix_sort // Options: bubble_sort, insertion_sort, selection_sort, lamp_sort, gnome_sort, cocktail_sort, radix_sort, pancake_sort
#define FPS  100
#define SIZE 190
#define RADIX_BLOCK_SIZES 2
#define COMPLETED_COLOR GRN
#define SELECTED_COLOR CYN
// ----------------------------


// Note: pour ajouter un autre algorithme au code;
//   -> créer un .h et un .c aux endroits adéquats
//   -> inclure le header dans ce fichier (main.h)
//   -> inclure ce fichier (main.h) dans le .c de l'algorithms
//   -> ajouter une ligne dans le fichier main.c, dans la fonction launch_sort_alg_from_str, pour convertir l'argument en appel de fonction


#define SET_CUR_POS(x, y) printf("\x1B[%d,%dH", x, y);
#define MOV_CUR_UP(x)     printf("\x1B[%dA", x);
#define MOV_CUR_DOWN(x)   printf("\x1B[%dB", x);
#define MOV_CUR_RIGHT(x)  printf("\x1B[%dC", x);
#define MOV_CUR_LEFT(x)   printf("\x1B[%dD", x);

#define SAVE_CUR_POS      printf("\x1B[s");   
#define RESTORE_CUR_POS   printf("\x1B[u");   
#define HIDE_CUR          printf("\x1B[?25l"); 
#define SHOW_CUR          printf("\x1B[?25h");

#define SLEEP (usleep((useconds_t) 1000000/FPS))


void launch_sort_alg_from_str(int* tab, int n, char* alg_str);
int* create_array(int n);
void shuffle(int *tab, int n);
void check_if_sorted(int* tab, int n);
void draw(int* tab, int n, int mode, int index);

#endif