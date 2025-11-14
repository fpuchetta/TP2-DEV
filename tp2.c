#include "ansi.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "stdlib.h"

#include "src/menu.h"

typedef struct {
    int contador;
} app_ctx_t;

void accion_saludar(void *ctx_void){
    (void)ctx_void;
    printf("Hola! 👋 Soy una acción del menú.\n");
}

void accion_incrementar(void *ctx_void){
    app_ctx_t *ctx = ctx_void;
    ctx->contador++;
    printf("Contador incrementado. Valor actual: %d\n", ctx->contador);
}

void accion_mostrar_contador(void *ctx_void){
    app_ctx_t *ctx = ctx_void;
    printf("Valor actual del contador: %d\n", ctx->contador);
}

void accion_resetear_contador(void *ctx_void){
    app_ctx_t *ctx = ctx_void;
    ctx->contador = 0;
    printf("Contador reseteado a 0.\n");
}

void estilo_matrix(char tecla, char *nombre){
    printf(ANSI_COLOR_GREEN ">> %c :: %s" ANSI_COLOR_RESET "\n",
           tecla, nombre);
}

void estilo_caja(char tecla, char *nombre){
    printf("  [" ANSI_COLOR_BOLD "%c" ANSI_COLOR_RESET "]  →  %s\n",
           tecla, nombre);
}

void estilo_cyan_neon(char tecla, char *nombre){
    printf("  " ANSI_COLOR_CYAN ANSI_COLOR_BOLD "[%c]" ANSI_COLOR_RESET
           "  " ANSI_COLOR_WHITE "%s" ANSI_COLOR_RESET "\n",
           tecla, nombre);
}

void estilo_bloque_rojo(char tecla, char *nombre){
    printf("  " ANSI_BG_RED ANSI_COLOR_WHITE " %c " ANSI_BG_RESET
           "  " ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET "\n",
           tecla, nombre);
}

void estilo_amarillo_retro(char tecla, char *texto) {
    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "<%c>" ANSI_COLOR_RESET " %s\n",
           tecla, texto);
}

void estilo_cyber_panel(char tecla, char *texto) {
    printf(ANSI_BG_BLUE ANSI_COLOR_BOLD ANSI_COLOR_CYAN
           " %c "
           ANSI_COLOR_RESET " %s\n",
           tecla, texto);
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    app_ctx_t ctx = { .contador = 0 };

    // Crear menú base
    menu_t *menu_principal = menu_crear_base("Menu principal de prueba",estilo_matrix);
    if (!menu_principal) {
        fprintf(stderr, "Error: no se pudo crear el menú principal\n");
        return 1;
    }

    if (!menu_agregar_estilo(menu_principal,estilo_caja)){
        fprintf(stderr, "Error al agregar estilo nuevo.\n");
        return 1;
    }

    if (!menu_agregar_estilo(menu_principal,estilo_bloque_rojo)){
        fprintf(stderr, "Error al agregar estilo nuevo.\n");
        return 1;
    }

    if (!menu_agregar_estilo(menu_principal,estilo_cyan_neon)){
        fprintf(stderr, "Error al agregar estilo nuevo.\n");
        return 1;
    }

    if (!menu_agregar_estilo(menu_principal,estilo_amarillo_retro)){
        fprintf(stderr, "Error al agregar estilo nuevo.\n");
        return 1;
    }
    
    if (!menu_agregar_estilo(menu_principal,estilo_cyber_panel)){
        fprintf(stderr, "Error al agregar estilo nuevo.\n");
        return 1;
    }    

    // Agregar algunas acciones al menú principal
    if (!menu_agregar_accion(menu_principal, 'h', "Saludar", accion_saludar)){
        fprintf(stderr, "Error agregando opción 'h'\n");
        menu_destruir_todo(menu_principal);
        return 1;
    }

    if (!menu_agregar_accion(menu_principal, 'i', "Incrementar contador", accion_incrementar)){
        fprintf(stderr, "Error agregando opción 'i'\n");
        menu_destruir_todo(menu_principal);
        return 1;
    }

    if (!menu_agregar_accion(menu_principal, 'm', "Mostrar contador", accion_mostrar_contador)){
        fprintf(stderr, "Error agregando opción 'm'\n");
        menu_destruir_todo(menu_principal);
        return 1;
    }

    // Crear un submenú para manejar el contador
    menu_t *menu_contador = menu_crear_submenu(menu_principal, 'c', "Menu del contador");
    if (!menu_contador){
        fprintf(stderr, "Error creando submenu 'c'\n");
        menu_destruir_todo(menu_principal);
        return 1;
    }

    // Agregar acciones al submenú
    if (!menu_agregar_accion(menu_contador, 'r', "Resetear contador", accion_resetear_contador)){
        fprintf(stderr, "Error agregando opción 'r' en submenu\n");
        menu_destruir_todo(menu_principal);
        return 1;
    }

    if (!menu_agregar_accion(menu_contador, 'v', "Ver contador", accion_mostrar_contador)){
        fprintf(stderr, "Error agregando opción 'v' en submenu\n");
        menu_destruir_todo(menu_principal);
        return 1;
    }

    printf("Iniciando menú de prueba.\n");
    printf("En el menú raíz deberías ver, además de estas opciones:\n");
    printf(" - [q] Salir (o la tecla que hayas definido para MENU_TECLA_SALIR)\n");
    printf("En el submenu del contador deberías ver:\n");
    printf(" - [b] Volver (o la tecla que hayas definido para MENU_TECLA_VOLVER)\n\n");

    // Ejecutar el menú
    menu_ejecutar(menu_principal, &ctx);

    // Destruir todo el árbol de menús
    menu_destruir_todo(menu_principal);

    return 0;
}