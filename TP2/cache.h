#ifndef CACHE_H
#define CACHE_H

// definicion estructura cache 
typedef struct cache {
	int* cache_1st_way;
	int* cache_2nd_way;
	int misses_count;
	unsigned int miss_rate;
}cache_t;

void init();
unsigned char read_byte(int address);
int write_byte(int address, unsigned char value);
unsigned int get_miss_rate();
void destroy_cache();

#endif // CACHE_H
