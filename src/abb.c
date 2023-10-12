#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>

void iterar_preorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *),
		     void *aux, size_t *contador, bool *seguir_iterando)
{
	if (nodo == NULL || !(*seguir_iterando)) {
		return;
	}

	if ((*seguir_iterando)) {
		(*seguir_iterando) = funcion(nodo->elemento, aux);
		(*contador)++;
	}

	iterar_preorden(nodo->izquierda, funcion, aux, contador,
			seguir_iterando);
	iterar_preorden(nodo->derecha, funcion, aux, contador, seguir_iterando);
}

void iterar_inorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *),
		    void *aux, size_t *contador, bool *seguir_iterando)
{
	if (nodo == NULL || !(*seguir_iterando)) {
		return;
	}

	iterar_inorden(nodo->izquierda, funcion, aux, contador,
		       seguir_iterando);

	if ((*seguir_iterando)) {
		(*seguir_iterando) = funcion(nodo->elemento, aux);
		(*contador)++;
	}

	iterar_inorden(nodo->derecha, funcion, aux, contador, seguir_iterando);
}

void iterar_postorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *),
		      void *aux, size_t *contador, bool *seguir_iterando)
{
	if (nodo == NULL || !(*seguir_iterando)) {
		return;
	}

	iterar_postorden(nodo->izquierda, funcion, aux, contador,
			 seguir_iterando);
	iterar_postorden(nodo->derecha, funcion, aux, contador,
			 seguir_iterando);

	if ((*seguir_iterando)) {
		(*seguir_iterando) = funcion(nodo->elemento, aux);
		(*contador)++;
	}
}

void recorrer_preorden(nodo_abb_t *nodo, void **array, size_t tamanio_vector,
		       size_t *cantidad_almacenada)
{
	if (nodo == NULL || *cantidad_almacenada == tamanio_vector) {
		return;
	}

	array[*cantidad_almacenada] = nodo->elemento;
	(*cantidad_almacenada)++;

	recorrer_preorden(nodo->izquierda, array, tamanio_vector,
			  cantidad_almacenada);
	recorrer_preorden(nodo->derecha, array, tamanio_vector,
			  cantidad_almacenada);
}

void recorrer_inorden(nodo_abb_t *nodo, void **array, size_t tamanio_vector,
		      size_t *cantidad_almacenada)
{
	if (nodo == NULL || *cantidad_almacenada == tamanio_vector) {
		return;
	}

	recorrer_inorden(nodo->izquierda, array, tamanio_vector,
			 cantidad_almacenada);

	array[*cantidad_almacenada] = nodo->elemento;
	(*cantidad_almacenada)++;

	recorrer_inorden(nodo->derecha, array, tamanio_vector,
			 cantidad_almacenada);
}

void recorrer_postorden(nodo_abb_t *nodo, void **array, size_t tamanio_vector,
			size_t *cantidad_almacenada)
{
	if (nodo == NULL) {
		return;
	}

	recorrer_postorden(nodo->izquierda, array, tamanio_vector,
			   cantidad_almacenada);
	recorrer_postorden(nodo->derecha, array, tamanio_vector,
			   cantidad_almacenada);
	if (*cantidad_almacenada == tamanio_vector) {
		return;
	}
	array[*cantidad_almacenada] = nodo->elemento;
	(*cantidad_almacenada)++;
}

int cantidad_de_hijos(nodo_abb_t *nodo)
{
	if (nodo->izquierda != NULL && nodo->derecha != NULL) {
		return 2;
	}

	if ((nodo->izquierda != NULL || nodo->derecha != NULL)) {
		return 1;
	}

	return 0;
}

nodo_abb_t *buscar_predecesor_inorder(nodo_abb_t *nodo)
{
	nodo_abb_t *actual = nodo->izquierda;
	nodo_abb_t *padre = nodo;

	while (actual->derecha != NULL) {
		padre = actual;
		actual = actual->derecha;
	}

	if (padre != nodo) {
		padre->derecha = actual->izquierda;
	} else {
		nodo->izquierda = actual->izquierda;
	}

	return actual;
}

void destruirNodos(nodo_abb_t *nodo)
{
	if (nodo == NULL) {
		return;
	}

	destruirNodos(nodo->izquierda);
	destruirNodos(nodo->derecha);
	free(nodo);
}

void abb_destruir_con_funcion(nodo_abb_t *nodo, void (*destructor)(void *))
{
	if (nodo == NULL) {
		return;
	}
	abb_destruir_con_funcion(nodo->izquierda, destructor);
	abb_destruir_con_funcion(nodo->derecha, destructor);
	destructor(nodo->elemento);
	free(nodo);
}

nodo_abb_t *crear_nodo_con_elemento(void *elemento)
{
	nodo_abb_t *nuevo_nodo = calloc(1, sizeof(nodo_abb_t));

	if (nuevo_nodo == NULL) {
		return NULL;
	}

	nuevo_nodo->elemento = elemento;

	return nuevo_nodo;
}

abb_t *buscar_posicion_e_insertar_nodo(abb_t *arbol,
				       nodo_abb_t *nodo_a_insertar)
{
	bool posicion_correcta_encontrada = false;

	nodo_abb_t *nodo_actual = arbol->nodo_raiz;

	while (!posicion_correcta_encontrada) {
		int comparacion = arbol->comparador(nodo_a_insertar->elemento,
						    nodo_actual->elemento);

		if (comparacion <= 0) {
			if (nodo_actual->izquierda == NULL) {
				posicion_correcta_encontrada = true;
				nodo_actual->izquierda = nodo_a_insertar;
			}
			nodo_actual = nodo_actual->izquierda;
		}

		else if (comparacion > 0) {
			if (nodo_actual->derecha == NULL) {
				posicion_correcta_encontrada = true;
				nodo_actual->derecha = nodo_a_insertar;
			}
			nodo_actual = nodo_actual->derecha;
		}
	}
	arbol->tamanio++;
	return arbol;
}

abb_t *abb_crear(abb_comparador comparador)
{
	if (comparador == NULL) {
		return NULL;
	}

	abb_t *arbol = calloc(1, sizeof(abb_t));

	if (arbol == NULL) {
		return NULL;
	}

	arbol->comparador = comparador;

	return arbol;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (arbol == NULL) {
		return NULL;
	}

	nodo_abb_t *nodo_a_insertar = crear_nodo_con_elemento(elemento);

	if (nodo_a_insertar == NULL) {
		return NULL;
	}

	if (abb_vacio(arbol)) {
		arbol->nodo_raiz = nodo_a_insertar;
		arbol->tamanio++;
		return arbol;
	}

	return buscar_posicion_e_insertar_nodo(arbol, nodo_a_insertar);
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (abb_vacio(arbol)) {
		return NULL;
	}

	nodo_abb_t *actual = arbol->nodo_raiz;
	nodo_abb_t *padre = NULL;

	while (actual != NULL) {
		int comparacion = arbol->comparador(elemento, actual->elemento);

		if (comparacion == 0) {
			int cantidad_hijos = cantidad_de_hijos(actual);

			if (cantidad_hijos == 0) {
				if (padre == NULL) {
					arbol->nodo_raiz = NULL;
				} else if (padre->izquierda == actual) {
					padre->izquierda = NULL;
				} else {
					padre->derecha = NULL;
				}
			}

			if (cantidad_hijos == 1) {
				if (padre == NULL) {
					if (actual->izquierda != NULL) {
						arbol->nodo_raiz =
							actual->izquierda;
					} else {
						arbol->nodo_raiz =
							actual->derecha;
					}
				} else if ((actual->izquierda != NULL &&
					    padre->izquierda == actual)) {
					padre->izquierda = actual->izquierda;
				} else if ((actual->izquierda != NULL &&
					    padre->derecha == actual)) {
					padre->derecha = actual->izquierda;
				} else if ((actual->derecha != NULL &&
					    padre->izquierda == actual)) {
					padre->izquierda = actual->derecha;
				} else {
					padre->derecha = actual->derecha;
				}
			}

			if (cantidad_hijos == 2) {
				nodo_abb_t *sucesor =
					buscar_predecesor_inorder(actual);
				if (padre == NULL) {
					arbol->nodo_raiz = sucesor;
				} else if (padre->izquierda == actual) {
					padre->izquierda = sucesor;
				} else {
					padre->derecha = sucesor;
				}
				sucesor->izquierda = actual->izquierda;
				sucesor->derecha = actual->derecha;
			}

			void *elemento_eliminado = actual->elemento;
			free(actual);
			arbol->tamanio--;
			return elemento_eliminado;
		}
		if (comparacion < 0) {
			padre = actual;
			actual = actual->izquierda;
		} else if (comparacion > 0) {
			padre = actual;
			actual = actual->derecha;
		}
	}

	return NULL;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (abb_vacio(arbol)) {
		return NULL;
	}

	nodo_abb_t *nodo_actual = arbol->nodo_raiz;

	while (nodo_actual != NULL) {
		int comparacion =
			arbol->comparador(elemento, nodo_actual->elemento);

		if (comparacion == 0) {
			return nodo_actual->elemento;
		}
		if (comparacion < 0) {
			nodo_actual = nodo_actual->izquierda;
		} else if (comparacion > 0) {
			nodo_actual = nodo_actual->derecha;
		}
	}

	return NULL;
}

bool abb_vacio(abb_t *arbol)
{
	if (arbol == NULL || arbol->nodo_raiz == NULL) {
		return true;
	}
	return false;
}

size_t abb_tamanio(abb_t *arbol)
{
	if (abb_vacio(arbol)) {
		return 0;
	}

	return arbol->tamanio;
}

void abb_destruir(abb_t *arbol)
{
	if (arbol != NULL) {
		destruirNodos(arbol->nodo_raiz);
		free(arbol);
	}
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (arbol == NULL) {
		return;
	}

	if (destructor == NULL) {
		abb_destruir(arbol);
		return;
	}

	abb_destruir_con_funcion(arbol->nodo_raiz, destructor);
	free(arbol);
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (abb_vacio(arbol) || funcion == NULL) {
		return 0;
	}

	bool seguir_iterando = true;
	size_t elementos_iterados = 0;

	switch (recorrido) {
	case PREORDEN:
		iterar_preorden(arbol->nodo_raiz, funcion, aux,
				&elementos_iterados, &seguir_iterando);
		break;
	case INORDEN:
		iterar_inorden(arbol->nodo_raiz, funcion, aux,
			       &elementos_iterados, &seguir_iterando);
		break;
	case POSTORDEN:
		iterar_postorden(arbol->nodo_raiz, funcion, aux,
				 &elementos_iterados, &seguir_iterando);
		break;
	}

	return elementos_iterados;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (abb_vacio(arbol) || array == NULL) {
		return 0;
	}

	size_t elementos_almacenados = 0;

	switch (recorrido) {
	case PREORDEN:
		recorrer_preorden(arbol->nodo_raiz, array, tamanio_array,
				  &elementos_almacenados);
		break;
	case INORDEN:
		recorrer_inorden(arbol->nodo_raiz, array, tamanio_array,
				 &elementos_almacenados);
		break;
	case POSTORDEN:
		recorrer_postorden(arbol->nodo_raiz, array, tamanio_array,
				   &elementos_almacenados);
		break;
	}

	return elementos_almacenados;
}
