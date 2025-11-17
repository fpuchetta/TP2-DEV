#ifndef JUEGO_H_
#define JUEGO_H_

#include "tp1.h"
#include "lista.h"
#include "cola.h"
#include "functions.h"

#include "stdlib.h"
#include "stdbool.h"

#define TOTAL_CARTAS 18
#define PAREJAS 9
#define FILAS 3
#define COLUMNAS 6

typedef struct juego juego_t;

/*
    Pre: -
    
    Post: Devuelve un juego creado. Si archivo es NULL, crea juego sin pokedex.
          Devuelve NULL en caso de error.
*/
juego_t *juego_crear(const char *archivo);

/*
    Pre: juego no debe ser NULL.
    
    Post: Prepara el juego con la semilla especificada.
          Devuelve true si se preparó correctamente, false en caso de error.
*/
bool juego_preparar(juego_t *juego, unsigned int semilla);

/*
    Pre: -
    
    Post: Ejecuta el juego completo.
          Devuelve true si terminó correctamente, false en caso de error.
*/
bool juego_jugar(juego_t *juego, unsigned int semilla);

/*
    Pre: juego no debe ser NULL.
    
    Post: Reinicia el juego al estado inicial con la semilla especificada.
          Devuelve true si se reinició correctamente, false en caso de error.
*/
bool juego_reiniciar(juego_t *juego, unsigned int semilla);

/*
    Pre: juego no debe ser NULL.
    
    Post: Devuelve true si el juego ha terminado (todas las cartas descubiertas).
*/
bool juego_terminado(juego_t *juego);

/*
    Pre: juego no debe ser NULL.
    
    Post: Devuelve la pokedex asociada al juego.
          Devuelve NULL si no hay pokedex cargada.
*/
tp1_t *juego_obtener_pokedex(juego_t *juego);

/*
    Pre: juego no debe ser NULL.
    
    Post: Establece una nueva pokedex para el juego.
          Libera la pokedex anterior si existe.
          Devuelve true si se estableció correctamente, false en caso de error.
*/
bool juego_establecer_pokedex(juego_t *juego, tp1_t *nueva_pokedex);

/*
    Pre: juego no debe ser NULL.
    
    Post: Devuelve true si el juego tiene una pokedex cargada.
*/
bool juego_tiene_pokedex(juego_t *juego);

/*
    Pre: juego no debe ser NULL.
    
    Post: Libera toda la memoria asociada al juego.
*/
void juego_destruir(juego_t *juego);

#endif // JUEGO_H_