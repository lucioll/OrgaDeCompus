#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <stdio.h>
#include <stdlib.h>

//Inicializa matrix
int file_parser(char *file_name, int *fil, int *col, long long **m);

// Imprime la transpuesta
void print_traspuesta(unsigned int fila, unsigned int columna, long long *matrix);

#endif //MATRIX
