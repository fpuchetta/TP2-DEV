#ifndef JUEGO_H_
#define JUEGO_H_

#include "tp1.h"
#include "lista.h"
#include "cola.h"
#include "functions.h"

#include "stdlib.h"
#include "stdbool.h"

#define FILAS 3
#define COLUMNAS 6

typedef struct juego juego_t;

typedef struct carta{
    struct pokemon *pokemon;
    bool visible;
    bool encontrada;    
}carta_t;

typedef struct mapa{
    size_t filas;
    size_t columnas;    
    carta_t **celdas; 
}mapa_t;

typedef struct jugador{
    lista_t *movimientos;
    int puntaje;
}jugador_t;

struct juego{
    tp1_t *pokedex; // o hash 
    mapa_t* cuadrilla;
    jugador_t *jugadores;
    cola_t *movimientos;
    int semilla;
};

/*
    Pre: -

    Post: Reserva la memoria necesaria para un juego_t y lo devuelve
          Devuelve NULL en caso de error.
*/
juego_t *juego_crear(const char *archivo);

void juego_jugar(juego_t* juego,unsigned int semilla);

#endif // JUEGO_H_