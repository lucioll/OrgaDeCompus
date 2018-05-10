#include <stdbool.h>
#include <getopt.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "matrix.h"

#define VERSION 1.0
#define ERROR -1


size_t fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0) {
        return st.st_size;
	}
	fprintf(stderr, "Cannot determine size of %s: %s\n", filename, strerror(errno));
	return ERROR;
}

void show_version() {
	printf("Version: %.1f\n", VERSION);
}

void show_help() {
	char archivo_help[] = "help.txt";
	FILE *fp = fopen(archivo_help, "r");
	if (!fp) return;

	size_t size_archive = fsize(archivo_help);
	char buffer[size_archive + 1];

	size_t read_bytes = fread(buffer, sizeof(char), size_archive, fp);
	if (size_archive != read_bytes) {
		printf("Error: %s\n", strerror(errno));
		return;
	}
	buffer[size_archive] = '\0';

	puts(buffer);
	fclose(fp);
}

int save_matrix(char *output_name, unsigned int filas, unsigned int columnas, long long *matrix) {
	FILE* output;

	if (!output_name) {
		output = stdout;
	} else {
		output = fopen(output_name, "w");	
	}
	if (!output) {
		fprintf(stderr,"Error: %s\n", strerror(errno));
		return -1;
	}
	// Encabezado
	fprintf(output,"%d %d\n", filas, columnas);
    
    // Matriz
    unsigned int pos = 0;
    for (unsigned int i = 0; i < filas ; ++i) {
        for (unsigned int j = 0; j < columnas ; ++j) {
            fprintf(output,"%lld ", (long long)matrix[pos]);
            pos++;
        }
        fprintf(output,"\n");        
    }
    if (output != stdout) {
		fclose(output);
	}
    return 0;
}


int main (int argc, char *argv[]) {
	bool help, version, output, input;
	help = version = output = input = false;
	char* output_file;
	char* input_file;
	output_file = NULL;
	input_file = NULL;
	int flag = 0;
	
	if (argc < 2){
		fprintf(stderr,"Se debe ingresar un archivo de entrada.\n");
        return ERROR;
    }
	input_file = argv[1];
	input = true;
	struct option opts[] = {
		{"version", no_argument, 0, 'V'},
		{"help", no_argument, 0, 'h'},
		{"output", required_argument, 0, 'o'},
	};

	while ((flag = getopt_long(argc, argv, "Vho:i:", opts, NULL)) != -1) {
		switch (flag) {
			case 'V' :
				version = true;
				break;
			case 'h' :
				help = true;
				break;
			case 'o' :
				output_file = optarg;
				output = true;
				break;
		}
	}
	
	if (help) {
		show_help();
	} else if (version) {
		show_version();
	} else {
		unsigned int fila, columna;
		long long *entrada;
		if(file_parser(input_file, &fila, &columna, &entrada) != 0) {
			return ERROR;
		}
		
		long long *salida = (long long *) malloc(fila * \
			columna * sizeof(long long));
		trasponer(fila, columna, entrada, salida);
		save_matrix(output_file, columna, fila, salida);
		free(entrada);
		free(salida);
	} 
	return EXIT_SUCCESS;
}
