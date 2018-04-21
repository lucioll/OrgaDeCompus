#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "matrix.h"

#define ERROR_PARAM "Encabezado %s no cmpatible, deberia ser n_fil n_col. \n"
#define ERROR_FILE "No se puede abrir el archivo <%s>.\n"
#define ERROR_PARSER "Error en linea: \n%sDistintos campos a %d esperados \
o un esapcio en final de linea.\n"
#define ERROR_FILA "Error en el parametro de filas, esperadas: %d.\n"
#define ERROR_VALUE "Error en la linea: \n%s<%s> no es un numero.\n"

size_t contar_sep(const char *str, char sep)
{
	size_t cant_sep = 0, i = 0;
	while(str[i] != '\0'){
		if(str[i] == sep && str[i+1] != '\n') cant_sep++;
		i++;
	}
	return cant_sep; 
}

char **split(const char *str, char sep, size_t *size)
{	
	if(sep == '\0' || !str) return NULL;
	
	size_t cant_sep = contar_sep(str,sep) + 2;
	*size = cant_sep;
	
	char **vec_split = malloc(cant_sep * sizeof(char*));
	if(vec_split == NULL) return NULL;
	
	if(!str[0]){	//Vacio si la cadena es vacia.
		vec_split[0] = strndup(str,0);
		vec_split[1] = NULL; 
		return vec_split;
	}
	
	size_t inicio = 0, final = 0, pos_vec = 0, i = 0;
	while(str[i] != '\0'){
		while(str[final] != sep && str[final] != '\0'){
			final++;
		}
		//Agrego copia de cadena de largo (final-inicio).
		vec_split[pos_vec] = strndup(str+inicio, final-inicio);
		i = final;
		inicio = ++final;
		pos_vec++;
	} 
	vec_split[cant_sep-1] = NULL; //NULL al final.
	return vec_split;
}

void free_strv(char *strv[])
{	
	size_t pos = 0;
	while(strv[pos] != NULL){
		free(strv[pos]);
		pos++;
	}
	free(strv);
}

void parser_destroy(FILE *fp, char **split, char* buffer)
{
	if(fp) fclose(fp);
	if(split) free_strv(split);
	if(buffer) free(buffer);
}

int file_parser(char *file_name, int *fil, int *col, long long **m)
{
	char *buffer = NULL;
	char **s;
	size_t len = 0, split_len = 0;
	int read;
	
	FILE *fp = fopen(file_name, "r");
	if (!fp) {
		fprintf(stderr, ERROR_FILE, file_name);
		return 1;
	}
	// Encabezado
	read = getline(&buffer, &len, fp);
	s = split(buffer, ' ', &split_len);
	
	if((split_len-1) != 2) {
		fprintf(stderr, ERROR_PARAM, buffer);
		parser_destroy(fp, s, buffer);
		return 1;
	}
	*fil = atoi(s[0]);
	*col = atoi(s[1]);
	
	long long *matrix = (long long *) malloc(*fil * \
	*col * sizeof(long long));
	
	free_strv(s);
	int f = 0, pos = 0;
    while ((read = getline(&buffer, &len, fp)) != -1) {
       	s = split(buffer, ' ', &split_len);
       	if((split_len-1) != *col) {
			fprintf(stderr, ERROR_PARSER, buffer, *col);
			parser_destroy(fp, s, buffer);
			free(matrix);
			return 1;
		}
		for (int i = 0; i < *col; i++){
			matrix[pos] = atoi(s[i]); //Falta verificar si es una valor posible.
			pos++;
		}
       	f++;
       	free_strv(s);
    }
    
    if(f != *fil){
		fprintf(stderr, ERROR_FILA, *fil);
		parser_destroy(fp, NULL, buffer);
		free(matrix);
		return 1;
	}
	
	*m = matrix;
	free(buffer);
	fclose(fp);
	return 0;
}

void print_traspuesta(unsigned int fila, unsigned int columna, long long *matrix)
{
	for(int i = 0; i < columna; i++){
		for(int j = 0; j < fila; j++){
			printf("%d ", (int)matrix[i+j*columna]);
		}
		printf("\n");
	}
}
