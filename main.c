#include "ansi.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "stdlib.h"

#include "tp2.h"
#include "src/menu.h"
#include "src/functions.h"


int main(int argc, char *argv[])
{
    // FALTA EJECUTAR POR CSV POR PARAMETRO.
    //
    //
    tp2_t *tp2=tp2_crear("ejemplos/pokedex.csv");
    if (!tp2){
        printf("Error al crear el tp2\n");
        return -1;
    }

//    ejecutar_menu_interactivo(tp2_obtener_menu(tp2));

    if (!tp2_ejecutar(tp2)){
        tp2_destruir_todo(tp2);
        printf("Error al correr el tp2\n");
        return -1;
    }

    tp2_destruir_todo(tp2);
    return 0;
}