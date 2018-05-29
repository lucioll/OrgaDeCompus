//
// Created by lucio on 15/05/18.
//
#define _GNU_SOURCE
#include <stdbool.h>
#include <getopt.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "cache.h"

#define VERSION 1.0
#define ERROR -1
#define MEMORY_SIZE 4096 // in bytes
#define BAD_ADDRESS "Direccion negativa o mayor a 4096<%s>.\n"
#define ERROR_OPEN "No se puede abrir el archivo <%s>.\n"
#define READ_BYTE "R"
#define WRITE_BYTE "W"
#define MISS_RATE "MR"

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
        fprintf(stderr,"Error: %s\n", strerror(errno));
        return;
    }
    buffer[size_archive] = '\0';
    
    puts(buffer);
    fclose(fp);
}

int file_parser(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        fprintf(stderr, ERROR_OPEN, file_name);
        return 1;
    }
    cache_t cache_;
    init(&cache_);
    
    char *line = NULL;
    size_t len = 0;
    
    while (getline(&line, &len, fp) != -1) {
        char *token;
        /* get the first token */
        token = strtok(line, " ");
        printf("Instruccion: %s\n",token);
        if (strcmp(token, READ_BYTE) == 0){
            token = strtok(NULL, ",");
            int address = atoi(token);
            if ((address > MEMORY_SIZE) || (address < 0)) {
                fprintf(stderr, BAD_ADDRESS, file_name);
                return 1;
            }
            printf("Resultado: %d\n", read_byte(&cache_, address));
        } else if (strcmp(token, WRITE_BYTE) == 0) {
            token = strtok(NULL, ",");
            int address = atoi(token);
            token = strtok(NULL, ",");
            int value = atoi(token);
            printf("Resultado: %d\n", write_byte(&cache_, address, value));
        } else {
            printf("Resultado: %u\n", get_miss_rate(&cache_));
        }
        printf("\n");
    }
    free(line);
    fclose(fp);
    destroy_cache(&cache_);
    return 0;
}


int main (int argc, char *argv[]) {
    bool help, version, input;
    help = version = input = false;
    char* input_file;
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
            {"input", required_argument, 0, 'i'},
    };
    
    while ((flag = getopt_long(argc, argv, "Vhi:", opts, NULL)) != -1) {
        switch (flag) {
            case 'V' :
                version = true;
                break;
            case 'h' :
                help = true;
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
    } else {
        if(file_parser(input_file) != 0){
            return ERROR;
        }
    }
    return EXIT_SUCCESS;
}
