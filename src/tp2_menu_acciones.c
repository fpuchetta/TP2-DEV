#include "tp2_menu_acciones.h"
#include "tp1.h"
#include <stdio.h>
#include <limits.h>

bool accion_cargar_archivo(void *user_data) {
    //juego_t *juego = user_data;
    return true;
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

bool accion_buscar_por_nombre(void *user_data) {
    juego_t *juego = user_data;

    printf("Ingrese el nombre del Pokémon: ");
    fflush(stdout);

    char *nombre = leer_linea_dinamica();
    if (!nombre) {
        printf("Error leyendo nombre.\n");
        return true;
    }

    struct pokemon *p = tp1_buscar_nombre(juego->pokedex, nombre);

    if (!p) {
        printf("No existe un Pokémon con ese nombre.\n");
        free(nombre);
        return true;
    }

    imprimir_pokemon(p);

    free(nombre);
    return true;
}

int string_a_int(char *linea){
    char *endptr = NULL;
    long id_long = strtol(linea, &endptr, 10);
    free(linea);

    if (endptr == linea) {
        printf("Entrada inválida: no se reconoció un número.\n");
        return -1;
    }

    if (id_long < INT_MIN || id_long > INT_MAX) {
        printf("ID fuera de rango.\n");
        return -1;
    }
    return (int)id_long;
}

bool accion_buscar_por_id(void *user_data) {
    juego_t *juego = user_data;

    printf("Ingrese el ID del Pokémon: ");
    fflush(stdout);

    char *linea = leer_linea_dinamica();
    if (!linea) {
        printf("Error leyendo id.\n");
        return false;
    }

    int id = string_a_int(linea);
    if (id==-1){
        return true;
    }

    struct pokemon *p = tp1_buscar_id(juego->pokedex, id);
    if (!p) {
        printf("No existe un Pokémon con ID %d.\n", id);
        return true;
    }

    imprimir_pokemon(p);

    return true;
}

bool recolectar_pokemones(struct pokemon *p, void *extra)
{
	recolector_t *r = extra;
	if (r->n == r->cap) {
		r->error = 1;
		return false;
	}
	r->v[r->n++] = p;
	return true;
}


bool accion_mostrar_por_nombre(void *user_data) {
    juego_t *juego = user_data;
    size_t n = tp1_cantidad(juego->pokedex);
	struct pokemon **tmp = malloc(n * sizeof *tmp);
	if (!tmp)
		return false;

	recolector_t r = { .v = tmp, .n = 0, .cap = n, .error = 0 };
	size_t aplicados = tp1_con_cada_pokemon(juego->pokedex, recolectar_pokemones, &r);
	if (r.error || aplicados != n) {
		free(tmp);
		return false;
	}

	ordenar_vec_pokemones(tmp, n, sizeof(struct pokemon *),
			      cmp_pokemon_nombre_ptr);

	for (size_t i = 0; i < n; i++) {
		struct pokemon *p = tmp[i];
		imprimir_pokemon(p);
	}

	free(tmp);
    return true;
}

bool pokedex_mostrar_nombres(struct pokemon *pokemon_a_evaluar, void *ctx)
{
	ctx = NULL;
	imprimir_pokemon(pokemon_a_evaluar);
	return true;
}


bool accion_mostrar_por_id(void *user_data) {
    juego_t *juego = user_data;
	size_t iterados =
		tp1_con_cada_pokemon(juego->pokedex, pokedex_mostrar_nombres, NULL);
	if (iterados != tp1_cantidad(juego->pokedex))
		return false;

    return true;
}

static unsigned int juego_semilla_default() {
    return (unsigned int)rand();
}

bool accion_jugar(void *user_data) {
    juego_t *juego = user_data;
    unsigned int semilla = juego_semilla_default();   // viene de rand()
    return juego_jugar(juego, semilla);
}

bool accion_jugar_con_semilla(void *user_data) {
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
        return true;
    }

    // limpiar resto de la línea
    int ch = getchar();
    while (ch != '\n' && ch != EOF) {
        ch = getchar();
    }

    return juego_jugar(juego, (unsigned int)semilla_usuario);
}