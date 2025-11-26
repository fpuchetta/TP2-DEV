#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include "tp1.h"
#include <string.h>
#include <limits.h>

#define ANSI_CURSOR_HOME "\x1b[H"
#define ANSI_CLEAR_SCROLLBACK "\x1b[3J"

#define MODO_LECTURA "r"
#define MODO_ESCRITURA "w"

#define ERROR_PARSEO -2
#define ERROR_MEMORIA -1

#define COLUMNAS_CSV 6
#define MAX_TIPO 4
#define FACTOR_AUMENTO_TAMANIO 2
#define CAPACIDAD_MINIMA 10

#define CANTIDAD_COMANDOS 1

typedef struct {
	struct pokemon **v;
	size_t n;
	size_t cap;
	int error;
} recolector_t;

typedef struct comando {
	const char *nombre;
	int (*f)(tp1_t *, int, char **);
} comando_t;

typedef enum operacion { UNION, INTERSECCION, DIFERENCIA } operacion_t;

/*
	Pre: Los parametros no pueden ser NULL, 
	Deben ser punteros validos a struct pokemon

	Post: Devuelve el resultado de strcmp, 
	>0 si el nombre de a va antes que el de b,
	<0 si el nombre de b va an tes que el de a
	funciona como cmp_pokemon_id si son iguales
*/
int cmp_pokemon_nombre_ptr(void *pa, void *pb);

/*
  Pre: pa y pb no pueden ser NULL. *pa y *pb deben ser punteros validos a struct pokemon.
  Post:Devuelve  > 0  si a->id  > b->id
       Devuelve  = 0  si a->id == b->id
       Devuelve  < 0  si a->id  < b->id
*/
int cmp_pokemon_id_ptr(void *pa, void *pb);

/*
	Pre: -
	Post: Ordena el vector pasado por parametro.
*/
void ordenar_vec_pokemones(void *vec, size_t tamanio, size_t tamanio_elemento,
			   int (*cmp)(void *, void *));

/*
	Pre: -
	Post: Hace una copia del string pasado por parametro
	      Devuelve NULL en caso de error
*/
char *mi_strdup(const char *s);

/*
	Pre: N tiene que ser positivo
	Post: Devuelve la cantidad de caracteres necesarios para representar N
*/
size_t int_tam(int n);

/*
	Pre: El pokemon pasado por parametro no puede ser NULL
	Post: Devuelve el largo total al pasar el pokemon a formato CSV
*/
size_t calcular_largo_linea(struct pokemon *p);

/*
	Pre: -
	Post: Devuelve el enum correspondiente al tipo pasado por parametro,
		  Devuelve ERROR_PARSEO en caso de error.
*/
int parsear_tipo(const char tipo[MAX_TIPO]);

/*
	Pre: El pokemon pasado por parametro no debe ser NULL
	Post: Devuelve el string correspondiente al enum del parametro
*/
const char *calcular_tipo(struct pokemon *p);

/*
	Pre: -
	Post: Devuelve una copia del pokemon pasado por parametro
*/
struct pokemon *pokemon_clonar(struct pokemon *p);

/*
	Pre: El tp1, los comandos y el argv no pueden ser NULL,
	la cantidad de argumentos tiene que ser mayor que cero
	Post: Devuelve 1 en caso de haberse ejecutado el comando correctamente
		  Devuelve -1 en caso de error
*/
int accionar_comando(tp1_t *tp1, const comando_t *comandos, int argc,
		     char **argv);

/*
	Pre: "un_tp", "otro_tp", "csv_resultado" y la funcion no pueden ser NULL
	Post: Devuelve 1 en caso de haber ejecutado la operacion de conjuntos correctamente
		  Devuelve -1 en caso de error.
*/
int operacion_con_tps(tp1_t *un_tp, tp1_t *otro_tp, const char *csv_resultado,
		      tp1_t *(*operacion)(tp1_t *, tp1_t *));

/*
	Pre: "tp" y "argv" no pueden ser NULL
	Post: Devuelve 1 en caso de haber podido realizar la union entre tps,
		  Devuelve -1 en caso de error.
*/
int union_pokemones(tp1_t *tp, int argc, char **argv);

/*
	Pre: "tp" y "argv" no pueden ser NULL
	Post: Devuelve 1 en caso de haber podido realizar la interseccion entre tps,
		  Devuelve -1 en caso de error.
*/
int interseccion_pokemones(tp1_t *tp, int argc, char **argv);

/*
	Pre: "tp" y "argv" no pueden ser NULL
	Post: Devuelve 1 en caso de haber podido realizar la diferencia entre tps,
		  Devuelve -1 en caso de error.
*/
int diferencia_pokemones(tp1_t *tp, int argc, char **argv);

/////////

/*
    Pre: -

    Post: Limpia la pantalla de la terminal.
*/
void limpiar_pantalla();

/*
    Pre: -

    Post: Limpia el stdin restante.
*/
void limpiar_buffer();

/*
    Pre: -

    Post: Espera el input de ENTER para salir de la accion correspondiente.
*/
void esperar_enter();

/*
	Pre: La linea no debe ser NULL;

	Post: Devuelve el string pasado por parametro en int.
		  Devuelve -1 en caso de error.
*/
int string_a_int(char *linea);

/*
	Pre: -

	Post: Devuelve un puntero con la linea
		  leida por stdin reservada en memoria.
*/
char *leer_linea_dinamica();

/*
    Pre: linea, token1 y token2 no deben ser NULL.

    Post: Devuelve true si la linea contiene exactamente dos tokens separados por espacio.
          En caso de exito: *token1 y *token2 contienen los tokens reservados en memoria.
          En caso de error: Devuelve false y no asigna memoria o libera la asignada.
*/
bool separar_dos_tokens(const char *linea, char **token1, char **token2);

/*
    Pre: linea, num1 y num2 no deben ser NULL.

    Post: Devuelve true si la línea contiene exactamente dos números válidos.
          En caso de éxito: *num1 y *num2 contienen los números convertidos.
          En caso de error: Devuelve false y libera cualquier memoria temporal.
*/
bool parsear_dos_numeros(const char *linea, int *num1, int *num2);

#endif // FUNCTIONS_H_