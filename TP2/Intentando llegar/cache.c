#include <stdlib.h>
#include "cache.h"
#include "lista.h"
#define MEMORY_SIZE 4096 // in bytes
#define CACHE_SIZE 1024
#define WORDS_BY_BLOCK 32 // in bytes
#define N_WAYS 2
#define BLOCK_SIZE 32
#define BLOCKS_PER_WAY CACHE_SIZE/(N_WAYS * BLOCK_SIZE)
#define CACHE_WAY_SIZE 512 // in bytes
#define VALID_BITS_SIZE 2 // in bytes
#define TAG_MEMORY_SIZE 6 // in bytes

#define TAG_MASK 0xE00
#define SET_NUMBER_MASK 0x1FC
#define INDEX_MASK 0x1E0
#define WORD_OFFSET_MASK 0x1C
#define BLOCK_OFFSET_MASK 0x3
#include <stdio.h>

cache_block_t* create_cache_block() {
    cache_block_t* cache_block_ = (cache_block_t*) malloc(sizeof(cache_block_t));
    cache_block_->LRU_index = 0;
    cache_block_->data = (unsigned char*) malloc(WORDS_BY_BLOCK);
    cache_block_->V = 0; //Valido es 1 e invalido es 0
    cache_block_->tag = 0;
    return cache_block_;
}


void init(cache_t* cache_memory) {
	cache_memory->memory = (unsigned char*) malloc(MEMORY_SIZE);
    cache_memory->misses = 0;
    cache_memory->accesses = 0;
    
    cache_memory->cache_1st_way = (cache_block_t**) malloc(sizeof(cache_block_t*) * BLOCKS_PER_WAY);
    cache_memory->cache_2nd_way = (cache_block_t**) malloc(sizeof(cache_block_t*) * BLOCKS_PER_WAY);
    for (int i = 0; i < BLOCKS_PER_WAY; ++i) {
        cache_memory->cache_1st_way[i] = create_cache_block();
        cache_memory->cache_2nd_way[i] = create_cache_block();
    }
}


unsigned char get_data(cache_block_t* block, int word_offset, int block_offset) {
	return block->data[4 * word_offset + block_offset];
}

/*	FALTA CARGAR EL BLOQUE EN LA CACHE CUANDO HAY UN MISS.
 * 	
 *	PARA ESO FALTA IMPLEMENTAR LA LISTA LRU (EL BLOQUE N DE LA VIA 1 COMPARTE
 * 	LA LISTA LRU CON EL BLOQUE N DE LA VIA 2). SI LA LISTA ESTA LLENA (ES DECIR
 *	EL LARGO DE LA LISTA ES 2) SE BORRA EL MENOS USADO RECIENTEMENTE.
 * 
*/

int read_byte(cache_t* cache_memory, int address) {
	int tag = address & TAG_MASK;
	// shift right tag 9 bits
	tag = tag >> 9;
	
	printf("TAG: %d\n",tag);
	
	int block_offset = address & BLOCK_OFFSET_MASK;
	
	int index = address & INDEX_MASK;
	// shift right index 5 bits
	index = index >> 5;
	
	int word_offset = address & WORD_OFFSET_MASK;
	// shift right word_offset 2 bits
	word_offset = word_offset >> 2;
	
	cache_block_t* block_1st_way = cache_memory->cache_1st_way[index];
	cache_block_t* block_2nd_way = cache_memory->cache_2nd_way[index];
	cache_memory->accesses++;
 
	if (block_1st_way->V == 1) {
		if (tag == block_1st_way->tag) {
			// hit en esta via
			printf("HIT 1!\n");
			block_1st_way->LRU_index = 1;
			block_2nd_way->LRU_index = 0;
			return get_data(block_1st_way, word_offset, block_offset);
		} 
		// miss en esta via, puede estar en la otra
		// sino esta en la otra cargar el bloque a cache, actualizar bit V
	}
 
	if (block_1st_way->V == 1) {
		if (tag == block_2nd_way->tag) {
			// hit
			printf("HIT 2!\n");
            block_1st_way->LRU_index = 0;
            block_2nd_way->LRU_index = 1;
			return get_data(block_1st_way, word_offset, block_offset);
		} 
		// miss en esta via, tampoco esta en la otra entonces
		// hay que cargar el bloque a cache, actualizar bit V
	}
	// Si la ejecucion llego aca significa que hubo un miss, cargar bloque, 
	// actualizar tag y bit V
	cache_block_t* block_to_replace;
	if (block_1st_way->LRU_index == 1) {
		block_to_replace = block_2nd_way;
        block_1st_way->LRU_index = 0;
        block_to_replace->LRU_index = 1;
	} else {
		block_to_replace = block_1st_way;
        block_2nd_way->LRU_index = 0;
        block_to_replace->LRU_index = 1;
	}
	int j = 0;
	int init_address = (address / BLOCK_SIZE) * BLOCK_SIZE; //Inicio de direccion a copiar
    printf("Cargo bloque en MC desde %d a %d\n",init_address,init_address + WORDS_BY_BLOCK);
	for (int i = init_address ; i < (init_address + WORDS_BY_BLOCK) ; ++i) {
		block_to_replace->data[j] = cache_memory->memory[i];
		j++;
	}
	block_to_replace->tag = tag;
	block_to_replace->V = 1;

	return -1;
}


int write_byte(cache_t* cache_memory, int address, unsigned char value) {
	int tag = address & TAG_MASK;
	// shift right tag 9 bits
	tag = tag >> 9;
	
	int block_offset = address & BLOCK_OFFSET_MASK;
	
	int index = address & INDEX_MASK;
	// shift right index 5 bits
	index = index >> 5;
	
	int word_offset = address & WORD_OFFSET_MASK;
	// shift right word_offset 2 bits
	word_offset = word_offset >> 2;
	
	cache_block_t* block_1st_way = cache_memory->cache_1st_way[index];
	cache_block_t* block_2nd_way = cache_memory->cache_2nd_way[index];
	int is_a_cache_miss;
	
	if (block_1st_way->V == 1) {
		// Verifico si hay un hit en cache
		if (tag == block_1st_way->tag) {
			//Se dio el hit y como la politica es WT hay que escribir en MC y MP
            printf("Escribo en MC y MP\n");
            
            //Escribimos en MP
			cache_memory->memory[address] = value;
			//Escribimos en MC
			block_1st_way->data[4 * word_offset + block_offset] = value;
			is_a_cache_miss = 0;
		} else {
			// Se dio MISS y como la politica es ~WA escribimos solo en MP
            printf("Escribo solo en MP\n");
			cache_memory->memory[address] = value;
			cache_memory->misses++;
			is_a_cache_miss = -1;
		}
	} else if (block_2nd_way->V == 1) {
        // Verifico si hay un hit en cache
		if (tag == block_2nd_way->tag) {
            //Se dio el hit y como la politica es WT hay que escribir en MC y MP
            printf("Escribo en MC y MP\n");
            //Escribimos en MP
			cache_memory->memory[address] = value;
            //Escribimos en MC
			block_1st_way->data[4 * word_offset + block_offset] = value;
			is_a_cache_miss = 0;
		} else {
            // Se dio MISS y como la politica es ~WA escribimos solo en MP
            printf("Escribo solo en MP\n");
			cache_memory->memory[address] = value;
			cache_memory->misses++;
			is_a_cache_miss = -1;
		}
	} else {
		// el dato no fue almacenado
        
        //Escribimos en MP
        printf("Escribo solo en MP\n");
		cache_memory->memory[address] = value;
		cache_memory->misses++;
		is_a_cache_miss = -1;
	}
	return is_a_cache_miss;
}


unsigned int get_miss_rate(cache_t* cache_memory) {
	return (unsigned int)((float)((float)cache_memory->misses / (float)cache_memory->accesses) * 100);
}


void destroy_cache(cache_t* cache_memory) {
	for (int i = 0; i < BLOCKS_PER_WAY; ++i) {
        free(cache_memory->cache_1st_way[i]->data);
        free(cache_memory->cache_2nd_way[i]->data);
        free(cache_memory->cache_1st_way[i]);
        free(cache_memory->cache_2nd_way[i]);
    }
	free(cache_memory->cache_1st_way);
	free(cache_memory->cache_2nd_way);
	free(cache_memory->memory);
}
