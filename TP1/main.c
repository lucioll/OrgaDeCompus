#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "matrix.h"
#include "trasponer.h"

#define VERSION 1.0
#define POS_MATRIX 1
#define ERROR -1

#define SIZE_LONG 8

size_t fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n",
            filename, strerror(errno));

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

void print_matriz(int fila, int columna, long long *matrix) {
	for(int i = 0; i < columna; i++){
		for(int j = 0; j < fila; j++){
			printf("%d ", (int)matrix[i+j*fila]);
		}
		printf("\n");
	}
}


int main (int argc, char *argv[]) {
	bool help, version, output;
	help = version = output = false;
	char* output_file;
	output_file = NULL;
	int flag = 0;
	struct option opts[] = {
		{"version", no_argument, 0, 'V'},
		{"help", no_argument, 0, 'h'},
		{"output", required_argument, 0, 'o'}
	};

	while ((flag = getopt_long(argc, argv, "Vho:", opts, NULL)) != -1) {
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
	} else if (output) {
		printf("%s\n", output_file);
		//(output_file);
	} else {
		unsigned int fila, columna;
		long long *entrada;
		long long * salida = malloc(fila * columna * SIZE_LONG);
		if(file_parser(argv[POS_MATRIX], &fila, &columna, &entrada) != 0){
			return ERROR;
		}
		print_traspuesta(fila, columna, entrada);
		trasponer(fila, columna, entrada, salida);
		print_matriz(fila, columna, salida);
		free(entrada);
		free(salida);
	}
	return EXIT_SUCCESS;
}
