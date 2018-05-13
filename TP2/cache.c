#include "cache.h"
#define BLOCK_SIZE 32 // in bytes
#define CACHE_WAY_SIZE 512 // in bytes


void init() {
	this->cache_1st_way = (int*) malloc(CACHE_WAY_SIZE);
	this->cache_2nd_way = (int*) malloc(CACHE_WAY_SIZE);
	this->miss_rate = 0;
}


unsigned char read_byte(int address);


int write_byte(int address, unsigned char value);


unsigned int get_miss_rate() {
	return this->miss_rate;
}

void destroy_cache() {
	free(this->cache_1st_way);
	free(this->cache_2nd_way);
}
