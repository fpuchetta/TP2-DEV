#ifndef MENU_H_
#define MENU_H_

#include <stdlib.h>
#include <stdbool.h>

#define MENU_TECLA_SALIR  'Q'
#define MENU_TECLA_VOLVER 'A'
#define MENU_TECLA_ESTILOS 'E'

typedef struct menu menu_t;
typedef bool (*menu_accion_t)(void *user_data);
typedef void (*menu_mostrar_t)(char tecla, char *nombre);

/*
    Pre: -

    Post: Devuelve un menu_t* reservado en memoria con los campos
          inicializados.
          Devuelve NULL en caso de error.
*/
menu_t *menu_crear_base(const char *titulo, menu_mostrar_t estilo);

/*
    Pre: -
    
    Post: Devuelve un puntero a menu_t reservado en memoria con los campos correspondientes a los parametros.
          Devuelve NULL en caso de error.
*/
menu_t *menu_crear_submenu(menu_t *padre, char tecla, const char *nombre);

/*
    Pre: -
    
    Post: Devuelve true si se pudo agregar la accion correspondiente al menu pasado por parametro.
          Devuelve false en caso de error.
*/
bool menu_agregar_accion(menu_t *menu, char tecla, const char *nombre, menu_accion_t accion);

/*
    Pre: -

    Post: Devuelve true si se pudo agregar el estilo al menu pasado por parametro.
          Devuelve false en caso de error.
*/
bool menu_agregar_estilo(menu_t* menu, menu_mostrar_t estilo);

/*
    Pre: -

    Post: Inicializa los campos para correr un menu y lo ejecuta.
          Devuelve true si se termino de ejecutar correctamente.
          Devuelve false si se termino de ejecutar erroneamente.
*/
bool menu_ejecutar(menu_t *menu_base, void *user_data);

/*
    Pre: -

    Post: Libera la memoria para el menu pasado por parametro.
*/
void menu_destruir(menu_t *menu);

/*
    Pre: -

    Post: Libera toda la memoria reservada para un menu raiz.
          Libera todos los submenus asociados y sus opciones.
*/
void menu_destruir_todo(menu_t *menu_base);

#endif // MENU_H_