#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <stdio.h>
#include <stdlib.h>

//Inicializa matrix
int file_parser(char *file_name, unsigned int *fil, unsigned int *col, long long **m);

// Traspone la matrix
int trasponer(unsigned int filas, unsigned int columnas,long long *entrada, \
	 long long *salida);

#endif //MATRIX
