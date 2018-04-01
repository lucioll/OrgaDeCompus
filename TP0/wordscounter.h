#ifndef __WORDSCOUNTER_H__
#define __WORDSCOUNTER_H__

#include <string.h>
#include <stdio.h>

// Tipo wordscounter_t: procesa cantidad de palabras dentro de un archivo.
typedef struct {
    size_t words;
} wordscounter_t;

// Initializa la instancia self para ser utilizada
// Pre: self apunta un sector válido de memoria
void wordscounter_create(wordscounter_t *self);

// Destruye la instancia self liberando sus recursos.
// Pre: self fue inicializado mediante wordscounter_create
void wordscounter_destroy(wordscounter_t *self);

// Retorna la cantidad de palabras procesadas
// Pre: self fue inicializado mediante wordscounter_create
size_t wordscounter_get_words(wordscounter_t *self);

// Procesa el contenido de text_file, contando sus palabras.
// Pre: self fue inicializado mediante wordscounter_create.
//      text_file es un archivo válido, abierto para lectura.
void wordscounter_process(wordscounter_t *self, FILE *text_file);

#endif
 
