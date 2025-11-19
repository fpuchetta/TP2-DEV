#ifndef TP2_H_
#define TP2_H_
#include "menu.h"

#include "stdlib.h"

typedef struct tp2 tp2_t;

/*
    Pre: -

    Post: Reserva la memoria para el tp2 correspondiente.
          Devuelve un puntero a tp2_t con los campos inicializados.
*/
tp2_t *tp2_crear(const char *nombre_archivo);

tp2_t *tp2_ejecutar(tp2_t * tp2);

void tp2_destruir_todo(tp2_t *tp2);

#endif // TP2_H_