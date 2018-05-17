				//Mariano Hielpos
				//91244
				//lista.c

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "lista.h"

/**********************************************************
		Definición de los tipos de datos
**********************************************************/

typedef struct nodo nodo_t;

struct nodo
{
	void* dato;
	nodo_t* siguiente;
};


struct lista
{
	nodo_t* primero;
	nodo_t* ultimo;
	size_t largo;
};

struct iter
{
	nodo_t* anterior;
	nodo_t* actual;
};

/**********************************************************
 * 			Funciones auxiliares
 * ********************************************************/
 
nodo_t *crear_nodo(void)
{
	nodo_t *nodo = malloc(sizeof(nodo_t));
	return nodo;
}

/**********************************************************
		Primitivas de la lista
**********************************************************/

//Crea una lista.
//Post: Devuelve un puntero a una lista vacía.
lista_t *lista_crear()
{

	lista_t *lista = malloc(sizeof(lista_t));
	
	if( lista != NULL )
	{
		lista->primero = NULL;
		lista->ultimo = NULL;
		lista->largo = 0;
	}
	
	return lista;
	
	
}

//Verifica si la lista está vacía.
//Pre: La lista fue creada.
//Post: Devuelve un booleano dependiendo si está vacía o no.
bool lista_esta_vacia(const lista_t *lista)
{
	return lista->primero == NULL;
}

//Inserta un dato genérico al principio de la lista.
//Pre: La lista fué creada.
//Post: Devuelve un booleano si pudo o no insertar el dato.
bool lista_insertar_primero(lista_t *lista, void *dato)
{
	nodo_t *nodo = crear_nodo();
	if( nodo == NULL ) return false;
	
	nodo->dato = dato;
	nodo->siguiente = lista->primero;
	
	if( lista_esta_vacia(lista) )
	{
		lista->ultimo = nodo;
	}
	
	lista->primero = nodo;
	
	lista->largo++;
	
	
	return true;
}
	

//Inserta un dato genérico al final de la lista.
//Pre: La lista fué creada.
//Post: Devuelve un booleano si pudo o no insertar el dato.
bool lista_insertar_ultimo(lista_t *lista, void *dato)
{
	if( lista_esta_vacia(lista) )
	{
		return lista_insertar_primero(lista, dato);
	}
	
	nodo_t *nodo = crear_nodo();
	if( nodo == NULL ) return false;
	
	nodo->dato = dato;
	nodo->siguiente = NULL;
	
	(lista->ultimo)->siguiente = nodo;
	lista->ultimo = nodo;
	lista->largo++;
	
	return true;
}

//Borra el primer dato de la lista.
//Pre: La lista fué creada.
//Post: Devuelve un puntero al dato o NULL si está vacía.
void *lista_borrar_primero(lista_t *lista)
{
	if( lista_esta_vacia(lista) )
	{
		return NULL;
	}
	
	nodo_t *auxiliar = lista->primero;
	void *dato = auxiliar->dato;
	
	lista->primero = (lista->primero)->siguiente;
	lista->largo--;
	
	free(auxiliar);
	
	return dato;
}

//Devuelve un puntero al primer dato de la lista
//Pre: La lista fue creada.
//Post: Devuelve un puntero al primer dato, o NULL si está vacía.
void *lista_ver_primero(const lista_t *lista)
{
	return (lista_esta_vacia(lista)? NULL:lista->primero->dato);
}

//Devuelve el tamaño de la lista.
//Pre: La lista fué creada
//Post: Devuelve el largo de la lista.
size_t lista_largo(const lista_t *lista)
{
	return lista->largo;
}

//Destruye solo la lista. Si destruir_dato es NULL, no destruye
//los datos almacenados.
//Pre: La lista fué creada.
//Post: Destruye la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *))
{	
	while( !lista_esta_vacia(lista) )
	{
		if( destruir_dato != NULL )
		{
			destruir_dato( lista_ver_primero(lista) );
		}
		lista_borrar_primero(lista);
	}
	free(lista);
}
	
	

/**********************************************************
		Primitivas de la iteración
**********************************************************/

//Crea un iterador.
//Pre: Se creó una lista.
//Post: devuelve un puntero a un iterador vacío.
//NULL si hubo algún problema.
lista_iter_t *lista_iter_crear(const lista_t *lista)
{
	lista_iter_t *iter = malloc(sizeof(lista_iter_t));
	if( iter == NULL )
	{
		return iter;
	}
	
	iter->actual = lista->primero;
	iter->anterior = NULL;
		
	return iter;
}
	

//Avanza una posició en la lista.
//Pre: La lista se creó.
//Post: Avanza una posición y devuelve true. Si se llegó al final devuelve false.
bool lista_iter_avanzar(lista_iter_t *iter)
{
	if( lista_iter_al_final(iter) )
	{
		return false;
	}
	
	iter->anterior = iter->actual;
	iter->actual = iter->anterior->siguiente;
	
	return true;
}

//Muestra el item en donde está parado el iterador.
//Pre: Se creó el iterador.
//Post: Devuelve un puntero al elemento. Si la lista
//está vacía devuelve NULL.
void *lista_iter_ver_actual(const lista_iter_t *iter)
{
	return lista_iter_al_final(iter)? NULL:iter->actual->dato;
}

//Ve si el iterador está al final.
//Pre: El iterador fué creado.
//Post: Devuelve true si está y false si no.
bool lista_iter_al_final(const lista_iter_t *iter)
{
	return iter->actual == NULL;
}

//Destruye el iterador.
//Pre: El iterador fué creado.
//Post: Se destruyó el iterador.
void lista_iter_destruir(lista_iter_t *iter)
{
	free(iter);
}

/**********************************************************
	Primitivas de listas junto con iterador
**********************************************************/

//Inserta un dato genérico en la posición en la que está el iterador.
//Pre: La lista y el iterador fueron creados.
//Post: Devuelve true o false si pudo o no agregar el dato.
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato)
{
	if( iter->anterior == NULL )
	{
		if( ! lista_insertar_primero(lista, dato) ) return false;
		
		iter->actual = lista->primero;
		return true;
	}
	
	nodo_t *nodo = crear_nodo();
	if( nodo == NULL ) return false;
	
	nodo->dato = dato;
	nodo->siguiente = iter->actual;
	
	if( lista_iter_al_final(iter) )
	{
		lista->ultimo = nodo;
	}

	iter->actual = nodo;
	(iter->anterior)->siguiente = nodo;
	lista->largo++;
	
	return true;
}

//Borra el dato en donde está el iterador.
//Pre: La lista y el iterador fueron creados.
//Post: Borra el dato y devuelve el puntero. Si está al final devuelve NULL.
void *lista_borrar(lista_t *lista, lista_iter_t *iter)
{
	if( lista_iter_al_final(iter) )
	{
		return NULL;
	}
	
	void* dato_devolver = iter->actual->dato;
	nodo_t *nodo = iter->actual;
	iter->actual = iter->actual->siguiente;
	
	if( iter->anterior != NULL )
	{
		iter->anterior->siguiente = iter->actual;
	}
	else
	{
		lista->primero = iter->actual;
	}
	
	if( lista_iter_al_final(iter) )
	{
		lista->ultimo = iter->anterior;
	}
	
	free(nodo);
	lista->largo--;
	
	return dato_devolver;
}

/**********************************************************
	Primitiva del iterador interno
**********************************************************/

//Itera todo el conjunto según la directiva de la función visitar.
//Pre: La lista fué creada, recibe una función para saber hasta cuando iterar,
//con dos parámetros, el extra puede ser obviado.
//Post: Itera y deja la lista como estaba.
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra)
{
	lista_iter_t *iter = lista_iter_crear(lista);
	void *dato;
	
	do
	{
		dato = lista_iter_ver_actual(iter);
		lista_iter_avanzar(iter);
	} while( dato != NULL && visitar(dato, extra) );
	
	lista_iter_destruir(iter);
}
