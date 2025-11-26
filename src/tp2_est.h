#ifndef TP2_EST_H_
#define TP2_EST_H_
#include "menu.h"

#include "stdlib.h"

typedef struct tp2 tp2_t;

/*
    Pre: -

    Post: Reserva la memoria para el tp2 correspondiente.
          Devuelve un puntero a tp2_t con los campos inicializados.
*/
tp2_t *tp2_crear(const char *nombre_archivo);

/*
    Pre: -

    Post: Ejecuta el tp2, empezando por el menu y pidiendo inputs al usuario
          para navegar o jugar.
          Devuelve el tp2 ejecutado si la ejecucion termino correctamente.
          Devuelve NULL en caso de error.
*/
tp2_t *tp2_ejecutar(tp2_t *tp2);

/*
    Pre: -

    Post: Libera toda la memoria reservada para el tp2 pasado por parametro.
*/
void tp2_destruir_todo(tp2_t *tp2);

#endif // TP2_EST_H_