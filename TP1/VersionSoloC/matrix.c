#include <stdbool.h>
#include <getopt.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include "matrix.h"

#define ERROR_OPEN "No se puede abrir el archivo <%s>.\n"
#define ERROR_FILA "Error al obtener parametro de filas esperadas: %d.\n"
#define ERROR_COLUMNA "Error al obtener parametro de columnas esperadas: %d.\n"
#define ERROR_FILE "Revisar archivo, pueden faltar/sobrar datos o no contener numeros enteros. \n"


int leer_encabezado(FILE *fp, unsigned int *fil, unsigned int *col) {
	int read = fscanf(fp, "%u", fil);
	if(read != 1){
		fprintf(stderr, ERROR_FILA, *fil);
        return 1;
    }
	read = fscanf(fp, "%u", col);
	if (read != 1) {
		fprintf(stderr, ERROR_COLUMNA, *col);
		return 1;
	}
	return 0;
}

int file_parser(char *file_name,unsigned int *fil, unsigned int *col, long long **m) {
	int read;
	
	FILE *fp = fopen(file_name, "r");
	if (!fp) {
		fprintf(stderr, ERROR_OPEN, file_name);
		return 1;
	}
	// Encabezado
	int error = leer_encabezado(fp, fil, col);
	if (error == 1) {
		fclose(fp);
		return 1;
	}
	
	long long *matrix = (long long *) malloc(*fil * \
		*col * sizeof(long long));

	long long numero = 0;
	unsigned int pos = 0;
    for (unsigned int i = 0; i < *fil ; ++i) {
        for (unsigned int j = 0; j < *col ; ++j) {
            read = fscanf(fp,"%lld",&numero);
            if(read != 1){
                fprintf(stderr, ERROR_FILE);
                free(matrix);
                fclose(fp);
                return 1;
            }
            matrix[pos] = numero;
            pos++;
        }	
	}
	// verifico que no hayan datos de mas
	read = fscanf(fp,"%lld",&numero);
	if (read == 1) {
		fprintf(stderr, ERROR_FILE);
		free(matrix);
		fclose(fp);
		return 1;
	}
		
	*m = matrix;
	fclose(fp);
	return 0;
}

int trasponer(unsigned int filas, unsigned int columnas,long long *entrada, \
	 long long *salida) {
	unsigned int pos = 0;
	for(unsigned int i = 0; i < columnas; i++) {
		for(unsigned int j = 0; j < filas; j++) {
			salida[pos] = (long long) entrada[(i+j*columnas)];
			pos++;
		}
	}
	return 0;
}
