#include <stdlib.h>
#include "cache.h"
#include "lista.h"
#define MEMORY_SIZE 4096 // in bytes
#define BLOCK_SIZE 32 // in bytes
#define BLOCKS_PER_WAY 16
#define CACHE_WAY_SIZE 512 // in bytes
#define VALID_BITS_SIZE 2 // in bytes
#define TAG_MEMORY_SIZE 6 // in bytes

#define TAG_MASK 0xE00
#define SET_NUMBER_MASK 0x1FC
#define INDEX_MASK 0x1E0
#define WORD_OFFSET_MASK 0x1C
#define BLOCK_OFFSET_MASK 0x3

unsigned char* create_memory() {
    unsigned char* v = (unsigned char*) malloc(MEMORY_SIZE);
    return v;
}


cache_block_t* create_cache_block() {
    cache_block_t* cache_block_ = (cache_block_t*) malloc(sizeof(cache_block_t));;
    unsigned char *v = (unsigned char*) malloc(BLOCK_SIZE);
    
    cache_block_->LRU_list = lista_crear();
    cache_block_->data = v;
    cache_block_->V = 0; //Valido es 1 e invalido es 0
    cache_block_->tag = 0;
    return cache_block_;
}

//Memoria principal -variable global
unsigned char *memory;

//Memoria cache -variable global
cache_t *cache_memory;

void init() {
	memory = create_memory();
    cache_memory->misses = 0;
    cache_memory->accesses = 0;
    for (int i = 0; i < BLOCKS_PER_WAY; ++i) {
        cache_memory->cache_1st_way[i] = create_cache_block();
        cache_memory->cache_2nd_way[i] = create_cache_block();
    }
}

unsigned char get_data(cache_block_t* block, int word_offset, int block_offset) {
	return block->data[4 * word_offset + block_offset];
}

unsigned char read_byte(int address) {
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
	unsigned char data = '1';
	cache_memory->accesses++;
	
	if (tag == block_1st_way->tag) {
		if (block_1st_way->V == 1) {
			// hit
			data = get_data(block_1st_way, word_offset, block_offset);
		} else {
			// miss, cargar bloque, actualizar tag y bit V
			unsigned char memory_data = memory[address];
			
			if (block_1st_way->LRU_list->lista_largo == 32) {
				// esta llena. Reemplazar el primero que enliste
				
			} else {
				block_1st_way->data[4 * word_offset + block_offset] = memory_data;
			}
			block_1st_way->V = 1;
			cache_memory->misses++;
		}
	} else if (tag == block_2nd_way->tag) {
		if (block_1st_way->V == 1) {
			// hit
			data = get_data(block_1st_way, word_offset, block_offset);
		} else {
			// miss, cargar bloque, actualizar tag y bit V
			unsigned char memory_data = memory[address];
			
			if (block_1st_way->LRU_list->lista_largo == 32) {
				// esta llena
				
			} else {
				block_1st_way->data[4 * word_offset + block_offset] = memory_data;
			}
			block_1st_way->V = 1;
			cache_memory->misses++;
		}
	} else {
		// miss, cargar bloque, actualizar tag y bit V
		unsigned char memory_data = memory[address];
		if (block_1st_way->LRU_list->lista_largo == 32) {
			// esta llena
				
		} else {
			block_1st_way->data[4 * word_offset + block_offset] = memory_data;
		}
		cache_memory->misses++;
	}
	
	return data;
}


int write_byte(int address, unsigned char value) {
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
		// verifico si el dato ya fue almacenado en cache
		if (tag == block_1st_way->tag) {
			// el dato ya fue almacenado en cache. Lo almaceno en memoria ppal.
			// y actualizo el dato en cache
			memory[address] = value;
			block_1st_way->data[4 * word_offset + block_offset] = value;
			is_a_cache_miss = 0;
		} else {
			// el dato no fue almacenado en cache. Lo almaceno en memoria ppal.
			memory[address] = value;
			is_a_cache_miss = -1;
		}
	} else if (block_2nd_way->V == 1) {
		// verifico si el dato ya fue almacenado en cache
		if (tag == block_2nd_way->tag) {
			// el dato ya fue almacenado en cache. Lo almaceno en memoria ppal.
			// y actualizo el dato en cache
			memory[address] = value;
			block_1st_way->data[4 * word_offset + block_offset] = value;
			is_a_cache_miss = 0;
		} else {
			// el dato no fue almacenado en cache. Lo almaceno en memoria ppal.
			memory[address] = value;
			is_a_cache_miss = -1;
		}
	} else {
		// el dato no fue almacenado
		memory[address] = value;
		is_a_cache_miss = -1;
	}
	
	return is_a_cache_miss;
}


unsigned int get_miss_rate() {
	return ((cache_memory->misses / cache_memory->accesses) * 100);
}


void destroy_cache() {
	for (int i = 0; i < BLOCKS_PER_WAY; ++i) {
		lista_destruir(cache_memory->cache_1st_way[i]->LRU_list, NULL);
		lista_destruir(cache_memory->cache_2nd_way[i]->LRU_list, NULL);
        free(cache_memory->cache_1st_way[i]);
        free(cache_memory->cache_2nd_way[i]);
    }
	
	free(memory);
}
