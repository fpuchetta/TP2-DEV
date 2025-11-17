#ifndef TP2_MENU_ACCIONES_H
#define TP2_MENU_ACCIONES_H

#include "juego.h"

bool accion_cargar_archivo(void *user_data);

bool accion_buscar_por_nombre(void *user_data);

bool accion_buscar_por_id(void *user_data);

bool accion_mostrar_por_nombre(void *user_data);

bool accion_mostrar_por_id(void *user_data);

bool accion_jugar(void *user_data);

bool accion_jugar_con_semilla(void *user_data);

#endif
