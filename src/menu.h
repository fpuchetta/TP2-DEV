#ifndef MENU_H_
#define MENU_H_

#include <stdlib.h>
#include <stdbool.h>

#define MENU_TECLA_SALIR  'Q'
#define MENU_TECLA_VOLVER 'A'
#define MENU_TECLA_ESTILOS 'E'

typedef struct menu menu_t;
typedef void (*menu_accion_t)(void *user_data);
typedef void (*menu_mostrar_t)(char tecla, char *nombre);


menu_t *menu_crear_base(const char *titulo, menu_mostrar_t estilo);

menu_t *menu_crear_submenu(menu_t *padre, char tecla, const char *nombre);

bool menu_agregar_accion(menu_t *menu, char tecla, const char *nombre, menu_accion_t accion);

bool menu_agregar_estilo(menu_t* menu, menu_mostrar_t estilo);

void menu_ejecutar(menu_t *menu_base, void *user_data);

void menu_destruir(menu_t *menu);

void menu_destruir_todo(menu_t *menu_base);

#endif // MENU_H_