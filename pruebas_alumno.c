#include "pa2m.h"
#include "src/abb.h"
#include <stdbool.h>
#include <stdlib.h>

int comparador(void *elemento, void *contexto)
{
	int numero = *(int *)elemento;
	int comparador = *(int *)contexto;

	if (numero > comparador) {
		return 1;
	}

	if (numero < comparador) {
		return -1;
	}

	return 0;
}

void funcion_destructora(void *elemento)
{
	int *numero = (int *)elemento;

	if (*numero % 2 == 0) {
		(*numero)++;
	}
}

bool funcion_iteradora(void *elemento, void *contexto)
{
	if (contexto == NULL) {
		return true;
	}
	int *numero = (int *)elemento;
	int *comparador = (int *)contexto;

	if (*numero == *comparador) {
		return false;
	}

	return true;
}

abb_t *crear_abb_cargado(int *vector)
{
	abb_t *arbol = abb_crear(comparador);
	for (int i = 0; i < 9; i++) {
		abb_insertar(arbol, &vector[i]);
	}

	return arbol;
}

void pruebas_crear_abb()
{
	pa2m_afirmar(
		abb_crear(NULL) == NULL,
		"Se prueba crear un ABB con un comparador nulo, devuelve NULL.");
	abb_t *arbol = abb_crear(comparador);
	pa2m_afirmar(
		arbol != NULL,
		"Se prueba crear un ABB con un comparador válido, devuelve un puntero al ABB creado.");
	pa2m_afirmar(abb_tamanio(arbol) == 0,
		     "Se prueba que la cantidad de elementos sea 0.");
	pa2m_afirmar(abb_vacio(arbol) == true,
		     "Se prueba que el ABB esté vacio.");

	abb_destruir(arbol);
}

void pruebas_insertar_abb()
{
	int numeros[] = { 13, 7, 16, 5, 3, 8, 14, 18, 20 };

	abb_t *arbol = abb_crear(comparador);
	pa2m_afirmar(abb_insertar(NULL, NULL) == NULL,
		     "Se prueba insertar en un ABB nulo, devuelve NULL.");
	pa2m_afirmar(
		abb_insertar(arbol, NULL) == arbol,
		"Se prueba insertar un NULL en un ABB válido, devuelve el ABB.");
	abb_destruir(arbol);
	arbol = abb_crear(comparador);
	pa2m_afirmar(
		abb_insertar(arbol, &numeros[0]) == arbol,
		"Se prueba insertar un número en un ABB vacío, devuelve el ABB.");
	pa2m_afirmar(abb_tamanio(arbol) == 1,
		     "Se verifica que el ABB tiene 1 elemento.");
	for (int i = 1; i < 9; i++) {
		pa2m_afirmar(
			abb_insertar(arbol, &numeros[i]) == arbol &&
				abb_tamanio(arbol) == i + 1,
			"Se prueba insertar otro elemento, la cantidad es la correcta.");
	}
	pa2m_afirmar(
		abb_tamanio(arbol) == 9,
		"Se insertan 9 elementos en el ABB, se verifica que contenga 9.");
	pa2m_afirmar(
		abb_insertar(arbol, &numeros[0]) == arbol,
		"Se prueba insertar un elemento repetido, devuelve el ABB.");
	int *array[20] = { 0 };
	abb_recorrer(arbol, INORDEN, (void **)array, 20);
	pa2m_afirmar(
		array[4] == &numeros[0],
		"El elemento repetido se encuentra en la posición correcta.");
	pa2m_afirmar(abb_tamanio(arbol) == 10,
		     "La cantida de elementos del ABB es 10.");

	abb_destruir(arbol);
}

void vaciar_array(int **array, int ml)
{
	for (int i = 0; i < ml; i++)
		array[i] = NULL;
}

void pruebas_quitar_abb()
{
	int numeros[] = { 13, 7, 16, 5, 3, 8, 14, 18, 20 };

	int *array[20] = { 0 };
	abb_t *arbol = crear_abb_cargado(numeros);
	abb_recorrer(arbol, INORDEN, (void **)array, 20);

	pa2m_afirmar(
		abb_quitar(NULL, &numeros[2]) == NULL,
		"Se prueba quitar un elemento de un ABB nulo, devuelve NULL");
	pa2m_afirmar(
		abb_quitar(arbol, &arbol) == NULL,
		"Se prueba quitar un elemento que no se encuentra en el ABB, se devuelve NULL.");
	pa2m_afirmar(
		abb_quitar(arbol, &numeros[8]) == &numeros[8],
		"Se prueba quitar un nodo hoja, se devuelve el elemento correcto.");
	vaciar_array(array, 20);
	abb_recorrer(arbol, INORDEN, (void **)array, 20);

	pa2m_afirmar(abb_buscar(arbol, &numeros[8]) == NULL,
		     "El nodo eliminado no existe más en el ABB.");
	pa2m_afirmar(array[8] == NULL,
		     "El nodo se elimino de la posicion correcta.");
	pa2m_afirmar(abb_tamanio(arbol) == 8,
		     "La cantidad de elementos del ABB ahora es 8.");
	pa2m_afirmar(
		abb_quitar(arbol, &numeros[3]) == &numeros[3],
		"Se prueba quitar un nodo con un hijo, devuelve el nodo eliminado");
	vaciar_array(array, 20);
	abb_recorrer(arbol, INORDEN, (void **)array, 20);
	pa2m_afirmar(abb_buscar(arbol, &numeros[3]) == NULL,
		     "El nodo eliminado no existe más en el ABB.");
	pa2m_afirmar(array[1] == &numeros[1],
		     "El nodo que se encuentra en su lugar es el hijo.");
	pa2m_afirmar(abb_tamanio(arbol) == 7,
		     "La cantidad de elementos en el ABB es 7.");
	pa2m_afirmar(
		abb_quitar(arbol, &numeros[2]) == &numeros[2],
		"Se prueba quitar un nodo con dos hijos, devuelve el nodo eliminado.");
	vaciar_array(array, 20);
	abb_recorrer(arbol, INORDEN, (void **)array, 20);
	pa2m_afirmar(abb_buscar(arbol, &numeros[2]) == NULL,
		     "El nodo eliminado no existe más en el ABB.");
	pa2m_afirmar(array[6] != &numeros[2],
		     "El nodo se elimino de la posicion correcta.");
	pa2m_afirmar(
		array[4] == &numeros[6],
		"El elemento que queda en su lugar es el predecesor inorden.");
	pa2m_afirmar(abb_tamanio(arbol) == 6,
		     "La cantidad de elementos en el ABB es 6.");

	pa2m_afirmar(
		abb_quitar(arbol, &numeros[0]) == &numeros[0],
		"Se prueba quitar el nodo raiz con dos hijos, se devuelve el elemento correcto");
	vaciar_array(array, 20);
	abb_recorrer(arbol, INORDEN, (void **)array, 20);
	pa2m_afirmar(abb_buscar(arbol, &numeros[0]) == NULL,
		     "El elemento eliminado no existe más en el ABB.");
	pa2m_afirmar(
		array[2] == &numeros[5],
		"El elemento que queda en su posición es el predecesor inorden.");
	pa2m_afirmar(abb_tamanio(arbol) == 5,
		     "La cantidad de elementos en el ABB es 5.");
	abb_quitar(arbol, &numeros[4]);
	abb_quitar(arbol, &numeros[7]);
	pa2m_afirmar(
		abb_quitar(arbol, &numeros[5]) == &numeros[5],
		"Se prueba quitar el nodo raiz con dos hijos, devuelve el elemento correcto.");
	vaciar_array(array, 20);
	abb_recorrer(arbol, INORDEN, (void **)array, 20);
	pa2m_afirmar(abb_buscar(arbol, &numeros[5]) == NULL,
		     "El elemento eliminado no existe más en el ABB.");
	pa2m_afirmar(
		array[0] == &numeros[1],
		"El elemento que queda en su posición es el predecesor inorden.");
	pa2m_afirmar(abb_tamanio(arbol) == 2,
		     "La cantidad de elementos en el ABB es 2.");
	pa2m_afirmar(
		abb_quitar(arbol, &numeros[1]) == &numeros[1],
		"Se prueba quitar la raiz con un solo hijo, se devuelve el elemento correcto.");
	vaciar_array(array, 20);
	abb_recorrer(arbol, INORDEN, (void **)array, 20);
	pa2m_afirmar(abb_buscar(arbol, &numeros[1]) == NULL,
		     "El elemento eliminado no existe más en el ABB.");
	pa2m_afirmar(
		array[0] == &numeros[6],
		"El elemento que se encuentra en su posición es su único hijo.");
	pa2m_afirmar(abb_tamanio(arbol) == 1,
		     "La cantidad de elementos en el ABB es 1.");
	pa2m_afirmar(
		abb_quitar(arbol, &numeros[6]) == &numeros[6],
		"Se prueba quitar el único elemento que contiene el ABB, devuelve el elemento correcto.");
	pa2m_afirmar(abb_tamanio(arbol) == 0 && abb_vacio(arbol),
		     "El ABB está vacío.");
	pa2m_afirmar(
		abb_quitar(arbol, &numeros[2]) == NULL,
		"Se prueba quitar un elemento de un ABB vacío, resulta NULL.");

	abb_destruir(arbol);
}

void pruebas_busqueda_abb()
{
	int numeros[] = { 13, 7, 16, 5, 3, 8, 14, 18, 20 };

	abb_t *arbol = crear_abb_cargado(numeros);
	pa2m_afirmar(
		abb_buscar(NULL, &numeros[2]) == NULL,
		"Se prueba buscar un elemento en un abb nulo, resulta NULL.");

	for (int i = 0; i < 9; i++)
		pa2m_afirmar(
			abb_buscar(arbol, &numeros[i]) == &numeros[i],
			"Se prueba buscar un elemento existente en el ABB, devuelve el elemento buscado.");
	pa2m_afirmar(
		abb_buscar(arbol, &arbol) == NULL,
		"Se prueba buscar un elemento inexistente dentro del ABB, devuelve NULL");
	abb_destruir(arbol);

	arbol = abb_crear(comparador);
	abb_insertar(arbol, &numeros[0]);
	pa2m_afirmar(
		abb_buscar(arbol, &numeros[0]) == &numeros[0],
		"Se prueba buscar un elemento en un ABB con un solo nodo, devuelve el elemento buscado");
	abb_destruir(arbol);
}

void pruebas_tam_abb()
{
	abb_t *arbol = abb_crear(comparador);
	int numeros[] = { 13, 7, 16, 5, 3, 8, 14, 18, 20 };
	pa2m_afirmar(abb_vacio(arbol), "El ABB está vacío.");
	pa2m_afirmar(abb_tamanio(arbol) == 0,
		     "Se verifica que un ABB vacío contenga 0 elementos.");
	abb_destruir(arbol);
	arbol = crear_abb_cargado(numeros);
	pa2m_afirmar(
		abb_tamanio(arbol) == 9,
		"Se cargan elementos a un ABB y se verifica que contenga 9 elementos.");
	pa2m_afirmar(!abb_vacio(arbol), "El ABB no está vacío.");
	abb_destruir(arbol);
}

void pruebas_destruir_todo()
{
	int numeros[] = { 13, 7, 16, 5, 3, 8, 14, 18, 20 };
	abb_t *arbol = crear_abb_cargado(numeros);
	abb_destruir_todo(arbol, funcion_destructora);

	bool destruccion_correcta = false;

	if (numeros[0] == 13 && numeros[1] == 7 && numeros[2] == 17 &&
	    numeros[3] == 5 && numeros[4] == 3 && numeros[5] == 9 &&
	    numeros[6] == 15 && numeros[7] == 19 && numeros[8] == 21) {
		destruccion_correcta = true;
	}
	pa2m_afirmar(
		destruccion_correcta == true,
		"Se prueba que la función destructora se aplique correctamente.");
	pa2m_afirmar(
		destruccion_correcta == true,
		"La destrucción no deja memoria perdida en ningun bloque (checkear tambien con valgrind).");
}

void pruebas_iterador()
{
	int numeros[] = { 13, 7, 16, 5, 3, 8, 14, 18, 20 };
	abb_t *arbol = crear_abb_cargado(numeros);
	pa2m_afirmar(
		abb_con_cada_elemento(NULL, INORDEN, funcion_iteradora,
				      &numeros[7]) == 0,
		"Se prueba iterar un ABB nulo de manera INORDEN, devuelve 0.");
	pa2m_afirmar(
		abb_con_cada_elemento(NULL, PREORDEN, funcion_iteradora,
				      &numeros[7]) == 0,
		"Se prueba iterar un ABB nulo de manera PREORDEN, devuelve 0.");
	pa2m_afirmar(
		abb_con_cada_elemento(NULL, POSTORDEN, funcion_iteradora,
				      &numeros[7]) == 0,
		"Se prueba iterar un ABB nulo de manera POSTORDEN, devuelve 0.");
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, INORDEN, NULL, &numeros[7]) == 0,
		"Se prueba iterar un ABB válido de manera INORDEN con una función nula, devuelve 0.");
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, PREORDEN, NULL, &numeros[7]) == 0,
		"Se prueba iterar un ABB válido de manera PREORDEN con una función nula, devuelve 0.");
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, POSTORDEN, NULL, &numeros[7]) == 0,
		"Se prueba iterar un ABB válido de manera POSTORDEN con una función nula, devuelve 0.");
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, INORDEN, funcion_iteradora,
				      &numeros[7]) == 8,
		"Se prueba iterar en un ABB válido de manera INORDEN, devuelve la cantidad correcta.");
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, PREORDEN, funcion_iteradora,
				      &numeros[7]) == 8,
		"Se prueba iterar en un ABB válido de manera PREORDEN, devuelve la cantidad correcta.");
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, POSTORDEN, funcion_iteradora,
				      &numeros[7]) == 7,
		"Se prueba iterar en un ABB válido de manera POSTORDEN, devuelve la cantidad correcta.");
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, INORDEN, funcion_iteradora,
				      NULL) == abb_tamanio(arbol),
		"Se prueba iterar un ABB de manera INORDEN en su totalidad con un aux nulo y se devuelve la cantidad de elementos.");
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, PREORDEN, funcion_iteradora,
				      NULL) == abb_tamanio(arbol),
		"Se prueba iterar un ABB de manera PREORDEN en su totalidad con un aux nulo y se devuelve la cantidad de elementos.");
	pa2m_afirmar(
		abb_con_cada_elemento(arbol, POSTORDEN, funcion_iteradora,
				      NULL) == abb_tamanio(arbol),
		"Se prueba iterar un ABB de manera POSTORDEN en su totalidad con un aux nulo y se devuelve la cantidad de elementos.");
	abb_destruir(arbol);
}

void pruebas_recorrido_inorden()
{
	int numeros[] = { 13, 7, 16, 5, 3, 8, 14, 18, 20 };
	int *array_grande[20] = { 0 };
	int *array_chico[6] = { 0 };

	abb_t *arbol = crear_abb_cargado(numeros);
	pa2m_afirmar(
		abb_recorrer(NULL, INORDEN, (void **)array_grande, 20) == 0,
		"Se prueba recorrer un ABB nulo de manera INORDEN, se devuelve 0");
	pa2m_afirmar(abb_recorrer(arbol, INORDEN, (void **)array_grande, 20) ==
			     abb_tamanio(arbol),
		     "Se recorre el ABB y se almacenan todos los elementos.");
	bool bien_ordenado = false;

	if (array_grande[0] == &numeros[4] && array_grande[1] == &numeros[3] &&
	    array_grande[2] == &numeros[1] && array_grande[3] == &numeros[5] &&
	    array_grande[4] == &numeros[0] && array_grande[5] == &numeros[6] &&
	    array_grande[6] == &numeros[2] && array_grande[7] == &numeros[7] &&
	    array_grande[8] == &numeros[8]) {
		bien_ordenado = true;
	}

	pa2m_afirmar(bien_ordenado == true,
		     "Los elementos en el vector tienen el orden esperado.");
	pa2m_afirmar(
		abb_recorrer(arbol, INORDEN, (void **)array_chico, 5) == 5,
		"Se recorre el ABB y se almacenan menos elementos que los existentes, devuelve la cantidad correcta.");

	bien_ordenado = false;
	if (array_chico[0] == &numeros[4] && array_chico[1] == &numeros[3] &&
	    array_chico[2] == &numeros[1] && array_chico[3] == &numeros[5] &&
	    array_chico[4] == &numeros[0] && array_chico[5] == NULL) {
		bien_ordenado = true;
	}
	pa2m_afirmar(bien_ordenado == true,
		     "Los elementos en el vector tienen el orden esperado.");

	vaciar_array(array_grande, 20);
	pa2m_afirmar(
		abb_recorrer(arbol, INORDEN, (void **)array_grande, 13) ==
			abb_tamanio(arbol),
		"Se pide recorrer más elementos de los existentes, se devuelve la cantiadad de elementos total del ABB.");
	bien_ordenado = false;
	if (array_grande[0] == &numeros[4] && array_grande[1] == &numeros[3] &&
	    array_grande[2] == &numeros[1] && array_grande[3] == &numeros[5] &&
	    array_grande[4] == &numeros[0] && array_grande[5] == &numeros[6] &&
	    array_grande[6] == &numeros[2] && array_grande[7] == &numeros[7] &&
	    array_grande[8] == &numeros[8] && array_grande[9] == NULL) {
		bien_ordenado = true;
	}
	pa2m_afirmar(bien_ordenado == true,
		     "Los elementos en el vector tienen el orden esperado.");

	abb_destruir(arbol);
}

void pruebas_recorrido_preorden()
{
	int numeros[] = { 13, 7, 16, 5, 3, 8, 14, 18, 20 };
	int *array_grande[20] = { 0 };
	int *array_chico[6] = { 0 };

	abb_t *arbol = crear_abb_cargado(numeros);
	pa2m_afirmar(
		abb_recorrer(NULL, PREORDEN, (void **)array_grande, 20) == 0,
		"Se prueba recorrer un ABB nulo de manera PREORDEN, se devuelve 0");
	pa2m_afirmar(abb_recorrer(arbol, PREORDEN, (void **)array_grande, 20) ==
			     abb_tamanio(arbol),
		     "Se recorre el ABB y se almacenan todos los elementos.");
	bool bien_ordenado = false;

	if (array_grande[0] == &numeros[0] && array_grande[1] == &numeros[1] &&
	    array_grande[2] == &numeros[3] && array_grande[3] == &numeros[4] &&
	    array_grande[4] == &numeros[5] && array_grande[5] == &numeros[2] &&
	    array_grande[6] == &numeros[6] && array_grande[7] == &numeros[7] &&
	    array_grande[8] == &numeros[8]) {
		bien_ordenado = true;
	}

	pa2m_afirmar(bien_ordenado == true,
		     "Los elementos en el vector tienen el orden esperado.");
	pa2m_afirmar(
		abb_recorrer(arbol, PREORDEN, (void **)array_chico, 5) == 5,
		"Se recorre el ABB y se almacenan menos elementos que los existentes, devuelve la cantidad correcta.");

	bien_ordenado = false;
	if (array_chico[0] == &numeros[0] && array_chico[1] == &numeros[1] &&
	    array_chico[2] == &numeros[3] && array_chico[3] == &numeros[4] &&
	    array_chico[4] == &numeros[5] && array_chico[5] == NULL) {
		bien_ordenado = true;
	}
	pa2m_afirmar(bien_ordenado == true,
		     "Los elementos en el vector tienen el orden esperado.");

	vaciar_array(array_grande, 20);
	pa2m_afirmar(
		abb_recorrer(arbol, PREORDEN, (void **)array_grande, 13) ==
			abb_tamanio(arbol),
		"Se pide recorrer más elementos de los existentes, se devuelve la cantiadad de elementos total del ABB.");
	bien_ordenado = false;
	if (array_grande[0] == &numeros[0] && array_grande[1] == &numeros[1] &&
	    array_grande[2] == &numeros[3] && array_grande[3] == &numeros[4] &&
	    array_grande[4] == &numeros[5] && array_grande[5] == &numeros[2] &&
	    array_grande[6] == &numeros[6] && array_grande[7] == &numeros[7] &&
	    array_grande[8] == &numeros[8] && array_grande[9] == NULL) {
		bien_ordenado = true;
	}
	pa2m_afirmar(bien_ordenado == true,
		     "Los elementos en el vector tienen el orden esperado.");

	abb_destruir(arbol);
}

void pruebas_recorrido_postorden()
{
	int numeros[] = { 13, 7, 16, 5, 3, 8, 14, 18, 20 };
	int *array_grande[20] = { 0 };
	int *array_chico[6] = { 0 };

	abb_t *arbol = crear_abb_cargado(numeros);
	pa2m_afirmar(
		abb_recorrer(NULL, POSTORDEN, (void **)array_grande, 20) == 0,
		"Se prueba recorrer un ABB nulo de manera POSTORDEN, se devuelve 0");
	pa2m_afirmar(abb_recorrer(arbol, POSTORDEN, (void **)array_grande,
				  20) == abb_tamanio(arbol),
		     "Se recorre el ABB y se almacenan todos los elementos.");
	bool bien_ordenado = false;

	if (array_grande[0] == &numeros[4] && array_grande[1] == &numeros[3] &&
	    array_grande[2] == &numeros[5] && array_grande[3] == &numeros[1] &&
	    array_grande[4] == &numeros[6] && array_grande[5] == &numeros[8] &&
	    array_grande[6] == &numeros[7] && array_grande[7] == &numeros[2] &&
	    array_grande[8] == &numeros[0]) {
		bien_ordenado = true;
	}

	pa2m_afirmar(bien_ordenado == true,
		     "Los elementos en el vector tienen el orden esperado.");
	pa2m_afirmar(
		abb_recorrer(arbol, PREORDEN, (void **)array_chico, 5) == 5,
		"Se recorre el ABB y se almacenan menos elementos que los existentes, devuelve la cantidad correcta.");

	bien_ordenado = false;
	if (array_chico[0] == &numeros[0] && array_chico[1] == &numeros[1] &&
	    array_chico[2] == &numeros[3] && array_chico[3] == &numeros[4] &&
	    array_chico[4] == &numeros[5] && array_chico[5] == NULL) {
		bien_ordenado = true;
	}
	pa2m_afirmar(bien_ordenado == true,
		     "Los elementos en el vector tienen el orden esperado.");

	vaciar_array(array_grande, 20);
	pa2m_afirmar(
		abb_recorrer(arbol, PREORDEN, (void **)array_grande, 13) ==
			abb_tamanio(arbol),
		"Se pide recorrer más elementos de los existentes, se devuelve la cantiadad de elementos total del ABB.");
	bien_ordenado = false;
	if (array_grande[0] == &numeros[0] && array_grande[1] == &numeros[1] &&
	    array_grande[2] == &numeros[3] && array_grande[3] == &numeros[4] &&
	    array_grande[4] == &numeros[5] && array_grande[5] == &numeros[2] &&
	    array_grande[6] == &numeros[6] && array_grande[7] == &numeros[7] &&
	    array_grande[8] == &numeros[8] && array_grande[9] == NULL) {
		bien_ordenado = true;
	}
	pa2m_afirmar(bien_ordenado == true,
		     "Los elementos en el vector tienen el orden esperado.");

	abb_destruir(arbol);
}

int main()
{
	pa2m_nuevo_grupo(
		"\n======================== PRUEBAS TDA ABB - PADRON 110703 ========================");

	pa2m_nuevo_grupo("Pruebas de crear ABB");
	pruebas_crear_abb();
	pa2m_nuevo_grupo("Pruebas de insertar");
	pruebas_insertar_abb();
	pa2m_nuevo_grupo("Pruebas de quitar");
	pruebas_quitar_abb();
	pa2m_nuevo_grupo("Pruebas de búsqueda");
	pruebas_busqueda_abb();
	pa2m_nuevo_grupo("Pruebas de tamaño");
	pruebas_tam_abb();
	pa2m_nuevo_grupo("Pruebas de destructor");
	pruebas_destruir_todo();
	pa2m_nuevo_grupo("Pruebas de iterador");
	pruebas_iterador();
	pa2m_nuevo_grupo("Pruebas de recorrido");
	pa2m_nuevo_grupo("Recorrido Inorden");
	pruebas_recorrido_inorden();
	pa2m_nuevo_grupo("Recorrido Preorden");
	pruebas_recorrido_preorden();
	pa2m_nuevo_grupo("Recorrido Postorden");
	pruebas_recorrido_postorden();

	return pa2m_mostrar_reporte();
}
