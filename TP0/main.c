#include <stdbool.h>
#include <stdio.h>
#include "contador_carac_palab_lineas.h"
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#define VERSION 2.0

size_t fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n",
            filename, strerror(errno));

    return -1;
}


int analyse_text(bool lines, bool words, bool characters, char *input_file) {
	FILE* input;

	if (!input_file) {
		input = stdin;
		input_file = "STDIN";
	} else {
		input = fopen(input_file, "r");	}
	if (!input) {
		printf("Error: %s\n", strerror(errno));
		return -1;
	}
    
    resultado_t contador;
    contador_init(&contador);
    contador_main(&contador, input);

    printf("\n");
    if (lines) {
    	size_t lineas = contador_get_cant_lineas(&contador);
    	fprintf(stdout, "%zu %s\n", lineas, input_file);
    }

    if (words) {
    	size_t palabras = contador_get_cant_palabras(&contador);
    	fprintf(stdout, "%zu %s\n", palabras, input_file);
    }

    if (characters) {
        size_t caracteres = contador_get_cant_caracteres(&contador);
    	fprintf(stdout, "%zu %s\n", caracteres, input_file);
    }

    fclose(input);
    return 0;
}


void show_version() {
	printf("v%f\n", VERSION);
}

void show_help() {
	char archivo_help[] = "help.txt";
	FILE *fp = fopen(archivo_help, "r");
	if (!fp) {
		printf("Error: %s\n", strerror(errno));
		return;
	}

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

int main (int argc, char *argv[]) {
	bool help, version, lines,words, characters, input;
	help = version = lines = words = characters = input = false;
	char* input_file = NULL;
	int flag = 0;
	struct option opts[] = {
		{"version", no_argument, 0, 'V'},
		{"help", no_argument, 0, 'h'},
		{"lines", no_argument, 0, 'l'},
		{"words", no_argument, 0, 'w'},
		{"characters", no_argument, 0, 'c'},
		{"input", required_argument, 0, 'i'}
	};

	while ((flag = getopt_long(argc, argv, "Vhlwci:", opts, NULL)) != -1) {
		switch (flag) {
			case 'V' :
				version = true;
				break;
			case 'h' :
				help = true;
				break;
			case 'l' :
				lines = true;
				break;
			case 'w' :
				words = true;
				break;
			case 'c' :
				characters = true;
				break;
			case 'i' :
				input_file = optarg;
				input = true;
				break;
		}
	}

	if (help) {
		show_help();
	} else if (version) {
		show_version();
	} else if (lines) {
		clock_t tiempo_inicio, tiempo_final;
		double segundos;

		tiempo_inicio = clock();
		analyse_text(lines, words, characters,input_file);

		tiempo_final = clock();

		segundos = (difftime(tiempo_inicio,tiempo_final) * 1000 * (-1)) / CLOCKS_PER_SEC; //tiempo en ms

		printf("Tiempo insumido:%lfms\n",segundos); 
	} else if (words) {
		clock_t tiempo_inicio, tiempo_final;
		double segundos;

		tiempo_inicio = clock();
		analyse_text(lines, words, characters,input_file);
		tiempo_final = clock();
		segundos = (difftime(tiempo_inicio,tiempo_final) * 1000 * (-1)) / CLOCKS_PER_SEC; //tiempo en ms

		printf("Tiempo insumido:%lfms\n",segundos); 
	} else if (characters) {
		clock_t tiempo_inicio, tiempo_final;
		double segundos;

		tiempo_inicio = clock();
		analyse_text(lines, words, characters,input_file);
		tiempo_final = clock();
		segundos = (difftime(tiempo_inicio,tiempo_final) * 1000 * (-1)) / CLOCKS_PER_SEC; //tiempo en ms

		printf("Tiempo insumido:%lfms\n",segundos);
	} else if ((!lines) && (!words) && (!characters)) {
		clock_t tiempo_inicio, tiempo_final;
		double segundos;

		tiempo_inicio = clock();
		analyse_text(true, true, true, input_file);
		tiempo_final = clock();
		segundos = (difftime(tiempo_inicio,tiempo_final) * 1000 * (-1)) / CLOCKS_PER_SEC; //tiempo en ms

		printf("Tiempo insumido:%lfms\n",segundos);

	} else if (!input) {
		clock_t tiempo_inicio, tiempo_final;
		double segundos;

		tiempo_inicio = clock();
		analyse_text(lines, words, characters,input_file);
		tiempo_final = clock();
		segundos = (difftime(tiempo_inicio,tiempo_final) * 1000 * (-1)) / CLOCKS_PER_SEC; //tiempo en ms

		printf("Tiempo insumido:%lfms\n",segundos);
	}
	return EXIT_SUCCESS;
}
