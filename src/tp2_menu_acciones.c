#include "tp2_menu_acciones.h"
#include "tp1.h"
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>

#include "ansi.h"

bool accion_cargar_archivo(void *user_data)
{
	limpiar_pantalla();
	juego_t *juego = user_data;

	if (juego_tiene_pokedex(juego)) {
		printf("Ya hay un archivo cargado. ¿Desea recargar? (s/N): ");
		fflush(stdout);

		int c = getchar();
		if (c != '\n')
			limpiar_buffer();

		if (c != 's' && c != 'S') {
			printf("Recarga cancelada.\n");
			esperar_enter();
			return true;
		}

		printf("Recargando archivo...\n");
		juego_establecer_pokedex(juego, NULL);
	}

	printf("Ingrese ruta del archivo: ");
	fflush(stdout);

	char *ruta = leer_linea_dinamica();
	if (!ruta)
		return false;

	tp1_t *nueva_pokedex = tp1_leer_archivo(ruta);
	if (!nueva_pokedex) {
		printf("Error cargando archivo '%s'\n", ruta);
		free(ruta);
		esperar_enter();
		return false;
	}

	bool exito = juego_establecer_pokedex(juego, nueva_pokedex);
	printf("Archivo '%s' cargado exitosamente.\n", ruta);
	free(ruta);
	esperar_enter();
	return exito;
}

void imprimir_pokemon(const struct pokemon *pokemon)
{
	printf("%s(%i) - Tipo:%s A:%u D:%u V:%u\n", pokemon->nombre,
	       pokemon->id, calcular_tipo((struct pokemon *)pokemon),
	       pokemon->ataque, pokemon->defensa, pokemon->velocidad);
}

bool accion_buscar_por_nombre(void *user_data)
{
	limpiar_pantalla();
	juego_t *juego = user_data;

	if (!juego_tiene_pokedex(juego)) {
		printf("Error: No hay archivo cargado.\n");
		esperar_enter();
		return true;
	}

	printf("Ingrese el nombre del Pokemon: ");
	fflush(stdout);

	char *nombre = leer_linea_dinamica();
	if (!nombre) {
		printf("Error leyendo nombre.\n");
		esperar_enter();
		return false;
	}

	tp1_t *pokedex = juego_obtener_pokedex(juego);
	struct pokemon *p = tp1_buscar_nombre(pokedex, nombre);
	if (!p) {
		printf("No existe un Pokemon con ese nombre.\n");
		free(nombre);
		esperar_enter();
		return true;
	}

	imprimir_pokemon(p);
	free(nombre);
	esperar_enter();
	return true;
}

bool accion_buscar_por_id(void *user_data)
{
	limpiar_pantalla();
	juego_t *juego = user_data;

	if (!juego_tiene_pokedex(juego)) {
		printf("Error: No hay archivo cargado.\n");
		esperar_enter();
		return true;
	}

	printf("Ingrese el ID del Pokemon: ");
	fflush(stdout);

	char *linea = leer_linea_dinamica();
	if (!linea) {
		printf("Error leyendo id.\n");
		esperar_enter();
		return false;
	}

	int id = string_a_int(linea);
	free(linea);
	if (id == -1) {
		return true;
	}

	tp1_t *pokedex = juego_obtener_pokedex(juego);
	struct pokemon *p = tp1_buscar_id(pokedex, id);
	if (!p) {
		printf("No existe un Pokemon con ID %d.\n", id);
		esperar_enter();
		return true;
	}

	imprimir_pokemon(p);
	esperar_enter();
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

bool accion_mostrar_por_nombre(void *user_data)
{
	limpiar_pantalla();
	juego_t *juego = user_data;

	if (!juego_tiene_pokedex(juego)) {
		printf("Error: No hay archivo cargado.\n");
		esperar_enter();
		return true;
	}

	tp1_t *pokedex = juego_obtener_pokedex(juego);
	size_t n = tp1_cantidad(pokedex);
	struct pokemon **tmp = malloc(n * sizeof(*tmp));
	if (!tmp)
		return false;

	recolector_t r = { .v = tmp, .n = 0, .cap = n, .error = 0 };
	size_t aplicados =
		tp1_con_cada_pokemon(pokedex, recolectar_pokemones, &r);
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
	esperar_enter();
	return true;
}

bool pokedex_mostrar_nombres(struct pokemon *pokemon_a_evaluar, void *ctx)
{
	ctx = NULL;
	imprimir_pokemon(pokemon_a_evaluar);
	return true;
}

bool accion_mostrar_por_id(void *user_data)
{
	limpiar_pantalla();
	juego_t *juego = user_data;

	if (!juego_tiene_pokedex(juego)) {
		printf("Error: No hay archivo cargado.\n");
		esperar_enter();
		return true;
	}

	tp1_t *pokedex = juego_obtener_pokedex(juego);
	size_t iterados =
		tp1_con_cada_pokemon(pokedex, pokedex_mostrar_nombres, NULL);
	if (iterados != tp1_cantidad(pokedex))
		return false;

	esperar_enter();
	return true;
}

static unsigned int obtener_semilla_aleatoria()
{
	return (unsigned int)time(NULL);
}

bool validar_formato(const char *linea, int *num1, int *num2)
{
	return parsear_dos_numeros(linea, num1, num2);
}

void mostrar_mensaje_error(estado_jugada_t resultado)
{
	switch (resultado) {
	case JUGADA_CARTA_YA_DESCUBIERTA:
		printf("Error: Una de las cartas ya fue descubierta. Intente nuevamente.\n");
		break;
	case JUGADA_MISMA_CARTA:
		printf("Error: No puede seleccionar la misma carta dos veces. Intente nuevamente.\n");
		break;
	case JUGADA_CARTA_INVALIDA:
	default:
		printf("Error: Por favor elija carta dentro de los limites.\n");
		break;
	}
}

bool pedir_y_validar_par_cartas(int jugador_actual, int *idx1, int *idx2)
{
	const char *color_jugador = (jugador_actual == 0) ? COLOR_JUGADOR_1 :
							    COLOR_JUGADOR_2;
	printf("%sJugador %i%s ingrese dos cartas separadas por espacio: ",
	       color_jugador, jugador_actual + 1, ANSI_COLOR_RESET);
	fflush(stdout);

	char *linea = leer_linea_dinamica();
	if (!linea)
		return false;

	int num1, num2;
	bool formato_valido = validar_formato(linea, &num1, &num2);
	if (!formato_valido) {
		*idx1 = -1;
		*idx2 = -1;
		printf("Error: Formato incorrecto. Use: numero espacio numero\n");
	} else {
		*idx1 = num1 - 1;
		*idx2 = num2 - 1;
	}

	free(linea);
	return true;
}

bool procesar_turno(juego_t *juego)
{
	if (!juego)
		return false;

	limpiar_pantalla();
	mostrar_layout_completo(juego);

	estado_jugada_t resultado = JUGADA_VALIDA;
	bool jugada_valida = false;
	int idx1, idx2;

	while (!jugada_valida) {
		if (!pedir_y_validar_par_cartas(
			    juego_obtener_jugador_actual(juego), &idx1,
			    &idx2)) {
			return false;
		}
		resultado = juego_validar_jugada(juego, idx1, idx2);
		if (resultado != JUGADA_VALIDA) {
			mostrar_mensaje_error(resultado);
		} else {
			limpiar_pantalla();
			mostrar_layout_completo(juego);
			printf("Mostrando cartas seleccionadas...\n\n");

			esperar_enter();

			resultado = juego_ejecutar_jugada(juego, idx1, idx2);
			if (resultado == JUGADA_ERROR_MEMORIA) {
				return false;
			}
			jugada_valida = true;
		}
	}

	limpiar_pantalla();
	mostrar_layout_completo(juego);

	if (resultado == JUGADA_FORMO_PAR)
		printf("¡Acierto! Punto para el jugador actual.\n");
	else
		printf("No es un acierto. Turno del siguiente jugador.\n");

	return true;
}

bool juego_interactivo(juego_t *juego)
{
	if (!juego)
		return false;

	bool terminado = false;
	while (!terminado) {
		if (!procesar_turno(juego)) {
			return false;
		}

		if (juego_terminado(juego)) {
			terminado = true;
			printf("¡Juego terminado!\n");
		}
		esperar_enter();
	}

	limpiar_pantalla();
	mostrar_resultado_final(juego);
	esperar_enter();
	return terminado;
}

bool juego_jugar(juego_t *juego, unsigned int semilla)
{
	if (!juego_preparar(juego, semilla)) {
		printf("Error preparando el juego.\n");
		esperar_enter();
		return false;
	}

	return juego_interactivo(juego);
}

bool accion_jugar(void *user_data)
{
	limpiar_pantalla();
	juego_t *juego = user_data;

	if (!juego_tiene_pokedex(juego)) {
		printf("Por favor, primero cargue un archivo con la opcion C en el menu principal.\n");
		return true;
	}

	return juego_jugar(juego, obtener_semilla_aleatoria());
}

bool accion_jugar_con_semilla(void *user_data)
{
	limpiar_pantalla();
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
		printf("Semilla invalida.\n");
		return true;
	}

	return juego_jugar(juego, (unsigned int)semilla_usuario);
}