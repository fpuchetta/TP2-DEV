#ifndef MENU_H_
#define MENU_H_

#include <stdlib.h>
#include <stdbool.h>

#define MENU_TECLA_SALIR  'Q'
#define MENU_TECLA_VOLVER 'A'
#define MENU_TECLA_ESTILOS 'E'

typedef struct menu menu_t;
typedef struct menu_navegador menu_navegador_t;

typedef bool (*menu_accion_t)(void *user_data);
typedef void (*menu_mostrar_opciones_t)(char tecla, char *nombre);
typedef void (*menu_mostrar_titulo_t)(char *titulo);

typedef enum {
    MENU_NAVEGACION_CONTINUAR,
    MENU_NAVEGACION_TERMINAR,
    MENU_NAVEGACION_ERROR
} menu_navegacion_estado_t;

/*
    Pre: -

    Post: Devuelve un menu_t* reservado en memoria con los campos
          inicializados.
          Devuelve NULL en caso de error.
*/
menu_t *menu_crear_base(const char *titulo, menu_mostrar_opciones_t estilo_opciones, menu_mostrar_titulo_t estilo_titulo);

/*
    Pre: -
    
    Post: Devuelve un puntero a menu_t reservado en memoria con los campos correspondientes a los parametros.
          Devuelve NULL en caso de error.
          Las teclas 'Q', 'A', 'E' estan restringidas.
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
bool menu_agregar_estilo(menu_t* menu, menu_mostrar_opciones_t estilo_opciones, menu_mostrar_titulo_t estilo_titulo);

/*
    Pre: -
    
    Post: Devuelve el título del menu.
*/
const char *menu_obtener_titulo(menu_t *menu);

/*
    Pre: -
    
    Post: Devuelve la cantidad de acciones del menu.
*/
size_t menu_cantidad_acciones(menu_t *menu);

/*
    Pre: -
    
    Post: Devuelve true si el menu tiene acciones.
          Devuelve false en caso contrario.
*/
bool menu_tiene_acciones(menu_t *menu);

/*
    Pre: -
    
    Post: Devuelve la cantidad de submenus del menu.
*/
size_t menu_cantidad_submenus(menu_t *menu);

/*
    Pre: menu no debe ser NULL.
    
    Post: Devuelve true si el menu tiene submenus.
*/
bool menu_tiene_submenus(menu_t *menu);

/*
    Pre: -

    Post: Devuelve la cantidad de estilos que posee el menu.
          Si el menu no es de tipo raiz devuelve cero.
*/
size_t menu_cantidad_estilos(menu_t *menu);

/*
    Pre: menu no debe ser NULL.
    
    Post: Devuelve true si el menu tiene estilos.
*/
bool menu_tiene_estilos(menu_t *menu);

/*
    Pre: -

    Post: Devuelve true si existe una opcion en el menu
          con la tecla pasada por parametro.
          Devuelve false en caso contrario.
*/
bool menu_existe_opcion(menu_t *menu, char tecla);

/*
    Pre: menu no debe ser NULL.
    
    Post: Devuelve el nombre de la opción asociada a una tecla.
          Devuelve NULL si no existe.
*/
const char *menu_obtener_nombre_opcion(const menu_t *menu, char tecla);

/*
    Pre: -

    Post: Inicializa los campos para correr un menu y lo ejecuta.
          Devuelve true si se termino de ejecutar correctamente.
          Devuelve false si se termino de ejecutar erroneamente.
*/
bool menu_ejecutar(menu_t *menu_base, void *user_data);

/*
    Pre: menu no debe ser NULL.
    
    Post: Elimina la acción asociada a la tecla del menu.
          Libera la memoria de la opción eliminada.
          Devuelve true si existía y era una acción.
          Devuelve false si no existía, era submenu o hubo error.
*/
bool menu_sacar_accion(menu_t *menu, char tecla);

/*
    Pre: menu padre no debe ser NULL.
    
    Post: Elimina el submenu asociado a la tecla del menu padre.
          Devuelve el submenu eliminado.
          Devuelve NULL si no existía, era acción o hubo error.
*/
menu_t *menu_sacar_submenu(menu_t* padre, char tecla);

/*
    Pre: -

    Post: Libera la memoria para el menu pasado por parametro.
          No libera los submenus asociados.
*/
void menu_destruir(menu_t *menu);

/*
    Pre: -

    Post: Libera toda la memoria reservada para un menu raiz.
          Libera todos los submenus asociados y sus opciones.
*/
void menu_destruir_todo(menu_t *menu_base);


menu_navegador_t *menu_navegador_crear(menu_t *menu_base, void *user_data);

menu_navegacion_estado_t menu_navegador_procesar_tecla(menu_navegador_t *nav, char tecla);

void menu_navegador_mostrar(const menu_navegador_t *nav);

menu_t *menu_navegador_obtener_actual(const menu_navegador_t *nav);

bool menu_navegador_esta_terminado(const menu_navegador_t *nav);

void menu_navegador_destruir(menu_navegador_t *nav);

#endif // MENU_H_