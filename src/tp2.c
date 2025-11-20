#include "tp2.h"
#include "menu.h"
#include "ansi.h"
#include "tp2_menu_acciones.h"

#include <stdio.h>

struct tp2{
    menu_t* menu_tp2;
    juego_t* juego;
};

void estilo_titulo_pokemon(char *titulo) {
    printf(ANSI_COLOR_RED ANSI_COLOR_BOLD);
    printf("%s", titulo);
    printf(ANSI_COLOR_RESET "\n");
}

void estilo_panel_verde_retro(char tecla, char *texto) {
    printf(ANSI_COLOR_GREEN "(" 
           ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "%c" 
           ANSI_COLOR_RESET ANSI_COLOR_GREEN ") " 
           ANSI_COLOR_RESET "%s\n",
           tecla, texto);
}

void estilo_panel_azul(char tecla, char *texto) {
    printf(ANSI_BG_BLUE ANSI_COLOR_BOLD ANSI_COLOR_WHITE
           "[ %c ]"
           ANSI_COLOR_RESET " %s\n",
           tecla, texto);
}

void estilo_cyber_panel(char tecla, char *texto) {
    printf(ANSI_BG_BLUE ANSI_COLOR_BOLD ANSI_COLOR_CYAN
           " %c "
           ANSI_COLOR_RESET " %s\n",
           tecla, texto);
}

static bool agregar_accion_o_fallar(menu_t *menu_raiz,
                                    menu_t *menu_destino,
                                    char tecla,
                                    const char *texto,
                                    bool (*accion)(void *))
{
    if (!menu_agregar_accion(menu_destino, tecla, texto, accion)) {
        menu_destruir_todo(menu_raiz);
        return false;
    }
    return true;
}

bool inicializar_menu_principal(menu_t *menu_principal)
{
    if (!agregar_accion_o_fallar(menu_principal, menu_principal,
                                 'C', "Cargar archivo", accion_cargar_archivo))
        return false;

    if (!agregar_accion_o_fallar(menu_principal, menu_principal,
                                 'J', "Jugar", accion_jugar))
        return false;

    if (!agregar_accion_o_fallar(menu_principal, menu_principal,
                                 'S', "Jugar con semilla", accion_jugar_con_semilla))
        return false;

    return true;
}

menu_t *crear_submenu_buscar(menu_t *menu_principal)
{
    menu_t *menu_buscar = menu_crear_submenu(menu_principal, 'B', "Buscar");
    if (!menu_buscar) {
        menu_destruir_todo(menu_principal);
        return NULL;
    }

    if (!agregar_accion_o_fallar(menu_principal, menu_buscar,
                                 'N', "Buscar por nombre", accion_buscar_por_nombre))
        return NULL;

    if (!agregar_accion_o_fallar(menu_principal, menu_buscar,
                                 'I', "Buscar por ID", accion_buscar_por_id))
        return NULL;

    return menu_buscar;
}

menu_t *crear_submenu_mostrar(menu_t *menu_principal)
{
    menu_t *menu_mostrar = menu_crear_submenu(menu_principal, 'M', "Mostrar");
    if (!menu_mostrar) {
        menu_destruir_todo(menu_principal);
        return NULL;
    }

    if (!agregar_accion_o_fallar(menu_principal, menu_mostrar,
                                 'N', "Mostrar por nombre", accion_mostrar_por_nombre))
        return NULL;

    if (!agregar_accion_o_fallar(menu_principal, menu_mostrar,
                                 'I', "Mostrar por ID", accion_mostrar_por_id))
        return NULL;

    return menu_mostrar;
}

menu_t *inicializar_menu_tp2()
{
    menu_t *menu_principal = menu_crear_base(
        "Menu principal TP2 - Pokedex",
        estilo_cyber_panel,estilo_titulo_pokemon
    );
    if (!menu_principal)
        return NULL;

    if (!menu_agregar_estilo(menu_principal,estilo_panel_azul,estilo_titulo_pokemon)){
        free(menu_principal);
        return NULL;
    }

    if (!menu_agregar_estilo(menu_principal,estilo_panel_verde_retro,estilo_titulo_pokemon)){
        menu_destruir_todo(menu_principal);
        return NULL;
    }

    if (!inicializar_menu_principal(menu_principal))
        return NULL;

    if (!crear_submenu_buscar(menu_principal))
        return NULL;

    if (!crear_submenu_mostrar(menu_principal))
        return NULL;

    return menu_principal;
}

tp2_t *tp2_crear(const char *nombre_archivo){
    tp2_t *tp2=calloc(1,sizeof(tp2_t));
    if (!tp2) return NULL;

    tp2->menu_tp2=inicializar_menu_tp2();
    if (!tp2->menu_tp2){
        free(tp2);
        return NULL;
    }

    tp2->juego=juego_crear(nombre_archivo);
    if (!tp2->juego){
        printf("Error al crear el juego\n");
        menu_destruir_todo(tp2->menu_tp2);
        free(tp2);
        return NULL;
    }

    return tp2;
}

char obtener_tecla_segura() {
    char tecla = '\0';
    // CORREGIDO: Verificar retorno de scanf
    if (scanf(" %c", &tecla) != 1) {
        return '\0';
    }
    return tecla;
}

bool ejecutar_menu_interactivo(menu_t* menu, juego_t* juego) {
    menu_navegador_t *nav = menu_navegador_crear(menu, juego);
    if (!nav) {
        return false;
    }

    while (!menu_navegador_esta_terminado(nav)) {
        limpiar_pantalla();
        menu_navegador_mostrar(nav);
        char tecla = obtener_tecla_segura();
        if (tecla == '\0') {
            menu_navegador_destruir(nav);
            return false;
        }

        limpiar_buffer();
        menu_navegacion_estado_t estado = menu_navegador_procesar_tecla(nav, tecla);
        
        if (estado == MENU_NAVEGACION_ERROR) {
            menu_navegador_destruir(nav);
            return false;
        }
        
        if (estado == MENU_NAVEGACION_TERMINAR) {
            menu_navegador_destruir(nav);
            return true;
        }
    }
    
    menu_navegador_destruir(nav);
    return true;
}

tp2_t *tp2_ejecutar(tp2_t *tp2){
    if (!tp2) return NULL;

    if (!ejecutar_menu_interactivo(tp2->menu_tp2, tp2->juego)) return NULL;

    return tp2;
}

void tp2_destruir_todo(tp2_t *tp2){
    if (!tp2) return;

    menu_destruir_todo(tp2->menu_tp2);
    juego_destruir(tp2->juego);
    free(tp2);
}