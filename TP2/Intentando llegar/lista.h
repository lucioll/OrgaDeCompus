#ifndef	LISTA_T
#define LISTA_T

#include <stdbool.h>
#include <stddef.h>

/**********************************************************
		Definición de los tipos de datos
**********************************************************/

typedef struct lista lista_t;

typedef struct iter lista_iter_t;

/**********************************************************
		Primitivas de la lista
**********************************************************/

//Crea una lista.
//Post: Devuelve un puntero a una lista vacía.
lista_t *lista_crear();

//Verifica si la lista está vacía.
//Pre: La lista fue creada.
//Post: Devuelve un booleano dependiendo si está vacía o no.
bool lista_esta_vacia(const lista_t *lista);

//Inserta un dato genérico al principio de la lista.
//Pre: La lista fué creada.
//Post: Devuelve un booleano si pudo o no insertar el dato.
bool lista_insertar_primero(lista_t *lista, void *dato);

//Inserta un dato genérico al final de la lista.
//Pre: La lista fué creada.
//Post: Devuelve un booleano si pudo o no insertar el dato.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

//Borra el primer dato de la lista.
//Pre: La lista fué creada.
//Post: Devuelve un puntero al dato o NULL si está vacía.
void *lista_borrar_primero(lista_t *lista);

//Devuelve un puntero al primer dato de la lista
//Pre: La lista fue creada.
//Post: Devuelve un puntero al primer dato, o NULL si está vacía.
void *lista_ver_primero(const lista_t *lista);

//Devuelve el tamaño de la lista.
//Pre: La lista fué creada
//Post: Devuelve el largo de la lista.
size_t lista_largo(const lista_t *lista);

//Destruye solo la lista. Si destruir_dato es NULL, no destruye
//los datos almacenados.
//Pre: La lista fué creada.
//Post: Destruye la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/**********************************************************
		Primitivas de la iteración
**********************************************************/

//Crea un iterador.
//Pre: Se creó una lista.
//Post: devuelve un puntero a un iterador vacío.
//NULL si hubo algún problema.
lista_iter_t *lista_iter_crear(const lista_t *lista);

//Avanza una posició en la lista.
//Pre: La lista se creó.
//Post: Avanza una posición y devuelve true. Si se llegó al final devuelve false.
bool lista_iter_avanzar(lista_iter_t *iter);

//Muestra el item en donde está parado el iterador.
//Pre: Se creó el iterador.
//Post: Devuelve un puntero al elemento. Si la lista
//está vacía devuelve NULL.
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Ve si el iterador está al final.
//Pre: El iterador fué creado.
//Post: Devuelve true si está y false si no.
bool lista_iter_al_final(const lista_iter_t *iter);

//Destruye el iterador.
//Pre: El iterador fué creado.
//Post: Se destruyó el iterador.
void lista_iter_destruir(lista_iter_t *iter);

/**********************************************************
	Primitivas de listas junto con iterador
**********************************************************/

//Inserta un dato genérico en la posición en la que está el iterador.
//Pre: La lista y el iterador fueron creados.
//Post: Devuelve true o false si pudo o no agregar el dato.
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato);

//Borra el dato en donde está el iterador.
//Pre: La lista y el iterador fueron creados.
//Post: Borra el dato.
void *lista_borrar(lista_t *lista, lista_iter_t *iter);

/**********************************************************
	Primitiva del iterador interno
**********************************************************/

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra);

#endif
