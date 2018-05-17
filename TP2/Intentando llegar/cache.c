#include <stdlib.h>
#include <stdio.h>
#include "cache.h"
#include <string.h>
#define MEMORY_SIZE 4096 //4kb
#define BLOCK_SIZE 32 // in bytes
#define LINES_PER_WAY 16

unsigned char *create_memory() {
    unsigned char *v = NULL;
    v = malloc(MEMORY_SIZE * sizeof(unsigned char));
    
    //Salimos del programa si no hay memoria
    if(NULL == v) {
        printf("No se ha podido reservar memoria para representar a la memoria principal");
        exit(EXIT_FAILURE);
    }
    return v;
}

cache_block_t* create_cache_block() {
    cache_block_t *cache;
    unsigned char *v = NULL;
    v = (unsigned char*) malloc(BLOCK_SIZE * sizeof(unsigned char));
    
    //Salimos del programa si no hay memoria
    if(NULL == v) {
        printf("No se ha podido reservar memoria para representar a la memoria principal");
        exit(EXIT_FAILURE);
    }
    cache->data = v;
    cache->V = 0; //Valido es 1 e invalido es 0
    cache->tag = 0;
    return cache;
}

//Memoria principal -variable global
unsigned char *memory;

//Memoria cache -variable global
cache_t *cache_memory;

void init() {
    memory = create_memory();
    cache_memory->miss = 0;
    cache_memory->access = 0;
    for (int i = 0; i < LINES_PER_WAY; ++i) {
        cache_memory->cache_1st_way[i] = create_cache_block();
        cache_memory->cache_2nd_way[i] = create_cache_block();
    }
}

unsigned char read_byte(int address) {
    //Debemos buscar la direccion en cache
    int block_number_in_memory = address % BLOCK_SIZE;
    int index = block_number_in_memory % LINES_PER_WAY;
    int tag = block_number_in_memory / LINES_PER_WAY;
    int offset = address & 0x1F;

    if (cache_memory->cache_1st_way[index]->V == 1) {
        //Comparo tags
        if (cache_memory->cache_1st_way[index]->tag == tag) {
            //Hay hit y devolvemos el dato guardado en cache
            unsigned char dato = cache_memory->cache_1st_way[index]->
                    data[block_number_in_memory - 1];
            printf("El dato guardado en cache es: %u", dato);
            return dato;
        }
    } else if (cache_memory->cache_2nd_way[index]->V == 1) {
        //Comparo tags
        if (cache_memory->cache_2nd_way[index]->tag == tag) {
            //Hay hit y devolvemos el dato guardado en cache
            unsigned char dato = cache_memory->cache_2nd_way[index]->
                    data[block_number_in_memory - 1];
            printf("El dato guardado en cache es: %u", dato);
            return dato;
        }
    } else {
        //Hay MISS entonces guardamos el dato de memoria en cache con politica LRU si esta lleno
        if (lista_largo(cache_memory->LRU_list) == 32) {
            //Hay que sacar el del final que seria el menos usado
            
        }
    }
}
    


int write_byte(int address, unsigned char value);


unsigned int get_miss_rate() {
	//return this->miss_rate;
    return 0;
}

void destroy_cache() {
	//free(this->cache_1st_way);
	//free(this->cache_2nd_way);
}
