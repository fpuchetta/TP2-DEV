#include "tp2_menu_acciones.h"
#include "tp1.h"
#include <stdio.h>
#include <limits.h>

void accion_cargar_archivo(void *user_data) {
    //juego_t *juego = user_data;
}

char *leer_linea_dinamica() {
    size_t cap = 4;
    size_t len = 0;
    char *buffer = malloc(cap);
    if (!buffer) return NULL;

    int c = 0;

    while ((c = getchar()) != '\n' && c != EOF) {

        // si hace falta crecer:
        if (len + 1 >= cap) {
            cap *= 2;
            char *nuevo = realloc(buffer, cap);
            if (!nuevo) {
                free(buffer);
                return NULL;
            }
            buffer = nuevo;
        }

        buffer[len++] = (char)c;
    }

    buffer[len] = '\0';
    return buffer;
}

void imprimir_pokemon(const struct pokemon *pokemon)
{
	printf("%s(%i) - Tipo:%s A:%u D:%u V:%u\n", pokemon->nombre,
	       pokemon->id, calcular_tipo((struct pokemon *)pokemon),
	       pokemon->ataque, pokemon->defensa, pokemon->velocidad);
}

void accion_buscar_por_nombre(void *user_data) {
    juego_t *juego = user_data;

    printf("Ingrese el nombre del Pokémon: ");
    fflush(stdout);

    char *nombre = leer_linea_dinamica();
    if (!nombre) {
        printf("Error leyendo nombre.\n");
        return;
    }

    struct pokemon *p = tp1_buscar_nombre(juego->pokedex, nombre);

    if (!p) {
        printf("No existe un Pokémon con ese nombre.\n");
        free(nombre);
        return;
    }

    imprimir_pokemon(p);

    free(nombre);
}

void accion_buscar_por_id(void *user_data) {
    juego_t *juego = user_data;
    if (!juego || !juego->pokedex) {
        printf("Error: juego o pokedex no inicializados.\n");
        return;
    }

    printf("Ingrese el ID del Pokémon: ");
    fflush(stdout);

    char *linea = leer_linea_dinamica();
    if (!linea) {
        printf("Error leyendo la entrada.\n");
        return;
    }

    // Convertir a entero con algo de validación
    char *endptr = NULL;
    long id_long = strtol(linea, &endptr, 10);
    free(linea);

    if (endptr == linea) {
        printf("Entrada inválida: no se reconoció un número.\n");
        return;
    }

    if (id_long < INT_MIN || id_long > INT_MAX) {
        printf("ID fuera de rango.\n");
        return;
    }

    int id = (int)id_long;

    struct pokemon *p = tp1_buscar_id(juego->pokedex, id);
    if (!p) {
        printf("No existe un Pokémon con ID %d.\n", id);
        return;
    }

    imprimir_pokemon(p);
}

void accion_mostrar_por_nombre(void *user_data) {
    //juego_t *juego = user_data;
    // TODO
}

void accion_mostrar_por_id(void *user_data) {
    //juego_t *juego = user_data;
    // TODO
}

static unsigned int juego_semilla_default(void) {
    return (unsigned int)rand();
}

void accion_jugar(void *user_data) {
    juego_t *juego = user_data;
    unsigned int semilla = juego_semilla_default();   // viene de rand()
    juego_jugar(juego, semilla);
}

void accion_jugar_con_semilla(void *user_data) {
    juego_t *juego = user_data;
    int semilla_usuario = 0;

    printf("Ingrese una semilla (entero): ");
    if (scanf("%d", &semilla_usuario) != 1) {
        printf("Entrada inválida.\n");
        // limpiar stdin
        int ch = getchar();
        while (ch != '\n' && ch != EOF) {
            ch = getchar();
        }
        return;
    }

    // limpiar resto de la línea
    int ch = getchar();
    while (ch != '\n' && ch != EOF) {
        ch = getchar();
    }

    juego_jugar(juego, (unsigned int)semilla_usuario);
}