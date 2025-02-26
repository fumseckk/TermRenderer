#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include "../../headers/main.h"
#include "../../headers/algorithms/lamp_sort.h"


Stack* init (){
  Stack * p =   (Stack *)malloc(sizeof(Stack));
  p->top = NULL;
  return p;
}

bool is_empty(Stack * p){
  return (bool) (p->top==NULL);
}

Intervalle pop(Stack *pg){
  assert(!is_empty(pg));/*on suppose pg non vide*/
  maillon * pm = pg->top;
  Intervalle x = pm->val;
  pg->top=pg->top->prev;// la pg pointe sur le pred du maillon 1
  free(pm);
  return x;
}

void push(Stack *pt, Intervalle* v){
  // ajoute au sommet de la pile
  maillon * pm = (maillon *)malloc(sizeof(maillon));// nv maillon
  pm->val = *v;
  pm->prev = pt->top;
  pt->top = pm;
}


// ------------------ DETERMINISTIC IMPLEMENTATION ------------------

int partition(int* tab, int n, int g, int d) {
    assert(g <= d);
    if (g == d) {
        draw(tab, n, 0, g);
        return g;
    }
    int j = g + 1; // indice qu'on augmente petit à petit, dès qu'on voit un nb plus petit que le pivot
    int p = g; // pivot
    int buff; // buffer pour échanges
    for (int i = g+1; i<d; i++) {
        draw(tab, n, 0, i);
        if (tab[i] <= tab[p]) {
            buff = tab[i];
            tab[i] = tab[j];
            tab[j] = buff;
            j++;
            draw(tab, n, 0, j);
        }
    }
    buff = tab[p];
    tab[p] = tab[j-1];
    tab[j-1] = buff;
    draw(tab, n, 0, j-1);
    return j-1;
}

void lamp_sort(int* tab, int n) {
    Stack* stack = init();

    Intervalle inter;
    inter.g = 0;
    inter.d = n;
    push(stack, &inter);

    do {
        inter = pop(stack);
        if (inter.d - inter.g != 0 && inter.d - inter.g != 1) {
            int p = partition(tab, n, inter.g, inter.d);
            Intervalle inter_g;
            Intervalle inter_d;
            inter_g.g = inter.g;
            inter_g.d = p;
            inter_d.g = p + 1;
            inter_d.d = inter.d;
            push(stack, &inter_d);
            push(stack, &inter_g);
        }
    } while (!is_empty(stack));
}


// ------------------ RANDOM IMPLEMENTATION ------------------

int choix(int n) {
    return (int) (rand() / (double) RAND_MAX * (n-1));
}

int partition_rand(int* tab, int n, int g, int d) {
    assert(g <= d);
    n = n;
    if (g == d) return g;
    int j = g + 1; // indice qu'on augmente petit à petit, dès qu'on voit un nb plus petit que le pivot
    int p = choix(d - g) + g; // pivot
    int buff; // buffer pour échanges
    buff = tab[g];
    tab[g]    = tab[p];
    tab[p] = buff;
    p = g;
    for (int i = g+1; i<d; i++) {
        if (tab[i] <= tab[p]) {
            buff = tab[i];
            tab[i] = tab[j];
            tab[j] = buff;
            j++;
        }
    }
    buff = tab[p];
    tab[p] = tab[j-1];
    tab[j-1] = buff;
    return j-1;
}

void lamp_sort_rand(int* tab, int n) {
    Stack* stack = init();
    Intervalle inter;
    inter.g = 0;
    inter.d = n;
    push(stack, &inter);

    do {
        inter = pop(stack);
        if (inter.d - inter.g != 0 && inter.d - inter.g != 1) {
            int p = partition_rand(tab, n, inter.g, inter.d);
            Intervalle inter_g;
            Intervalle inter_d;
            inter_g.g = inter.g;
            inter_g.d = p;
            inter_d.g = p + 1;
            inter_d.d = inter.d;
            push(stack, &inter_g);
            push(stack, &inter_d);
        }
    } while (!is_empty(stack));
}