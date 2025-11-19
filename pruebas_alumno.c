#include "pa2m.h"
#include "src/menu.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Funciones auxiliares para pruebas
bool accion_prueba_true(void *user_data) { return true; }
bool accion_prueba_false(void *user_data) { return false; }
void mostrar_estilo_simple(char tecla, char *nombre) { printf("%c) %s\n", tecla, nombre); }
void mostrar_estilo_detallado(char tecla, char *nombre) { printf("Opción %c: %s\n", tecla, nombre); }

// Pruebas para menu_crear_base
void menu_crear_base_crea_menu_valido()
{
    menu_t *menu = menu_crear_base("Menu Principal", mostrar_estilo_simple);
    pa2m_afirmar(menu != NULL, "Se crea un menu base con parámetros válidos");
    
    // Verificar que los campos se inicializaron correctamente
    const char *titulo = menu_obtener_titulo(menu);
    pa2m_afirmar(titulo != NULL && strcmp(titulo, "Menu Principal") == 0, 
                 "El título se asignó correctamente");
    
    pa2m_afirmar(menu_cantidad_acciones(menu) == 0, 
                 "El menu se crea sin acciones");
    
    pa2m_afirmar(menu_cantidad_submenus(menu) == 0, 
                 "El menu se crea sin submenus");
    
    menu_destruir_todo(menu);
}

void menu_crear_base_con_estilo_nulo_usa_default()
{
    menu_t *menu = menu_crear_base("Menu", NULL);
    pa2m_afirmar(menu != NULL, "Se crea menu con estilo NULL (usa estilo por defecto)");
    
    // Verificar que el menu se puede usar normalmente
    bool agregado = menu_agregar_accion(menu, 'B', "Acción", accion_prueba_true);
    pa2m_afirmar(agregado == true, "Se puede agregar acción a menu con estilo NULL");
    
    menu_destruir_todo(menu);
}

void menu_crear_base_con_titulo_vacio()
{
    menu_t *menu = menu_crear_base("", mostrar_estilo_simple);
    pa2m_afirmar(menu != NULL, "Se crea menu con título vacío");
    
    const char *titulo = menu_obtener_titulo(menu);
    pa2m_afirmar(titulo != NULL && strcmp(titulo, "") == 0, 
                 "El título vacío se asignó correctamente");
    
    menu_destruir_todo(menu);
}

void menu_crear_base_con_titulo_muy_largo()
{
    char titulo_largo[1000];
    memset(titulo_largo, 'A', 999);
    titulo_largo[999] = '\0';
    
    menu_t *menu = menu_crear_base(titulo_largo, mostrar_estilo_simple);
    pa2m_afirmar(menu != NULL, "Se crea menu con título muy largo");
    
    const char *titulo = menu_obtener_titulo(menu);
    pa2m_afirmar(titulo != NULL && strlen(titulo) == 999, 
                 "El título largo se asignó completamente");
    
    menu_destruir_todo(menu);
}

void menu_crear_base_multiples_menus()
{
    menu_t *menu1 = menu_crear_base("Menu 1", mostrar_estilo_simple);
    menu_t *menu2 = menu_crear_base("Menu 2", mostrar_estilo_detallado);
    menu_t *menu3 = menu_crear_base("Menu 3", NULL);
    
    pa2m_afirmar(menu1 != NULL && menu2 != NULL && menu3 != NULL, 
                 "Se crean múltiples menus simultáneamente");
    
    pa2m_afirmar(strcmp(menu_obtener_titulo(menu1), "Menu 1") == 0, 
                 "Menu 1 tiene título correcto");
    pa2m_afirmar(strcmp(menu_obtener_titulo(menu2), "Menu 2") == 0, 
                 "Menu 2 tiene título correcto");
    pa2m_afirmar(strcmp(menu_obtener_titulo(menu3), "Menu 3") == 0, 
                 "Menu 3 tiene título correcto");
    
    menu_destruir_todo(menu1);
    menu_destruir_todo(menu2);
    menu_destruir_todo(menu3);
}

void menu_crear_base_con_titulo_espacios()
{
    menu_t *menu = menu_crear_base("   ", mostrar_estilo_simple);
    pa2m_afirmar(menu != NULL, "Se crea menu con título de solo espacios");
    
    const char *titulo = menu_obtener_titulo(menu);
    pa2m_afirmar(titulo != NULL && strcmp(titulo, "   ") == 0, 
                 "El título con espacios se asignó correctamente");
    
    menu_destruir_todo(menu);
}

void menu_crear_base_con_titulo_caracteres_especiales()
{
    menu_t *menu = menu_crear_base("Menú con ñ y áéíóú", mostrar_estilo_simple);
    pa2m_afirmar(menu != NULL, "Se crea menu con título con caracteres especiales");
    
    const char *titulo = menu_obtener_titulo(menu);
    pa2m_afirmar(titulo != NULL && strcmp(titulo, "Menú con ñ y áéíóú") == 0, 
                 "El título con caracteres especiales se asignó correctamente");
    
    menu_destruir_todo(menu);
}

void menu_crear_base_con_titulo_numeros()
{
    menu_t *menu = menu_crear_base("Menu 12345", mostrar_estilo_simple);
    pa2m_afirmar(menu != NULL, "Se crea menu con título que contiene números");
    
    const char *titulo = menu_obtener_titulo(menu);
    pa2m_afirmar(titulo != NULL && strcmp(titulo, "Menu 12345") == 0, 
                 "El título con números se asignó correctamente");
    
    menu_destruir_todo(menu);
}

void menu_crear_base_verificar_tipo_raiz()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    pa2m_afirmar(menu != NULL, "Menu base creado correctamente");
    
    // Verificar que se puede agregar estilos (solo menus raíz pueden)
    bool estilo_agregado = menu_agregar_estilo(menu, mostrar_estilo_detallado);
    pa2m_afirmar(estilo_agregado == true, 
                 "Menu base permite agregar estilos adicionales");
    
    menu_destruir_todo(menu);
}

void menu_crear_base_con_titulo_nulo()
{
    menu_t *menu = menu_crear_base(NULL, mostrar_estilo_simple);
    pa2m_afirmar(menu == NULL, "No se puede crear menu con título NULL");
}

void pruebas_menu_crear_base()
{
    pa2m_nuevo_grupo("Pruebas menu_crear_base parámetros válidos");
    menu_crear_base_crea_menu_valido();
    menu_crear_base_con_estilo_nulo_usa_default();
    menu_crear_base_con_titulo_vacio();
    menu_crear_base_con_titulo_muy_largo();
    menu_crear_base_multiples_menus();
    menu_crear_base_con_titulo_espacios();
    menu_crear_base_con_titulo_caracteres_especiales();
    menu_crear_base_con_titulo_numeros();
    menu_crear_base_verificar_tipo_raiz();

    pa2m_nuevo_grupo("Pruebas menu_crear_base parámetros inválidos");
    menu_crear_base_con_titulo_nulo();
}

// Pruebas para menu_crear_submenu
void menu_crear_submenu_crea_submenu_valido()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
    
    pa2m_afirmar(submenu != NULL, "Se crea un submenu con parámetros válidos");
    
    // Verificar que se agregó al padre
    bool existe = menu_existe_opcion(padre, 'S');
    pa2m_afirmar(existe == true, "La tecla 'S' existe en el menu padre");
    
    // Verificar que el submenu tiene el tipo correcto
    const char *nombre = menu_obtener_nombre_opcion(padre, 'S');
    pa2m_afirmar(nombre != NULL && strcmp(nombre, "Submenu") == 0, 
                 "El submenu tiene el nombre correcto");
    
    pa2m_afirmar(menu_cantidad_submenus(padre) == 1, 
                 "El contador de submenus del padre se incrementó");
    
    menu_destruir_todo(padre);
}

void menu_crear_submenu_tecla_en_mayuscula()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 's', "Submenu Minuscula"); // 's' minúscula
    
    pa2m_afirmar(submenu != NULL, "Se crea submenu con tecla minúscula");
    
    // Verificar que se convirtió a mayúscula
    bool existe_mayus = menu_existe_opcion(padre, 'S');
    bool existe_minus = menu_existe_opcion(padre, 's');
    
    pa2m_afirmar(existe_mayus == true, "La tecla se convirtió a mayúscula ('S')");
    pa2m_afirmar(existe_minus == false, "No existe la tecla minúscula ('s')");
    
    menu_destruir_todo(padre);
}

void menu_crear_submenu_multiples_submenus()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple);
    menu_t *sub1 = menu_crear_submenu(padre, 'B', "Submenu B");
    menu_t *sub2 = menu_crear_submenu(padre, 'C', "Submenu C");
    menu_t *sub3 = menu_crear_submenu(padre, 'D', "Submenu D");
    
    pa2m_afirmar(sub1 != NULL && sub2 != NULL && sub3 != NULL, 
                 "Se crean múltiples submenus");
    
    pa2m_afirmar(menu_cantidad_submenus(padre) == 3, 
                 "El padre tiene 3 submenus");
    
    pa2m_afirmar(menu_existe_opcion(padre, 'B') && 
                 menu_existe_opcion(padre, 'C') && 
                 menu_existe_opcion(padre, 'D'), 
                 "Todas las teclas de submenus existen");
    
    menu_destruir_todo(padre);
}

void menu_crear_submenu_nombre_muy_largo()
{
    char nombre_largo[500];
    memset(nombre_largo, 'X', 499);
    nombre_largo[499] = '\0';
    
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'L', nombre_largo);
    
    pa2m_afirmar(submenu != NULL, "Se crea submenu con nombre muy largo");
    
    const char *nombre = menu_obtener_nombre_opcion(padre, 'L');
    pa2m_afirmar(nombre != NULL && strlen(nombre) == 499, 
                 "El nombre largo se asignó completamente");
    
    menu_destruir_todo(padre);
}

void menu_crear_submenu_teclas_especiales_caracteres()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple);
    
    // Probar con diferentes caracteres especiales
    menu_t *sub1 = menu_crear_submenu(padre, '+', "Suma");
    menu_t *sub2 = menu_crear_submenu(padre, '-', "Resta");
    menu_t *sub3 = menu_crear_submenu(padre, '*', "Multiplicar");
    menu_t *sub4 = menu_crear_submenu(padre, '1', "Numero 1");
    
    pa2m_afirmar(sub1 != NULL, "Se crea submenu con tecla '+'");
    pa2m_afirmar(sub2 != NULL, "Se crea submenu con tecla '-'");
    pa2m_afirmar(sub3 != NULL, "Se crea submenu con tecla '*'");
    pa2m_afirmar(sub4 != NULL, "Se crea submenu con tecla '1'");
    
    menu_destruir_todo(padre);
}

void menu_crear_submenu_nombre_vacio()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'V', ""); // Nombre vacío
    
    pa2m_afirmar(submenu == NULL, "No se puede crear submenu con nombre vacío");
    
    // Verificar que no se agregó nada al padre
    bool existe = menu_existe_opcion(padre, 'V');
    pa2m_afirmar(existe == false, "No existe el submenu con nombre vacío en el padre");
    
    menu_destruir_todo(padre);
}

void menu_crear_submenu_con_padre_nulo()
{
    menu_t *submenu = menu_crear_submenu(NULL, 'S', "Submenu");
    pa2m_afirmar(submenu == NULL, "No se puede crear submenu con padre NULL");
}

void menu_crear_submenu_con_nombre_nulo()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'S', NULL);
    
    pa2m_afirmar(submenu == NULL, "No se puede crear submenu con nombre NULL");
    
    menu_destruir_todo(padre);
}

void menu_crear_submenu_con_tecla_invalida()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple);
    menu_t *submenu = menu_crear_submenu(padre, '\0', "Submenu");
    
    pa2m_afirmar(submenu == NULL, "No se puede crear submenu con tecla inválida");
    
    menu_destruir_todo(padre);
}

void menu_crear_submenu_con_teclas_especiales_bloqueadas()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple);
    
    // Intentar crear submenus con teclas reservadas
    menu_t *sub1 = menu_crear_submenu(padre, MENU_TECLA_SALIR, "Salir");
    menu_t *sub2 = menu_crear_submenu(padre, MENU_TECLA_VOLVER, "Volver");
    menu_t *sub3 = menu_crear_submenu(padre, MENU_TECLA_ESTILOS, "Estilos");
    
    pa2m_afirmar(sub1 == NULL, "No se puede crear submenu con tecla SALIR");
    pa2m_afirmar(sub2 == NULL, "No se puede crear submenu con tecla VOLVER");
    pa2m_afirmar(sub3 == NULL, "No se puede crear submenu con tecla ESTILOS");
    
    menu_destruir_todo(padre);
}

void menu_crear_submenu_tecla_duplicada()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple);
    
    // Crear primer submenu
    menu_t *sub1 = menu_crear_submenu(padre, 'X', "Submenu X");
    pa2m_afirmar(sub1 != NULL, "Primer submenu creado correctamente");
    
    // Intentar crear segundo submenu con misma tecla
    menu_t *sub2 = menu_crear_submenu(padre, 'X', "Submenu X Duplicado");
    pa2m_afirmar(sub2 == NULL, "No se puede crear submenu con tecla duplicada");
    
    // Verificar que el primero sigue existiendo
    bool existe = menu_existe_opcion(padre, 'X');
    pa2m_afirmar(existe == true, "El primer submenu sigue existiendo");
    
    menu_destruir_todo(padre);
}

void pruebas_menu_crear_submenu()
{
    pa2m_nuevo_grupo("Pruebas menu_crear_submenu parámetros válidos");
    menu_crear_submenu_crea_submenu_valido();
    menu_crear_submenu_tecla_en_mayuscula();
    menu_crear_submenu_multiples_submenus();
    menu_crear_submenu_nombre_muy_largo();
    menu_crear_submenu_teclas_especiales_caracteres();
	
    pa2m_nuevo_grupo("Pruebas menu_crear_submenu parámetros inválidos");
    menu_crear_submenu_nombre_vacio();
    menu_crear_submenu_con_padre_nulo();
    menu_crear_submenu_con_nombre_nulo();
    menu_crear_submenu_con_tecla_invalida();
    menu_crear_submenu_con_teclas_especiales_bloqueadas();
    menu_crear_submenu_tecla_duplicada();
}

// Pruebas para menu_agregar_accion
void menu_agregar_accion_agrega_correctamente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    bool resultado = menu_agregar_accion(menu, 'X', "Acción X", accion_prueba_true);
    
    pa2m_afirmar(resultado == true, "Se agrega acción correctamente al menu");
    
    // Verificar que realmente se agregó
    bool existe = menu_existe_opcion(menu, 'X');
    pa2m_afirmar(existe == true, "La acción existe en el menu después de agregar");
    
    const char *nombre = menu_obtener_nombre_opcion(menu, 'X');
    pa2m_afirmar(nombre != NULL && strcmp(nombre, "Acción X") == 0, 
                 "La acción tiene el nombre correcto");
    
    pa2m_afirmar(menu_cantidad_acciones(menu) == 1, 
                 "El contador de acciones se incrementó correctamente");
    
    menu_destruir_todo(menu);
}

void menu_agregar_accion_tecla_en_mayuscula()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    bool resultado = menu_agregar_accion(menu, 'x', "Acción Minuscula", accion_prueba_true); // 'a' minúscula
    
    pa2m_afirmar(resultado == true, "Se agrega acción con tecla minúscula");
    
    // Verificar que se convirtió a mayúscula
    bool existe_mayus = menu_existe_opcion(menu, 'X');
    bool existe_minus = menu_existe_opcion(menu, 'x');
    
    pa2m_afirmar(existe_mayus == true, "La tecla se convirtió a mayúscula ('X')");
    pa2m_afirmar(existe_minus == false, "No existe la tecla minúscula ('x')");
    
    menu_destruir_todo(menu);
}

void menu_agregar_accion_multiples_acciones()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    bool resultado1 = menu_agregar_accion(menu, 'X', "Acción X", accion_prueba_true);
    bool resultado2 = menu_agregar_accion(menu, 'Y', "Acción Y", accion_prueba_false);
    bool resultado3 = menu_agregar_accion(menu, 'Z', "Acción Z", accion_prueba_true);
    
    pa2m_afirmar(resultado1 && resultado2 && resultado3, 
                 "Se agregan múltiples acciones correctamente");
    
    pa2m_afirmar(menu_cantidad_acciones(menu) == 3, 
                 "El menu tiene 3 acciones");
    
    pa2m_afirmar(menu_existe_opcion(menu, 'X') && 
                 menu_existe_opcion(menu, 'Y') && 
                 menu_existe_opcion(menu, 'Z'), 
                 "Todas las acciones existen en el menu");
    
    menu_destruir_todo(menu);
}

void menu_agregar_accion_nombre_muy_largo()
{
    char nombre_largo[500];
    memset(nombre_largo, 'Y', 499);
    nombre_largo[499] = '\0';
    
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    bool resultado = menu_agregar_accion(menu, 'L', nombre_largo, accion_prueba_true);
    
    pa2m_afirmar(resultado == true, "Se agrega acción con nombre muy largo");
    
    const char *nombre = menu_obtener_nombre_opcion(menu, 'L');
    pa2m_afirmar(nombre != NULL && strlen(nombre) == 499, 
                 "El nombre largo se asignó completamente");
    
    menu_destruir_todo(menu);
}

void menu_agregar_accion_teclas_especiales_caracteres()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    
    // Probar con diferentes caracteres especiales
    bool res1 = menu_agregar_accion(menu, '+', "Suma", accion_prueba_true);
    bool res2 = menu_agregar_accion(menu, '-', "Resta", accion_prueba_true);
    bool res3 = menu_agregar_accion(menu, '*', "Multiplicar", accion_prueba_true);
    bool res4 = menu_agregar_accion(menu, '1', "Numero 1", accion_prueba_true);
    
    pa2m_afirmar(res1 == true, "Se agrega acción con tecla '+'");
    pa2m_afirmar(res2 == true, "Se agrega acción con tecla '-'");
    pa2m_afirmar(res3 == true, "Se agrega acción con tecla '*'");
    pa2m_afirmar(res4 == true, "Se agrega acción con tecla '1'");
    
    menu_destruir_todo(menu);
}

void menu_agregar_accion_nombre_vacio()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    bool resultado = menu_agregar_accion(menu, 'E', "", accion_prueba_true);
    
    pa2m_afirmar(resultado == false, "No se puede agregar acción con nombre vacío");
    
    // Verificar que no se agregó nada
    bool existe = menu_existe_opcion(menu, 'E');
    pa2m_afirmar(existe == false, "No existe la opción con nombre vacío");
    
    menu_destruir_todo(menu);
}

void menu_agregar_accion_con_menu_nulo()
{
    bool resultado = menu_agregar_accion(NULL, 'X', "Acción", accion_prueba_true);
    pa2m_afirmar(resultado == false, "No se puede agregar acción a menu NULL");
}

void menu_agregar_accion_con_nombre_nulo()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    bool resultado = menu_agregar_accion(menu, 'X', NULL, accion_prueba_true);
    
    pa2m_afirmar(resultado == false, "No se puede agregar acción con nombre NULL");
    
    // Verificar que no se agregó nada
    bool existe = menu_existe_opcion(menu, 'X');
    pa2m_afirmar(existe == false, "No existe la opción después de intentar agregar con nombre NULL");
    
    menu_destruir_todo(menu);
}

void menu_agregar_accion_con_accion_nula()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    bool resultado = menu_agregar_accion(menu, 'X', "Acción", NULL);
    
    pa2m_afirmar(resultado == false, "No se puede agregar acción nula");
    
    // Verificar que no se agregó nada
    bool existe = menu_existe_opcion(menu, 'X');
    pa2m_afirmar(existe == false, "No existe la opción después de intentar agregar con acción NULL");
    
    menu_destruir_todo(menu);
}

void menu_agregar_accion_con_tecla_invalida()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    
    // Probar diferentes teclas inválidas
    bool res1 = menu_agregar_accion(menu, '\0', "Acción Nulo", accion_prueba_true);
    bool res2 = menu_agregar_accion(menu, '\n', "Acción Newline", accion_prueba_true);
    bool res3 = menu_agregar_accion(menu, '\t', "Acción Tab", accion_prueba_true);
    
    pa2m_afirmar(res1 == false, "No se puede agregar acción con tecla '\\0'");
    pa2m_afirmar(res2 == false, "No se puede agregar acción con tecla '\\n'");
    pa2m_afirmar(res3 == false, "No se puede agregar acción con tecla '\\t'");
    
    menu_destruir_todo(menu);
}

void menu_agregar_accion_con_teclas_especiales_bloqueadas()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    
    // Intentar agregar acciones con teclas reservadas
    bool res1 = menu_agregar_accion(menu, MENU_TECLA_SALIR, "Salir", accion_prueba_true);
    bool res2 = menu_agregar_accion(menu, MENU_TECLA_VOLVER, "Volver", accion_prueba_true);
    bool res3 = menu_agregar_accion(menu, MENU_TECLA_ESTILOS, "Estilos", accion_prueba_true);
    
    pa2m_afirmar(res1 == false, "No se puede agregar acción con tecla SALIR ('%c')", MENU_TECLA_SALIR);
    pa2m_afirmar(res2 == false, "No se puede agregar acción con tecla VOLVER ('%c')", MENU_TECLA_VOLVER);
    pa2m_afirmar(res3 == false, "No se puede agregar acción con tecla ESTILOS ('%c')", MENU_TECLA_ESTILOS);
    
    // Verificar que el menú sigue vacío
    pa2m_afirmar(menu_cantidad_acciones(menu) == 0, 
                 "El menú sigue vacío después de intentar agregar teclas reservadas");
    
    menu_destruir_todo(menu);
}

void menu_agregar_accion_tecla_duplicada()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    
    // Agregar primera acción
    bool res1 = menu_agregar_accion(menu, 'X', "Acción X", accion_prueba_true);
    pa2m_afirmar(res1 == true, "Primera acción agregada correctamente");
    
    // Verificar que se agregó
    pa2m_afirmar(menu_cantidad_acciones(menu) == 1, 
                 "El menú tiene 1 acción después de agregar la primera");
    
    // Intentar agregar segunda acción con misma tecla
    bool res2 = menu_agregar_accion(menu, 'X', "Acción X Duplicada", accion_prueba_false);
    pa2m_afirmar(res2 == false, "No se puede agregar acción con tecla duplicada");
    
    // Verificar que el contador no cambió
    pa2m_afirmar(menu_cantidad_acciones(menu) == 1, 
                 "El contador de acciones no cambió después de intentar duplicar");
    
    // Verificar que la primera acción sigue existiendo con su nombre original
    const char *nombre = menu_obtener_nombre_opcion(menu, 'X');
    pa2m_afirmar(nombre != NULL && strcmp(nombre, "Acción X") == 0, 
                 "La primera acción mantiene su nombre original");
    
    menu_destruir_todo(menu);
}

void pruebas_menu_agregar_accion()
{
    pa2m_nuevo_grupo("Pruebas menu_agregar_accion parámetros válidos");
    menu_agregar_accion_agrega_correctamente();
    menu_agregar_accion_tecla_en_mayuscula();
    menu_agregar_accion_multiples_acciones();
    menu_agregar_accion_nombre_muy_largo();
    menu_agregar_accion_teclas_especiales_caracteres();
	
    pa2m_nuevo_grupo("Pruebas menu_agregar_accion parámetros inválidos");
    menu_agregar_accion_nombre_vacio();
    menu_agregar_accion_con_menu_nulo();
    menu_agregar_accion_con_nombre_nulo();
    menu_agregar_accion_con_accion_nula();
    menu_agregar_accion_con_tecla_invalida();
    menu_agregar_accion_con_teclas_especiales_bloqueadas();
    menu_agregar_accion_tecla_duplicada();
}

// Pruebas para menu_agregar_estilo
void menu_agregar_estilo_agrega_correctamente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    bool resultado = menu_agregar_estilo(menu, mostrar_estilo_detallado);
    
    pa2m_afirmar(resultado == true, "Se agrega estilo correctamente al menu");
    
    // Verificar que se puede seguir usando el menu normalmente
    bool accion_agregada = menu_agregar_accion(menu, 'X', "Acción", accion_prueba_true);
    pa2m_afirmar(accion_agregada == true, "El menu sigue funcionando después de agregar estilo");
    
    menu_destruir_todo(menu);
}

void menu_agregar_estilo_multiples_estilos()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    bool resultado1 = menu_agregar_estilo(menu, mostrar_estilo_detallado);
    bool resultado2 = menu_agregar_estilo(menu, mostrar_estilo_simple); // Podría ser otro estilo
    bool resultado3 = menu_agregar_estilo(menu, mostrar_estilo_detallado); // Mismo estilo otra vez
    
    pa2m_afirmar(resultado1 && resultado2 && resultado3, 
                 "Se agregan múltiples estilos correctamente");
    
    menu_destruir_todo(menu);
}

void menu_agregar_estilo_con_menu_nulo()
{
    bool resultado = menu_agregar_estilo(NULL, mostrar_estilo_detallado);
    pa2m_afirmar(resultado == false, "No se puede agregar estilo a menu NULL");
}

void menu_agregar_estilo_con_estilo_nulo()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    bool resultado = menu_agregar_estilo(menu, NULL);
    
    pa2m_afirmar(resultado == false, "No se puede agregar estilo nulo");
    
    menu_destruir_todo(menu);
}

void menu_agregar_estilo_a_submenu()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
    
    // Intentar agregar estilo a submenu (debería fallar)
    bool resultado = menu_agregar_estilo(submenu, mostrar_estilo_detallado);
    pa2m_afirmar(resultado == false, "No se puede agregar estilo a submenu");
    
    menu_destruir_todo(padre);
}

void pruebas_menu_agregar_estilo()
{
    pa2m_nuevo_grupo("Pruebas menu_agregar_estilo parámetros válidos");
    menu_agregar_estilo_agrega_correctamente();
    menu_agregar_estilo_multiples_estilos();

    pa2m_nuevo_grupo("Pruebas menu_agregar_estilo parámetros inválidos");
    menu_agregar_estilo_con_menu_nulo();
    menu_agregar_estilo_con_estilo_nulo();
    menu_agregar_estilo_a_submenu();
}

// Pruebas para menu_obtener_titulo
void menu_obtener_titulo_devuelve_titulo_correcto()
{
    menu_t *menu = menu_crear_base("Menu Principal", mostrar_estilo_simple);
    const char *titulo = menu_obtener_titulo(menu);
    
    pa2m_afirmar(titulo != NULL && strcmp(titulo, "Menu Principal") == 0,
                 "Se obtiene el título correctamente");
    
    menu_destruir_todo(menu);
}

void menu_obtener_titulo_con_menu_nulo()
{
    const char *titulo = menu_obtener_titulo(NULL);
    pa2m_afirmar(titulo == NULL, "Obtener título de menu NULL devuelve NULL");
}

void pruebas_menu_obtener_titulo()
{
    pa2m_nuevo_grupo("Pruebas menu_obtener_titulo parámetros válidos");
    menu_obtener_titulo_devuelve_titulo_correcto();

    pa2m_nuevo_grupo("Pruebas menu_obtener_titulo parámetros inválidos");
    menu_obtener_titulo_con_menu_nulo();
}

// Pruebas para menu_cantidad_acciones
void menu_cantidad_acciones_menu_vacio()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    size_t cantidad = menu_cantidad_acciones(menu);
    
    pa2m_afirmar(cantidad == 0, "Menu vacío tiene 0 acciones");
    
    menu_destruir_todo(menu);
}

void menu_cantidad_acciones_con_acciones()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    menu_agregar_accion(menu, 'A', "Acción A", accion_prueba_true);
    menu_agregar_accion(menu, 'B', "Acción B", accion_prueba_false);
    
    size_t cantidad = menu_cantidad_acciones(menu);
    pa2m_afirmar(cantidad == 2, "Menu con 2 acciones devuelve cantidad correcta");
    
    menu_destruir_todo(menu);
}

void menu_cantidad_acciones_con_menu_nulo()
{
    size_t cantidad = menu_cantidad_acciones(NULL);
    pa2m_afirmar(cantidad == 0, "Cantidad de acciones de menu NULL es 0");
}

void pruebas_menu_cantidad_acciones()
{
    pa2m_nuevo_grupo("Pruebas menu_cantidad_acciones parámetros válidos");
    menu_cantidad_acciones_menu_vacio();
    menu_cantidad_acciones_con_acciones();

    pa2m_nuevo_grupo("Pruebas menu_cantidad_acciones parámetros inválidos");
    menu_cantidad_acciones_con_menu_nulo();
}

// Pruebas para menu_tiene_acciones
void menu_tiene_acciones_menu_vacio()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    bool tiene = menu_tiene_acciones(menu);
    
    pa2m_afirmar(tiene == false, "Menu vacío no tiene acciones");
    
    menu_destruir_todo(menu);
}

void menu_tiene_acciones_con_acciones()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    menu_agregar_accion(menu, 'A', "Acción", accion_prueba_true);
    bool tiene = menu_tiene_acciones(menu);
    
    pa2m_afirmar(tiene == true, "Menu con acciones devuelve true");
    
    menu_destruir_todo(menu);
}

void menu_tiene_acciones_con_menu_nulo()
{
    bool tiene = menu_tiene_acciones(NULL);
    pa2m_afirmar(tiene == false, "Menu NULL no tiene acciones");
}

void pruebas_menu_tiene_acciones()
{
    pa2m_nuevo_grupo("Pruebas menu_tiene_acciones parámetros válidos");
    menu_tiene_acciones_menu_vacio();
    menu_tiene_acciones_con_acciones();

    pa2m_nuevo_grupo("Pruebas menu_tiene_acciones parámetros inválidos");
    menu_tiene_acciones_con_menu_nulo();
}

// Pruebas para menu_cantidad_submenus
void menu_cantidad_submenus_sin_submenus()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    size_t cantidad = menu_cantidad_submenus(menu);
    
    pa2m_afirmar(cantidad == 0, "Menu sin submenus devuelve 0");
    
    menu_destruir_todo(menu);
}

void menu_cantidad_submenus_con_submenus()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    menu_crear_submenu(menu, 'S', "Submenu");
    
    size_t cantidad = menu_cantidad_submenus(menu);
    pa2m_afirmar(cantidad == 1, "Menu con 1 submenu devuelve cantidad correcta");
    
    menu_destruir_todo(menu);
}

void menu_cantidad_submenus_con_menu_nulo()
{
    size_t cantidad = menu_cantidad_submenus(NULL);
    pa2m_afirmar(cantidad == 0, "Cantidad de submenus de menu NULL es 0");
}

void pruebas_menu_cantidad_submenus()
{
    pa2m_nuevo_grupo("Pruebas menu_cantidad_submenus parámetros válidos");
    menu_cantidad_submenus_sin_submenus();
    menu_cantidad_submenus_con_submenus();

    pa2m_nuevo_grupo("Pruebas menu_cantidad_submenus parámetros inválidos");
    menu_cantidad_submenus_con_menu_nulo();
}

// Pruebas para menu_tiene_submenus
void menu_tiene_submenus_sin_submenus()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    bool tiene = menu_tiene_submenus(menu);
    
    pa2m_afirmar(tiene == false, "Menu sin submenus devuelve false");
    
    menu_destruir_todo(menu);
}

void menu_tiene_submenus_con_submenus()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    menu_crear_submenu(menu, 'S', "Submenu");
    bool tiene = menu_tiene_submenus(menu);
    
    pa2m_afirmar(tiene == true, "Menu con submenus devuelve true");
    
    menu_destruir_todo(menu);
}

void menu_tiene_submenus_con_menu_nulo()
{
    bool tiene = menu_tiene_submenus(NULL);
    pa2m_afirmar(tiene == false, "Menu NULL no tiene submenus");
}

void pruebas_menu_tiene_submenus()
{
    pa2m_nuevo_grupo("Pruebas menu_tiene_submenus parámetros válidos");
    menu_tiene_submenus_sin_submenus();
    menu_tiene_submenus_con_submenus();

    pa2m_nuevo_grupo("Pruebas menu_tiene_submenus parámetros inválidos");
    menu_tiene_submenus_con_menu_nulo();
}

// Pruebas para menu_existe_opcion
void menu_existe_opcion_con_opcion_existente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    menu_agregar_accion(menu, 'A', "Acción", accion_prueba_true);
    bool existe = menu_existe_opcion(menu, 'A');
    
    pa2m_afirmar(existe == true, "Opción existente devuelve true");
    
    menu_destruir_todo(menu);
}

void menu_existe_opcion_con_opcion_inexistente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    bool existe = menu_existe_opcion(menu, 'Z');
    
    pa2m_afirmar(existe == false, "Opción inexistente devuelve false");
    
    menu_destruir_todo(menu);
}

void menu_existe_opcion_con_menu_nulo()
{
    bool existe = menu_existe_opcion(NULL, 'A');
    pa2m_afirmar(existe == false, "Opción en menu NULL devuelve false");
}

void pruebas_menu_existe_opcion()
{
    pa2m_nuevo_grupo("Pruebas menu_existe_opcion parámetros válidos");
    menu_existe_opcion_con_opcion_existente();
    menu_existe_opcion_con_opcion_inexistente();

    pa2m_nuevo_grupo("Pruebas menu_existe_opcion parámetros inválidos");
    menu_existe_opcion_con_menu_nulo();
}

// Pruebas para menu_obtener_nombre_opcion
void menu_obtener_nombre_opcion_con_opcion_existente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    menu_agregar_accion(menu, 'A', "Mi Acción", accion_prueba_true);
    const char *nombre = menu_obtener_nombre_opcion(menu, 'A');
    
    pa2m_afirmar(nombre != NULL && strcmp(nombre, "Mi Acción") == 0,
                 "Se obtiene el nombre correcto de la opción");
    
    menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_con_opcion_inexistente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    const char *nombre = menu_obtener_nombre_opcion(menu, 'Z');
    
    pa2m_afirmar(nombre == NULL, "Opción inexistente devuelve NULL");
    
    menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_con_menu_nulo()
{
    const char *nombre = menu_obtener_nombre_opcion(NULL, 'A');
    pa2m_afirmar(nombre == NULL, "Obtener nombre de menu NULL devuelve NULL");
}

void pruebas_menu_obtener_nombre_opcion()
{
    pa2m_nuevo_grupo("Pruebas menu_obtener_nombre_opcion parámetros válidos");
    menu_obtener_nombre_opcion_con_opcion_existente();
    menu_obtener_nombre_opcion_con_opcion_inexistente();

    pa2m_nuevo_grupo("Pruebas menu_obtener_nombre_opcion parámetros inválidos");
    menu_obtener_nombre_opcion_con_menu_nulo();
}

// Pruebas para menu_sacar_accion
void menu_sacar_accion_elimina_correctamente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    menu_agregar_accion(menu, 'A', "Acción", accion_prueba_true);
    
    bool eliminada = menu_sacar_accion(menu, 'A');
    pa2m_afirmar(eliminada == true, "Se elimina acción existente correctamente");
    
    bool existe_despues = menu_existe_opcion(menu, 'A');
    pa2m_afirmar(existe_despues == false, "La acción ya no existe después de eliminar");
    
    menu_destruir_todo(menu);
}

void menu_sacar_accion_con_accion_inexistente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    bool eliminada = menu_sacar_accion(menu, 'Z');
    
    pa2m_afirmar(eliminada == false, "Eliminar acción inexistente devuelve false");
    
    menu_destruir_todo(menu);
}

void menu_sacar_accion_intentar_eliminar_submenu()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    menu_crear_submenu(menu, 'S', "Submenu");
    
    bool eliminada = menu_sacar_accion(menu, 'S');
    pa2m_afirmar(eliminada == false, "No se puede eliminar submenu con menu_sacar_accion");
    
    menu_destruir_todo(menu);
}

void menu_sacar_accion_con_menu_nulo()
{
    bool eliminada = menu_sacar_accion(NULL, 'A');
    pa2m_afirmar(eliminada == false, "Eliminar acción de menu NULL devuelve false");
}

void pruebas_menu_sacar_accion()
{
    pa2m_nuevo_grupo("Pruebas menu_sacar_accion parámetros válidos");
    menu_sacar_accion_elimina_correctamente();

    pa2m_nuevo_grupo("Pruebas menu_sacar_accion parámetros inválidos");
    menu_sacar_accion_con_accion_inexistente();
    menu_sacar_accion_intentar_eliminar_submenu();
    menu_sacar_accion_con_menu_nulo();
}

// Pruebas para menu_sacar_submenu
void menu_sacar_submenu_elimina_correctamente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    menu_crear_submenu(menu, 'S', "Submenu");
    
    menu_t *submenu = menu_sacar_submenu(menu, 'S');
    pa2m_afirmar(submenu != NULL, "Se elimina submenu existente correctamente");
    
    bool existe_despues = menu_existe_opcion(menu, 'S');
    pa2m_afirmar(existe_despues == false, "El submenu ya no existe después de eliminar");
    
    menu_destruir(submenu);
    menu_destruir_todo(menu);
}

void menu_sacar_submenu_con_submenu_inexistente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    menu_t *submenu = menu_sacar_submenu(menu, 'Z');
    
    pa2m_afirmar(submenu == NULL, "Eliminar submenu inexistente devuelve NULL");
    
    menu_destruir_todo(menu);
}

void menu_sacar_submenu_intentar_eliminar_accion()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    menu_agregar_accion(menu, 'A', "Acción", accion_prueba_true);
    
    menu_t *submenu = menu_sacar_submenu(menu, 'A');
    pa2m_afirmar(submenu == NULL, "No se puede eliminar acción con menu_sacar_submenu");
    
    menu_destruir_todo(menu);
}

void menu_sacar_submenu_con_menu_nulo()
{
    menu_t *submenu = menu_sacar_submenu(NULL, 'S');
    pa2m_afirmar(submenu == NULL, "Eliminar submenu de menu NULL devuelve NULL");
}

void pruebas_menu_sacar_submenu()
{
    pa2m_nuevo_grupo("Pruebas menu_sacar_submenu parámetros válidos");
    menu_sacar_submenu_elimina_correctamente();

    pa2m_nuevo_grupo("Pruebas menu_sacar_submenu parámetros inválidos");
    menu_sacar_submenu_con_submenu_inexistente();
    menu_sacar_submenu_intentar_eliminar_accion();
    menu_sacar_submenu_con_menu_nulo();
}

// Pruebas para menu_ejecutar
void menu_ejecutar_con_menu_nulo()
{
    bool resultado = menu_ejecutar(NULL, NULL);
    pa2m_afirmar(resultado == false, "Ejecutar menu NULL devuelve false");
}

void menu_ejecutar_menu_vacio()
{
    menu_t *menu = menu_crear_base("Menu Vacío", mostrar_estilo_simple);
    bool resultado = menu_ejecutar(menu, NULL);
    
    pa2m_afirmar(resultado == true, "Ejecutar menu vacío devuelve true");
    
    menu_destruir_todo(menu);
}

void pruebas_menu_ejecutar()
{
    pa2m_nuevo_grupo("Pruebas menu_ejecutar parámetros válidos");
    menu_ejecutar_menu_vacio();

    pa2m_nuevo_grupo("Pruebas menu_ejecutar parámetros inválidos");
    menu_ejecutar_con_menu_nulo();
}

// Pruebas para menu_destruir
void menu_destruir_menu_nulo()
{
    menu_destruir(NULL);
    pa2m_afirmar(true, "Destruir menu NULL no genera errores");
}

void menu_destruir_menu_valido()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple);
    menu_destruir(menu);
    pa2m_afirmar(true, "Destruir menu válido no genera errores");
}

void pruebas_menu_destruir()
{
    pa2m_nuevo_grupo("Pruebas menu_destruir parámetros válidos");
    menu_destruir_menu_valido();

    pa2m_nuevo_grupo("Pruebas menu_destruir parámetros inválidos");
    menu_destruir_menu_nulo();
}

// Pruebas para menu_destruir_todo
void menu_destruir_todo_menu_nulo()
{
    menu_destruir_todo(NULL);
    pa2m_afirmar(true, "Destruir todo menu NULL no genera errores");
}

void menu_destruir_todo_menu_base()
{
    menu_t *menu = menu_crear_base("Menu Base", mostrar_estilo_simple);
    menu_destruir_todo(menu);
    pa2m_afirmar(true, "Destruir todo menu base no genera errores");
}

void menu_destruir_todo_con_submenus()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple);
    menu_crear_submenu(padre, '1', "Submenu 1");
    menu_crear_submenu(padre, '2', "Submenu 2");
    
    menu_destruir_todo(padre);
    pa2m_afirmar(true, "Destruir todo menu con submenus no genera errores");
}

void pruebas_menu_destruir_todo()
{
    pa2m_nuevo_grupo("Pruebas menu_destruir_todo parámetros válidos");
    menu_destruir_todo_menu_base();
    menu_destruir_todo_con_submenus();

    pa2m_nuevo_grupo("Pruebas menu_destruir_todo parámetros inválidos");
    menu_destruir_todo_menu_nulo();
}

void ejecutar_pruebas_menu(){
    pruebas_menu_crear_base();
    pruebas_menu_crear_submenu();
    pruebas_menu_agregar_accion();
    pruebas_menu_agregar_estilo();
    pruebas_menu_obtener_titulo();
    pruebas_menu_cantidad_acciones();
    pruebas_menu_tiene_acciones();
    pruebas_menu_cantidad_submenus();
    pruebas_menu_tiene_submenus();
    pruebas_menu_existe_opcion();
    pruebas_menu_obtener_nombre_opcion();
    pruebas_menu_sacar_accion();
    pruebas_menu_sacar_submenu();
    //pruebas_menu_ejecutar();
    pruebas_menu_destruir();
    pruebas_menu_destruir_todo();
}

int main()
{
    pa2m_nuevo_grupo("============== PRUEBAS TDA MENU ===============");
    
	ejecutar_pruebas_menu();

    return pa2m_mostrar_reporte();
}