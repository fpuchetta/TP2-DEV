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

typedef enum {
    JUGADA_ERROR_MEMORIA =-1,
    JUGADA_VALIDA,
    JUGADA_CARTA_INVALIDA,
    JUGADA_CARTA_YA_DESCUBIERTA,
    JUGADA_MISMA_CARTA,
    JUGADA_FORMO_PAR,
    JUGADA_NO_FORMO_PAR
} estado_jugada_t;

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

//////////////////////////////////////////

estado_jugada_t juego_ejecutar_jugada(juego_t *juego, int carta1, int carta2);

estado_jugada_t juego_validar_jugada(juego_t *juego, int carta1, int carta2);

/*
    Pre: juego no debe ser NULL.
    Post: Devuelve el jugador actual (0 o 1).
*/
int juego_obtener_jugador_actual(const juego_t *juego);

/*
    Pre: juego no debe ser NULL.
    Post: Devuelve el puntaje del jugador especificado.
*/
int juego_obtener_puntaje(const juego_t *juego, int jugador);

/*
    Pre: juego no debe ser NULL.
    Post: Devuelve true si la carta está descubierta.
*/
bool juego_es_carta_descubierta(const juego_t *juego, int indice_carta);

/*
    Pre: juego no debe ser NULL.
    Post: Devuelve true si la carta está visible temporalmente.
*/
bool juego_es_carta_visible(const juego_t *juego, int indice_carta);

/*
    Pre: juego no debe ser NULL.
    Post: Devuelve el pokemon asociado a una carta (NULL si no tiene).
*/
struct pokemon* juego_obtener_pokemon_carta(const juego_t *juego, int indice_carta);

void mostrar_resultado_final(juego_t* juego);

/*
    Pre: juego no debe ser NULL.
    
    Post: Libera toda la memoria asociada al juego.
*/
void juego_destruir(juego_t *juego);

#endif // JUEGO_H_