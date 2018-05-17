#ifndef CACHE_H
#define CACHE_H
#include "lista.h"

//Estructura para un bloque de cache
typedef struct cache_block {
	lista_t *LRU_list; 
    unsigned char* data;
    unsigned char tag; // 3 bits
    unsigned char V; // 1 bit
}cache_block_t;


// definicion estructura cache 
typedef struct cache {
	cache_block_t **cache_1st_way; 
	cache_block_t **cache_2nd_way; 
	
	unsigned char* memory;
	
	unsigned int misses;
	unsigned int accesses;
}cache_t;

void init(cache_t* cache_memory);
unsigned char read_byte(cache_t* cache_memory, int address);
int write_byte(cache_t* cache_memory, int address, unsigned char value);
unsigned int get_miss_rate();
void destroy_cache();

#endif // CACHE_H
