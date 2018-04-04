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

#define VERSION 1.0

size_t fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n",
            filename, strerror(errno));

    return -1;
}

void show_version() {
	printf("v%f\n", VERSION);
}

void show_help() {
	char archivo_help[] = "help.txt";
	FILE *fp = fopen(archivo_help, "r");
	if (!fp) return;

	size_t size_archive = fsize(archivo_help);
	char buffer[size_archive + 1];

	size_t read_bytes = fread(buffer, sizeof(char), size_archive, fp);
	if (size_archive != read_bytes) 
	{
		printf("Error with help.txt\n");
		return;
	}
	buffer[size_archive] = '\0';

	puts(buffer);
	fclose(fp);
}

void print_num_lines(char input_file[]) {
    FILE* input;
    input = fopen(input_file, "r");
   
    if (! input) {
        printf("Error with %s\n",input_file);
    } else {
        resultado_t contador;
        contador_init(&contador);
        contador_main(&contador, input);
        size_t lineas = contador_get_cant_lineas(&contador);
        printf("%zu\n", lineas);
    }
    fclose(input);
}

void print_num_words(char input_file[]) {
    FILE* input;
    input = fopen(input_file, "r");
   
    if (! input) {
        printf("Error with %s\n",input_file);
    } else {
        resultado_t contador;
        contador_init(&contador);
        contador_main(&contador, input);
        size_t palabras = contador_get_cant_palabras(&contador);
        printf("%zu\n", palabras);
    }
    fclose(input);
}

void print_num_characters(const char input_file[]) {
	FILE *input = fopen(input_file, "r");
	if (!input) return;

	resultado_t contador;
    contador_init(&contador);
    contador_main(&contador, input);
    size_t caracteres = contador_get_cant_caracteres(&contador);
    printf("%zu\n", caracteres);
	fclose(input);
}


int main (int argc, char *argv[]) {
	bool help, version, lines,words,characters, input;
	help = version = lines = words = characters = input = false;
	char *input_file;
	input_file = NULL;
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
			case 'w' :
				words = true;
			case 'c' :
				characters = true;
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
	} else if (lines && input) {
		clock_t tiempo_inicio, tiempo_final;
		double segundos;

		tiempo_inicio = clock();
		print_num_lines(input_file);

		tiempo_final = clock();

		segundos = (difftime(tiempo_inicio,tiempo_final) * 1000 * (-1)) / CLOCKS_PER_SEC; //tiempo en ms

		printf("Tiempo insumido:%lfms\n",segundos); 
		
	} else if (words && input) {
		clock_t tiempo_inicio, tiempo_final;
		double segundos;

		tiempo_inicio = clock();
		print_num_words(input_file);
		tiempo_final = clock();
		segundos = (difftime(tiempo_inicio,tiempo_final) * 1000 * (-1)) / CLOCKS_PER_SEC; //tiempo en ms

		printf("Tiempo insumido:%lfms\n",segundos); 
		
	} else if (characters && input) {
		clock_t tiempo_inicio, tiempo_final;
		double segundos;

		tiempo_inicio = clock();
		print_num_characters(input_file);
		tiempo_final = clock();
		segundos = (difftime(tiempo_inicio,tiempo_final) * 1000 * (-1)) / CLOCKS_PER_SEC; //tiempo en ms

		printf("Tiempo insumido:%lfms\n",segundos);

	}else if (!feof(stdin)) {
		
	}
	return EXIT_SUCCESS;
}

