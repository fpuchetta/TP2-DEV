#include "functions.h"
#include "ansi.h"

#include <stdio.h>
#include <string.h>

int cmp_pokemon_id_ptr(void *pa, void *pb)
{
	struct pokemon *a = *(struct pokemon **)pa;
	struct pokemon *b = *(struct pokemon **)pb;
	return (a->id > b->id) - (a->id < b->id);
}

int cmp_pokemon_nombre_ptr(void *pa, void *pb)
{
	struct pokemon *a = *(struct pokemon **)pa;
	struct pokemon *b = *(struct pokemon **)pb;
	int c = strcmp(a->nombre, b->nombre);
	return (c != 0) ? c : ((a->id > b->id) - (a->id < b->id));
}

void ordenar_vec_pokemones(void *vec, size_t tamanio, size_t tamanio_elemento,
			   int (*cmp)(void *, void *))
{
	if (!vec || !cmp || tamanio_elemento == 0 || tamanio < 2)
		return;

	char *v = vec;

	for (size_t i = 0; i < tamanio - 1; i++) {
		for (size_t j = 0; j < tamanio - i - 1; j++) {
			char *a = v + j * tamanio_elemento;
			char *b = a + tamanio_elemento;

			if (cmp(a, b) > 0) {
				char tmp[tamanio_elemento];
				memcpy(tmp, a, tamanio_elemento);
				memcpy(a, b, tamanio_elemento);
				memcpy(b, tmp, tamanio_elemento);
			}
		}
	}
}

char *mi_strdup(const char *s)
{
	if (!s)
		return NULL;
	size_t len = strlen(s) + 1;
	char *copia = malloc(len);
	if (!copia)
		return NULL;
	memcpy(copia, s, len);
	return copia;
}

size_t int_tam(int n)
{
	size_t len = 0;
	if (n == 0)
		return 1;

	while (n > 0) {
		len++;
		n /= 10;
	}
	return len;
}

size_t calcular_largo_linea(struct pokemon *p)
{
	size_t tam = int_tam(p->id);
	tam += int_tam(p->ataque);
	tam += int_tam(p->defensa);
	tam += int_tam(p->velocidad);
	tam += strlen(p->nombre);

	tam += 5;
	tam += 1;
	tam += 1;
	return tam;
}

int parsear_tipo(const char tipo[MAX_TIPO])
{
	if (strcmp(tipo, "ELEC") == 0)
		return TIPO_ELEC;
	if (strcmp(tipo, "FUEG") == 0)
		return TIPO_FUEG;
	if (strcmp(tipo, "PLAN") == 0)
		return TIPO_PLAN;
	if (strcmp(tipo, "AGUA") == 0)
		return TIPO_AGUA;
	if (strcmp(tipo, "NORM") == 0)
		return TIPO_NORM;
	if (strcmp(tipo, "FANT") == 0)
		return TIPO_FANT;
	if (strcmp(tipo, "PSI") == 0)
		return TIPO_PSI;
	if (strcmp(tipo, "LUCH") == 0)
		return TIPO_LUCH;

	return ERROR_PARSEO;
}

const char *calcular_tipo(struct pokemon *p)
{
	switch (p->tipo) {
	case TIPO_ELEC:
		return "ELEC";
	case TIPO_FUEG:
		return "FUEG";
	case TIPO_PLAN:
		return "PLAN";
	case TIPO_AGUA:
		return "AGUA";
	case TIPO_NORM:
		return "NORM";
	case TIPO_FANT:
		return "FANT";
	case TIPO_PSI:
		return "PSI";
	case TIPO_LUCH:
		return "LUCH";
	default:
		return "DESCONOCIDO";
	}
}

struct pokemon *pokemon_clonar(struct pokemon *p)
{
	if (!p)
		return NULL;

	struct pokemon *nuevo = malloc(sizeof *nuevo);
	if (!nuevo)
		return NULL;

	*nuevo = *p;

	if (p->nombre) {
		nuevo->nombre = mi_strdup(p->nombre);
		if (!nuevo->nombre) {
			free(nuevo);
			return NULL;
		}
	}

	return nuevo;
}

int accionar_comando(tp1_t *tp1, const comando_t *comandos, int argc,
		     char **argv)
{
	for (int i = 0; i < CANTIDAD_COMANDOS; i++) {
		if (strcmp(argv[2], comandos[i].nombre) == 0) {
			return comandos[i].f(tp1, argc, argv);
		}
	}
	return -1;
}

int operacion_con_tps(tp1_t *un_tp, tp1_t *otro_tp, const char *csv_resultado,
		      tp1_t *(*operacion)(tp1_t *, tp1_t *))
{
	tp1_t *tp_resultado = operacion(un_tp, otro_tp);
	if (!tp_resultado) {
		tp1_destruir(otro_tp);
		return -1;
	}
	tp1_t *aux = tp1_guardar_archivo(tp_resultado, csv_resultado);
	if (!aux) {
		tp1_destruir(tp_resultado);
		tp1_destruir(otro_tp);
		return -1;
	}
	tp1_destruir(otro_tp);
	tp1_destruir(tp_resultado);
	return 1;
}

int union_pokemones(tp1_t *tp, int argc, char **argv)
{
	const char *archivo2 = argv[3];
	const char *csv_resultado = argv[4];

	tp1_t *otro_tp = tp1_leer_archivo(archivo2);
	if (!otro_tp)
		return -1;

	return operacion_con_tps(tp, otro_tp, csv_resultado, tp1_union);
}

int interseccion_pokemones(tp1_t *tp, int argc, char **argv)
{
	const char *archivo2 = argv[3];
	const char *csv_resultado = argv[4];

	tp1_t *otro_tp = tp1_leer_archivo(archivo2);
	if (!otro_tp)
		return -1;

	return operacion_con_tps(tp, otro_tp, csv_resultado, tp1_interseccion);
}

int diferencia_pokemones(tp1_t *tp, int argc, char **argv)
{
	const char *archivo2 = argv[3];
	const char *csv_resultado = argv[4];

	tp1_t *otro_tp = tp1_leer_archivo(archivo2);
	if (!otro_tp)
		return -1;

	return operacion_con_tps(tp, otro_tp, csv_resultado, tp1_diferencia);
}

void limpiar_pantalla(){
    printf(ANSI_CLEAR_SCREEN ANSI_CURSOR_HOME ANSI_CLEAR_SCROLLBACK);
    fflush(stdout);
}

void limpiar_buffer(){
    int ch = getchar();
    while (ch != '\n' && ch != EOF){
        ch = getchar();
    }
}

void esperar_enter(){
    int c;

    printf("\n\nPresione ENTER para volver al menú...");
    fflush(stdout);
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int string_a_int(char *linea){
    char *endptr = NULL;
    long id_long = strtol(linea, &endptr, 10);

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

bool separar_dos_tokens(const char *linea, char **token1, char **token2) {
    size_t i = 0;
    while (linea[i] != ' ' && linea[i] != '\0')
        i++;
    
    if (linea[i] != ' ' || i == 0) return false;

    *token1 = malloc(i + 1);
    if (!*token1) return false;
    
    strncpy(*token1, linea, i);
    (*token1)[i] = '\0';
    
    size_t j = i + 1;
    while (linea[j] == ' ') j++;
    if (linea[j] == '\0') {
        free(*token1);
        return false;
    }

    *token2 = mi_strdup(&linea[j]);
    if (!*token2) {
        free(*token1);
        return false;
    }
    
    return true;
}

bool parsear_dos_numeros(const char *linea, int *num1, int *num2) {
    char *token1 = NULL;
    char *token2 = NULL;
    bool exito = false;
    
    if (separar_dos_tokens(linea, &token1, &token2)) {
        int n1 = string_a_int(token1);
        int n2 = string_a_int(token2);
        
        if (n1 != -1 && n2 != -1) {
            *num1 = n1;
            *num2 = n2;
            exito = true;
        }

        free(token1);
        free(token2);
    }
    
    return exito;
}
