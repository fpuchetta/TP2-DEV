#include "tp2_menu_acciones.h"
#include "tp1.h"
#include <stdio.h>
#include <limits.h>
#include <time.h>

bool accion_cargar_archivo(void *user_data) {
    juego_t *juego = user_data;
    
    if (juego_tiene_pokedex(juego)) {
        printf("Ya hay un archivo cargado. ¿Desea recargar? (s/N): ");
        fflush(stdout);
        
        int c = getchar();
        if (c != '\n') limpiar_buffer();
        
        if (c != 's' && c != 'S') {
            printf("Recarga cancelada.\n");
            return true;
        }
        
        printf("Recargando archivo...\n");
        // Usar la función pública en lugar de acceder directamente
        juego_establecer_pokedex(juego, NULL);
    }
    
    printf("Ingrese ruta del archivo: ");
    fflush(stdout);
    
    char *ruta = leer_linea_dinamica();
    if (!ruta) return false;
    
    tp1_t *nueva_pokedex = tp1_leer_archivo(ruta);
    if (!nueva_pokedex) {
        printf("Error cargando archivo '%s'\n", ruta);
        free(ruta);
        return false;
    }
    
    bool exito = juego_establecer_pokedex(juego, nueva_pokedex);
    printf("Archivo '%s' cargado exitosamente.\n", ruta);
    free(ruta);
    return exito;
}

void imprimir_pokemon(const struct pokemon *pokemon)
{
	printf("%s(%i) - Tipo:%s A:%u D:%u V:%u\n", pokemon->nombre,
	       pokemon->id, calcular_tipo((struct pokemon *)pokemon),
	       pokemon->ataque, pokemon->defensa, pokemon->velocidad);
}

bool accion_buscar_por_nombre(void *user_data) {
    juego_t *juego = user_data;

    if (!juego_tiene_pokedex(juego)) {
        printf("Error: No hay archivo cargado.\n");
        return true;
    }

    printf("Ingrese el nombre del Pokémon: ");
    fflush(stdout);

    char *nombre = leer_linea_dinamica();
    if (!nombre) {
        printf("Error leyendo nombre.\n");
        return false;
    }

    // Usar función de acceso
    tp1_t *pokedex = juego_obtener_pokedex(juego);
    struct pokemon *p = tp1_buscar_nombre(pokedex, nombre);

    if (!p) {
        printf("No existe un Pokémon con ese nombre.\n");
        free(nombre);
        return true;
    }

    imprimir_pokemon(p);
    free(nombre);
    return true;
}

bool accion_buscar_por_id(void *user_data) {
    juego_t *juego = user_data;

    if (!juego_tiene_pokedex(juego)) {
        printf("Error: No hay archivo cargado.\n");
        return true;
    }

    printf("Ingrese el ID del Pokémon: ");
    fflush(stdout);

    char *linea = leer_linea_dinamica();
    if (!linea) {
        printf("Error leyendo id.\n");
        return false;
    }

    int id = string_a_int(linea);
    free(linea);
    if (id == -1){
        return true;
    }

    tp1_t *pokedex = juego_obtener_pokedex(juego);
    struct pokemon *p = tp1_buscar_id(pokedex, id);
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
    
    if (!juego_tiene_pokedex(juego)) {
        printf("Error: No hay archivo cargado.\n");
        return true;
    }

    tp1_t *pokedex = juego_obtener_pokedex(juego);
    size_t n = tp1_cantidad(pokedex);
	struct pokemon **tmp = malloc(n * sizeof(*tmp));
	if (!tmp)
		return false;

	recolector_t r = { .v = tmp, .n = 0, .cap = n, .error = 0 };
	size_t aplicados = tp1_con_cada_pokemon(pokedex, recolectar_pokemones, &r);
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
    
    if (!juego_tiene_pokedex(juego)) {
        printf("Error: No hay archivo cargado.\n");
        return true;
    }

    tp1_t *pokedex = juego_obtener_pokedex(juego);
	size_t iterados = tp1_con_cada_pokemon(pokedex, pokedex_mostrar_nombres, NULL);
	if (iterados != tp1_cantidad(pokedex))
		return false;

    return true;
}

static unsigned int obtener_semilla_aleatoria() {
    return (unsigned int)time(NULL);
}

bool accion_jugar(void *user_data) {
    juego_t *juego = user_data;
    
    if (!juego_tiene_pokedex(juego)) {
        printf("Por favor, primero cargue un archivo con la opcion C en el menu principal.\n");
        return true;
    }

    return juego_jugar(juego,obtener_semilla_aleatoria());
}

bool accion_jugar_con_semilla(void *user_data) {
    juego_t *juego = user_data;
    
    if (!juego_tiene_pokedex(juego)) {
        printf("Por favor, primero cargue un archivo con la opcion C en el menu principal.\n");
        return true;
    }
    
    printf("Ingrese una semilla (entero): ");
    fflush(stdout);
    
    char *linea = leer_linea_dinamica();
    if (!linea) {
        printf("Error leyendo semilla.\n");
        return false;
    }
    
    int semilla_usuario = string_a_int(linea);
    free(linea);
    
    if (semilla_usuario == -1) {
        printf("Semilla inválida.\n");
        return true;
    }
    
    return juego_jugar(juego, (unsigned int)semilla_usuario);
}