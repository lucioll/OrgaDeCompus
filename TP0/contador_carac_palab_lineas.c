#include "contador_carac_palab_lineas.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#define STATE_WAITING_WORD 0
#define STATE_IN_WORD 1
#define STATE_FINISHED 2
#define DELIM_CARACTERES " ,.;:\v\r\t\f\n"
#define FIN_LINEA "\n"
#define ERROR 1


void contador_init(resultado_t* res) {
    res->cant_caracteres = 0;
    res->cant_palabras = 0;
    res->cant_lineas = 0;
}

size_t contador_get_cant_caracteres(resultado_t* res) {
    return res->cant_caracteres;
}

size_t contador_get_cant_palabras(resultado_t* res) {
    return res->cant_palabras;
}

size_t contador_get_cant_lineas(resultado_t* res) {
    return res->cant_lineas;
}

void contador_main(resultado_t* res, FILE* archivo) {
    char state = STATE_WAITING_WORD;
    do {
        int c = getc(archivo);
        state = contador_next_state(res, state, c);
    } while (state != STATE_FINISHED);
}

char contador_next_state(resultado_t* res, char state, char c) {
    char next_state = state;
    if (c != EOF) res->cant_caracteres++;

    if (state == STATE_WAITING_WORD) {
        if (c == EOF) { 
            next_state = STATE_FINISHED;
        } else if (strchr(DELIM_CARACTERES, c) == NULL) {
            next_state = STATE_IN_WORD;
        }else if (strchr(FIN_LINEA, c) != NULL) {
                res->cant_lineas++;
            }
    } else if (state == STATE_IN_WORD) {
        if (c == EOF) { 
            next_state = STATE_FINISHED;
            res->cant_palabras++;
            res->cant_lineas++;
        } else if (strchr(DELIM_CARACTERES, c) != NULL) {
            res->cant_palabras++;
            next_state = STATE_WAITING_WORD;
            if (strchr(FIN_LINEA, c) != NULL) {
				res->cant_lineas++;
			}
        }
    }
    return next_state;
}

