#ifndef CACHE_H
#define CACHE_H

#include "lista.h"

//Estructura para un bloque de cache
typedef struct cache_block {
    unsigned char *data;
    int tag;
    int V;
}cache_block_t;

//Estructura para una cache
typedef struct cache {
    lista_t *LRU_list; //Lista enlazada de 32 lineas
    cache_block_t **cache_1st_way; //vector de 16 lineas de 32 bloques
    cache_block_t **cache_2nd_way; //vector de 16 lineas de 32 bloques
    unsigned int miss;
    unsigned int miss_rate; //Creo que es al pedo
    unsigned int access; //¿Aumenta solo al acceder a memoria principal?
}cache_t;


//counters
int hit_count = 0;
int miss_count = 0;
int eviction_count = 0;

void init();
unsigned char read_byte(int address);
int write_byte(int address, unsigned char value);
unsigned int get_miss_rate();
void destroy_cache();

#endif // CACHE_H
