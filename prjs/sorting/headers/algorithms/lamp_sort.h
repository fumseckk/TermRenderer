#ifndef LAMP_H
#define LAMP_H
#include <stdbool.h>


typedef struct {
  int g; // borne gauche
  int d; //borne droite
} Intervalle; // modélise l'intervalle d'entiers [g,d]

typedef struct _m{
  Intervalle val;
  struct _m * prev; 
} maillon;

typedef struct {
  maillon * top;
} Stack;//poignee de pile : pointe sur l'élément le + récent

//Stack implementation
Stack* init();
bool is_empty(Stack * p);
void push(Stack * p, Intervalle * v);
Intervalle pop(Stack * p); 

//Sort algo
int partition(int* tab, int n, int g, int d);
void lamp_sort(int* tab, int n);
int choix(int n);
int partition_rand(int* tab, int n, int g, int d);
void lampSort_rand(int* tab, int n);
int quick_select(int* tab, int k, int n);
#endif
