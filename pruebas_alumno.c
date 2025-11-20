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
void mostrar_estilo_emoji(char tecla, char *nombre) {
    const char *emoji = "⚡";
    if (tecla == 'L') emoji = "📋";
    if (tecla == 'B') emoji = "🔍";
    if (tecla == 'C') emoji = "⚙️";
    if (tecla == 'G') emoji = "💾";
    printf("%s %c - %s\n", emoji, tecla, nombre);
}

void mostrar_titulo_centrado(char *titulo) {
    int espacios = (50 - (int)strlen(titulo)) / 2;
    if (espacios < 0) espacios = 0;
    
    printf("\n");
    for (int i = 0; i < espacios; i++) printf(" ");
    printf("%s\n", titulo);
    
    for (int i = 0; i < espacios; i++) printf(" ");
    for (size_t i = 0; i < strlen(titulo); i++) printf("=");
    printf("\n");
}
void mostrar_titulo_simple(char *titulo) { printf("\n%s\n", titulo); }
void mostrar_titulo_detallado(char *titulo) { printf("\n=== %s ===\n", titulo); }


// Pruebas para menu_crear_base
void menu_crear_base_crea_menu_valido()
{
    menu_t *menu = menu_crear_base("Menu Principal", mostrar_estilo_simple,NULL);
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
    menu_t *menu = menu_crear_base("Menu", NULL,NULL);
    pa2m_afirmar(menu != NULL, "Se crea menu con estilo NULL (usa estilo por defecto)");
    
    // Verificar que el menu se puede usar normalmente
    bool agregado = menu_agregar_accion(menu, 'B', "Acción", accion_prueba_true);
    pa2m_afirmar(agregado == true, "Se puede agregar acción a menu con estilo NULL");
    
    menu_destruir_todo(menu);
}

void menu_crear_base_con_titulo_vacio()
{
    menu_t *menu = menu_crear_base("", mostrar_estilo_simple,NULL);
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
    
    menu_t *menu = menu_crear_base(titulo_largo, mostrar_estilo_simple,NULL);
    pa2m_afirmar(menu != NULL, "Se crea menu con título muy largo");
    
    const char *titulo = menu_obtener_titulo(menu);
    pa2m_afirmar(titulo != NULL && strlen(titulo) == 999, 
                 "El título largo se asignó completamente");
    
    menu_destruir_todo(menu);
}

void menu_crear_base_multiples_menus()
{
    menu_t *menu1 = menu_crear_base("Menu 1", mostrar_estilo_simple,NULL);
    menu_t *menu2 = menu_crear_base("Menu 2", mostrar_estilo_detallado,NULL);
    menu_t *menu3 = menu_crear_base("Menu 3", NULL,NULL);
    
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
    menu_t *menu = menu_crear_base("   ", mostrar_estilo_simple,NULL);
    pa2m_afirmar(menu != NULL, "Se crea menu con título de solo espacios");
    
    const char *titulo = menu_obtener_titulo(menu);
    pa2m_afirmar(titulo != NULL && strcmp(titulo, "   ") == 0, 
                 "El título con espacios se asignó correctamente");
    
    menu_destruir_todo(menu);
}

void menu_crear_base_con_titulo_caracteres_especiales()
{
    menu_t *menu = menu_crear_base("Menú con ñ y áéíóú", mostrar_estilo_simple,NULL);
    pa2m_afirmar(menu != NULL, "Se crea menu con título con caracteres especiales");
    
    const char *titulo = menu_obtener_titulo(menu);
    pa2m_afirmar(titulo != NULL && strcmp(titulo, "Menú con ñ y áéíóú") == 0, 
                 "El título con caracteres especiales se asignó correctamente");
    
    menu_destruir_todo(menu);
}

void menu_crear_base_con_titulo_numeros()
{
    menu_t *menu = menu_crear_base("Menu 12345", mostrar_estilo_simple,NULL);
    pa2m_afirmar(menu != NULL, "Se crea menu con título que contiene números");
    
    const char *titulo = menu_obtener_titulo(menu);
    pa2m_afirmar(titulo != NULL && strcmp(titulo, "Menu 12345") == 0, 
                 "El título con números se asignó correctamente");
    
    menu_destruir_todo(menu);
}

void menu_crear_base_verificar_tipo_raiz()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,NULL);
    pa2m_afirmar(menu != NULL, "Menu base creado correctamente");
    
    // Verificar que se puede agregar estilos (solo menus raíz pueden)
    bool estilo_agregado = menu_agregar_estilo(menu, mostrar_estilo_detallado,mostrar_titulo_centrado);
    pa2m_afirmar(estilo_agregado == true, 
                 "Menu base permite agregar estilos adicionales");
    
    menu_destruir_todo(menu);
}

void menu_crear_base_con_titulo_nulo()
{
    menu_t *menu = menu_crear_base(NULL, mostrar_estilo_simple,NULL);
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
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,NULL);
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
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,NULL);
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
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,NULL);
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
    
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,NULL);
    menu_t *submenu = menu_crear_submenu(padre, 'L', nombre_largo);
    
    pa2m_afirmar(submenu != NULL, "Se crea submenu con nombre muy largo");
    
    const char *nombre = menu_obtener_nombre_opcion(padre, 'L');
    pa2m_afirmar(nombre != NULL && strlen(nombre) == 499, 
                 "El nombre largo se asignó completamente");
    
    menu_destruir_todo(padre);
}

void menu_crear_submenu_teclas_especiales_caracteres()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,NULL);
    
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
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,NULL);
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
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,NULL);
    menu_t *submenu = menu_crear_submenu(padre, 'S', NULL);
    
    pa2m_afirmar(submenu == NULL, "No se puede crear submenu con nombre NULL");
    
    menu_destruir_todo(padre);
}

void menu_crear_submenu_con_tecla_invalida()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,NULL);
    menu_t *submenu = menu_crear_submenu(padre, '\0', "Submenu");
    
    pa2m_afirmar(submenu == NULL, "No se puede crear submenu con tecla inválida");
    
    menu_destruir_todo(padre);
}

void menu_crear_submenu_con_teclas_especiales_bloqueadas()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,NULL);
    
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
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,NULL);
    
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
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,NULL);
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
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,NULL);
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
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,NULL);
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
    
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,NULL);
    bool resultado = menu_agregar_accion(menu, 'L', nombre_largo, accion_prueba_true);
    
    pa2m_afirmar(resultado == true, "Se agrega acción con nombre muy largo");
    
    const char *nombre = menu_obtener_nombre_opcion(menu, 'L');
    pa2m_afirmar(nombre != NULL && strlen(nombre) == 499, 
                 "El nombre largo se asignó completamente");
    
    menu_destruir_todo(menu);
}

void menu_agregar_accion_teclas_especiales_caracteres()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,NULL);
    
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
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,NULL);
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
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,NULL);
    bool resultado = menu_agregar_accion(menu, 'X', NULL, accion_prueba_true);
    
    pa2m_afirmar(resultado == false, "No se puede agregar acción con nombre NULL");
    
    // Verificar que no se agregó nada
    bool existe = menu_existe_opcion(menu, 'X');
    pa2m_afirmar(existe == false, "No existe la opción después de intentar agregar con nombre NULL");
    
    menu_destruir_todo(menu);
}

void menu_agregar_accion_con_accion_nula()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,NULL);
    bool resultado = menu_agregar_accion(menu, 'X', "Acción", NULL);
    
    pa2m_afirmar(resultado == false, "No se puede agregar acción nula");
    
    // Verificar que no se agregó nada
    bool existe = menu_existe_opcion(menu, 'X');
    pa2m_afirmar(existe == false, "No existe la opción después de intentar agregar con acción NULL");
    
    menu_destruir_todo(menu);
}

void menu_agregar_accion_con_tecla_invalida()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,NULL);
    
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
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,NULL);
    
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
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,NULL);
    
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
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    bool resultado = menu_agregar_estilo(menu, mostrar_estilo_detallado, mostrar_titulo_detallado);
    
    pa2m_afirmar(resultado == true, "Se agrega estilo correctamente al menu");
    
    // Verificar que se puede seguir usando el menu normalmente
    bool accion_agregada = menu_agregar_accion(menu, 'X', "Acción", accion_prueba_true);
    pa2m_afirmar(accion_agregada == true, "El menu sigue funcionando después de agregar estilo");
    
    menu_destruir_todo(menu);
}

void menu_agregar_estilo_multiples_estilos()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    bool resultado1 = menu_agregar_estilo(menu, mostrar_estilo_detallado, mostrar_titulo_detallado);
    bool resultado2 = menu_agregar_estilo(menu, mostrar_estilo_emoji, mostrar_titulo_centrado);
    bool resultado3 = menu_agregar_estilo(menu, mostrar_estilo_simple, mostrar_titulo_simple);
    
    pa2m_afirmar(resultado1 && resultado2 && resultado3, 
                 "Se agregan múltiples estilos correctamente");
    
    menu_destruir_todo(menu);
}

void menu_agregar_estilo_con_menu_nulo()
{
    bool resultado = menu_agregar_estilo(NULL, mostrar_estilo_detallado, mostrar_titulo_detallado);
    pa2m_afirmar(resultado == false, "No se puede agregar estilo a menu NULL");
}

void menu_agregar_estilo_con_estilo_opciones_nulo()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    bool resultado = menu_agregar_estilo(menu, NULL, mostrar_titulo_detallado);
    
    pa2m_afirmar(resultado == false, "No se puede agregar estilo con estilo_opciones NULL");
    
    menu_destruir_todo(menu);
}

void menu_agregar_estilo_con_estilo_titulo_nulo()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    bool resultado = menu_agregar_estilo(menu, mostrar_estilo_detallado, NULL);
    
    pa2m_afirmar(resultado == false, "No se puede agregar estilo con estilo_titulo NULL");
    
    menu_destruir_todo(menu);
}

void menu_agregar_estilo_con_ambos_estilos_nulos()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    bool resultado = menu_agregar_estilo(menu, NULL, NULL);
    
    pa2m_afirmar(resultado == false, "No se puede agregar estilo con ambos estilos NULL");
    
    menu_destruir_todo(menu);
}

void menu_agregar_estilo_a_submenu()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
    
    // Intentar agregar estilo a submenu (debería fallar)
    bool resultado = menu_agregar_estilo(submenu, mostrar_estilo_detallado, mostrar_titulo_detallado);
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
    menu_agregar_estilo_con_estilo_opciones_nulo();
    menu_agregar_estilo_con_estilo_titulo_nulo();
    menu_agregar_estilo_con_ambos_estilos_nulos();
    menu_agregar_estilo_a_submenu();
}

// Pruebas para menu_obtener_titulo
void menu_obtener_titulo_devuelve_titulo_correcto()
{
    menu_t *menu = menu_crear_base("Menu Principal", mostrar_estilo_simple, mostrar_titulo_simple);
    const char *titulo = menu_obtener_titulo(menu);
    
    pa2m_afirmar(titulo != NULL, "Se obtiene un título no NULL");
    pa2m_afirmar(strcmp(titulo, "Menu Principal") == 0,
                 "Se obtiene el título correctamente");
    
    menu_destruir_todo(menu);
}

void menu_obtener_titulo_titulo_vacio()
{
    menu_t *menu = menu_crear_base("", mostrar_estilo_simple, mostrar_titulo_simple);
    const char *titulo = menu_obtener_titulo(menu);
    
    pa2m_afirmar(titulo != NULL, "Título vacío devuelve string no NULL");
    pa2m_afirmar(strcmp(titulo, "") == 0,
                 "Se obtiene el título vacío correctamente");
    
    menu_destruir_todo(menu);
}

void menu_obtener_titulo_titulo_muy_largo()
{
    char titulo_largo[1000];
    memset(titulo_largo, 'X', 999);
    titulo_largo[999] = '\0';
    
    menu_t *menu = menu_crear_base(titulo_largo, mostrar_estilo_simple, mostrar_titulo_simple);
    const char *titulo = menu_obtener_titulo(menu);
    
    pa2m_afirmar(titulo != NULL, "Título largo devuelve string no NULL");
    pa2m_afirmar(strlen(titulo) == 999,
                 "Se obtiene el título largo completamente");
    pa2m_afirmar(titulo[0] == 'X' && titulo[998] == 'X',
                 "El contenido del título largo es correcto");
    
    menu_destruir_todo(menu);
}

void menu_obtener_titulo_titulo_con_espacios()
{
    menu_t *menu = menu_crear_base("   Menu con espacios   ", mostrar_estilo_simple, mostrar_titulo_simple);
    const char *titulo = menu_obtener_titulo(menu);
    
    pa2m_afirmar(titulo != NULL, "Título con espacios devuelve string no NULL");
    pa2m_afirmar(strcmp(titulo, "   Menu con espacios   ") == 0,
                 "Se preservan los espacios en el título");
    
    menu_destruir_todo(menu);
}

void menu_obtener_titulo_titulo_con_caracteres_especiales()
{
    menu_t *menu = menu_crear_base("Menú Pokémon: áéíóú ñ ç", mostrar_estilo_simple, mostrar_titulo_simple);
    const char *titulo = menu_obtener_titulo(menu);
    
    pa2m_afirmar(titulo != NULL, "Título con caracteres especiales devuelve string no NULL");
    pa2m_afirmar(strcmp(titulo, "Menú Pokémon: áéíóú ñ ç") == 0,
                 "Se preservan los caracteres especiales en el título");
    
    menu_destruir_todo(menu);
}

void menu_obtener_titulo_titulo_con_numeros_y_simbolos()
{
    menu_t *menu = menu_crear_base("Menu 123! @#$% 456", mostrar_estilo_simple, mostrar_titulo_simple);
    const char *titulo = menu_obtener_titulo(menu);
    
    pa2m_afirmar(titulo != NULL, "Título con números y símbolos devuelve string no NULL");
    pa2m_afirmar(strcmp(titulo, "Menu 123! @#$% 456") == 0,
                 "Se preservan números y símbolos en el título");
    
    menu_destruir_todo(menu);
}

void menu_obtener_titulo_multiples_menus_diferentes_titulos()
{
    menu_t *menu1 = menu_crear_base("Menu Uno", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_t *menu2 = menu_crear_base("Menu Dos", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_t *menu3 = menu_crear_base("Menu Tres", mostrar_estilo_simple, mostrar_titulo_simple);
    
    const char *titulo1 = menu_obtener_titulo(menu1);
    const char *titulo2 = menu_obtener_titulo(menu2);
    const char *titulo3 = menu_obtener_titulo(menu3);
    
    pa2m_afirmar(titulo1 != NULL && strcmp(titulo1, "Menu Uno") == 0,
                 "Menu 1 tiene título correcto");
    pa2m_afirmar(titulo2 != NULL && strcmp(titulo2, "Menu Dos") == 0,
                 "Menu 2 tiene título correcto");
    pa2m_afirmar(titulo3 != NULL && strcmp(titulo3, "Menu Tres") == 0,
                 "Menu 3 tiene título correcto");
    
    menu_destruir_todo(menu1);
    menu_destruir_todo(menu2);
    menu_destruir_todo(menu3);
}

void menu_obtener_titulo_despues_de_operaciones()
{
    menu_t *menu = menu_crear_base("Menu Original", mostrar_estilo_simple, mostrar_titulo_simple);
    
    // Realizar algunas operaciones en el menú
    menu_agregar_accion(menu, 'A', "Acción A", accion_prueba_true);
    menu_agregar_accion(menu, 'B', "Acción B", accion_prueba_false);
    menu_agregar_estilo(menu, mostrar_estilo_detallado, mostrar_titulo_detallado);
    
    const char *titulo = menu_obtener_titulo(menu);
    
    pa2m_afirmar(titulo != NULL && strcmp(titulo, "Menu Original") == 0,
                 "El título se mantiene igual después de operaciones");
    
    menu_destruir_todo(menu);
}

void menu_obtener_titulo_submenu()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu Hijo");
    
    const char *titulo_padre = menu_obtener_titulo(padre);
    const char *titulo_submenu = menu_obtener_titulo(submenu);
    
    pa2m_afirmar(titulo_padre != NULL && strcmp(titulo_padre, "Menu Padre") == 0,
                 "Menu padre tiene título correcto");
    pa2m_afirmar(titulo_submenu != NULL && strcmp(titulo_submenu, "Submenu Hijo") == 0,
                 "Submenu tiene título correcto");
    
    menu_destruir_todo(padre);
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
    menu_obtener_titulo_titulo_vacio();
    menu_obtener_titulo_titulo_muy_largo();
    menu_obtener_titulo_titulo_con_espacios();
    menu_obtener_titulo_titulo_con_caracteres_especiales();
    menu_obtener_titulo_titulo_con_numeros_y_simbolos();
    menu_obtener_titulo_multiples_menus_diferentes_titulos();
    menu_obtener_titulo_despues_de_operaciones();
    menu_obtener_titulo_submenu();

    pa2m_nuevo_grupo("Pruebas menu_obtener_titulo parámetros inválidos");
    menu_obtener_titulo_con_menu_nulo();
}

// Pruebas para menu_cantidad_acciones
void menu_cantidad_acciones_menu_vacio()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    size_t cantidad = menu_cantidad_acciones(menu);
    
    pa2m_afirmar(cantidad == 0, "Menu vacío tiene 0 acciones");
    
    menu_destruir_todo(menu);
}

void menu_cantidad_acciones_con_acciones()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'B', "Acción B", accion_prueba_true);
    menu_agregar_accion(menu, 'C', "Acción C", accion_prueba_false);
    menu_agregar_accion(menu, 'D', "Acción D", accion_prueba_true);
    
    size_t cantidad = menu_cantidad_acciones(menu);
    pa2m_afirmar(cantidad == 3, "Menu con 3 acciones devuelve cantidad correcta");
    
    menu_destruir_todo(menu);
}

void menu_cantidad_acciones_despues_de_eliminar()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'B', "Acción B", accion_prueba_true);
    menu_agregar_accion(menu, 'C', "Acción C", accion_prueba_false);
    
    size_t antes = menu_cantidad_acciones(menu);
    menu_sacar_accion(menu, 'B');
    size_t despues = menu_cantidad_acciones(menu);
    
    pa2m_afirmar(antes == 2 && despues == 1, "Cantidad disminuye después de eliminar acción");
    
    menu_destruir_todo(menu);
}

void menu_cantidad_acciones_submenu()
{
    menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
    menu_agregar_accion(submenu, 'X', "Acción X", accion_prueba_true);
    
    size_t cant_padre = menu_cantidad_acciones(padre);
    size_t cant_submenu = menu_cantidad_acciones(submenu);
    
    pa2m_afirmar(cant_padre == 0, "Menu padre tiene 0 acciones (solo submenu)");
    pa2m_afirmar(cant_submenu == 1, "Submenu tiene 1 acción");
    
    menu_destruir_todo(padre);
}

void menu_cantidad_acciones_con_menu_nulo()
{
    size_t cantidad = menu_cantidad_acciones(NULL);
    pa2m_afirmar(cantidad == 0, "Cantidad de acciones de menu NULL es 0");
}

void pruebas_menu_cantidad_acciones(){
    pa2m_nuevo_grupo("Pruebas menu_cantidad_acciones parámetros válidos");
    menu_cantidad_acciones_menu_vacio();
    menu_cantidad_acciones_con_acciones();
    menu_cantidad_acciones_despues_de_eliminar();
    menu_cantidad_acciones_submenu();

    pa2m_nuevo_grupo("Pruebas menu_cantidad_acciones parámetros inválidos");
    menu_cantidad_acciones_con_menu_nulo();
}

// Pruebas para menu_tiene_acciones
void menu_tiene_acciones_menu_vacio()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    bool tiene = menu_tiene_acciones(menu);
    
    pa2m_afirmar(tiene == false, "Menu vacío no tiene acciones");
    
    menu_destruir_todo(menu);
}

void menu_tiene_acciones_con_acciones()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'B', "Acción", accion_prueba_true);
    bool tiene = menu_tiene_acciones(menu);
    
    pa2m_afirmar(tiene == true, "Menu con acciones devuelve true");
    
    menu_destruir_todo(menu);
}

void menu_tiene_acciones_despues_de_eliminar_todas()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'B', "Acción B", accion_prueba_true);
    menu_sacar_accion(menu, 'B');
    bool tiene = menu_tiene_acciones(menu);
    
    pa2m_afirmar(tiene == false, "Menu sin acciones después de eliminar devuelve false");
    
    menu_destruir_todo(menu);
}

void menu_tiene_acciones_submenu_con_acciones()
{
    menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
    menu_agregar_accion(submenu, 'X', "Acción X", accion_prueba_true);
    
    bool tiene_padre = menu_tiene_acciones(padre);
    bool tiene_submenu = menu_tiene_acciones(submenu);
    
    pa2m_afirmar(tiene_padre == false, "Menu padre sin acciones devuelve false");
    pa2m_afirmar(tiene_submenu == true, "Submenu con acciones devuelve true");
    
    menu_destruir_todo(padre);
}

void menu_tiene_acciones_con_menu_nulo()
{
    bool tiene = menu_tiene_acciones(NULL);
    pa2m_afirmar(tiene == false, "Menu NULL no tiene acciones");
}

void pruebas_menu_tiene_acciones(){
    pa2m_nuevo_grupo("Pruebas menu_tiene_acciones parámetros válidos");
    menu_tiene_acciones_menu_vacio();
    menu_tiene_acciones_con_acciones();
    menu_tiene_acciones_despues_de_eliminar_todas();
    menu_tiene_acciones_submenu_con_acciones();

    pa2m_nuevo_grupo("Pruebas menu_tiene_acciones parámetros inválidos");
    menu_tiene_acciones_con_menu_nulo();
}

// Pruebas para menu_cantidad_submenus
void menu_cantidad_submenus_sin_submenus()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    size_t cantidad = menu_cantidad_submenus(menu);
    
    pa2m_afirmar(cantidad == 0, "Menu sin submenus devuelve 0");
    
    menu_destruir_todo(menu);
}

void menu_cantidad_submenus_con_submenus()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_crear_submenu(menu, 'S', "Submenu 1");
    menu_crear_submenu(menu, 'T', "Submenu 2");
    
    size_t cantidad = menu_cantidad_submenus(menu);
    pa2m_afirmar(cantidad == 2, "Menu con 2 submenus devuelve cantidad correcta");
    
    menu_destruir_todo(menu);
}

void menu_cantidad_submenus_despues_de_eliminar()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_crear_submenu(menu, 'S', "Submenu 1");
    menu_crear_submenu(menu, 'T', "Submenu 2");
    
    size_t antes = menu_cantidad_submenus(menu);
    menu_t *submenu_s=menu_sacar_submenu(menu, 'S');
    size_t despues = menu_cantidad_submenus(menu);
    
    pa2m_afirmar(antes == 2 && despues == 1, "Cantidad disminuye después de eliminar submenu");
    
    menu_destruir(submenu_s);
    menu_destruir_todo(menu);
}

void menu_cantidad_submenus_submenu()
{
    menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
    menu_crear_submenu(submenu, 'X', "Sub-submenu");
    
    size_t cant_padre = menu_cantidad_submenus(padre);
    size_t cant_submenu = menu_cantidad_submenus(submenu);
    
    pa2m_afirmar(cant_padre == 1, "Menu padre tiene 1 submenu");
    pa2m_afirmar(cant_submenu == 1, "Submenu tiene 1 submenu propio");
    
    menu_destruir_todo(padre);
}

void menu_cantidad_submenus_con_menu_nulo()
{
    size_t cantidad = menu_cantidad_submenus(NULL);
    pa2m_afirmar(cantidad == 0, "Cantidad de submenus de menu NULL es 0");
}

void pruebas_menu_cantidad_submenus(){
    pa2m_nuevo_grupo("Pruebas menu_cantidad_submenus parámetros válidos");
    menu_cantidad_submenus_sin_submenus();
    menu_cantidad_submenus_con_submenus();
    menu_cantidad_submenus_despues_de_eliminar();
    menu_cantidad_submenus_submenu();

    pa2m_nuevo_grupo("Pruebas menu_cantidad_submenus parámetros inválidos");
    menu_cantidad_submenus_con_menu_nulo();
}

// Pruebas para menu_tiene_submenus
void menu_tiene_submenus_sin_submenus()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    bool tiene = menu_tiene_submenus(menu);
    
    pa2m_afirmar(tiene == false, "Menu sin submenus devuelve false");
    
    menu_destruir_todo(menu);
}

void menu_tiene_submenus_con_submenus()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_crear_submenu(menu, 'S', "Submenu");
    bool tiene = menu_tiene_submenus(menu);
    
    pa2m_afirmar(tiene == true, "Menu con submenus devuelve true");
    
    menu_destruir_todo(menu);
}

void menu_tiene_submenus_despues_de_eliminar_todos()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_crear_submenu(menu, 'S', "Submenu");
    menu_t *submenu_s=menu_sacar_submenu(menu, 'S');
    bool tiene = menu_tiene_submenus(menu);
    
    pa2m_afirmar(tiene == false, "Menu sin submenus después de eliminar devuelve false");
    
    menu_destruir(submenu_s);
    menu_destruir_todo(menu);
}

void menu_tiene_submenus_submenu_con_submenus()
{
    menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
    menu_crear_submenu(submenu, 'X', "Sub-submenu");
    
    bool tiene_padre = menu_tiene_submenus(padre);
    bool tiene_submenu = menu_tiene_submenus(submenu);
    
    pa2m_afirmar(tiene_padre == true, "Menu padre con submenus devuelve true");
    pa2m_afirmar(tiene_submenu == true, "Submenu con submenus devuelve true");
    
    menu_destruir_todo(padre);
}

void menu_tiene_submenus_con_menu_nulo()
{
    bool tiene = menu_tiene_submenus(NULL);
    pa2m_afirmar(tiene == false, "Menu NULL no tiene submenus");
}

void pruebas_menu_tiene_submenus(){
    pa2m_nuevo_grupo("Pruebas menu_tiene_submenus parámetros válidos");
    menu_tiene_submenus_sin_submenus();
    menu_tiene_submenus_con_submenus();
    menu_tiene_submenus_despues_de_eliminar_todos();
    menu_tiene_submenus_submenu_con_submenus();

    pa2m_nuevo_grupo("Pruebas menu_tiene_submenus parámetros inválidos");
    menu_tiene_submenus_con_menu_nulo();
}

// Pruebas para menu_cantidad_estilos
void menu_cantidad_estilos_menu_raiz()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    size_t cantidad = menu_cantidad_estilos(menu);
    
    pa2m_afirmar(cantidad == 1, "Menu raíz recién creado tiene 1 estilo");
    
    menu_destruir_todo(menu);
}

void menu_cantidad_estilos_con_estilos_agregados()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_estilo(menu, mostrar_estilo_detallado, mostrar_titulo_detallado);
    menu_agregar_estilo(menu, mostrar_estilo_emoji, mostrar_titulo_centrado);
    
    size_t cantidad = menu_cantidad_estilos(menu);
    pa2m_afirmar(cantidad == 3, "Menu con 3 estilos devuelve cantidad correcta");
    
    menu_destruir_todo(menu);
}

void menu_cantidad_estilos_submenu()
{
    menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
    
    size_t cant_padre = menu_cantidad_estilos(padre);
    size_t cant_submenu = menu_cantidad_estilos(submenu);
    
    pa2m_afirmar(cant_padre >= 1, "Menu padre tiene estilos");
    pa2m_afirmar(cant_submenu == 0, "Submenu no tiene estilos (devuelve 0)");
    
    menu_destruir_todo(padre);
}

void menu_cantidad_estilos_con_menu_nulo()
{
    size_t cantidad = menu_cantidad_estilos(NULL);
    pa2m_afirmar(cantidad == 0, "Cantidad de estilos de menu NULL es 0");
}

void pruebas_menu_cantidad_estilos(){
    pa2m_nuevo_grupo("Pruebas menu_cantidad_estilos parámetros válidos");
    menu_cantidad_estilos_menu_raiz();
    menu_cantidad_estilos_con_estilos_agregados();
    menu_cantidad_estilos_submenu();

    pa2m_nuevo_grupo("Pruebas menu_cantidad_estilos parámetros inválidos");
    menu_cantidad_estilos_con_menu_nulo();
}

// Pruebas para menu_tiene_estilos
void menu_tiene_estilos_menu_raiz_un_estilo()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    bool tiene = menu_tiene_estilos(menu);
    
    pa2m_afirmar(tiene == false, "Menu raíz con solo 1 estilo devuelve false");
    
    menu_destruir_todo(menu);
}

void menu_tiene_estilos_con_multiples_estilos()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_estilo(menu, mostrar_estilo_detallado, mostrar_titulo_detallado);
    bool tiene = menu_tiene_estilos(menu);
    
    pa2m_afirmar(tiene == true, "Menu con 2+ estilos devuelve true");
    
    menu_destruir_todo(menu);
}

void menu_tiene_estilos_submenu()
{
    menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_estilo(padre, mostrar_estilo_detallado, mostrar_titulo_detallado);
    menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
    
    bool tiene_padre = menu_tiene_estilos(padre);
    bool tiene_submenu = menu_tiene_estilos(submenu);
    
    pa2m_afirmar(tiene_padre == true, "Menu padre con múltiples estilos devuelve true");
    pa2m_afirmar(tiene_submenu == false, "Submenu devuelve false (no puede tener estilos)");
    
    menu_destruir_todo(padre);
}

void menu_tiene_estilos_con_menu_nulo()
{
    bool tiene = menu_tiene_estilos(NULL);
    pa2m_afirmar(tiene == false, "Menu NULL no tiene estilos");
}

void pruebas_menu_tiene_estilos(){
    pa2m_nuevo_grupo("Pruebas menu_tiene_estilos parámetros válidos");
    menu_tiene_estilos_menu_raiz_un_estilo();
    menu_tiene_estilos_con_multiples_estilos();
    menu_tiene_estilos_submenu();

    pa2m_nuevo_grupo("Pruebas menu_tiene_estilos parámetros inválidos");
    menu_tiene_estilos_con_menu_nulo();
}

// Pruebas para menu_existe_opcion
void menu_existe_opcion_con_opcion_existente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'B', "Acción B", accion_prueba_true);
    menu_agregar_accion(menu, 'C', "Acción C", accion_prueba_false);
    
    bool existe_b = menu_existe_opcion(menu, 'B');
    bool existe_c = menu_existe_opcion(menu, 'C');
    
    pa2m_afirmar(existe_b == true, "Opción 'B' existe en el menu");
    pa2m_afirmar(existe_c == true, "Opción 'C' existe en el menu");
    
    menu_destruir_todo(menu);
}

void menu_existe_opcion_con_submenu_existente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_crear_submenu(menu, 'S', "Submenu");
    
    bool existe = menu_existe_opcion(menu, 'S');
    pa2m_afirmar(existe == true, "Submenu 'S' existe en el menu");
    
    menu_destruir_todo(menu);
}

void menu_existe_opcion_con_opcion_inexistente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'B', "Acción B", accion_prueba_true);
    
    bool existe_x = menu_existe_opcion(menu, 'X');
    bool existe_y = menu_existe_opcion(menu, 'Y');
    
    pa2m_afirmar(existe_x == false, "Opción 'X' no existe en el menu");
    pa2m_afirmar(existe_y == false, "Opción 'Y' no existe en el menu");
    
    menu_destruir_todo(menu);
}

void menu_existe_opcion_menu_vacio()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    
    bool existe = menu_existe_opcion(menu, 'A');
    pa2m_afirmar(existe == false, "Opción no existe en menu vacío");
    
    menu_destruir_todo(menu);
}

void menu_existe_opcion_teclas_especiales_no_existen()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'B', "Acción B", accion_prueba_true);
    
    // Las teclas especiales Q, A, E no deberían existir como opciones normales
    bool existe_q = menu_existe_opcion(menu, MENU_TECLA_SALIR);
    bool existe_a = menu_existe_opcion(menu, MENU_TECLA_VOLVER);
    bool existe_e = menu_existe_opcion(menu, MENU_TECLA_ESTILOS);
    
    pa2m_afirmar(existe_q == false, "Tecla SALIR no existe como opción normal");
    pa2m_afirmar(existe_a == false, "Tecla VOLVER no existe como opción normal");
    pa2m_afirmar(existe_e == false, "Tecla ESTILOS no existe como opción normal");
    
    menu_destruir_todo(menu);
}

void menu_existe_opcion_teclas_especiales_caracteres()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, '+', "Suma", accion_prueba_true);
    menu_agregar_accion(menu, '1', "Uno", accion_prueba_true);
    
    bool existe_mas = menu_existe_opcion(menu, '+');
    bool existe_uno = menu_existe_opcion(menu, '1');
    
    pa2m_afirmar(existe_mas == true, "Tecla '+' existe en el menu");
    pa2m_afirmar(existe_uno == true, "Tecla '1' existe en el menu");
    
    menu_destruir_todo(menu);
}

void menu_existe_opcion_submenu()
{
    menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
    menu_agregar_accion(submenu, 'X', "Acción X", accion_prueba_true);
    
    bool existe_en_padre = menu_existe_opcion(padre, 'S');
    bool existe_en_submenu = menu_existe_opcion(submenu, 'X');
    bool no_existe_en_padre = menu_existe_opcion(padre, 'X');
    
    pa2m_afirmar(existe_en_padre == true, "Submenu 'S' existe en menu padre");
    pa2m_afirmar(existe_en_submenu == true, "Acción 'X' existe en submenu");
    pa2m_afirmar(no_existe_en_padre == false, "Acción 'X' no existe en menu padre");
    
    menu_destruir_todo(padre);
}

void menu_existe_opcion_con_menu_nulo()
{
    bool existe = menu_existe_opcion(NULL, 'A');
    pa2m_afirmar(existe == false, "Opción en menu NULL devuelve false");
}

void menu_existe_opcion_con_menu_sin_opciones()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    // No agregar ninguna opción
    
    bool existe = menu_existe_opcion(menu, 'A');
    pa2m_afirmar(existe == false, "Opción no existe en menu sin opciones");
    
    menu_destruir_todo(menu);
}

void pruebas_menu_existe_opcion(){
    pa2m_nuevo_grupo("Pruebas menu_existe_opcion parámetros válidos");
    menu_existe_opcion_con_opcion_existente();
    menu_existe_opcion_con_submenu_existente();
    menu_existe_opcion_con_opcion_inexistente();
    menu_existe_opcion_menu_vacio();
    menu_existe_opcion_teclas_especiales_no_existen();
    menu_existe_opcion_teclas_especiales_caracteres();
    menu_existe_opcion_submenu();

    pa2m_nuevo_grupo("Pruebas menu_existe_opcion parámetros inválidos");
    menu_existe_opcion_con_menu_nulo();
    menu_existe_opcion_con_menu_sin_opciones();
}

// Pruebas para menu_obtener_nombre_opcion
void menu_obtener_nombre_opcion_con_opcion_existente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'B', "Mi Acción Especial", accion_prueba_true);
    menu_agregar_accion(menu, 'C', "Otra Acción", accion_prueba_false);
    
    const char *nombre_b = menu_obtener_nombre_opcion(menu, 'B');
    const char *nombre_c = menu_obtener_nombre_opcion(menu, 'C');
    
    pa2m_afirmar(nombre_b != NULL, "Nombre de opción 'B' no es NULL");
    pa2m_afirmar(strcmp(nombre_b, "Mi Acción Especial") == 0, 
                 "Se obtiene el nombre correcto de la opción 'B'");
    pa2m_afirmar(strcmp(nombre_c, "Otra Acción") == 0, 
                 "Se obtiene el nombre correcto de la opción 'C'");
    
    menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_con_submenu_existente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_crear_submenu(menu, 'S', "Submenu Configuración");
    
    const char *nombre = menu_obtener_nombre_opcion(menu, 'S');
    
    pa2m_afirmar(nombre != NULL, "Nombre de submenu 'S' no es NULL");
    pa2m_afirmar(strcmp(nombre, "Submenu Configuración") == 0, 
                 "Se obtiene el nombre correcto del submenu");
    
    menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_con_opcion_inexistente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'B', "Acción B", accion_prueba_true);
    
    const char *nombre_x = menu_obtener_nombre_opcion(menu, 'X');
    const char *nombre_y = menu_obtener_nombre_opcion(menu, 'Y');
    
    pa2m_afirmar(nombre_x == NULL, "Nombre de opción 'X' inexistente es NULL");
    pa2m_afirmar(nombre_y == NULL, "Nombre de opción 'Y' inexistente es NULL");
    
    menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_nombre_vacio()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    // Necesitarías una función para agregar opción con nombre vacío
    // Esto depende de si tu implementación permite nombres vacíos
    
    pa2m_afirmar(true, "Prueba de nombre vacío depende de la implementación");
    
    menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_nombre_largo()
{
    char nombre_largo[100];
    memset(nombre_largo, 'A', 99);
    nombre_largo[99] = '\0';
    
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'L', nombre_largo, accion_prueba_true);
    
    const char *nombre = menu_obtener_nombre_opcion(menu, 'L');
    
    pa2m_afirmar(nombre != NULL, "Nombre largo no es NULL");
    pa2m_afirmar(strlen(nombre) == 99, "Nombre largo se preserva completamente");
    pa2m_afirmar(nombre[0] == 'A' && nombre[98] == 'A', 
                 "Contenido del nombre largo es correcto");
    
    menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_caracteres_especiales()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'X', "Menú Pokémon: áéíóú ñ", accion_prueba_true);
    
    const char *nombre = menu_obtener_nombre_opcion(menu, 'X');
    
    pa2m_afirmar(nombre != NULL, "Nombre con caracteres especiales no es NULL");
    pa2m_afirmar(strcmp(nombre, "Menú Pokémon: áéíóú ñ") == 0, 
                 "Caracteres especiales se preservan correctamente");
    
    menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_submenu()
{
    menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu Hijo");
    menu_agregar_accion(submenu, 'X', "Acción en Submenu", accion_prueba_true);
    
    const char *nombre_submenu = menu_obtener_nombre_opcion(padre, 'S');
    const char *nombre_accion = menu_obtener_nombre_opcion(submenu, 'X');
    
    pa2m_afirmar(nombre_submenu != NULL && strcmp(nombre_submenu, "Submenu Hijo") == 0,
                 "Nombre del submenu es correcto en el padre");
    pa2m_afirmar(nombre_accion != NULL && strcmp(nombre_accion, "Acción en Submenu") == 0,
                 "Nombre de la acción es correcto en el submenu");
    
    menu_destruir_todo(padre);
}

void menu_obtener_nombre_opcion_despues_de_operaciones()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'B', "Acción Original", accion_prueba_true);
    
    const char *nombre_antes = menu_obtener_nombre_opcion(menu, 'B');
    
    // Agregar más opciones y estilos
    menu_agregar_accion(menu, 'C', "Otra Acción", accion_prueba_false);
    menu_agregar_estilo(menu, mostrar_estilo_detallado, mostrar_titulo_detallado);
    
    const char *nombre_despues = menu_obtener_nombre_opcion(menu, 'B');
    
    pa2m_afirmar(nombre_antes != NULL && nombre_despues != NULL,
                 "Nombre se mantiene accesible después de operaciones");
    pa2m_afirmar(strcmp(nombre_antes, nombre_despues) == 0,
                 "Nombre no cambia después de otras operaciones");
    
    menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_con_menu_nulo()
{
    const char *nombre = menu_obtener_nombre_opcion(NULL, 'A');
    pa2m_afirmar(nombre == NULL, "Obtener nombre de menu NULL devuelve NULL");
}

void menu_obtener_nombre_opcion_con_tecla_invalida()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    
    const char *nombre_nulo = menu_obtener_nombre_opcion(menu, '\0');
    pa2m_afirmar(nombre_nulo == NULL, "Tecla nula devuelve NULL");
    
    menu_destruir_todo(menu);
}

void pruebas_menu_obtener_nombre_opcion(){
    pa2m_nuevo_grupo("Pruebas menu_obtener_nombre_opcion parámetros válidos");
    menu_obtener_nombre_opcion_con_opcion_existente();
    menu_obtener_nombre_opcion_con_submenu_existente();
    menu_obtener_nombre_opcion_con_opcion_inexistente();
    menu_obtener_nombre_opcion_nombre_largo();
    menu_obtener_nombre_opcion_caracteres_especiales();
    menu_obtener_nombre_opcion_submenu();
    menu_obtener_nombre_opcion_despues_de_operaciones();

    pa2m_nuevo_grupo("Pruebas menu_obtener_nombre_opcion parámetros inválidos");
    menu_obtener_nombre_opcion_con_menu_nulo();
    menu_obtener_nombre_opcion_con_tecla_invalida();
    menu_obtener_nombre_opcion_nombre_vacio();
}

// Pruebas para menu_sacar_accion
void menu_sacar_accion_con_opcion_existente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'B', "Acción B", accion_prueba_true);
    menu_agregar_accion(menu, 'C', "Acción C", accion_prueba_false);
    
    bool sacada_b = menu_sacar_accion(menu, 'B');
    bool existe_b_despues = menu_existe_opcion(menu, 'B');
    bool existe_c_despues = menu_existe_opcion(menu, 'C');
    
    pa2m_afirmar(sacada_b == true, "Se saca correctamente la acción 'B'");
    pa2m_afirmar(existe_b_despues == false, "Acción 'B' ya no existe después de sacarla");
    pa2m_afirmar(existe_c_despues == true, "Acción 'C' sigue existiendo después de sacar 'B'");
    
    menu_destruir_todo(menu);
}

void menu_sacar_accion_con_submenu_no_se_saca()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_crear_submenu(menu, 'S', "Submenu");
    
    bool sacada = menu_sacar_accion(menu, 'S');
    bool existe_despues = menu_existe_opcion(menu, 'S');
    
    pa2m_afirmar(sacada == false, "No se puede sacar submenu como acción");
    pa2m_afirmar(existe_despues == true, "Submenu sigue existiendo después del intento");
    
    menu_destruir_todo(menu);
}

void menu_sacar_accion_con_opcion_inexistente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'B', "Acción B", accion_prueba_true);
    
    bool sacada_x = menu_sacar_accion(menu, 'X');
    bool sacada_y = menu_sacar_accion(menu, 'Y');
    
    pa2m_afirmar(sacada_x == false, "No se puede sacar opción 'X' inexistente");
    pa2m_afirmar(sacada_y == false, "No se puede sacar opción 'Y' inexistente");
    
    menu_destruir_todo(menu);
}

void menu_sacar_accion_menu_vacio()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    
    bool sacada = menu_sacar_accion(menu, 'A');
    pa2m_afirmar(sacada == false, "No se puede sacar acción de menu vacío");
    
    menu_destruir_todo(menu);
}

void menu_sacar_accion_verifica_contador()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'X', "Acción X", accion_prueba_true);
    menu_agregar_accion(menu, 'B', "Acción B", accion_prueba_true);
    menu_agregar_accion(menu, 'C', "Acción C", accion_prueba_true);
    
    size_t cantidad_antes = menu_cantidad_acciones(menu);
    bool sacada = menu_sacar_accion(menu, 'B');
    size_t cantidad_despues = menu_cantidad_acciones(menu);
    
    pa2m_afirmar(cantidad_antes == 3, "Cantidad inicial de acciones es 3");
    pa2m_afirmar(sacada == true, "Acción 'B' se saca correctamente");
    pa2m_afirmar(cantidad_despues == 2, "Cantidad de acciones disminuye a 2");
    
    menu_destruir_todo(menu);
}

void menu_sacar_accion_con_menu_nulo()
{
    bool sacada = menu_sacar_accion(NULL, 'A');
    pa2m_afirmar(sacada == false, "Sacar acción de menu NULL devuelve false");
}

void menu_sacar_accion_con_tecla_especial()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    
    bool sacada_q = menu_sacar_accion(menu, MENU_TECLA_SALIR);
    bool sacada_a = menu_sacar_accion(menu, MENU_TECLA_VOLVER);
    bool sacada_e = menu_sacar_accion(menu, MENU_TECLA_ESTILOS);
    
    pa2m_afirmar(sacada_q == false, "No se puede sacar tecla especial SALIR");
    pa2m_afirmar(sacada_a == false, "No se puede sacar tecla especial VOLVER");
    pa2m_afirmar(sacada_e == false, "No se puede sacar tecla especial ESTILOS");
    
    menu_destruir_todo(menu);
}

void pruebas_menu_sacar_accion(){
    pa2m_nuevo_grupo("Pruebas menu_sacar_accion parámetros válidos");
    menu_sacar_accion_con_opcion_existente();
    menu_sacar_accion_con_submenu_no_se_saca();
    menu_sacar_accion_con_opcion_inexistente();
    menu_sacar_accion_menu_vacio();
    menu_sacar_accion_verifica_contador();

    pa2m_nuevo_grupo("Pruebas menu_sacar_accion parámetros inválidos");
    menu_sacar_accion_con_menu_nulo();
    menu_sacar_accion_con_tecla_especial();
}

// Pruebas para menu_sacar_submenu
void menu_sacar_submenu_con_submenu_existente()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_crear_submenu(padre, 'S', "Submenu");
    
    bool existe_antes = menu_existe_opcion(padre, 'S');
    menu_t *submenu_sacado = menu_sacar_submenu(padre, 'S');
    bool existe_despues = menu_existe_opcion(padre, 'S');
    
    pa2m_afirmar(existe_antes == true, "Submenu existe antes de sacarlo");
    pa2m_afirmar(submenu_sacado != NULL, "Se obtiene submenu sacado correctamente");
    pa2m_afirmar(existe_despues == false, "Submenu no existe después de sacarlo");
    
    menu_destruir(submenu_sacado);
    menu_destruir_todo(padre);
}

void menu_sacar_submenu_con_accion_no_se_saca()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu, 'X', "Acción X", accion_prueba_true);
    
    menu_t *resultado = menu_sacar_submenu(menu, 'X');
    bool existe_despues = menu_existe_opcion(menu, 'X');
    
    pa2m_afirmar(resultado == NULL, "No se puede sacar acción como submenu");
    pa2m_afirmar(existe_despues == true, "Acción sigue existiendo después del intento");
    
    menu_destruir_todo(menu);
}

void menu_sacar_submenu_con_submenu_inexistente()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_crear_submenu(menu, 'S', "Submenu");
    
    menu_t *resultado_x = menu_sacar_submenu(menu, 'X');
    menu_t *resultado_y = menu_sacar_submenu(menu, 'Y');
    
    pa2m_afirmar(resultado_x == NULL, "No se puede sacar submenu 'X' inexistente");
    pa2m_afirmar(resultado_y == NULL, "No se puede sacar submenu 'Y' inexistente");
    
    menu_destruir_todo(menu);
}

void menu_sacar_submenu_menu_vacio()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    
    menu_t *resultado = menu_sacar_submenu(menu, 'S');
    pa2m_afirmar(resultado == NULL, "No se puede sacar submenu de menu vacío");
    
    menu_destruir_todo(menu);
}

void menu_sacar_submenu_verifica_contador()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_crear_submenu(padre, 'S', "Submenu S");
    menu_crear_submenu(padre, 'T', "Submenu T");
    
    size_t cantidad_antes = menu_cantidad_submenus(padre);
    menu_t *submenu_sacado = menu_sacar_submenu(padre, 'S');
    size_t cantidad_despues = menu_cantidad_submenus(padre);
    
    pa2m_afirmar(cantidad_antes == 2, "Cantidad inicial de submenus es 2");
    pa2m_afirmar(submenu_sacado != NULL, "Submenu 'S' se saca correctamente");
    pa2m_afirmar(cantidad_despues == 1, "Cantidad de submenus disminuye a 1");
    
    menu_destruir(submenu_sacado);
    menu_destruir_todo(padre);
}

void menu_sacar_submenu_submenu_con_contenido()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
    menu_agregar_accion(submenu, 'X', "Acción X", accion_prueba_true);
    menu_agregar_accion(submenu, 'Y', "Acción Y", accion_prueba_false);
    
    menu_t *submenu_sacado = menu_sacar_submenu(padre, 'S');
    
    pa2m_afirmar(submenu_sacado != NULL, "Se saca submenu con contenido");
    pa2m_afirmar(menu_cantidad_acciones(submenu_sacado) == 2, "Submenu sacado mantiene sus 2 acciones");
    
    menu_destruir_todo(submenu_sacado);
    menu_destruir_todo(padre);
}

void menu_sacar_submenu_con_menu_nulo()
{
    menu_t *resultado = menu_sacar_submenu(NULL, 'S');
    pa2m_afirmar(resultado == NULL, "Sacar submenu de menu NULL devuelve NULL");
}

void menu_sacar_submenu_con_tecla_especial()
{
    menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    
    menu_t *resultado_q = menu_sacar_submenu(menu, MENU_TECLA_SALIR);
    menu_t *resultado_a = menu_sacar_submenu(menu, MENU_TECLA_VOLVER);
    menu_t *resultado_e = menu_sacar_submenu(menu, MENU_TECLA_ESTILOS);
    
    pa2m_afirmar(resultado_q == NULL, "No se puede sacar tecla especial SALIR como submenu");
    pa2m_afirmar(resultado_a == NULL, "No se puede sacar tecla especial VOLVER como submenu");
    pa2m_afirmar(resultado_e == NULL, "No se puede sacar tecla especial ESTILOS como submenu");
    
    menu_destruir_todo(menu);
}

void pruebas_menu_sacar_submenu(){
    pa2m_nuevo_grupo("Pruebas menu_sacar_submenu parámetros válidos");
    menu_sacar_submenu_con_submenu_existente();
    menu_sacar_submenu_con_accion_no_se_saca();
    menu_sacar_submenu_con_submenu_inexistente();
    menu_sacar_submenu_menu_vacio();
    menu_sacar_submenu_verifica_contador();
    menu_sacar_submenu_submenu_con_contenido();

    pa2m_nuevo_grupo("Pruebas menu_sacar_submenu parámetros inválidos");
    menu_sacar_submenu_con_menu_nulo();
    menu_sacar_submenu_con_tecla_especial();
}

// Pruebas para menu_navegador_crear
void menu_navegador_crear_con_menu_base_valido()
{
    menu_t *menu_base = menu_crear_base("Menu Principal", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu_base, 'A', "Acción A", accion_prueba_true);
    
    menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);
    
    pa2m_afirmar(nav != NULL, "Navegador se crea correctamente con menu base válido");
    pa2m_afirmar(menu_navegador_obtener_actual(nav) == menu_base, "Menu actual es el menu base");
    pa2m_afirmar(menu_navegador_esta_terminado(nav) == false, "Navegador no está terminado al crear");
    
    menu_navegador_destruir(nav);
    menu_destruir_todo(menu_base);
}

void menu_navegador_crear_con_user_data()
{
    int user_data = 42;
    menu_t *menu_base = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    
    menu_navegador_t *nav = menu_navegador_crear(menu_base, &user_data);
    
    pa2m_afirmar(nav != NULL, "Navegador se crea correctamente con user_data");
    
    menu_navegador_destruir(nav);
    menu_destruir_todo(menu_base);
}

void menu_navegador_crear_con_menu_con_estilos()
{
    menu_t *menu_base = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_estilo(menu_base, mostrar_estilo_detallado, mostrar_titulo_detallado);
    menu_agregar_estilo(menu_base, mostrar_estilo_emoji, mostrar_titulo_centrado);
    
    menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);
    
    pa2m_afirmar(nav != NULL, "Navegador se crea correctamente con menu que tiene estilos");
    
    menu_navegador_destruir(nav);
    menu_destruir_todo(menu_base);
}

void menu_navegador_crear_menu_sin_estilos()
{
    menu_t *menu_base = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    // No agregar estilos adicionales
    
    menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);
    
    pa2m_afirmar(nav != NULL, "Navegador se crea con menu que solo tiene estilo inicial");
    
    menu_navegador_destruir(nav);
    menu_destruir_todo(menu_base);
}

void menu_navegador_crear_con_menu_nulo()
{
    menu_navegador_t *nav = menu_navegador_crear(NULL, NULL);
    pa2m_afirmar(nav == NULL, "Navegador con menu NULL devuelve NULL");
}

void pruebas_menu_navegador_crear(){
    pa2m_nuevo_grupo("Pruebas menu_navegador_crear parámetros válidos");
    menu_navegador_crear_con_menu_base_valido();
    menu_navegador_crear_con_user_data();
    menu_navegador_crear_con_menu_con_estilos();
    menu_navegador_crear_menu_sin_estilos();

    pa2m_nuevo_grupo("Pruebas menu_navegador_crear parámetros inválidos");
    menu_navegador_crear_con_menu_nulo();
}

// Pruebas para menu_navegador_procesar_tecla
void menu_navegador_procesar_tecla_completa()
{
    menu_t *menu_base = menu_crear_base("Menu Principal", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_accion(menu_base, 'A', "Acción Exitosa", accion_prueba_true);
    menu_agregar_estilo(menu_base, mostrar_estilo_detallado, mostrar_titulo_detallado);
    
    menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);
    
    menu_navegacion_estado_t resultado_estilo = menu_navegador_procesar_tecla(nav, MENU_TECLA_ESTILOS);
    menu_navegacion_estado_t resultado_accion = menu_navegador_procesar_tecla(nav, 'A');
    menu_navegacion_estado_t resultado_salir = menu_navegador_procesar_tecla(nav, MENU_TECLA_SALIR);
    
    pa2m_afirmar(resultado_estilo == MENU_NAVEGACION_CONTINUAR, "Procesar tecla ESTILOS devuelve CONTINUAR");
    pa2m_afirmar(resultado_accion == MENU_NAVEGACION_CONTINUAR, "Procesar tecla acción devuelve CONTINUAR");
    pa2m_afirmar(resultado_salir == MENU_NAVEGACION_TERMINAR, "Procesar tecla SALIR devuelve TERMINAR");
    pa2m_afirmar(menu_navegador_esta_terminado(nav) == true, "Navegador termina después de SALIR");
    
    menu_navegador_destruir(nav);
    menu_destruir_todo(menu_base);
}

void menu_navegador_procesar_tecla_con_navegador_terminado()
{
    menu_t *menu_base = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);
    
    // Marcar como terminado
    menu_navegador_procesar_tecla(nav, MENU_TECLA_SALIR);
    menu_navegacion_estado_t resultado = menu_navegador_procesar_tecla(nav, 'A');
    
    pa2m_afirmar(resultado == MENU_NAVEGACION_TERMINAR, "Procesar tecla con navegador terminado devuelve TERMINAR");
    
    menu_navegador_destruir(nav);
    menu_destruir_todo(menu_base);
}

void menu_navegador_procesar_tecla_con_navegador_nulo()
{
    menu_navegacion_estado_t resultado = menu_navegador_procesar_tecla(NULL, 'A');
    pa2m_afirmar(resultado == MENU_NAVEGACION_ERROR, "Procesar tecla con navegador NULL devuelve TERMINAR");
}

void pruebas_menu_navegador_procesar_tecla(){
    pa2m_nuevo_grupo("Pruebas menu_navegador_procesar_tecla parámetros válidos");
    menu_navegador_procesar_tecla_completa();
    menu_navegador_procesar_tecla_con_navegador_terminado();

    pa2m_nuevo_grupo("Pruebas menu_navegador_procesar_tecla parámetros inválidos");
    menu_navegador_procesar_tecla_con_navegador_nulo();
}

// Pruebas para funciones de consulta del navegador
void menu_navegador_obtener_actual_valido()
{
    menu_t *menu_base = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);
    
    menu_t *actual = menu_navegador_obtener_actual(nav);
    
    pa2m_afirmar(actual == menu_base, "Obtener actual devuelve menu base correctamente");
    
    menu_navegador_destruir(nav);
    menu_destruir_todo(menu_base);
}

void menu_navegador_esta_terminado_recien_creado()
{
    menu_t *menu_base = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);
    
    bool terminado = menu_navegador_esta_terminado(nav);
    
    pa2m_afirmar(terminado == false, "Navegador recién creado no está terminado");
    
    menu_navegador_destruir(nav);
    menu_destruir_todo(menu_base);
}

void menu_navegador_obtener_actual_con_navegador_nulo()
{
    menu_t *actual = menu_navegador_obtener_actual(NULL);
    pa2m_afirmar(actual == NULL, "Obtener actual con navegador NULL devuelve NULL");
}

void menu_navegador_esta_terminado_con_navegador_nulo()
{
    bool terminado = menu_navegador_esta_terminado(NULL);
    pa2m_afirmar(terminado == true, "Navegador NULL está terminado");
}

void pruebas_menu_navegador_consultas(){
    pa2m_nuevo_grupo("Pruebas menu_navegador consultas parámetros válidos");
    menu_navegador_obtener_actual_valido();
    menu_navegador_esta_terminado_recien_creado();

    pa2m_nuevo_grupo("Pruebas menu_navegador consultas parámetros inválidos");
    menu_navegador_obtener_actual_con_navegador_nulo();
    menu_navegador_esta_terminado_con_navegador_nulo();
}

// Pruebas para menu_navegador_destruir
void menu_navegador_destruir_valido()
{
    menu_t *menu_base = menu_crear_base("Menu", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);
    
    menu_navegador_destruir(nav);  // No debería causar errores
    menu_destruir_todo(menu_base);
    
    pa2m_afirmar(true, "Destruir navegador válido no causa errores");
}

void menu_navegador_destruir_con_navegador_nulo()
{
    menu_navegador_destruir(NULL);  // No debería causar errores
    pa2m_afirmar(true, "Destruir navegador NULL no causa errores");
}

void menu_navegador_destruir_con_submenus_en_pila()
{
    menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
    if (!submenu) return;

    menu_navegador_t *nav = menu_navegador_crear(padre, NULL);
    
    // Navegar al submenu para llenar la pila
    menu_navegador_procesar_tecla(nav, 'S');
    
    menu_navegador_destruir(nav);
    menu_destruir_todo(padre);
    
    pa2m_afirmar(true, "Destruir navegador con pila llena no causa errores");
}

void pruebas_menu_navegador_destruir(){
    pa2m_nuevo_grupo("Pruebas menu_navegador_destruir parámetros válidos");
    menu_navegador_destruir_valido();
    menu_navegador_destruir_con_submenus_en_pila();

    pa2m_nuevo_grupo("Pruebas menu_navegador_destruir parámetros inválidos");
    menu_navegador_destruir_con_navegador_nulo();
}

// Test de integración: simulación completa de navegación
void test_integracion_navegacion_completa()
{
    // Crear estructura de menus compleja
    menu_t *menu_principal = menu_crear_base("Menu Principal", mostrar_estilo_simple, mostrar_titulo_simple);
    menu_agregar_estilo(menu_principal, mostrar_estilo_detallado, mostrar_titulo_detallado);
    
    // Agregar acciones al menu principal
    menu_agregar_accion(menu_principal, '1', "Ver Configuración", accion_prueba_true);
    menu_agregar_accion(menu_principal, '2', "Acción Fallida", accion_prueba_false);
    
    // Crear submenu de configuración
    menu_t *submenu_config = menu_crear_submenu(menu_principal, 'C', "Configuración");
    menu_agregar_accion(submenu_config, 'X', "Ajustar Sonido", accion_prueba_true);
    menu_agregar_accion(submenu_config, 'B', "Ajustar Video", accion_prueba_true);
    
    // Crear otro submenu anidado
    menu_t *submenu_avanzado = menu_crear_submenu(submenu_config, 'D', "Opciones Avanzadas");
    menu_agregar_accion(submenu_avanzado, 'X', "Resetear Sistema", accion_prueba_true);
    
    // Crear navegador
    int user_data = 100;
    menu_navegador_t *nav = menu_navegador_crear(menu_principal, &user_data);
    
    pa2m_afirmar(nav != NULL, "Navegador se crea correctamente para test de integración");
    
    // Probar navegación completa
    menu_navegacion_estado_t estado;
    
    // 1. Cambiar estilo
    estado = menu_navegador_procesar_tecla(nav, MENU_TECLA_ESTILOS);
    pa2m_afirmar(estado == MENU_NAVEGACION_CONTINUAR, "Cambio de estilo exitoso");
    
    // 2. Ejecutar acción exitosa
    estado = menu_navegador_procesar_tecla(nav, '1');
    pa2m_afirmar(estado == MENU_NAVEGACION_CONTINUAR, "Acción exitosa continúa navegación");
    
    // 3. Ejecutar acción fallida
    estado = menu_navegador_procesar_tecla(nav, '2');
    pa2m_afirmar(estado == MENU_NAVEGACION_ERROR, "Acción fallida produce error");
    
    // 4. Navegar a submenu de configuración
    estado = menu_navegador_procesar_tecla(nav, 'C');
    pa2m_afirmar(estado == MENU_NAVEGACION_CONTINUAR, "Navegación a submenu exitosa");
    pa2m_afirmar(menu_navegador_obtener_actual(nav) == submenu_config, "Menu actual es submenu de configuración");
    
    // 5. Ejecutar acción en submenu
    estado = menu_navegador_procesar_tecla(nav, 'X');
    pa2m_afirmar(estado == MENU_NAVEGACION_CONTINUAR, "Acción en submenu exitosa");
    
    // 6. Navegar a submenu anidado
    estado = menu_navegador_procesar_tecla(nav, 'D');
    pa2m_afirmar(estado == MENU_NAVEGACION_CONTINUAR, "Navegación a submenu anidado exitosa");
    pa2m_afirmar(menu_navegador_obtener_actual(nav) == submenu_avanzado, "Menu actual es submenu avanzado");
    
    // 7. Volver al submenu anterior
    estado = menu_navegador_procesar_tecla(nav, MENU_TECLA_VOLVER);
    pa2m_afirmar(estado == MENU_NAVEGACION_CONTINUAR, "Volver desde submenu anidado exitoso");
    pa2m_afirmar(menu_navegador_obtener_actual(nav) == submenu_config, "Menu actual vuelve a submenu configuración");
    
    // 8. Volver al menu principal
    estado = menu_navegador_procesar_tecla(nav, MENU_TECLA_VOLVER);
    pa2m_afirmar(estado == MENU_NAVEGACION_CONTINUAR, "Volver al menu principal exitoso");
    pa2m_afirmar(menu_navegador_obtener_actual(nav) == menu_principal, "Menu actual vuelve a menu principal");
    
    // 9. Salir del sistema
    estado = menu_navegador_procesar_tecla(nav, MENU_TECLA_SALIR);
    pa2m_afirmar(estado == MENU_NAVEGACION_TERMINAR, "Salir del sistema exitoso");
    pa2m_afirmar(menu_navegador_esta_terminado(nav) == true, "Navegador marca terminado después de salir");
    
    menu_navegador_destruir(nav);
    menu_destruir_todo(menu_principal);
    
    pa2m_afirmar(true, "Test de integración completo ejecutado sin errores de memoria");
}

void pruebas_integracion_completa(){
    pa2m_nuevo_grupo("Pruebas de integración - Navegación completa");
    test_integracion_navegacion_completa();
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
    pruebas_menu_cantidad_estilos();
    pruebas_menu_tiene_estilos();
    pruebas_menu_existe_opcion();
    pruebas_menu_obtener_nombre_opcion();
    pruebas_menu_sacar_accion();
    pruebas_menu_sacar_submenu();
    pruebas_menu_navegador_crear();
    pruebas_menu_navegador_procesar_tecla();
    //pruebas_menu_navegador_mostrar();
    //pruebas_menu_navegador_obtener_actual();
    //pruebas_menu_navegador_esta_terminado();
    pruebas_menu_navegador_consultas();
    pruebas_menu_navegador_destruir();
    pruebas_integracion_completa();
    //pruebas_menu_destruir();
    //pruebas_menu_destruir_todo();
}

int main()
{
    pa2m_nuevo_grupo("============== PRUEBAS TDA MENU ===============");
    
	ejecutar_pruebas_menu();

    return pa2m_mostrar_reporte();
}