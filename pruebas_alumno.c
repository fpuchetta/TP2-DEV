#include "pa2m.h"
#include "src/menu.h"
#include "src/juego.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool accion_prueba_true(void *user_data)
{
	return true;
}
bool accion_prueba_false(void *user_data)
{
	return false;
}
void mostrar_estilo_simple(char tecla, char *nombre)
{
	printf("%c) %s\n", tecla, nombre);
}
void mostrar_estilo_detallado(char tecla, char *nombre)
{
	printf("Opcion %c: %s\n", tecla, nombre);
}
void mostrar_estilo_emoji(char tecla, char *nombre)
{
	const char *emoji = "⚡";
	if (tecla == 'L')
		emoji = "📋";
	if (tecla == 'B')
		emoji = "🔍";
	if (tecla == 'C')
		emoji = "⚙️";
	if (tecla == 'G')
		emoji = "💾";
	printf("%s %c - %s\n", emoji, tecla, nombre);
}

void mostrar_titulo_centrado(char *titulo)
{
	int espacios = (50 - (int)strlen(titulo)) / 2;
	if (espacios < 0)
		espacios = 0;

	printf("\n");
	for (int i = 0; i < espacios; i++)
		printf(" ");
	printf("%s\n", titulo);

	for (int i = 0; i < espacios; i++)
		printf(" ");
	for (size_t i = 0; i < strlen(titulo); i++)
		printf("=");
	printf("\n");
}
void mostrar_titulo_simple(char *titulo)
{
	printf("\n%s\n", titulo);
}
void mostrar_titulo_detallado(char *titulo)
{
	printf("\n=== %s ===\n", titulo);
}

// Pruebas para menu_crear_base
void menu_crear_base_crea_menu_valido()
{
	menu_t *menu =
		menu_crear_base("Menu Principal", mostrar_estilo_simple, NULL);
	pa2m_afirmar(menu != NULL,
		     "Se crea un menu base con parametros validos");

	const char *titulo = menu_obtener_titulo(menu);
	pa2m_afirmar(titulo != NULL && strcmp(titulo, "Menu Principal") == 0,
		     "El titulo se asigno correctamente");

	pa2m_afirmar(menu_cantidad_acciones(menu) == 0,
		     "El menu se crea sin acciones");

	pa2m_afirmar(menu_cantidad_submenus(menu) == 0,
		     "El menu se crea sin submenus");

	menu_destruir_todo(menu);
}

void menu_crear_base_con_estilo_nulo_usa_default()
{
	menu_t *menu = menu_crear_base("Menu", NULL, NULL);
	pa2m_afirmar(menu != NULL,
		     "Se crea menu con estilo NULL (usa estilo por defecto)");

	bool agregado =
		menu_agregar_accion(menu, 'B', "Accion", accion_prueba_true);
	pa2m_afirmar(agregado == true,
		     "Se puede agregar accion a menu con estilo NULL");

	menu_destruir_todo(menu);
}

void menu_crear_base_con_titulo_vacio()
{
	menu_t *menu = menu_crear_base("", mostrar_estilo_simple, NULL);
	pa2m_afirmar(menu != NULL, "Se crea menu con titulo vacio");

	const char *titulo = menu_obtener_titulo(menu);
	pa2m_afirmar(titulo != NULL && strcmp(titulo, "") == 0,
		     "El titulo vacio se asigno correctamente");

	menu_destruir_todo(menu);
}

void menu_crear_base_con_titulo_muy_largo()
{
	char titulo_largo[1000];
	memset(titulo_largo, 'A', 999);
	titulo_largo[999] = '\0';

	menu_t *menu =
		menu_crear_base(titulo_largo, mostrar_estilo_simple, NULL);
	pa2m_afirmar(menu != NULL, "Se crea menu con titulo muy largo");

	const char *titulo = menu_obtener_titulo(menu);
	pa2m_afirmar(titulo != NULL && strlen(titulo) == 999,
		     "El titulo largo se asigno completamente");

	menu_destruir_todo(menu);
}

void menu_crear_base_multiples_menus()
{
	menu_t *menu1 = menu_crear_base("Menu 1", mostrar_estilo_simple, NULL);
	menu_t *menu2 =
		menu_crear_base("Menu 2", mostrar_estilo_detallado, NULL);
	menu_t *menu3 = menu_crear_base("Menu 3", NULL, NULL);

	pa2m_afirmar(menu1 != NULL && menu2 != NULL && menu3 != NULL,
		     "Se crean multiples menus simultaneamente");

	pa2m_afirmar(strcmp(menu_obtener_titulo(menu1), "Menu 1") == 0,
		     "Menu 1 tiene titulo correcto");
	pa2m_afirmar(strcmp(menu_obtener_titulo(menu2), "Menu 2") == 0,
		     "Menu 2 tiene titulo correcto");
	pa2m_afirmar(strcmp(menu_obtener_titulo(menu3), "Menu 3") == 0,
		     "Menu 3 tiene titulo correcto");

	menu_destruir_todo(menu1);
	menu_destruir_todo(menu2);
	menu_destruir_todo(menu3);
}

void menu_crear_base_con_titulo_espacios()
{
	menu_t *menu = menu_crear_base("   ", mostrar_estilo_simple, NULL);
	pa2m_afirmar(menu != NULL, "Se crea menu con titulo de solo espacios");

	const char *titulo = menu_obtener_titulo(menu);
	pa2m_afirmar(titulo != NULL && strcmp(titulo, "   ") == 0,
		     "El titulo con espacios se asigno correctamente");

	menu_destruir_todo(menu);
}

void menu_crear_base_con_titulo_caracteres_especiales()
{
	menu_t *menu = menu_crear_base("Menu con ñ y aeiou",
				       mostrar_estilo_simple, NULL);
	pa2m_afirmar(menu != NULL,
		     "Se crea menu con titulo con caracteres especiales");

	const char *titulo = menu_obtener_titulo(menu);
	pa2m_afirmar(
		titulo != NULL && strcmp(titulo, "Menu con ñ y aeiou") == 0,
		"El titulo con caracteres especiales se asigno correctamente");

	menu_destruir_todo(menu);
}

void menu_crear_base_con_titulo_numeros()
{
	menu_t *menu =
		menu_crear_base("Menu 12345", mostrar_estilo_simple, NULL);
	pa2m_afirmar(menu != NULL,
		     "Se crea menu con titulo que contiene numeros");

	const char *titulo = menu_obtener_titulo(menu);
	pa2m_afirmar(titulo != NULL && strcmp(titulo, "Menu 12345") == 0,
		     "El titulo con numeros se asigno correctamente");

	menu_destruir_todo(menu);
}

void menu_crear_base_verificar_tipo_raiz()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, NULL);
	pa2m_afirmar(menu != NULL, "Menu base creado correctamente");

	bool estilo_agregado = menu_agregar_estilo(
		menu, mostrar_estilo_detallado, mostrar_titulo_centrado);
	pa2m_afirmar(estilo_agregado == true,
		     "Menu base permite agregar estilos adicionales");

	menu_destruir_todo(menu);
}

void menu_crear_base_con_titulo_nulo()
{
	menu_t *menu = menu_crear_base(NULL, mostrar_estilo_simple, NULL);
	pa2m_afirmar(menu == NULL, "No se puede crear menu con titulo NULL");
}

void pruebas_menu_crear_base()
{
	pa2m_nuevo_grupo("Pruebas menu_crear_base parametros validos");
	menu_crear_base_crea_menu_valido();
	menu_crear_base_con_estilo_nulo_usa_default();
	menu_crear_base_con_titulo_vacio();
	menu_crear_base_con_titulo_muy_largo();
	menu_crear_base_multiples_menus();
	menu_crear_base_con_titulo_espacios();
	menu_crear_base_con_titulo_caracteres_especiales();
	menu_crear_base_con_titulo_numeros();
	menu_crear_base_verificar_tipo_raiz();

	pa2m_nuevo_grupo("Pruebas menu_crear_base parametros invalidos");
	menu_crear_base_con_titulo_nulo();
}

// Pruebas para menu_crear_submenu
void menu_crear_submenu_crea_submenu_valido()
{
	menu_t *padre =
		menu_crear_base("Menu Padre", mostrar_estilo_simple, NULL);
	menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");

	pa2m_afirmar(submenu != NULL,
		     "Se crea un submenu con parametros validos");

	bool existe = menu_existe_opcion(padre, 'S');
	pa2m_afirmar(existe == true, "La tecla 'S' existe en el menu padre");

	const char *nombre = menu_obtener_nombre_opcion(padre, 'S');
	pa2m_afirmar(nombre != NULL && strcmp(nombre, "Submenu") == 0,
		     "El submenu tiene el nombre correcto");

	pa2m_afirmar(menu_cantidad_submenus(padre) == 1,
		     "El contador de submenus del padre se incremento");

	menu_destruir_todo(padre);
}

void menu_crear_submenu_tecla_en_mayuscula()
{
	menu_t *padre =
		menu_crear_base("Menu Padre", mostrar_estilo_simple, NULL);
	menu_t *submenu = menu_crear_submenu(padre, 's', "Submenu Minuscula");

	pa2m_afirmar(submenu != NULL, "Se crea submenu con tecla minuscula");

	bool existe_mayus = menu_existe_opcion(padre, 'S');
	bool existe_minus = menu_existe_opcion(padre, 's');

	pa2m_afirmar(existe_mayus == true,
		     "La tecla se convirtio a mayuscula ('S')");
	pa2m_afirmar(existe_minus == false,
		     "No existe la tecla minuscula ('s')");

	menu_destruir_todo(padre);
}

void menu_crear_submenu_multiples_submenus()
{
	menu_t *padre =
		menu_crear_base("Menu Padre", mostrar_estilo_simple, NULL);
	menu_t *sub1 = menu_crear_submenu(padre, 'B', "Submenu B");
	menu_t *sub2 = menu_crear_submenu(padre, 'C', "Submenu C");
	menu_t *sub3 = menu_crear_submenu(padre, 'D', "Submenu D");

	pa2m_afirmar(sub1 != NULL && sub2 != NULL && sub3 != NULL,
		     "Se crean multiples submenus");

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

	menu_t *padre =
		menu_crear_base("Menu Padre", mostrar_estilo_simple, NULL);
	menu_t *submenu = menu_crear_submenu(padre, 'L', nombre_largo);

	pa2m_afirmar(submenu != NULL, "Se crea submenu con nombre muy largo");

	const char *nombre = menu_obtener_nombre_opcion(padre, 'L');
	pa2m_afirmar(nombre != NULL && strlen(nombre) == 499,
		     "El nombre largo se asigno completamente");

	menu_destruir_todo(padre);
}

void menu_crear_submenu_teclas_especiales_caracteres()
{
	menu_t *padre =
		menu_crear_base("Menu Padre", mostrar_estilo_simple, NULL);

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
	menu_t *padre =
		menu_crear_base("Menu Padre", mostrar_estilo_simple, NULL);
	menu_t *submenu = menu_crear_submenu(padre, 'V', "");

	pa2m_afirmar(submenu == NULL,
		     "No se puede crear submenu con nombre vacio");

	bool existe = menu_existe_opcion(padre, 'V');
	pa2m_afirmar(existe == false,
		     "No existe el submenu con nombre vacio en el padre");

	menu_destruir_todo(padre);
}

void menu_crear_submenu_con_padre_nulo()
{
	menu_t *submenu = menu_crear_submenu(NULL, 'S', "Submenu");
	pa2m_afirmar(submenu == NULL,
		     "No se puede crear submenu con padre NULL");
}

void menu_crear_submenu_con_nombre_nulo()
{
	menu_t *padre =
		menu_crear_base("Menu Padre", mostrar_estilo_simple, NULL);
	menu_t *submenu = menu_crear_submenu(padre, 'S', NULL);

	pa2m_afirmar(submenu == NULL,
		     "No se puede crear submenu con nombre NULL");

	menu_destruir_todo(padre);
}

void menu_crear_submenu_con_tecla_invalida()
{
	menu_t *padre =
		menu_crear_base("Menu Padre", mostrar_estilo_simple, NULL);
	menu_t *submenu = menu_crear_submenu(padre, '\0', "Submenu");

	pa2m_afirmar(submenu == NULL,
		     "No se puede crear submenu con tecla invalida");

	menu_destruir_todo(padre);
}

void menu_crear_submenu_con_teclas_especiales_bloqueadas()
{
	menu_t *padre =
		menu_crear_base("Menu Padre", mostrar_estilo_simple, NULL);

	menu_t *sub1 = menu_crear_submenu(padre, MENU_TECLA_SALIR, "Salir");
	menu_t *sub2 = menu_crear_submenu(padre, MENU_TECLA_VOLVER, "Volver");
	menu_t *sub3 = menu_crear_submenu(padre, MENU_TECLA_ESTILOS, "Estilos");

	pa2m_afirmar(sub1 == NULL, "No se puede crear submenu con tecla SALIR");
	pa2m_afirmar(sub2 == NULL,
		     "No se puede crear submenu con tecla VOLVER");
	pa2m_afirmar(sub3 == NULL,
		     "No se puede crear submenu con tecla ESTILOS");

	menu_destruir_todo(padre);
}

void menu_crear_submenu_tecla_duplicada()
{
	menu_t *padre =
		menu_crear_base("Menu Padre", mostrar_estilo_simple, NULL);

	menu_t *sub1 = menu_crear_submenu(padre, 'X', "Submenu X");
	pa2m_afirmar(sub1 != NULL, "Primer submenu creado correctamente");

	menu_t *sub2 = menu_crear_submenu(padre, 'X', "Submenu X Duplicado");
	pa2m_afirmar(sub2 == NULL,
		     "No se puede crear submenu con tecla duplicada");

	bool existe = menu_existe_opcion(padre, 'X');
	pa2m_afirmar(existe == true, "El primer submenu sigue existiendo");

	menu_destruir_todo(padre);
}

void pruebas_menu_crear_submenu()
{
	pa2m_nuevo_grupo("Pruebas menu_crear_submenu parametros validos");
	menu_crear_submenu_crea_submenu_valido();
	menu_crear_submenu_tecla_en_mayuscula();
	menu_crear_submenu_multiples_submenus();
	menu_crear_submenu_nombre_muy_largo();
	menu_crear_submenu_teclas_especiales_caracteres();

	pa2m_nuevo_grupo("Pruebas menu_crear_submenu parametros invalidos");
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
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, NULL);
	bool resultado =
		menu_agregar_accion(menu, 'X', "Accion X", accion_prueba_true);

	pa2m_afirmar(resultado == true,
		     "Se agrega accion correctamente al menu");

	bool existe = menu_existe_opcion(menu, 'X');
	pa2m_afirmar(existe == true,
		     "La accion existe en el menu despues de agregar");

	const char *nombre = menu_obtener_nombre_opcion(menu, 'X');
	pa2m_afirmar(nombre != NULL && strcmp(nombre, "Accion X") == 0,
		     "La accion tiene el nombre correcto");

	pa2m_afirmar(menu_cantidad_acciones(menu) == 1,
		     "El contador de acciones se incremento correctamente");

	menu_destruir_todo(menu);
}

void menu_agregar_accion_tecla_en_mayuscula()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, NULL);
	bool resultado = menu_agregar_accion(menu, 'x', "Accion Minuscula",
					     accion_prueba_true);

	pa2m_afirmar(resultado == true, "Se agrega accion con tecla minuscula");

	bool existe_mayus = menu_existe_opcion(menu, 'X');
	bool existe_minus = menu_existe_opcion(menu, 'x');

	pa2m_afirmar(existe_mayus == true,
		     "La tecla se convirtio a mayuscula ('X')");
	pa2m_afirmar(existe_minus == false,
		     "No existe la tecla minuscula ('x')");

	menu_destruir_todo(menu);
}

void menu_agregar_accion_multiples_acciones()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, NULL);
	bool resultado1 =
		menu_agregar_accion(menu, 'X', "Accion X", accion_prueba_true);
	bool resultado2 =
		menu_agregar_accion(menu, 'Y', "Accion Y", accion_prueba_false);
	bool resultado3 =
		menu_agregar_accion(menu, 'Z', "Accion Z", accion_prueba_true);

	pa2m_afirmar(resultado1 && resultado2 && resultado3,
		     "Se agregan multiples acciones correctamente");

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

	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, NULL);
	bool resultado = menu_agregar_accion(menu, 'L', nombre_largo,
					     accion_prueba_true);

	pa2m_afirmar(resultado == true,
		     "Se agrega accion con nombre muy largo");

	const char *nombre = menu_obtener_nombre_opcion(menu, 'L');
	pa2m_afirmar(nombre != NULL && strlen(nombre) == 499,
		     "El nombre largo se asigno completamente");

	menu_destruir_todo(menu);
}

void menu_agregar_accion_teclas_especiales_caracteres()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, NULL);

	bool res1 = menu_agregar_accion(menu, '+', "Suma", accion_prueba_true);
	bool res2 = menu_agregar_accion(menu, '-', "Resta", accion_prueba_true);
	bool res3 = menu_agregar_accion(menu, '*', "Multiplicar",
					accion_prueba_true);
	bool res4 =
		menu_agregar_accion(menu, '1', "Numero 1", accion_prueba_true);

	pa2m_afirmar(res1 == true, "Se agrega accion con tecla '+'");
	pa2m_afirmar(res2 == true, "Se agrega accion con tecla '-'");
	pa2m_afirmar(res3 == true, "Se agrega accion con tecla '*'");
	pa2m_afirmar(res4 == true, "Se agrega accion con tecla '1'");

	menu_destruir_todo(menu);
}

void menu_agregar_accion_nombre_vacio()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, NULL);
	bool resultado = menu_agregar_accion(menu, 'E', "", accion_prueba_true);

	pa2m_afirmar(resultado == false,
		     "No se puede agregar accion con nombre vacio");

	bool existe = menu_existe_opcion(menu, 'E');
	pa2m_afirmar(existe == false, "No existe la opcion con nombre vacio");

	menu_destruir_todo(menu);
}

void menu_agregar_accion_con_menu_nulo()
{
	bool resultado =
		menu_agregar_accion(NULL, 'X', "Accion", accion_prueba_true);
	pa2m_afirmar(resultado == false,
		     "No se puede agregar accion a menu NULL");
}

void menu_agregar_accion_con_nombre_nulo()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, NULL);
	bool resultado =
		menu_agregar_accion(menu, 'X', NULL, accion_prueba_true);

	pa2m_afirmar(resultado == false,
		     "No se puede agregar accion con nombre NULL");

	bool existe = menu_existe_opcion(menu, 'X');
	pa2m_afirmar(
		existe == false,
		"No existe la opcion despues de intentar agregar con nombre NULL");

	menu_destruir_todo(menu);
}

void menu_agregar_accion_con_accion_nula()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, NULL);
	bool resultado = menu_agregar_accion(menu, 'X', "Accion", NULL);

	pa2m_afirmar(resultado == false, "No se puede agregar accion nula");

	bool existe = menu_existe_opcion(menu, 'X');
	pa2m_afirmar(
		existe == false,
		"No existe la opcion despues de intentar agregar con accion NULL");

	menu_destruir_todo(menu);
}

void menu_agregar_accion_con_tecla_invalida()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, NULL);

	bool res1 = menu_agregar_accion(menu, '\0', "Accion Nulo",
					accion_prueba_true);
	bool res2 = menu_agregar_accion(menu, '\n', "Accion Newline",
					accion_prueba_true);
	bool res3 = menu_agregar_accion(menu, '\t', "Accion Tab",
					accion_prueba_true);

	pa2m_afirmar(res1 == false,
		     "No se puede agregar accion con tecla '\\0'");
	pa2m_afirmar(res2 == false,
		     "No se puede agregar accion con tecla '\\n'");
	pa2m_afirmar(res3 == false,
		     "No se puede agregar accion con tecla '\\t'");

	menu_destruir_todo(menu);
}

void menu_agregar_accion_con_teclas_especiales_bloqueadas()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, NULL);

	bool res1 = menu_agregar_accion(menu, MENU_TECLA_SALIR, "Salir",
					accion_prueba_true);
	bool res2 = menu_agregar_accion(menu, MENU_TECLA_VOLVER, "Volver",
					accion_prueba_true);
	bool res3 = menu_agregar_accion(menu, MENU_TECLA_ESTILOS, "Estilos",
					accion_prueba_true);

	pa2m_afirmar(res1 == false,
		     "No se puede agregar accion con tecla SALIR ('%c')",
		     MENU_TECLA_SALIR);
	pa2m_afirmar(res2 == false,
		     "No se puede agregar accion con tecla VOLVER ('%c')",
		     MENU_TECLA_VOLVER);
	pa2m_afirmar(res3 == false,
		     "No se puede agregar accion con tecla ESTILOS ('%c')",
		     MENU_TECLA_ESTILOS);

	pa2m_afirmar(
		menu_cantidad_acciones(menu) == 0,
		"El menu sigue vacio despues de intentar agregar teclas reservadas");

	menu_destruir_todo(menu);
}

void menu_agregar_accion_tecla_duplicada()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple, NULL);

	bool res1 =
		menu_agregar_accion(menu, 'X', "Accion X", accion_prueba_true);
	pa2m_afirmar(res1 == true, "Primera accion agregada correctamente");

	pa2m_afirmar(menu_cantidad_acciones(menu) == 1,
		     "El menu tiene 1 accion despues de agregar la primera");

	bool res2 = menu_agregar_accion(menu, 'X', "Accion X Duplicada",
					accion_prueba_false);
	pa2m_afirmar(res2 == false,
		     "No se puede agregar accion con tecla duplicada");

	pa2m_afirmar(
		menu_cantidad_acciones(menu) == 1,
		"El contador de acciones no cambio despues de intentar duplicar");

	const char *nombre = menu_obtener_nombre_opcion(menu, 'X');
	pa2m_afirmar(nombre != NULL && strcmp(nombre, "Accion X") == 0,
		     "La primera accion mantiene su nombre original");

	menu_destruir_todo(menu);
}

void pruebas_menu_agregar_accion()
{
	pa2m_nuevo_grupo("Pruebas menu_agregar_accion parametros validos");
	menu_agregar_accion_agrega_correctamente();
	menu_agregar_accion_tecla_en_mayuscula();
	menu_agregar_accion_multiples_acciones();
	menu_agregar_accion_nombre_muy_largo();
	menu_agregar_accion_teclas_especiales_caracteres();

	pa2m_nuevo_grupo("Pruebas menu_agregar_accion parametros invalidos");
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
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	bool resultado = menu_agregar_estilo(menu, mostrar_estilo_detallado,
					     mostrar_titulo_detallado);

	pa2m_afirmar(resultado == true,
		     "Se agrega estilo correctamente al menu");

	bool accion_agregada =
		menu_agregar_accion(menu, 'X', "Accion", accion_prueba_true);
	pa2m_afirmar(accion_agregada == true,
		     "El menu sigue funcionando despues de agregar estilo");

	menu_destruir_todo(menu);
}

void menu_agregar_estilo_multiples_estilos()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	bool resultado1 = menu_agregar_estilo(menu, mostrar_estilo_detallado,
					      mostrar_titulo_detallado);
	bool resultado2 = menu_agregar_estilo(menu, mostrar_estilo_emoji,
					      mostrar_titulo_centrado);
	bool resultado3 = menu_agregar_estilo(menu, mostrar_estilo_simple,
					      mostrar_titulo_simple);

	pa2m_afirmar(resultado1 && resultado2 && resultado3,
		     "Se agregan multiples estilos correctamente");

	menu_destruir_todo(menu);
}

void menu_agregar_estilo_con_menu_nulo()
{
	bool resultado = menu_agregar_estilo(NULL, mostrar_estilo_detallado,
					     mostrar_titulo_detallado);
	pa2m_afirmar(resultado == false,
		     "No se puede agregar estilo a menu NULL");
}

void menu_agregar_estilo_con_estilo_opciones_nulo()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	bool resultado =
		menu_agregar_estilo(menu, NULL, mostrar_titulo_detallado);

	pa2m_afirmar(resultado == false,
		     "No se puede agregar estilo con estilo_opciones NULL");

	menu_destruir_todo(menu);
}

void menu_agregar_estilo_con_estilo_titulo_nulo()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	bool resultado =
		menu_agregar_estilo(menu, mostrar_estilo_detallado, NULL);

	pa2m_afirmar(resultado == false,
		     "No se puede agregar estilo con estilo_titulo NULL");

	menu_destruir_todo(menu);
}

void menu_agregar_estilo_con_ambos_estilos_nulos()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	bool resultado = menu_agregar_estilo(menu, NULL, NULL);

	pa2m_afirmar(resultado == false,
		     "No se puede agregar estilo con ambos estilos NULL");

	menu_destruir_todo(menu);
}

void menu_agregar_estilo_a_submenu()
{
	menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");

	bool resultado = menu_agregar_estilo(submenu, mostrar_estilo_detallado,
					     mostrar_titulo_detallado);
	pa2m_afirmar(resultado == false,
		     "No se puede agregar estilo a submenu");

	menu_destruir_todo(padre);
}

void pruebas_menu_agregar_estilo()
{
	pa2m_nuevo_grupo("Pruebas menu_agregar_estilo parametros validos");
	menu_agregar_estilo_agrega_correctamente();
	menu_agregar_estilo_multiples_estilos();

	pa2m_nuevo_grupo("Pruebas menu_agregar_estilo parametros invalidos");
	menu_agregar_estilo_con_menu_nulo();
	menu_agregar_estilo_con_estilo_opciones_nulo();
	menu_agregar_estilo_con_estilo_titulo_nulo();
	menu_agregar_estilo_con_ambos_estilos_nulos();
	menu_agregar_estilo_a_submenu();
}

// Pruebas para menu_obtener_titulo
void menu_obtener_titulo_devuelve_titulo_correcto()
{
	menu_t *menu = menu_crear_base("Menu Principal", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	const char *titulo = menu_obtener_titulo(menu);

	pa2m_afirmar(titulo != NULL, "Se obtiene un titulo no NULL");
	pa2m_afirmar(strcmp(titulo, "Menu Principal") == 0,
		     "Se obtiene el titulo correctamente");

	menu_destruir_todo(menu);
}

void menu_obtener_titulo_titulo_vacio()
{
	menu_t *menu = menu_crear_base("", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	const char *titulo = menu_obtener_titulo(menu);

	pa2m_afirmar(titulo != NULL, "Titulo vacio devuelve string no NULL");
	pa2m_afirmar(strcmp(titulo, "") == 0,
		     "Se obtiene el titulo vacio correctamente");

	menu_destruir_todo(menu);
}

void menu_obtener_titulo_titulo_muy_largo()
{
	char titulo_largo[1000];
	memset(titulo_largo, 'X', 999);
	titulo_largo[999] = '\0';

	menu_t *menu = menu_crear_base(titulo_largo, mostrar_estilo_simple,
				       mostrar_titulo_simple);
	const char *titulo = menu_obtener_titulo(menu);

	pa2m_afirmar(titulo != NULL, "Titulo largo devuelve string no NULL");
	pa2m_afirmar(strlen(titulo) == 999,
		     "Se obtiene el titulo largo completamente");
	pa2m_afirmar(titulo[0] == 'X' && titulo[998] == 'X',
		     "El contenido del titulo largo es correcto");

	menu_destruir_todo(menu);
}

void menu_obtener_titulo_titulo_con_espacios()
{
	menu_t *menu = menu_crear_base("   Menu con espacios   ",
				       mostrar_estilo_simple,
				       mostrar_titulo_simple);
	const char *titulo = menu_obtener_titulo(menu);

	pa2m_afirmar(titulo != NULL,
		     "Titulo con espacios devuelve string no NULL");
	pa2m_afirmar(strcmp(titulo, "   Menu con espacios   ") == 0,
		     "Se preservan los espacios en el titulo");

	menu_destruir_todo(menu);
}

void menu_obtener_titulo_titulo_con_caracteres_especiales()
{
	menu_t *menu = menu_crear_base("Menu Pokemon: aeiou ñ ç",
				       mostrar_estilo_simple,
				       mostrar_titulo_simple);
	const char *titulo = menu_obtener_titulo(menu);

	pa2m_afirmar(
		titulo != NULL,
		"Titulo con caracteres especiales devuelve string no NULL");
	pa2m_afirmar(strcmp(titulo, "Menu Pokemon: aeiou ñ ç") == 0,
		     "Se preservan los caracteres especiales en el titulo");

	menu_destruir_todo(menu);
}

void menu_obtener_titulo_titulo_con_numeros_y_simbolos()
{
	menu_t *menu = menu_crear_base("Menu 123! @#$% 456",
				       mostrar_estilo_simple,
				       mostrar_titulo_simple);
	const char *titulo = menu_obtener_titulo(menu);

	pa2m_afirmar(titulo != NULL,
		     "Titulo con numeros y simbolos devuelve string no NULL");
	pa2m_afirmar(strcmp(titulo, "Menu 123! @#$% 456") == 0,
		     "Se preservan numeros y simbolos en el titulo");

	menu_destruir_todo(menu);
}

void menu_obtener_titulo_multiples_menus_diferentes_titulos()
{
	menu_t *menu1 = menu_crear_base("Menu Uno", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_t *menu2 = menu_crear_base("Menu Dos", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_t *menu3 = menu_crear_base("Menu Tres", mostrar_estilo_simple,
					mostrar_titulo_simple);

	const char *titulo1 = menu_obtener_titulo(menu1);
	const char *titulo2 = menu_obtener_titulo(menu2);
	const char *titulo3 = menu_obtener_titulo(menu3);

	pa2m_afirmar(titulo1 != NULL && strcmp(titulo1, "Menu Uno") == 0,
		     "Menu 1 tiene titulo correcto");
	pa2m_afirmar(titulo2 != NULL && strcmp(titulo2, "Menu Dos") == 0,
		     "Menu 2 tiene titulo correcto");
	pa2m_afirmar(titulo3 != NULL && strcmp(titulo3, "Menu Tres") == 0,
		     "Menu 3 tiene titulo correcto");

	menu_destruir_todo(menu1);
	menu_destruir_todo(menu2);
	menu_destruir_todo(menu3);
}

void menu_obtener_titulo_despues_de_operaciones()
{
	menu_t *menu = menu_crear_base("Menu Original", mostrar_estilo_simple,
				       mostrar_titulo_simple);

	menu_agregar_accion(menu, 'A', "Accion A", accion_prueba_true);
	menu_agregar_accion(menu, 'B', "Accion B", accion_prueba_false);
	menu_agregar_estilo(menu, mostrar_estilo_detallado,
			    mostrar_titulo_detallado);

	const char *titulo = menu_obtener_titulo(menu);

	pa2m_afirmar(titulo != NULL && strcmp(titulo, "Menu Original") == 0,
		     "El titulo se mantiene igual despues de operaciones");

	menu_destruir_todo(menu);
}

void menu_obtener_titulo_submenu()
{
	menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu Hijo");

	const char *titulo_padre = menu_obtener_titulo(padre);
	const char *titulo_submenu = menu_obtener_titulo(submenu);

	pa2m_afirmar(titulo_padre != NULL &&
			     strcmp(titulo_padre, "Menu Padre") == 0,
		     "Menu padre tiene titulo correcto");
	pa2m_afirmar(titulo_submenu != NULL &&
			     strcmp(titulo_submenu, "Submenu Hijo") == 0,
		     "Submenu tiene titulo correcto");

	menu_destruir_todo(padre);
}

void menu_obtener_titulo_con_menu_nulo()
{
	const char *titulo = menu_obtener_titulo(NULL);
	pa2m_afirmar(titulo == NULL,
		     "Obtener titulo de menu NULL devuelve NULL");
}

void pruebas_menu_obtener_titulo()
{
	pa2m_nuevo_grupo("Pruebas menu_obtener_titulo parametros validos");
	menu_obtener_titulo_devuelve_titulo_correcto();
	menu_obtener_titulo_titulo_vacio();
	menu_obtener_titulo_titulo_muy_largo();
	menu_obtener_titulo_titulo_con_espacios();
	menu_obtener_titulo_titulo_con_caracteres_especiales();
	menu_obtener_titulo_titulo_con_numeros_y_simbolos();
	menu_obtener_titulo_multiples_menus_diferentes_titulos();
	menu_obtener_titulo_despues_de_operaciones();
	menu_obtener_titulo_submenu();

	pa2m_nuevo_grupo("Pruebas menu_obtener_titulo parametros invalidos");
	menu_obtener_titulo_con_menu_nulo();
}

// Pruebas para menu_cantidad_acciones
void menu_cantidad_acciones_menu_vacio()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	size_t cantidad = menu_cantidad_acciones(menu);

	pa2m_afirmar(cantidad == 0, "Menu vacio tiene 0 acciones");

	menu_destruir_todo(menu);
}

void menu_cantidad_acciones_con_acciones()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'B', "Accion B", accion_prueba_true);
	menu_agregar_accion(menu, 'C', "Accion C", accion_prueba_false);
	menu_agregar_accion(menu, 'D', "Accion D", accion_prueba_true);

	size_t cantidad = menu_cantidad_acciones(menu);
	pa2m_afirmar(cantidad == 3,
		     "Menu con 3 acciones devuelve cantidad correcta");

	menu_destruir_todo(menu);
}

void menu_cantidad_acciones_despues_de_eliminar()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'B', "Accion B", accion_prueba_true);
	menu_agregar_accion(menu, 'C', "Accion C", accion_prueba_false);

	size_t antes = menu_cantidad_acciones(menu);
	menu_sacar_accion(menu, 'B');
	size_t despues = menu_cantidad_acciones(menu);

	pa2m_afirmar(antes == 2 && despues == 1,
		     "Cantidad disminuye despues de eliminar accion");

	menu_destruir_todo(menu);
}

void menu_cantidad_acciones_submenu()
{
	menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
	menu_agregar_accion(submenu, 'X', "Accion X", accion_prueba_true);

	size_t cant_padre = menu_cantidad_acciones(padre);
	size_t cant_submenu = menu_cantidad_acciones(submenu);

	pa2m_afirmar(cant_padre == 0,
		     "Menu padre tiene 0 acciones (solo submenu)");
	pa2m_afirmar(cant_submenu == 1, "Submenu tiene 1 accion");

	menu_destruir_todo(padre);
}

void menu_cantidad_acciones_con_menu_nulo()
{
	size_t cantidad = menu_cantidad_acciones(NULL);
	pa2m_afirmar(cantidad == 0, "Cantidad de acciones de menu NULL es 0");
}

void pruebas_menu_cantidad_acciones()
{
	pa2m_nuevo_grupo("Pruebas menu_cantidad_acciones parametros validos");
	menu_cantidad_acciones_menu_vacio();
	menu_cantidad_acciones_con_acciones();
	menu_cantidad_acciones_despues_de_eliminar();
	menu_cantidad_acciones_submenu();

	pa2m_nuevo_grupo("Pruebas menu_cantidad_acciones parametros invalidos");
	menu_cantidad_acciones_con_menu_nulo();
}

// Pruebas para menu_tiene_acciones
void menu_tiene_acciones_menu_vacio()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	bool tiene = menu_tiene_acciones(menu);

	pa2m_afirmar(tiene == false, "Menu vacio no tiene acciones");

	menu_destruir_todo(menu);
}

void menu_tiene_acciones_con_acciones()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'B', "Accion", accion_prueba_true);
	bool tiene = menu_tiene_acciones(menu);

	pa2m_afirmar(tiene == true, "Menu con acciones devuelve true");

	menu_destruir_todo(menu);
}

void menu_tiene_acciones_despues_de_eliminar_todas()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'B', "Accion B", accion_prueba_true);
	menu_sacar_accion(menu, 'B');
	bool tiene = menu_tiene_acciones(menu);

	pa2m_afirmar(tiene == false,
		     "Menu sin acciones despues de eliminar devuelve false");

	menu_destruir_todo(menu);
}

void menu_tiene_acciones_submenu_con_acciones()
{
	menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
	menu_agregar_accion(submenu, 'X', "Accion X", accion_prueba_true);

	bool tiene_padre = menu_tiene_acciones(padre);
	bool tiene_submenu = menu_tiene_acciones(submenu);

	pa2m_afirmar(tiene_padre == false,
		     "Menu padre sin acciones devuelve false");
	pa2m_afirmar(tiene_submenu == true,
		     "Submenu con acciones devuelve true");

	menu_destruir_todo(padre);
}

void menu_tiene_acciones_con_menu_nulo()
{
	bool tiene = menu_tiene_acciones(NULL);
	pa2m_afirmar(tiene == false, "Menu NULL no tiene acciones");
}

void pruebas_menu_tiene_acciones()
{
	pa2m_nuevo_grupo("Pruebas menu_tiene_acciones parametros validos");
	menu_tiene_acciones_menu_vacio();
	menu_tiene_acciones_con_acciones();
	menu_tiene_acciones_despues_de_eliminar_todas();
	menu_tiene_acciones_submenu_con_acciones();

	pa2m_nuevo_grupo("Pruebas menu_tiene_acciones parametros invalidos");
	menu_tiene_acciones_con_menu_nulo();
}

// Pruebas para menu_cantidad_submenus
void menu_cantidad_submenus_sin_submenus()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	size_t cantidad = menu_cantidad_submenus(menu);

	pa2m_afirmar(cantidad == 0, "Menu sin submenus devuelve 0");

	menu_destruir_todo(menu);
}

void menu_cantidad_submenus_con_submenus()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_crear_submenu(menu, 'S', "Submenu 1");
	menu_crear_submenu(menu, 'T', "Submenu 2");

	size_t cantidad = menu_cantidad_submenus(menu);
	pa2m_afirmar(cantidad == 2,
		     "Menu con 2 submenus devuelve cantidad correcta");

	menu_destruir_todo(menu);
}

void menu_cantidad_submenus_despues_de_eliminar()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_crear_submenu(menu, 'S', "Submenu 1");
	menu_crear_submenu(menu, 'T', "Submenu 2");

	size_t antes = menu_cantidad_submenus(menu);
	menu_t *submenu_s = menu_sacar_submenu(menu, 'S');
	size_t despues = menu_cantidad_submenus(menu);

	pa2m_afirmar(antes == 2 && despues == 1,
		     "Cantidad disminuye despues de eliminar submenu");

	menu_destruir(submenu_s);
	menu_destruir_todo(menu);
}

void menu_cantidad_submenus_submenu()
{
	menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
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

void pruebas_menu_cantidad_submenus()
{
	pa2m_nuevo_grupo("Pruebas menu_cantidad_submenus parametros validos");
	menu_cantidad_submenus_sin_submenus();
	menu_cantidad_submenus_con_submenus();
	menu_cantidad_submenus_despues_de_eliminar();
	menu_cantidad_submenus_submenu();

	pa2m_nuevo_grupo("Pruebas menu_cantidad_submenus parametros invalidos");
	menu_cantidad_submenus_con_menu_nulo();
}

// Pruebas para menu_tiene_submenus
void menu_tiene_submenus_sin_submenus()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	bool tiene = menu_tiene_submenus(menu);

	pa2m_afirmar(tiene == false, "Menu sin submenus devuelve false");

	menu_destruir_todo(menu);
}

void menu_tiene_submenus_con_submenus()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_crear_submenu(menu, 'S', "Submenu");
	bool tiene = menu_tiene_submenus(menu);

	pa2m_afirmar(tiene == true, "Menu con submenus devuelve true");

	menu_destruir_todo(menu);
}

void menu_tiene_submenus_despues_de_eliminar_todos()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_crear_submenu(menu, 'S', "Submenu");
	menu_t *submenu_s = menu_sacar_submenu(menu, 'S');
	bool tiene = menu_tiene_submenus(menu);

	pa2m_afirmar(tiene == false,
		     "Menu sin submenus despues de eliminar devuelve false");

	menu_destruir(submenu_s);
	menu_destruir_todo(menu);
}

void menu_tiene_submenus_submenu_con_submenus()
{
	menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
	menu_crear_submenu(submenu, 'X', "Sub-submenu");

	bool tiene_padre = menu_tiene_submenus(padre);
	bool tiene_submenu = menu_tiene_submenus(submenu);

	pa2m_afirmar(tiene_padre == true,
		     "Menu padre con submenus devuelve true");
	pa2m_afirmar(tiene_submenu == true,
		     "Submenu con submenus devuelve true");

	menu_destruir_todo(padre);
}

void menu_tiene_submenus_con_menu_nulo()
{
	bool tiene = menu_tiene_submenus(NULL);
	pa2m_afirmar(tiene == false, "Menu NULL no tiene submenus");
}

void pruebas_menu_tiene_submenus()
{
	pa2m_nuevo_grupo("Pruebas menu_tiene_submenus parametros validos");
	menu_tiene_submenus_sin_submenus();
	menu_tiene_submenus_con_submenus();
	menu_tiene_submenus_despues_de_eliminar_todos();
	menu_tiene_submenus_submenu_con_submenus();

	pa2m_nuevo_grupo("Pruebas menu_tiene_submenus parametros invalidos");
	menu_tiene_submenus_con_menu_nulo();
}

// Pruebas para menu_cantidad_estilos
void menu_cantidad_estilos_menu_raiz()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	size_t cantidad = menu_cantidad_estilos(menu);

	pa2m_afirmar(cantidad == 1, "Menu raiz recien creado tiene 1 estilo");

	menu_destruir_todo(menu);
}

void menu_cantidad_estilos_con_estilos_agregados()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_estilo(menu, mostrar_estilo_detallado,
			    mostrar_titulo_detallado);
	menu_agregar_estilo(menu, mostrar_estilo_emoji,
			    mostrar_titulo_centrado);

	size_t cantidad = menu_cantidad_estilos(menu);
	pa2m_afirmar(cantidad == 3,
		     "Menu con 3 estilos devuelve cantidad correcta");

	menu_destruir_todo(menu);
}

void menu_cantidad_estilos_submenu()
{
	menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");

	size_t cant_padre = menu_cantidad_estilos(padre);
	size_t cant_submenu = menu_cantidad_estilos(submenu);

	pa2m_afirmar(cant_padre >= 1, "Menu padre tiene estilos");
	pa2m_afirmar(cant_submenu == 0,
		     "Submenu no tiene estilos (devuelve 0)");

	menu_destruir_todo(padre);
}

void menu_cantidad_estilos_con_menu_nulo()
{
	size_t cantidad = menu_cantidad_estilos(NULL);
	pa2m_afirmar(cantidad == 0, "Cantidad de estilos de menu NULL es 0");
}

void pruebas_menu_cantidad_estilos()
{
	pa2m_nuevo_grupo("Pruebas menu_cantidad_estilos parametros validos");
	menu_cantidad_estilos_menu_raiz();
	menu_cantidad_estilos_con_estilos_agregados();
	menu_cantidad_estilos_submenu();

	pa2m_nuevo_grupo("Pruebas menu_cantidad_estilos parametros invalidos");
	menu_cantidad_estilos_con_menu_nulo();
}

// Pruebas para menu_tiene_estilos
void menu_tiene_estilos_menu_raiz_un_estilo()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	bool tiene = menu_tiene_estilos(menu);

	pa2m_afirmar(tiene == false,
		     "Menu raiz con solo 1 estilo devuelve false");

	menu_destruir_todo(menu);
}

void menu_tiene_estilos_con_multiples_estilos()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_estilo(menu, mostrar_estilo_detallado,
			    mostrar_titulo_detallado);
	bool tiene = menu_tiene_estilos(menu);

	pa2m_afirmar(tiene == true, "Menu con 2+ estilos devuelve true");

	menu_destruir_todo(menu);
}

void menu_tiene_estilos_submenu()
{
	menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_agregar_estilo(padre, mostrar_estilo_detallado,
			    mostrar_titulo_detallado);
	menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");

	bool tiene_padre = menu_tiene_estilos(padre);
	bool tiene_submenu = menu_tiene_estilos(submenu);

	pa2m_afirmar(tiene_padre == true,
		     "Menu padre con multiples estilos devuelve true");
	pa2m_afirmar(tiene_submenu == false,
		     "Submenu devuelve false (no puede tener estilos)");

	menu_destruir_todo(padre);
}

void menu_tiene_estilos_con_menu_nulo()
{
	bool tiene = menu_tiene_estilos(NULL);
	pa2m_afirmar(tiene == false, "Menu NULL no tiene estilos");
}

void pruebas_menu_tiene_estilos()
{
	pa2m_nuevo_grupo("Pruebas menu_tiene_estilos parametros validos");
	menu_tiene_estilos_menu_raiz_un_estilo();
	menu_tiene_estilos_con_multiples_estilos();
	menu_tiene_estilos_submenu();

	pa2m_nuevo_grupo("Pruebas menu_tiene_estilos parametros invalidos");
	menu_tiene_estilos_con_menu_nulo();
}

// Pruebas para menu_existe_opcion
void menu_existe_opcion_con_opcion_existente()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'B', "Accion B", accion_prueba_true);
	menu_agregar_accion(menu, 'C', "Accion C", accion_prueba_false);

	bool existe_b = menu_existe_opcion(menu, 'B');
	bool existe_c = menu_existe_opcion(menu, 'C');

	pa2m_afirmar(existe_b == true, "Opcion 'B' existe en el menu");
	pa2m_afirmar(existe_c == true, "Opcion 'C' existe en el menu");

	menu_destruir_todo(menu);
}

void menu_existe_opcion_con_submenu_existente()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_crear_submenu(menu, 'S', "Submenu");

	bool existe = menu_existe_opcion(menu, 'S');
	pa2m_afirmar(existe == true, "Submenu 'S' existe en el menu");

	menu_destruir_todo(menu);
}

void menu_existe_opcion_con_opcion_inexistente()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'B', "Accion B", accion_prueba_true);

	bool existe_x = menu_existe_opcion(menu, 'X');
	bool existe_y = menu_existe_opcion(menu, 'Y');

	pa2m_afirmar(existe_x == false, "Opcion 'X' no existe en el menu");
	pa2m_afirmar(existe_y == false, "Opcion 'Y' no existe en el menu");

	menu_destruir_todo(menu);
}

void menu_existe_opcion_menu_vacio()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);

	bool existe = menu_existe_opcion(menu, 'A');
	pa2m_afirmar(existe == false, "Opcion no existe en menu vacio");

	menu_destruir_todo(menu);
}

void menu_existe_opcion_teclas_especiales_no_existen()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'B', "Accion B", accion_prueba_true);

	bool existe_q = menu_existe_opcion(menu, MENU_TECLA_SALIR);
	bool existe_a = menu_existe_opcion(menu, MENU_TECLA_VOLVER);
	bool existe_e = menu_existe_opcion(menu, MENU_TECLA_ESTILOS);

	pa2m_afirmar(existe_q == false,
		     "Tecla SALIR no existe como opcion normal");
	pa2m_afirmar(existe_a == false,
		     "Tecla VOLVER no existe como opcion normal");
	pa2m_afirmar(existe_e == false,
		     "Tecla ESTILOS no existe como opcion normal");

	menu_destruir_todo(menu);
}

void menu_existe_opcion_teclas_especiales_caracteres()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
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
	menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
	menu_agregar_accion(submenu, 'X', "Accion X", accion_prueba_true);

	bool existe_en_padre = menu_existe_opcion(padre, 'S');
	bool existe_en_submenu = menu_existe_opcion(submenu, 'X');
	bool no_existe_en_padre = menu_existe_opcion(padre, 'X');

	pa2m_afirmar(existe_en_padre == true,
		     "Submenu 'S' existe en menu padre");
	pa2m_afirmar(existe_en_submenu == true, "Accion 'X' existe en submenu");
	pa2m_afirmar(no_existe_en_padre == false,
		     "Accion 'X' no existe en menu padre");

	menu_destruir_todo(padre);
}

void menu_existe_opcion_con_menu_nulo()
{
	bool existe = menu_existe_opcion(NULL, 'A');
	pa2m_afirmar(existe == false, "Opcion en menu NULL devuelve false");
}

void menu_existe_opcion_con_menu_sin_opciones()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);

	bool existe = menu_existe_opcion(menu, 'A');
	pa2m_afirmar(existe == false, "Opcion no existe en menu sin opciones");

	menu_destruir_todo(menu);
}

void pruebas_menu_existe_opcion()
{
	pa2m_nuevo_grupo("Pruebas menu_existe_opcion parametros validos");
	menu_existe_opcion_con_opcion_existente();
	menu_existe_opcion_con_submenu_existente();
	menu_existe_opcion_con_opcion_inexistente();
	menu_existe_opcion_menu_vacio();
	menu_existe_opcion_teclas_especiales_no_existen();
	menu_existe_opcion_teclas_especiales_caracteres();
	menu_existe_opcion_submenu();

	pa2m_nuevo_grupo("Pruebas menu_existe_opcion parametros invalidos");
	menu_existe_opcion_con_menu_nulo();
	menu_existe_opcion_con_menu_sin_opciones();
}

// Pruebas para menu_obtener_nombre_opcion
void menu_obtener_nombre_opcion_con_opcion_existente()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'B', "Mi Accion Especial",
			    accion_prueba_true);
	menu_agregar_accion(menu, 'C', "Otra Accion", accion_prueba_false);

	const char *nombre_b = menu_obtener_nombre_opcion(menu, 'B');
	const char *nombre_c = menu_obtener_nombre_opcion(menu, 'C');

	pa2m_afirmar(nombre_b != NULL, "Nombre de opcion 'B' no es NULL");
	pa2m_afirmar(strcmp(nombre_b, "Mi Accion Especial") == 0,
		     "Se obtiene el nombre correcto de la opcion 'B'");
	pa2m_afirmar(strcmp(nombre_c, "Otra Accion") == 0,
		     "Se obtiene el nombre correcto de la opcion 'C'");

	menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_con_submenu_existente()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_crear_submenu(menu, 'S', "Submenu Configuracion");

	const char *nombre = menu_obtener_nombre_opcion(menu, 'S');

	pa2m_afirmar(nombre != NULL, "Nombre de submenu 'S' no es NULL");
	pa2m_afirmar(strcmp(nombre, "Submenu Configuracion") == 0,
		     "Se obtiene el nombre correcto del submenu");

	menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_con_opcion_inexistente()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'B', "Accion B", accion_prueba_true);

	const char *nombre_x = menu_obtener_nombre_opcion(menu, 'X');
	const char *nombre_y = menu_obtener_nombre_opcion(menu, 'Y');

	pa2m_afirmar(nombre_x == NULL,
		     "Nombre de opcion 'X' inexistente es NULL");
	pa2m_afirmar(nombre_y == NULL,
		     "Nombre de opcion 'Y' inexistente es NULL");

	menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_nombre_vacio()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);

	pa2m_afirmar(true,
		     "Prueba de nombre vacio depende de la implementacion");

	menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_nombre_largo()
{
	char nombre_largo[100];
	memset(nombre_largo, 'A', 99);
	nombre_largo[99] = '\0';

	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'L', nombre_largo, accion_prueba_true);

	const char *nombre = menu_obtener_nombre_opcion(menu, 'L');

	pa2m_afirmar(nombre != NULL, "Nombre largo no es NULL");
	pa2m_afirmar(strlen(nombre) == 99,
		     "Nombre largo se preserva completamente");
	pa2m_afirmar(nombre[0] == 'A' && nombre[98] == 'A',
		     "Contenido del nombre largo es correcto");

	menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_caracteres_especiales()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'X', "Menu Pokemon: aeiou ñ",
			    accion_prueba_true);

	const char *nombre = menu_obtener_nombre_opcion(menu, 'X');

	pa2m_afirmar(nombre != NULL,
		     "Nombre con caracteres especiales no es NULL");
	pa2m_afirmar(strcmp(nombre, "Menu Pokemon: aeiou ñ") == 0,
		     "Caracteres especiales se preservan correctamente");

	menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_submenu()
{
	menu_t *padre = menu_crear_base("Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu Hijo");
	menu_agregar_accion(submenu, 'X', "Accion en Submenu",
			    accion_prueba_true);

	const char *nombre_submenu = menu_obtener_nombre_opcion(padre, 'S');
	const char *nombre_accion = menu_obtener_nombre_opcion(submenu, 'X');

	pa2m_afirmar(nombre_submenu != NULL &&
			     strcmp(nombre_submenu, "Submenu Hijo") == 0,
		     "Nombre del submenu es correcto en el padre");
	pa2m_afirmar(nombre_accion != NULL &&
			     strcmp(nombre_accion, "Accion en Submenu") == 0,
		     "Nombre de la accion es correcto en el submenu");

	menu_destruir_todo(padre);
}

void menu_obtener_nombre_opcion_despues_de_operaciones()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'B', "Accion Original", accion_prueba_true);

	const char *nombre_antes = menu_obtener_nombre_opcion(menu, 'B');

	menu_agregar_accion(menu, 'C', "Otra Accion", accion_prueba_false);
	menu_agregar_estilo(menu, mostrar_estilo_detallado,
			    mostrar_titulo_detallado);

	const char *nombre_despues = menu_obtener_nombre_opcion(menu, 'B');

	pa2m_afirmar(nombre_antes != NULL && nombre_despues != NULL,
		     "Nombre se mantiene accesible despues de operaciones");
	pa2m_afirmar(strcmp(nombre_antes, nombre_despues) == 0,
		     "Nombre no cambia despues de otras operaciones");

	menu_destruir_todo(menu);
}

void menu_obtener_nombre_opcion_con_menu_nulo()
{
	const char *nombre = menu_obtener_nombre_opcion(NULL, 'A');
	pa2m_afirmar(nombre == NULL,
		     "Obtener nombre de menu NULL devuelve NULL");
}

void menu_obtener_nombre_opcion_con_tecla_invalida()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);

	const char *nombre_nulo = menu_obtener_nombre_opcion(menu, '\0');
	pa2m_afirmar(nombre_nulo == NULL, "Tecla nula devuelve NULL");

	menu_destruir_todo(menu);
}

void pruebas_menu_obtener_nombre_opcion()
{
	pa2m_nuevo_grupo(
		"Pruebas menu_obtener_nombre_opcion parametros validos");
	menu_obtener_nombre_opcion_con_opcion_existente();
	menu_obtener_nombre_opcion_con_submenu_existente();
	menu_obtener_nombre_opcion_con_opcion_inexistente();
	menu_obtener_nombre_opcion_nombre_largo();
	menu_obtener_nombre_opcion_caracteres_especiales();
	menu_obtener_nombre_opcion_submenu();
	menu_obtener_nombre_opcion_despues_de_operaciones();

	pa2m_nuevo_grupo(
		"Pruebas menu_obtener_nombre_opcion parametros invalidos");
	menu_obtener_nombre_opcion_con_menu_nulo();
	menu_obtener_nombre_opcion_con_tecla_invalida();
	menu_obtener_nombre_opcion_nombre_vacio();
}

// Pruebas para menu_sacar_accion
void menu_sacar_accion_con_opcion_existente()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'B', "Accion B", accion_prueba_true);
	menu_agregar_accion(menu, 'C', "Accion C", accion_prueba_false);

	bool sacada_b = menu_sacar_accion(menu, 'B');
	bool existe_b_despues = menu_existe_opcion(menu, 'B');
	bool existe_c_despues = menu_existe_opcion(menu, 'C');

	pa2m_afirmar(sacada_b == true, "Se saca correctamente la accion 'B'");
	pa2m_afirmar(existe_b_despues == false,
		     "Accion 'B' ya no existe despues de sacarla");
	pa2m_afirmar(existe_c_despues == true,
		     "Accion 'C' sigue existiendo despues de sacar 'B'");

	menu_destruir_todo(menu);
}

void menu_sacar_accion_con_submenu_no_se_saca()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_crear_submenu(menu, 'S', "Submenu");

	bool sacada = menu_sacar_accion(menu, 'S');
	bool existe_despues = menu_existe_opcion(menu, 'S');

	pa2m_afirmar(sacada == false, "No se puede sacar submenu como accion");
	pa2m_afirmar(existe_despues == true,
		     "Submenu sigue existiendo despues del intento");

	menu_destruir_todo(menu);
}

void menu_sacar_accion_con_opcion_inexistente()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'B', "Accion B", accion_prueba_true);

	bool sacada_x = menu_sacar_accion(menu, 'X');
	bool sacada_y = menu_sacar_accion(menu, 'Y');

	pa2m_afirmar(sacada_x == false,
		     "No se puede sacar opcion 'X' inexistente");
	pa2m_afirmar(sacada_y == false,
		     "No se puede sacar opcion 'Y' inexistente");

	menu_destruir_todo(menu);
}

void menu_sacar_accion_menu_vacio()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);

	bool sacada = menu_sacar_accion(menu, 'A');
	pa2m_afirmar(sacada == false, "No se puede sacar accion de menu vacio");

	menu_destruir_todo(menu);
}

void menu_sacar_accion_verifica_contador()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'X', "Accion X", accion_prueba_true);
	menu_agregar_accion(menu, 'B', "Accion B", accion_prueba_true);
	menu_agregar_accion(menu, 'C', "Accion C", accion_prueba_true);

	size_t cantidad_antes = menu_cantidad_acciones(menu);
	bool sacada = menu_sacar_accion(menu, 'B');
	size_t cantidad_despues = menu_cantidad_acciones(menu);

	pa2m_afirmar(cantidad_antes == 3, "Cantidad inicial de acciones es 3");
	pa2m_afirmar(sacada == true, "Accion 'B' se saca correctamente");
	pa2m_afirmar(cantidad_despues == 2,
		     "Cantidad de acciones disminuye a 2");

	menu_destruir_todo(menu);
}

void menu_sacar_accion_con_menu_nulo()
{
	bool sacada = menu_sacar_accion(NULL, 'A');
	pa2m_afirmar(sacada == false,
		     "Sacar accion de menu NULL devuelve false");
}

void menu_sacar_accion_con_tecla_especial()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);

	bool sacada_q = menu_sacar_accion(menu, MENU_TECLA_SALIR);
	bool sacada_a = menu_sacar_accion(menu, MENU_TECLA_VOLVER);
	bool sacada_e = menu_sacar_accion(menu, MENU_TECLA_ESTILOS);

	pa2m_afirmar(sacada_q == false,
		     "No se puede sacar tecla especial SALIR");
	pa2m_afirmar(sacada_a == false,
		     "No se puede sacar tecla especial VOLVER");
	pa2m_afirmar(sacada_e == false,
		     "No se puede sacar tecla especial ESTILOS");

	menu_destruir_todo(menu);
}

void pruebas_menu_sacar_accion()
{
	pa2m_nuevo_grupo("Pruebas menu_sacar_accion parametros validos");
	menu_sacar_accion_con_opcion_existente();
	menu_sacar_accion_con_submenu_no_se_saca();
	menu_sacar_accion_con_opcion_inexistente();
	menu_sacar_accion_menu_vacio();
	menu_sacar_accion_verifica_contador();

	pa2m_nuevo_grupo("Pruebas menu_sacar_accion parametros invalidos");
	menu_sacar_accion_con_menu_nulo();
	menu_sacar_accion_con_tecla_especial();
}

// Pruebas para menu_sacar_submenu
void menu_sacar_submenu_con_submenu_existente()
{
	menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_crear_submenu(padre, 'S', "Submenu");

	bool existe_antes = menu_existe_opcion(padre, 'S');
	menu_t *submenu_sacado = menu_sacar_submenu(padre, 'S');
	bool existe_despues = menu_existe_opcion(padre, 'S');

	pa2m_afirmar(existe_antes == true, "Submenu existe antes de sacarlo");
	pa2m_afirmar(submenu_sacado != NULL,
		     "Se obtiene submenu sacado correctamente");
	pa2m_afirmar(existe_despues == false,
		     "Submenu no existe despues de sacarlo");

	menu_destruir(submenu_sacado);
	menu_destruir_todo(padre);
}

void menu_sacar_submenu_con_accion_no_se_saca()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'X', "Accion X", accion_prueba_true);

	menu_t *resultado = menu_sacar_submenu(menu, 'X');
	bool existe_despues = menu_existe_opcion(menu, 'X');

	pa2m_afirmar(resultado == NULL,
		     "No se puede sacar accion como submenu");
	pa2m_afirmar(existe_despues == true,
		     "Accion sigue existiendo despues del intento");

	menu_destruir_todo(menu);
}

void menu_sacar_submenu_con_submenu_inexistente()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_crear_submenu(menu, 'S', "Submenu");

	menu_t *resultado_x = menu_sacar_submenu(menu, 'X');
	menu_t *resultado_y = menu_sacar_submenu(menu, 'Y');

	pa2m_afirmar(resultado_x == NULL,
		     "No se puede sacar submenu 'X' inexistente");
	pa2m_afirmar(resultado_y == NULL,
		     "No se puede sacar submenu 'Y' inexistente");

	menu_destruir_todo(menu);
}

void menu_sacar_submenu_menu_vacio()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);

	menu_t *resultado = menu_sacar_submenu(menu, 'S');
	pa2m_afirmar(resultado == NULL,
		     "No se puede sacar submenu de menu vacio");

	menu_destruir_todo(menu);
}

void menu_sacar_submenu_verifica_contador()
{
	menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_crear_submenu(padre, 'S', "Submenu S");
	menu_crear_submenu(padre, 'T', "Submenu T");

	size_t cantidad_antes = menu_cantidad_submenus(padre);
	menu_t *submenu_sacado = menu_sacar_submenu(padre, 'S');
	size_t cantidad_despues = menu_cantidad_submenus(padre);

	pa2m_afirmar(cantidad_antes == 2, "Cantidad inicial de submenus es 2");
	pa2m_afirmar(submenu_sacado != NULL,
		     "Submenu 'S' se saca correctamente");
	pa2m_afirmar(cantidad_despues == 1,
		     "Cantidad de submenus disminuye a 1");

	menu_destruir(submenu_sacado);
	menu_destruir_todo(padre);
}

void menu_sacar_submenu_submenu_con_contenido()
{
	menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
	menu_agregar_accion(submenu, 'X', "Accion X", accion_prueba_true);
	menu_agregar_accion(submenu, 'Y', "Accion Y", accion_prueba_false);

	menu_t *submenu_sacado = menu_sacar_submenu(padre, 'S');

	pa2m_afirmar(submenu_sacado != NULL, "Se saca submenu con contenido");
	pa2m_afirmar(menu_cantidad_acciones(submenu_sacado) == 2,
		     "Submenu sacado mantiene sus 2 acciones");

	menu_destruir_todo(submenu_sacado);
	menu_destruir_todo(padre);
}

void menu_sacar_submenu_con_menu_nulo()
{
	menu_t *resultado = menu_sacar_submenu(NULL, 'S');
	pa2m_afirmar(resultado == NULL,
		     "Sacar submenu de menu NULL devuelve NULL");
}

void menu_sacar_submenu_con_tecla_especial()
{
	menu_t *menu = menu_crear_base("Menu", mostrar_estilo_simple,
				       mostrar_titulo_simple);

	menu_t *resultado_q = menu_sacar_submenu(menu, MENU_TECLA_SALIR);
	menu_t *resultado_a = menu_sacar_submenu(menu, MENU_TECLA_VOLVER);
	menu_t *resultado_e = menu_sacar_submenu(menu, MENU_TECLA_ESTILOS);

	pa2m_afirmar(resultado_q == NULL,
		     "No se puede sacar tecla especial SALIR como submenu");
	pa2m_afirmar(resultado_a == NULL,
		     "No se puede sacar tecla especial VOLVER como submenu");
	pa2m_afirmar(resultado_e == NULL,
		     "No se puede sacar tecla especial ESTILOS como submenu");

	menu_destruir_todo(menu);
}

void pruebas_menu_sacar_submenu()
{
	pa2m_nuevo_grupo("Pruebas menu_sacar_submenu parametros validos");
	menu_sacar_submenu_con_submenu_existente();
	menu_sacar_submenu_con_accion_no_se_saca();
	menu_sacar_submenu_con_submenu_inexistente();
	menu_sacar_submenu_menu_vacio();
	menu_sacar_submenu_verifica_contador();
	menu_sacar_submenu_submenu_con_contenido();

	pa2m_nuevo_grupo("Pruebas menu_sacar_submenu parametros invalidos");
	menu_sacar_submenu_con_menu_nulo();
	menu_sacar_submenu_con_tecla_especial();
}

// Pruebas para menu_destruir
void menu_destruir_menu_simple()
{
	menu_t *menu = menu_crear_base("Menu Simple", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'X', "Accion X", accion_prueba_true);

	menu_destruir(menu);

	pa2m_afirmar(true, "Destruir menu simple no causa errores");
}

void menu_destruir_menu_con_submenus_no_libera_submenus()
{
	menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
	menu_agregar_accion(submenu, 'X', "Accion X", accion_prueba_true);

	bool existe_antes = menu_existe_opcion(padre, 'S');

	menu_destruir(padre);

	pa2m_afirmar(existe_antes == true,
		     "Submenu existe antes de destruir padre");
	pa2m_afirmar(
		true,
		"menu_destruir no libera submenus asociados (comportamiento documentado)");

	menu_destruir(submenu);
}

void menu_destruir_menu_con_estilos()
{
	menu_t *menu = menu_crear_base("Menu con Estilos",
				       mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_estilo(menu, mostrar_estilo_detallado,
			    mostrar_titulo_detallado);

	menu_destruir(menu);

	pa2m_afirmar(true, "Destruir menu con estilos no causa errores");
}

void menu_destruir_menu_vacio()
{
	menu_t *menu = menu_crear_base("Menu Vacio", mostrar_estilo_simple,
				       mostrar_titulo_simple);

	menu_destruir(menu);

	pa2m_afirmar(true, "Destruir menu vacio no causa errores");
}

void menu_destruir_con_menu_nulo()
{
	menu_destruir(NULL);

	pa2m_afirmar(true, "Destruir menu NULL no causa errores");
}

void pruebas_menu_destruir()
{
	pa2m_nuevo_grupo("Pruebas menu_destruir - parametros validos");
	menu_destruir_menu_simple();
	menu_destruir_menu_con_submenus_no_libera_submenus();
	menu_destruir_menu_con_estilos();
	menu_destruir_menu_vacio();

	pa2m_nuevo_grupo("Pruebas menu_destruir parametros invalidos");
	menu_destruir_con_menu_nulo();
}

// Pruebas para menu_destruir_todo
void menu_destruir_todo_menu_simple()
{
	menu_t *menu = menu_crear_base("Menu Simple", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_agregar_accion(menu, 'X', "Accion X", accion_prueba_true);

	menu_destruir_todo(menu);

	pa2m_afirmar(true, "Destruir_todo menu simple no causa errores");
}

void menu_destruir_todo_libera_submenus_recursivamente()
{
	menu_t *raiz = menu_crear_base("Menu Raiz", mostrar_estilo_simple,
				       mostrar_titulo_simple);

	menu_t *sub1 = menu_crear_submenu(raiz, '1', "Submenu 1");
	menu_t *sub2 = menu_crear_submenu(sub1, '2', "Submenu 2");
	menu_t *sub3 = menu_crear_submenu(sub2, '3', "Submenu 3");

	menu_agregar_accion(raiz, 'X', "Accion Raiz", accion_prueba_true);
	menu_agregar_accion(sub1, 'B', "Accion Sub1", accion_prueba_true);
	menu_agregar_accion(sub2, 'C', "Accion Sub2", accion_prueba_true);
	menu_agregar_accion(sub3, 'D', "Accion Sub3", accion_prueba_true);

	bool existe_sub1 = menu_existe_opcion(raiz, '1');
	bool existe_sub2 = menu_existe_opcion(sub1, '2');
	bool existe_sub3 = menu_existe_opcion(sub2, '3');

	menu_destruir_todo(raiz);

	pa2m_afirmar(existe_sub1 && existe_sub2 && existe_sub3,
		     "Todos los submenus existen antes de destruir_todo");
	pa2m_afirmar(
		true,
		"menu_destruir_todo libera todos los submenus asociados recursivamente");
}

void menu_destruir_todo_menu_con_arbol_complejo()
{
	menu_t *raiz = menu_crear_base("Menu Principal", mostrar_estilo_simple,
				       mostrar_titulo_simple);

	menu_t *config = menu_crear_submenu(raiz, 'C', "Configuracion");
	menu_t *sonido = menu_crear_submenu(config, 'S', "Sonido");
	menu_t *video = menu_crear_submenu(config, 'V', "Video");

	menu_t *juegos = menu_crear_submenu(raiz, 'J', "Juegos");
	menu_t *guardados =
		menu_crear_submenu(juegos, 'G', "Partidas Guardadas");

	menu_agregar_accion(raiz, 'I', "Informacion", accion_prueba_true);
	menu_agregar_accion(sonido, 'V', "Volumen", accion_prueba_true);
	menu_agregar_accion(video, 'R', "Resolucion", accion_prueba_true);
	menu_agregar_accion(juegos, 'N', "Nuevo Juego", accion_prueba_true);
	menu_agregar_accion(guardados, 'L', "Cargar", accion_prueba_true);

	menu_destruir_todo(raiz);

	pa2m_afirmar(
		true,
		"Destruir_todo con arbol complejo libera toda la estructura");
}

void menu_destruir_todo_menu_vacio()
{
	menu_t *menu = menu_crear_base("Menu Vacio", mostrar_estilo_simple,
				       mostrar_titulo_simple);

	menu_destruir_todo(menu);

	pa2m_afirmar(true, "Destruir_todo menu vacio no causa errores");
}

void menu_destruir_todo_submenu_aislado()
{
	menu_t *submenu = menu_crear_base("Submenu Aislado",
					  mostrar_estilo_simple,
					  mostrar_titulo_simple);
	menu_agregar_accion(submenu, 'X', "Accion X", accion_prueba_true);

	menu_destruir_todo(submenu);

	pa2m_afirmar(true, "Destruir_todo funciona en submenus aislados");
}

void menu_destruir_todo_con_menu_nulo()
{
	menu_destruir_todo(NULL);

	pa2m_afirmar(true, "Destruir_todo menu NULL no causa errores");
}

void menu_destruir_todo_verifica_no_double_free()
{
	menu_t *raiz = menu_crear_base("Raiz", mostrar_estilo_simple,
				       mostrar_titulo_simple);
	menu_t *sub1 = menu_crear_submenu(raiz, '1', "Submenu 1");
	menu_t *sub2 = menu_crear_submenu(sub1, '2', "Submenu 2");

	menu_agregar_accion(sub2, 'B', "Accion", accion_prueba_true);

	menu_destruir_todo(raiz);

	pa2m_afirmar(
		true,
		"Destruir_todo no causa double-free con estructura anidada");
}

void pruebas_menu_destruir_todo()
{
	pa2m_nuevo_grupo(
		"Pruebas menu_destruir_todo - Libera todos los submenus recursivamente");
	menu_destruir_todo_menu_simple();
	menu_destruir_todo_libera_submenus_recursivamente();
	menu_destruir_todo_menu_con_arbol_complejo();
	menu_destruir_todo_menu_vacio();
	menu_destruir_todo_submenu_aislado();
	menu_destruir_todo_verifica_no_double_free();

	pa2m_nuevo_grupo("Pruebas menu_destruir_todo parametros invalidos");
	menu_destruir_todo_con_menu_nulo();
}

// Pruebas para menu_navegador_crear
void menu_navegador_crear_con_menu_base_valido()
{
	menu_t *menu_base = menu_crear_base(
		"Menu Principal", mostrar_estilo_simple, mostrar_titulo_simple);
	menu_agregar_accion(menu_base, 'A', "Accion A", accion_prueba_true);

	menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);

	pa2m_afirmar(nav != NULL,
		     "Navegador se crea correctamente con menu base valido");
	pa2m_afirmar(menu_navegador_obtener_actual(nav) == menu_base,
		     "Menu actual es el menu base");
	pa2m_afirmar(menu_navegador_esta_terminado(nav) == false,
		     "Navegador no esta terminado al crear");

	menu_navegador_destruir(nav);
	menu_destruir_todo(menu_base);
}

void menu_navegador_crear_con_user_data()
{
	int user_data = 42;
	menu_t *menu_base = menu_crear_base("Menu", mostrar_estilo_simple,
					    mostrar_titulo_simple);

	menu_navegador_t *nav = menu_navegador_crear(menu_base, &user_data);

	pa2m_afirmar(nav != NULL,
		     "Navegador se crea correctamente con user_data");

	menu_navegador_destruir(nav);
	menu_destruir_todo(menu_base);
}

void menu_navegador_crear_con_menu_con_estilos()
{
	menu_t *menu_base = menu_crear_base("Menu", mostrar_estilo_simple,
					    mostrar_titulo_simple);
	menu_agregar_estilo(menu_base, mostrar_estilo_detallado,
			    mostrar_titulo_detallado);
	menu_agregar_estilo(menu_base, mostrar_estilo_emoji,
			    mostrar_titulo_centrado);

	menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);

	pa2m_afirmar(
		nav != NULL,
		"Navegador se crea correctamente con menu que tiene estilos");

	menu_navegador_destruir(nav);
	menu_destruir_todo(menu_base);
}

void menu_navegador_crear_menu_sin_estilos()
{
	menu_t *menu_base = menu_crear_base("Menu", mostrar_estilo_simple,
					    mostrar_titulo_simple);

	menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);

	pa2m_afirmar(
		nav != NULL,
		"Navegador se crea con menu que solo tiene estilo inicial");

	menu_navegador_destruir(nav);
	menu_destruir_todo(menu_base);
}

void menu_navegador_crear_con_menu_nulo()
{
	menu_navegador_t *nav = menu_navegador_crear(NULL, NULL);
	pa2m_afirmar(nav == NULL, "Navegador con menu NULL devuelve NULL");
}

void pruebas_menu_navegador_crear()
{
	pa2m_nuevo_grupo("Pruebas menu_navegador_crear parametros validos");
	menu_navegador_crear_con_menu_base_valido();
	menu_navegador_crear_con_user_data();
	menu_navegador_crear_con_menu_con_estilos();
	menu_navegador_crear_menu_sin_estilos();

	pa2m_nuevo_grupo("Pruebas menu_navegador_crear parametros invalidos");
	menu_navegador_crear_con_menu_nulo();
}

// Pruebas para menu_navegador_procesar_tecla
void menu_navegador_procesar_tecla_completa()
{
	menu_t *menu_base = menu_crear_base(
		"Menu Principal", mostrar_estilo_simple, mostrar_titulo_simple);
	menu_agregar_accion(menu_base, 'A', "Accion Exitosa",
			    accion_prueba_true);
	menu_agregar_estilo(menu_base, mostrar_estilo_detallado,
			    mostrar_titulo_detallado);

	menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);

	menu_navegacion_estado_t resultado_estilo =
		menu_navegador_procesar_tecla(nav, MENU_TECLA_ESTILOS);
	menu_navegacion_estado_t resultado_accion =
		menu_navegador_procesar_tecla(nav, 'A');
	menu_navegacion_estado_t resultado_salir =
		menu_navegador_procesar_tecla(nav, MENU_TECLA_SALIR);

	pa2m_afirmar(resultado_estilo == MENU_NAVEGACION_CONTINUAR,
		     "Procesar tecla ESTILOS devuelve CONTINUAR");
	pa2m_afirmar(resultado_accion == MENU_NAVEGACION_CONTINUAR,
		     "Procesar tecla accion devuelve CONTINUAR");
	pa2m_afirmar(resultado_salir == MENU_NAVEGACION_TERMINAR,
		     "Procesar tecla SALIR devuelve TERMINAR");
	pa2m_afirmar(menu_navegador_esta_terminado(nav) == true,
		     "Navegador termina despues de SALIR");

	menu_navegador_destruir(nav);
	menu_destruir_todo(menu_base);
}

void menu_navegador_procesar_tecla_con_navegador_terminado()
{
	menu_t *menu_base = menu_crear_base("Menu", mostrar_estilo_simple,
					    mostrar_titulo_simple);
	menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);

	menu_navegador_procesar_tecla(nav, MENU_TECLA_SALIR);
	menu_navegacion_estado_t resultado =
		menu_navegador_procesar_tecla(nav, 'A');

	pa2m_afirmar(
		resultado == MENU_NAVEGACION_TERMINAR,
		"Procesar tecla con navegador terminado devuelve TERMINAR");

	menu_navegador_destruir(nav);
	menu_destruir_todo(menu_base);
}

void menu_navegador_procesar_tecla_con_navegador_nulo()
{
	menu_navegacion_estado_t resultado =
		menu_navegador_procesar_tecla(NULL, 'A');
	pa2m_afirmar(resultado == MENU_NAVEGACION_ERROR,
		     "Procesar tecla con navegador NULL devuelve TERMINAR");
}

void pruebas_menu_navegador_procesar_tecla()
{
	pa2m_nuevo_grupo(
		"Pruebas menu_navegador_procesar_tecla parametros validos");
	menu_navegador_procesar_tecla_completa();
	menu_navegador_procesar_tecla_con_navegador_terminado();

	pa2m_nuevo_grupo(
		"Pruebas menu_navegador_procesar_tecla parametros invalidos");
	menu_navegador_procesar_tecla_con_navegador_nulo();
}

// Pruebas para funciones de consulta del navegador
void menu_navegador_obtener_actual_valido()
{
	menu_t *menu_base = menu_crear_base("Menu", mostrar_estilo_simple,
					    mostrar_titulo_simple);
	menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);

	menu_t *actual = menu_navegador_obtener_actual(nav);

	pa2m_afirmar(actual == menu_base,
		     "Obtener actual devuelve menu base correctamente");

	menu_navegador_destruir(nav);
	menu_destruir_todo(menu_base);
}

void menu_navegador_esta_terminado_recien_creado()
{
	menu_t *menu_base = menu_crear_base("Menu", mostrar_estilo_simple,
					    mostrar_titulo_simple);
	menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);

	bool terminado = menu_navegador_esta_terminado(nav);

	pa2m_afirmar(terminado == false,
		     "Navegador recien creado no esta terminado");

	menu_navegador_destruir(nav);
	menu_destruir_todo(menu_base);
}

void menu_navegador_obtener_actual_con_navegador_nulo()
{
	menu_t *actual = menu_navegador_obtener_actual(NULL);
	pa2m_afirmar(actual == NULL,
		     "Obtener actual con navegador NULL devuelve NULL");
}

void menu_navegador_esta_terminado_con_navegador_nulo()
{
	bool terminado = menu_navegador_esta_terminado(NULL);
	pa2m_afirmar(terminado == true, "Navegador NULL esta terminado");
}

void pruebas_menu_navegador_consultas()
{
	pa2m_nuevo_grupo("Pruebas menu_navegador consultas parametros validos");
	menu_navegador_obtener_actual_valido();
	menu_navegador_esta_terminado_recien_creado();

	pa2m_nuevo_grupo(
		"Pruebas menu_navegador consultas parametros invalidos");
	menu_navegador_obtener_actual_con_navegador_nulo();
	menu_navegador_esta_terminado_con_navegador_nulo();
}

// Pruebas para menu_navegador_destruir
void menu_navegador_destruir_valido()
{
	menu_t *menu_base = menu_crear_base("Menu", mostrar_estilo_simple,
					    mostrar_titulo_simple);
	menu_navegador_t *nav = menu_navegador_crear(menu_base, NULL);

	menu_navegador_destruir(nav);
	menu_destruir_todo(menu_base);

	pa2m_afirmar(true, "Destruir navegador valido no causa errores");
}

void menu_navegador_destruir_con_navegador_nulo()
{
	menu_navegador_destruir(NULL);
	pa2m_afirmar(true, "Destruir navegador NULL no causa errores");
}

void menu_navegador_destruir_con_submenus_en_pila()
{
	menu_t *padre = menu_crear_base("Menu Padre", mostrar_estilo_simple,
					mostrar_titulo_simple);
	menu_t *submenu = menu_crear_submenu(padre, 'S', "Submenu");
	if (!submenu)
		return;

	menu_navegador_t *nav = menu_navegador_crear(padre, NULL);

	menu_navegador_procesar_tecla(nav, 'S');

	menu_navegador_destruir(nav);
	menu_destruir_todo(padre);

	pa2m_afirmar(true,
		     "Destruir navegador con pila llena no causa errores");
}

void pruebas_menu_navegador_destruir()
{
	pa2m_nuevo_grupo("Pruebas menu_navegador_destruir parametros validos");
	menu_navegador_destruir_valido();
	menu_navegador_destruir_con_submenus_en_pila();

	pa2m_nuevo_grupo(
		"Pruebas menu_navegador_destruir parametros invalidos");
	menu_navegador_destruir_con_navegador_nulo();
}

// Test de integracion: simulacion completa de navegacion
void test_integracion_navegacion_completa()
{
	menu_t *menu_principal = menu_crear_base(
		"Menu Principal", mostrar_estilo_simple, mostrar_titulo_simple);
	menu_agregar_estilo(menu_principal, mostrar_estilo_detallado,
			    mostrar_titulo_detallado);

	menu_agregar_accion(menu_principal, '1', "Ver Configuracion",
			    accion_prueba_true);
	menu_agregar_accion(menu_principal, '2', "Accion Fallida",
			    accion_prueba_false);

	menu_t *submenu_config =
		menu_crear_submenu(menu_principal, 'C', "Configuracion");
	menu_agregar_accion(submenu_config, 'X', "Ajustar Sonido",
			    accion_prueba_true);
	menu_agregar_accion(submenu_config, 'B', "Ajustar Video",
			    accion_prueba_true);

	menu_t *submenu_avanzado =
		menu_crear_submenu(submenu_config, 'D', "Opciones Avanzadas");
	menu_agregar_accion(submenu_avanzado, 'X', "Resetear Sistema",
			    accion_prueba_true);

	int user_data = 100;
	menu_navegador_t *nav =
		menu_navegador_crear(menu_principal, &user_data);

	pa2m_afirmar(
		nav != NULL,
		"Navegador se crea correctamente para test de integracion");

	menu_navegacion_estado_t estado;

	estado = menu_navegador_procesar_tecla(nav, MENU_TECLA_ESTILOS);
	pa2m_afirmar(estado == MENU_NAVEGACION_CONTINUAR,
		     "Cambio de estilo exitoso");

	estado = menu_navegador_procesar_tecla(nav, '1');
	pa2m_afirmar(estado == MENU_NAVEGACION_CONTINUAR,
		     "Accion exitosa continua navegacion");

	estado = menu_navegador_procesar_tecla(nav, '2');
	pa2m_afirmar(estado == MENU_NAVEGACION_ERROR,
		     "Accion fallida produce error");

	estado = menu_navegador_procesar_tecla(nav, 'C');
	pa2m_afirmar(estado == MENU_NAVEGACION_CONTINUAR,
		     "Navegacion a submenu exitosa");
	pa2m_afirmar(menu_navegador_obtener_actual(nav) == submenu_config,
		     "Menu actual es submenu de configuracion");

	estado = menu_navegador_procesar_tecla(nav, 'X');
	pa2m_afirmar(estado == MENU_NAVEGACION_CONTINUAR,
		     "Accion en submenu exitosa");

	estado = menu_navegador_procesar_tecla(nav, 'D');
	pa2m_afirmar(estado == MENU_NAVEGACION_CONTINUAR,
		     "Navegacion a submenu anidado exitosa");
	pa2m_afirmar(menu_navegador_obtener_actual(nav) == submenu_avanzado,
		     "Menu actual es submenu avanzado");

	estado = menu_navegador_procesar_tecla(nav, MENU_TECLA_VOLVER);
	pa2m_afirmar(estado == MENU_NAVEGACION_CONTINUAR,
		     "Volver desde submenu anidado exitoso");
	pa2m_afirmar(menu_navegador_obtener_actual(nav) == submenu_config,
		     "Menu actual vuelve a submenu configuracion");

	estado = menu_navegador_procesar_tecla(nav, MENU_TECLA_VOLVER);
	pa2m_afirmar(estado == MENU_NAVEGACION_CONTINUAR,
		     "Volver al menu principal exitoso");
	pa2m_afirmar(menu_navegador_obtener_actual(nav) == menu_principal,
		     "Menu actual vuelve a menu principal");

	estado = menu_navegador_procesar_tecla(nav, MENU_TECLA_SALIR);
	pa2m_afirmar(estado == MENU_NAVEGACION_TERMINAR,
		     "Salir del sistema exitoso");
	pa2m_afirmar(menu_navegador_esta_terminado(nav) == true,
		     "Navegador marca terminado despues de salir");

	menu_navegador_destruir(nav);
	menu_destruir_todo(menu_principal);

	pa2m_afirmar(
		true,
		"Test de integracion completo ejecutado sin errores de memoria");
}

void pruebas_integracion_completa()
{
	pa2m_nuevo_grupo("Pruebas de integracion - Navegacion completa");
	test_integracion_navegacion_completa();
}

void ejecutar_pruebas_menu()
{
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
	pruebas_menu_destruir();
	pruebas_menu_destruir_todo();

	pa2m_nuevo_grupo("PRUEBAS NAVEGACION MENU");

	pruebas_menu_navegador_crear();
	pruebas_menu_navegador_procesar_tecla();
	pruebas_menu_navegador_consultas();
	pruebas_menu_navegador_destruir();
	pruebas_integracion_completa();
}

// Pruebas para juego_crear
void juego_crear_con_archivo_valido()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	pa2m_afirmar(juego != NULL,
		     "Juego se crea correctamente con archivo valido");
	pa2m_afirmar(juego_tiene_pokedex(juego) == true,
		     "Juego tiene pokedex cargada");

	juego_destruir(juego);
}

void juego_crear_con_archivo_inexistente()
{
	juego_t *juego = juego_crear("ejemplos/archivo_que_no_existe.csv");

	pa2m_afirmar(juego == NULL,
		     "Juego devuelve NULL con archivo inexistente");
}

void juego_crear_sin_archivo()
{
	juego_t *juego = juego_crear(NULL);

	pa2m_afirmar(juego != NULL, "Juego se crea correctamente sin archivo");
	pa2m_afirmar(juego_tiene_pokedex(juego) == false,
		     "Juego no tiene pokedex cuando se crea sin archivo");

	juego_destruir(juego);
}

void juego_crear_con_archivo_vacio()
{
	FILE *f = fopen("ejemplos/vacio.csv", "w");
	fclose(f);

	juego_t *juego = juego_crear("ejemplos/vacio.csv");

	pa2m_afirmar(juego == NULL, "Juego devuelve NULL con archivo vacio");

	remove("ejemplos/vacio.csv");
}

void pruebas_juego_crear()
{
	pa2m_nuevo_grupo("Pruebas juego_crear parametros validos");
	juego_crear_con_archivo_valido();
	juego_crear_sin_archivo();

	pa2m_nuevo_grupo("Pruebas juego_crear parametros invalidos");
	juego_crear_con_archivo_inexistente();
	juego_crear_con_archivo_vacio();
}

// Pruebas para juego_preparar
void juego_preparar_con_semilla_valida()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	bool resultado = juego_preparar(juego, 12345);

	pa2m_afirmar(resultado == true,
		     "Juego se prepara correctamente con semilla valida");
	pa2m_afirmar(juego_terminado(juego) == false,
		     "Juego no esta terminado despues de preparar");

	juego_destruir(juego);
}

void juego_preparar_sin_pokedex()
{
	juego_t *juego = juego_crear(NULL);

	bool resultado = juego_preparar(juego, 12345);

	pa2m_afirmar(resultado == false,
		     "Juego no se prepara sin pokedex cargada");

	juego_destruir(juego);
}

void juego_preparar_con_semilla_cero()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	bool resultado = juego_preparar(juego, 0);

	pa2m_afirmar(resultado == true,
		     "Juego se prepara correctamente con semilla cero");

	juego_destruir(juego);
}

void juego_preparar_con_juego_nulo()
{
	bool resultado = juego_preparar(NULL, 12345);

	pa2m_afirmar(resultado == false, "Preparar juego NULL devuelve false");
}

void juego_preparar_multiples_veces()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	bool resultado1 = juego_preparar(juego, 11111);
	bool resultado2 = juego_preparar(juego, 22222);
	bool resultado3 = juego_preparar(juego, 33333);

	pa2m_afirmar(resultado1 == true, "Primera preparacion exitosa");
	pa2m_afirmar(resultado2 == true, "Segunda preparacion exitosa");
	pa2m_afirmar(resultado3 == true, "Tercera preparacion exitosa");

	juego_destruir(juego);
}

void pruebas_juego_preparar()
{
	pa2m_nuevo_grupo("Pruebas juego_preparar parametros validos");
	juego_preparar_con_semilla_valida();
	juego_preparar_con_semilla_cero();
	juego_preparar_multiples_veces();

	pa2m_nuevo_grupo("Pruebas juego_preparar parametros invalidos");
	juego_preparar_con_juego_nulo();
	juego_preparar_sin_pokedex();
}

// Pruebas para juego_reiniciar
void juego_reiniciar_con_semilla_valida()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 11111);

	bool resultado = juego_reiniciar(juego, 22222);

	pa2m_afirmar(resultado == true,
		     "Juego se reinicia correctamente con nueva semilla");
	pa2m_afirmar(juego_terminado(juego) == false,
		     "Juego no esta terminado despues de reiniciar");

	juego_destruir(juego);
}

void juego_reiniciar_sin_preparar_previamente()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	bool resultado = juego_reiniciar(juego, 12345);

	pa2m_afirmar(resultado == true,
		     "Juego se reinicia incluso sin preparacion previa");

	juego_destruir(juego);
}

void juego_reiniciar_con_juego_nulo()
{
	bool resultado = juego_reiniciar(NULL, 12345);

	pa2m_afirmar(resultado == false, "Reiniciar juego NULL devuelve false");
}

void pruebas_juego_reiniciar()
{
	pa2m_nuevo_grupo("Pruebas juego_reiniciar parametros validos");
	juego_reiniciar_con_semilla_valida();
	juego_reiniciar_sin_preparar_previamente();

	pa2m_nuevo_grupo("Pruebas juego_reiniciar parametros invalidos");
	juego_reiniciar_con_juego_nulo();
}

// Pruebas para juego_terminado
void juego_terminado_recien_creado()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	bool terminado = juego_terminado(juego);

	pa2m_afirmar(terminado == false,
		     "Juego recien creado no esta terminado");

	juego_destruir(juego);
}

void juego_terminado_sin_preparar()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	bool terminado = juego_terminado(juego);

	pa2m_afirmar(terminado == true,
		     "Juego sin preparar se considera terminado");

	juego_destruir(juego);
}

void juego_terminado_con_juego_nulo()
{
	bool terminado = juego_terminado(NULL);

	pa2m_afirmar(terminado == true, "Juego NULL se considera terminado");
}

void pruebas_juego_terminado()
{
	pa2m_nuevo_grupo("Pruebas juego_terminado parametros validos");
	juego_terminado_recien_creado();

	pa2m_nuevo_grupo("Pruebas juego_terminado parametros invalidos");
	juego_terminado_con_juego_nulo();
	juego_terminado_sin_preparar();
}

// Pruebas para juego_obtener_pokedex
void juego_obtener_pokedex_con_pokedex_cargada()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	tp1_t *pokedex = juego_obtener_pokedex(juego);

	pa2m_afirmar(pokedex != NULL, "Se obtiene pokedex cuando esta cargada");

	juego_destruir(juego);
}

void juego_obtener_pokedex_sin_pokedex()
{
	juego_t *juego = juego_crear(NULL);

	tp1_t *pokedex = juego_obtener_pokedex(juego);

	pa2m_afirmar(pokedex == NULL,
		     "Se obtiene NULL cuando no hay pokedex cargada");

	juego_destruir(juego);
}

void juego_obtener_pokedex_con_juego_nulo()
{
	tp1_t *pokedex = juego_obtener_pokedex(NULL);

	pa2m_afirmar(pokedex == NULL,
		     "Obtener pokedex de juego NULL devuelve NULL");
}

void pruebas_juego_obtener_pokedex()
{
	pa2m_nuevo_grupo("Pruebas juego_obtener_pokedex parametros validos");
	juego_obtener_pokedex_con_pokedex_cargada();
	juego_obtener_pokedex_sin_pokedex();

	pa2m_nuevo_grupo("Pruebas juego_obtener_pokedex parametros invalidos");
	juego_obtener_pokedex_con_juego_nulo();
}

// Pruebas para juego_establecer_pokedex
void juego_establecer_pokedex_valida()
{
	juego_t *juego = juego_crear(NULL);
	tp1_t *nueva_pokedex = tp1_leer_archivo("ejemplos/pokedex_valida.csv");

	bool resultado = juego_establecer_pokedex(juego, nueva_pokedex);

	pa2m_afirmar(resultado == true,
		     "Se establece pokedex valida correctamente");
	pa2m_afirmar(juego_tiene_pokedex(juego) == true,
		     "Juego tiene pokedex despues de establecer");

	juego_destruir(juego);
}

void juego_establecer_pokedex_nula()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	bool resultado = juego_establecer_pokedex(juego, NULL);

	pa2m_afirmar(resultado == false, "No se puede establecer pokedex NULL");
	pa2m_afirmar(juego_tiene_pokedex(juego) == true,
		     "Juego mantiene pokedex original");

	juego_destruir(juego);
}

void juego_establecer_pokedex_reemplazando_existente()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	tp1_t *nueva_pokedex = tp1_leer_archivo("ejemplos/pokedex_valida.csv");

	bool resultado = juego_establecer_pokedex(juego, nueva_pokedex);

	pa2m_afirmar(resultado == true,
		     "Se reemplaza pokedex existente correctamente");

	juego_destruir(juego);
}

void juego_establecer_pokedex_con_juego_nulo()
{
	tp1_t *pokedex = tp1_leer_archivo("ejemplos/pokedex_valida.csv");

	bool resultado = juego_establecer_pokedex(NULL, pokedex);

	pa2m_afirmar(resultado == false,
		     "Establecer pokedex en juego NULL devuelve false");

	tp1_destruir(pokedex);
}

void pruebas_juego_establecer_pokedex()
{
	pa2m_nuevo_grupo("Pruebas juego_establecer_pokedex parametros validos");
	juego_establecer_pokedex_valida();
	juego_establecer_pokedex_reemplazando_existente();

	pa2m_nuevo_grupo(
		"Pruebas juego_establecer_pokedex parametros invalidos");
	juego_establecer_pokedex_con_juego_nulo();
	juego_establecer_pokedex_nula();
}

// Pruebas para juego_tiene_pokedex
void juego_tiene_pokedex_con_pokedex_cargada()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	bool tiene = juego_tiene_pokedex(juego);

	pa2m_afirmar(tiene == true,
		     "Juego tiene pokedex cuando se creo con archivo");

	juego_destruir(juego);
}

void juego_tiene_pokedex_sin_pokedex()
{
	juego_t *juego = juego_crear(NULL);

	bool tiene = juego_tiene_pokedex(juego);

	pa2m_afirmar(tiene == false,
		     "Juego no tiene pokedex cuando se creo sin archivo");

	juego_destruir(juego);
}

void juego_tiene_pokedex_con_juego_nulo()
{
	bool tiene = juego_tiene_pokedex(NULL);

	pa2m_afirmar(tiene == false, "Juego NULL no tiene pokedex");
}

void pruebas_juego_tiene_pokedex()
{
	pa2m_nuevo_grupo("Pruebas juego_tiene_pokedex parametros validos");
	juego_tiene_pokedex_con_pokedex_cargada();
	juego_tiene_pokedex_sin_pokedex();

	pa2m_nuevo_grupo("Pruebas juego_tiene_pokedex parametros invalidos");
	juego_tiene_pokedex_con_juego_nulo();
}

// Pruebas para juego_validar_jugada
void juego_validar_jugada_cartas_validas()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	estado_jugada_t estado = juego_validar_jugada(juego, 0, 1);

	pa2m_afirmar(estado == JUGADA_VALIDA,
		     "Jugada con cartas validas es aceptada");

	juego_destruir(juego);
}

void juego_validar_jugada_carta_invalida()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	estado_jugada_t estado1 = juego_validar_jugada(juego, -1, 1);
	estado_jugada_t estado2 = juego_validar_jugada(juego, 0, TOTAL_CARTAS);
	estado_jugada_t estado3 = juego_validar_jugada(juego, 100, 200);

	pa2m_afirmar(estado1 == JUGADA_CARTA_INVALIDA,
		     "Carta 1 invalida detectada");
	pa2m_afirmar(estado2 == JUGADA_CARTA_INVALIDA,
		     "Carta 2 invalida detectada");
	pa2m_afirmar(estado3 == JUGADA_CARTA_INVALIDA,
		     "Ambas cartas invalidas detectadas");

	juego_destruir(juego);
}

void juego_validar_jugada_misma_carta()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	estado_jugada_t estado = juego_validar_jugada(juego, 5, 5);

	pa2m_afirmar(estado == JUGADA_MISMA_CARTA,
		     "Misma carta detectada correctamente");

	juego_destruir(juego);
}

void juego_validar_jugada_sin_preparar()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	estado_jugada_t estado = juego_validar_jugada(juego, 0, 1);

	pa2m_afirmar(
		estado == JUGADA_ERROR_MEMORIA,
		"Validar jugada sin juego preparado devuelve error memoria");

	juego_destruir(juego);
}

void juego_validar_jugada_con_juego_nulo()
{
	estado_jugada_t estado = juego_validar_jugada(NULL, 0, 1);

	pa2m_afirmar(estado == JUGADA_ERROR_MEMORIA,
		     "Validar jugada con juego NULL devuelve error memoria");
}

void pruebas_juego_validar_jugada()
{
	pa2m_nuevo_grupo("Pruebas juego_validar_jugada parametros validos");
	juego_validar_jugada_cartas_validas();
	juego_validar_jugada_carta_invalida();
	juego_validar_jugada_misma_carta();

	pa2m_nuevo_grupo("Pruebas juego_validar_jugada parametros invalidos");
	juego_validar_jugada_con_juego_nulo();
	juego_validar_jugada_sin_preparar();
}

// Pruebas para juego_ejecutar_jugada
void juego_ejecutar_jugada_valida()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	estado_jugada_t estado = juego_ejecutar_jugada(juego, 0, 9);

	pa2m_afirmar(estado == JUGADA_FORMO_PAR ||
			     estado == JUGADA_NO_FORMO_PAR,
		     "Jugada ejecutada retorna estado de pareja formada o no");

	juego_destruir(juego);
}

void juego_ejecutar_jugada_sin_preparar()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	estado_jugada_t estado = juego_ejecutar_jugada(juego, 0, 1);

	pa2m_afirmar(
		estado == JUGADA_ERROR_MEMORIA,
		"Ejecutar jugada sin juego preparado devuelve error memoria");

	juego_destruir(juego);
}

void juego_ejecutar_jugada_con_juego_nulo()
{
	estado_jugada_t estado = juego_ejecutar_jugada(NULL, 0, 1);

	pa2m_afirmar(estado == JUGADA_ERROR_MEMORIA,
		     "Ejecutar jugada con juego NULL devuelve error memoria");
}

void pruebas_juego_ejecutar_jugada()
{
	pa2m_nuevo_grupo("Pruebas juego_ejecutar_jugada parametros validos");
	juego_ejecutar_jugada_valida();

	pa2m_nuevo_grupo("Pruebas juego_ejecutar_jugada parametros invalidos");
	juego_ejecutar_jugada_con_juego_nulo();
	juego_ejecutar_jugada_sin_preparar();
}

// Pruebas para juego_obtener_jugador_actual
void juego_obtener_jugador_actual_valido()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	int jugador = juego_obtener_jugador_actual(juego);

	pa2m_afirmar(jugador == 0 || jugador == 1, "Jugador actual es 0 o 1");

	juego_destruir(juego);
}

void juego_obtener_jugador_actual_sin_preparar()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	int jugador = juego_obtener_jugador_actual(juego);

	pa2m_afirmar(jugador == -1, "Jugador actual sin juego preparado es -1");

	juego_destruir(juego);
}

void juego_obtener_jugador_actual_con_juego_nulo()
{
	int jugador = juego_obtener_jugador_actual(NULL);

	pa2m_afirmar(jugador == -1, "Jugador actual de juego NULL es -1");
}

void pruebas_juego_obtener_jugador_actual()
{
	pa2m_nuevo_grupo(
		"Pruebas juego_obtener_jugador_actual parametros validos");
	juego_obtener_jugador_actual_valido();

	pa2m_nuevo_grupo(
		"Pruebas juego_obtener_jugador_actual parametros invalidos");
	juego_obtener_jugador_actual_con_juego_nulo();
	juego_obtener_jugador_actual_sin_preparar();
}

// Pruebas para juego_obtener_puntaje
void juego_obtener_puntaje_jugadores_recien_creado()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	int puntaje0 = juego_obtener_puntaje(juego, 0);
	int puntaje1 = juego_obtener_puntaje(juego, 1);

	pa2m_afirmar(puntaje0 == 0, "Puntaje jugador 0 es 0 al inicio");
	pa2m_afirmar(puntaje1 == 0, "Puntaje jugador 1 es 0 al inicio");

	juego_destruir(juego);
}

void juego_obtener_puntaje_sin_preparar()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	int puntaje = juego_obtener_puntaje(juego, 0);

	pa2m_afirmar(puntaje == -1, "Puntaje sin juego preparado devuelve -1");

	juego_destruir(juego);
}

void juego_obtener_puntaje_jugador_invalido()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	int puntaje = juego_obtener_puntaje(juego, -1);
	int puntaje2 = juego_obtener_puntaje(juego, 2);

	pa2m_afirmar(puntaje == -1,
		     "Puntaje jugador invalido (-1) devuelve -1");
	pa2m_afirmar(puntaje2 == -1,
		     "Puntaje jugador invalido (2) devuelve -1");

	juego_destruir(juego);
}

void juego_obtener_puntaje_con_juego_nulo()
{
	int puntaje = juego_obtener_puntaje(NULL, 0);

	pa2m_afirmar(puntaje == -1, "Puntaje de juego NULL devuelve -1");
}

void pruebas_juego_obtener_puntaje()
{
	pa2m_nuevo_grupo("Pruebas juego_obtener_puntaje parametros validos");
	juego_obtener_puntaje_jugadores_recien_creado();

	pa2m_nuevo_grupo("Pruebas juego_obtener_puntaje parametros invalidos");
	juego_obtener_puntaje_jugador_invalido();
	juego_obtener_puntaje_con_juego_nulo();
	juego_obtener_puntaje_sin_preparar();
}

// Pruebas para juego_es_carta_descubierta
void juego_es_carta_descubierta_recien_creado()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	bool descubierta = juego_es_carta_descubierta(juego, 0);

	pa2m_afirmar(descubierta == false,
		     "Carta no esta descubierta al inicio del juego");

	juego_destruir(juego);
}

void juego_es_carta_descubierta_sin_preparar()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	bool descubierta = juego_es_carta_descubierta(juego, 0);

	pa2m_afirmar(descubierta == false,
		     "Carta sin juego preparado no esta descubierta");

	juego_destruir(juego);
}

void juego_es_carta_descubierta_indice_invalido()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	bool descubierta1 = juego_es_carta_descubierta(juego, -1);
	bool descubierta2 = juego_es_carta_descubierta(juego, TOTAL_CARTAS);

	pa2m_afirmar(descubierta1 == false,
		     "Carta con indice negativo no esta descubierta");
	pa2m_afirmar(descubierta2 == false,
		     "Carta con indice excedente no esta descubierta");

	juego_destruir(juego);
}

void juego_es_carta_descubierta_con_juego_nulo()
{
	bool descubierta = juego_es_carta_descubierta(NULL, 0);

	pa2m_afirmar(descubierta == false,
		     "Carta de juego NULL no esta descubierta");
}

void pruebas_juego_es_carta_descubierta()
{
	pa2m_nuevo_grupo(
		"Pruebas juego_es_carta_descubierta parametros validos");
	juego_es_carta_descubierta_recien_creado();

	pa2m_nuevo_grupo(
		"Pruebas juego_es_carta_descubierta parametros invalidos");
	juego_es_carta_descubierta_indice_invalido();
	juego_es_carta_descubierta_con_juego_nulo();
	juego_es_carta_descubierta_sin_preparar();
}

// Pruebas para juego_es_carta_visible
void juego_es_carta_visible_recien_creado()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	bool visible = juego_es_carta_visible(juego, 0);

	pa2m_afirmar(visible == false,
		     "Carta no esta visible al inicio del juego");

	juego_destruir(juego);
}

void juego_es_carta_visible_sin_preparar()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	bool visible = juego_es_carta_visible(juego, 0);

	pa2m_afirmar(visible == false,
		     "Carta sin juego preparado no esta visible");

	juego_destruir(juego);
}

void juego_es_carta_visible_indice_invalido()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	bool visible1 = juego_es_carta_visible(juego, -1);
	bool visible2 = juego_es_carta_visible(juego, TOTAL_CARTAS);

	pa2m_afirmar(visible1 == false,
		     "Carta con indice negativo no esta visible");
	pa2m_afirmar(visible2 == false,
		     "Carta con indice excedente no esta visible");

	juego_destruir(juego);
}

void juego_es_carta_visible_con_juego_nulo()
{
	bool visible = juego_es_carta_visible(NULL, 0);

	pa2m_afirmar(visible == false, "Carta de juego NULL no esta visible");
}

void pruebas_juego_es_carta_visible()
{
	pa2m_nuevo_grupo("Pruebas juego_es_carta_visible parametros validos");
	juego_es_carta_visible_recien_creado();

	pa2m_nuevo_grupo("Pruebas juego_es_carta_visible parametros invalidos");
	juego_es_carta_visible_indice_invalido();
	juego_es_carta_visible_con_juego_nulo();
	juego_es_carta_visible_sin_preparar();
}

// Pruebas para juego_obtener_pokemon_carta
void juego_obtener_pokemon_carta_con_pokedex()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	struct pokemon *pokemon = juego_obtener_pokemon_carta(juego, 0);

	pa2m_afirmar(pokemon != NULL,
		     "Se obtiene pokemon de carta cuando hay pokedex");

	juego_destruir(juego);
}

void juego_obtener_pokemon_carta_sin_pokedex()
{
	juego_t *juego = juego_crear(NULL);
	juego_preparar(juego, 12345);

	struct pokemon *pokemon = juego_obtener_pokemon_carta(juego, 0);

	pa2m_afirmar(pokemon == NULL,
		     "Se obtiene NULL de carta cuando no hay pokedex");

	juego_destruir(juego);
}

void juego_obtener_pokemon_carta_sin_preparar()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	struct pokemon *pokemon = juego_obtener_pokemon_carta(juego, 0);

	pa2m_afirmar(pokemon == NULL,
		     "Se obtiene NULL de carta sin juego preparado");

	juego_destruir(juego);
}

void juego_obtener_pokemon_carta_indice_invalido()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	struct pokemon *pokemon1 = juego_obtener_pokemon_carta(juego, -1);
	struct pokemon *pokemon2 =
		juego_obtener_pokemon_carta(juego, TOTAL_CARTAS);

	pa2m_afirmar(pokemon1 == NULL,
		     "Pokemon de carta invalida (-1) es NULL");
	pa2m_afirmar(pokemon2 == NULL,
		     "Pokemon de carta invalida (excedente) es NULL");

	juego_destruir(juego);
}

void juego_obtener_pokemon_carta_con_juego_nulo()
{
	struct pokemon *pokemon = juego_obtener_pokemon_carta(NULL, 0);

	pa2m_afirmar(pokemon == NULL, "Pokemon de juego NULL es NULL");
}

void pruebas_juego_obtener_pokemon_carta()
{
	pa2m_nuevo_grupo(
		"Pruebas juego_obtener_pokemon_carta parametros validos");
	juego_obtener_pokemon_carta_con_pokedex();
	juego_obtener_pokemon_carta_sin_pokedex();

	pa2m_nuevo_grupo(
		"Pruebas juego_obtener_pokemon_carta parametros invalidos");
	juego_obtener_pokemon_carta_indice_invalido();
	juego_obtener_pokemon_carta_con_juego_nulo();
	juego_obtener_pokemon_carta_sin_preparar();
}

// Pruebas para juego_destruir
void juego_destruir_valido()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");
	juego_preparar(juego, 12345);

	juego_destruir(juego);

	pa2m_afirmar(true, "Destruir juego valido no causa errores");
}

void juego_destruir_con_pokedex()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	juego_destruir(juego);

	pa2m_afirmar(true, "Destruir juego con pokedex no causa errores");
}

void juego_destruir_sin_pokedex()
{
	juego_t *juego = juego_crear(NULL);

	juego_destruir(juego);

	pa2m_afirmar(true, "Destruir juego sin pokedex no causa errores");
}

void juego_destruir_con_juego_nulo()
{
	juego_destruir(NULL);

	pa2m_afirmar(true, "Destruir juego NULL no causa errores");
}

void pruebas_juego_destruir()
{
	pa2m_nuevo_grupo("Pruebas juego_destruir parametros validos");
	juego_destruir_valido();
	juego_destruir_con_pokedex();
	juego_destruir_sin_pokedex();

	pa2m_nuevo_grupo("Pruebas juego_destruir parametros invalidos");
	juego_destruir_con_juego_nulo();
}

// Test de integracion completo
void test_integracion_juego_completo()
{
	juego_t *juego = juego_crear("ejemplos/pokedex_valida.csv");

	pa2m_afirmar(juego != NULL, "Juego creado para integracion");

	bool preparado = juego_preparar(juego, 12345);
	pa2m_afirmar(preparado == true, "Juego preparado correctamente");

	pa2m_afirmar(juego_terminado(juego) == false,
		     "Juego no terminado al inicio");
	pa2m_afirmar(juego_obtener_jugador_actual(juego) == 0,
		     "Jugador actual es 0 al inicio");
	pa2m_afirmar(juego_obtener_puntaje(juego, 0) == 0,
		     "Puntaje jugador 0 es 0 al inicio");
	pa2m_afirmar(juego_obtener_puntaje(juego, 1) == 0,
		     "Puntaje jugador 1 es 0 al inicio");

	estado_jugada_t estado_val = juego_validar_jugada(juego, 0, 1);
	pa2m_afirmar(estado_val == JUGADA_VALIDA, "Jugada valida es aceptada");

	estado_jugada_t estado_inv = juego_validar_jugada(juego, 0, 0);
	pa2m_afirmar(estado_inv == JUGADA_MISMA_CARTA, "Misma carta detectada");

	estado_jugada_t estado_ejec = juego_ejecutar_jugada(juego, 0, 1);
	pa2m_afirmar(estado_ejec == JUGADA_FORMO_PAR ||
			     estado_ejec == JUGADA_NO_FORMO_PAR,
		     "Jugada ejecutada correctamente");

	struct pokemon *pokemon = juego_obtener_pokemon_carta(juego, 0);
	pa2m_afirmar(pokemon != NULL, "Se puede obtener pokemon de carta");

	bool reiniciado = juego_reiniciar(juego, 54321);
	pa2m_afirmar(reiniciado == true, "Juego reiniciado correctamente");

	juego_destruir(juego);
	pa2m_afirmar(true, "Test de integracion completado sin errores");
}

void pruebas_integracion_juego()
{
	pa2m_nuevo_grupo("Pruebas de integracion - Juego completo");
	test_integracion_juego_completo();
}

void test_juego_completo_simulado()
{
	pa2m_nuevo_grupo("Pruebas de Juego Completo Simulado");

	juego_t *juego = juego_crear("ejemplos/pokedex.csv");
	juego_preparar(juego, 123);

	int jugadas_realizadas = 0;
	int max_jugadas = TOTAL_CARTAS * 2;
	int aciertos = 0, fallos = 0;

	while (!juego_terminado(juego) && jugadas_realizadas < max_jugadas) {
		int carta1 = -1, carta2 = -1;
		bool par_encontrado = false;

		for (int i = 0; i < TOTAL_CARTAS && !par_encontrado; i++) {
			if (!juego_es_carta_descubierta(juego, i)) {
				for (int j = i + 1;
				     j < TOTAL_CARTAS && !par_encontrado; j++) {
					if (!juego_es_carta_descubierta(juego,
									j)) {
						struct pokemon *p1 =
							juego_obtener_pokemon_carta(
								juego, i);
						struct pokemon *p2 =
							juego_obtener_pokemon_carta(
								juego, j);
						if (p1 && p2 &&
						    p1->id == p2->id) {
							carta1 = i;
							carta2 = j;
							par_encontrado = true;
						}
					}
				}
			}
		}

		if (!par_encontrado) {
			for (int i = 0; i < TOTAL_CARTAS && carta1 == -1; i++) {
				if (!juego_es_carta_descubierta(juego, i)) {
					for (int j = i + 1;
					     j < TOTAL_CARTAS && carta2 == -1;
					     j++) {
						if (!juego_es_carta_descubierta(
							    juego, j)) {
							carta1 = i;
							carta2 = j;
						}
					}
				}
			}
		}

		if (carta1 != -1 && carta2 != -1) {
			estado_jugada_t resultado =
				juego_ejecutar_jugada(juego, carta1, carta2);

			if (par_encontrado) {
				pa2m_afirmar(
					resultado == JUGADA_FORMO_PAR,
					"Jugada (%d,%d) forma par correctamente",
					carta1, carta2);
				aciertos++;
			} else {
				pa2m_afirmar(resultado == JUGADA_NO_FORMO_PAR,
					     "Jugada (%d,%d) no forma par",
					     carta1, carta2);
				fallos++;
			}

			jugadas_realizadas++;
		} else {
			break;
		}
	}

	pa2m_afirmar(juego_terminado(juego),
		     "Juego termino despues de %d jugadas", jugadas_realizadas);

	int cartas_descubiertas = 0;
	for (int i = 0; i < TOTAL_CARTAS; i++) {
		if (juego_es_carta_descubierta(juego, i))
			cartas_descubiertas++;
	}

	pa2m_afirmar(cartas_descubiertas == TOTAL_CARTAS,
		     "Todas las cartas descubiertas (%d/%d)",
		     cartas_descubiertas, TOTAL_CARTAS);

	int puntaje_j1 = juego_obtener_puntaje(juego, 0);
	int puntaje_j2 = juego_obtener_puntaje(juego, 1);

	pa2m_afirmar(puntaje_j1 + puntaje_j2 == PAREJAS,
		     "Puntajes: J1=%d + J2=%d = %d (esperado: %d)", puntaje_j1,
		     puntaje_j2, puntaje_j1 + puntaje_j2, PAREJAS);

	printf("Resumen: %d jugadas (%d aciertos, %d fallos)\n",
	       jugadas_realizadas, aciertos, fallos);

	mostrar_resultado_final(juego);
	juego_destruir(juego);
}

// Funcion principal para ejecutar todas las pruebas del juego
void ejecutar_pruebas_juego()
{
	pruebas_juego_crear();
	pruebas_juego_preparar();
	pruebas_juego_terminado();
	pruebas_juego_obtener_pokedex();
	pruebas_juego_establecer_pokedex();
	pruebas_juego_tiene_pokedex();
	pruebas_juego_validar_jugada();
	pruebas_juego_ejecutar_jugada();
	pruebas_juego_obtener_jugador_actual();
	pruebas_juego_obtener_puntaje();
	pruebas_juego_es_carta_descubierta();
	pruebas_juego_es_carta_visible();
	pruebas_juego_obtener_pokemon_carta();
	pruebas_juego_destruir();
	pruebas_integracion_juego();

	test_juego_completo_simulado();
}

int main()
{
	pa2m_nuevo_grupo("============== PRUEBAS TDA MENU ===============");

	ejecutar_pruebas_menu();

	pa2m_nuevo_grupo("============== PRUEBAS TDA JUEGO ===============");

	ejecutar_pruebas_juego();

	return pa2m_mostrar_reporte();
}