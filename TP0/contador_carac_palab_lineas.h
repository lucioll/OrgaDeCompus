#ifndef CONTADOR_CARAC_PALAB_LINEAS_H
#define CONTADOR_CARAC_PALAB_LINEAS_H

#include <stdio.h>
#include <stddef.h>

typedef struct {
	size_t cant_caracteres;
	size_t cant_palabras;
	size_t cant_lineas;
}resultado_t;


void contador_init(resultado_t* res);
size_t contador_get_cant_caracteres(resultado_t* res);
size_t contador_get_cant_palabras(resultado_t* res);
size_t contador_get_cant_lineas(resultado_t* res);
void contador_main(resultado_t* res, FILE* archivo);
char contador_next_state(resultado_t* res, char state, char c);

#endif
