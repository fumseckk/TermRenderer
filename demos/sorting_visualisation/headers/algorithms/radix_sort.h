#ifndef RADIX_H
#define RADIX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "../main.h"

#define BLOCK_SIZE RADIX_BLOCK_SIZES // paramètre dans headers/main.h

#define RADIX ((int) pow(2, BLOCK_SIZE))
#define MASK  ((int) pow(2, BLOCK_SIZE) - 1)


void radix_sort(int* tab, int n);
void radix_pass(int* out, int* in, int n, int k, int* hist);
int** histograms(int* tab, int n, int digits);
int* effectifs_cumules(int* hist, int n);
int  _get_max(int* tab, int n);
int  _get_digits(int n);
int  _extract_digit(int n, int k);


#endif