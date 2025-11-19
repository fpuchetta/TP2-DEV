#include "pa2m.h"
#include "src/menu.h" // Asumiendo que guardaste el navegador en este archivo
#include <stdio.h>
#include <string.h>
#include "src/functions.h"

// Función segura para obtener tecla
char obtener_tecla_segura() {
    char tecla = '\0';
    // CORREGIDO: Verificar retorno de scanf
    if (scanf(" %c", &tecla) != 1) {
        return '\0';
    }
    return tecla;
}

// Acciones de prueba
bool accion_listar_pokemon(void *user_data) {
    printf("\n🎯 Listando Pokémon...\n");
    printf("1. Pikachu\n2. Charizard\n3. Bulbasaur\n");
    return true;
}

bool accion_buscar_pokemon(void *user_data) {  
    printf("\n🔍 Buscando Pokémon...\n");
    printf("Ingrese nombre: ");
    char nombre[50];
    
    // CORREGIDO: Verificar retorno de scanf
    if (scanf("%49s", nombre) != 1) {
        printf("Error leyendo nombre\n");
        return false;
    }
    
    printf("Buscando: %s\n", nombre);
    limpiar_buffer();
    return true;
}

bool accion_configurar_audio(void *user_data) { 
    printf("\n🔊 Configurando audio...\n");
    printf("Volumen: 80%%\nEfectos: Activados\nMúsica: Activada\n");
    return true;
}

bool accion_configurar_video(void *user_data) {   
    printf("\n🎥 Configurando video...\n");
    printf("Resolución: 1920x1080\nFullscreen: Sí\nCalidad: Alta\n");
    return true;
}

bool accion_guardar_partida(void *user_data) { 
    printf("\n💾 Guardando partida...\n");
    printf("Partida guardada exitosamente!\n");
    return true;
}

bool accion_cargar_partida(void *user_data) { 
    printf("\n📂 Cargando partida...\n");
    printf("Partida cargada exitosamente!\n");
    return true;
}

bool accion_salir_juego(void *user_data) {   
    printf("\n👋 Saliendo del juego...\n");
    return true;
}

// Estilos de prueba
void mostrar_estilo_simple(char tecla, char *nombre) {
    printf("%c) %s\n", tecla, nombre);
}

void mostrar_estilo_detallado(char tecla, char *nombre) {
    printf("• [%c] %s\n", tecla, nombre);
}

void mostrar_estilo_emoji(char tecla, char *nombre) {
    const char *emoji = "⚡";
    if (tecla == 'L') emoji = "📋";
    if (tecla == 'B') emoji = "🔍";
    if (tecla == 'C') emoji = "⚙️";
    if (tecla == 'G') emoji = "💾";
    printf("%s %c - %s\n", emoji, tecla, nombre);
}

// Crear menú completo de prueba
menu_t *crear_menu_prueba() {
    // Menú principal
    menu_t *menu_principal = menu_crear_base("MENU PRINCIPAL POKÉMON", mostrar_estilo_simple);
    if (!menu_principal) return NULL;
    
    // Agregar estilos adicionales
    menu_agregar_estilo(menu_principal, mostrar_estilo_detallado);
    menu_agregar_estilo(menu_principal, mostrar_estilo_emoji);
    
    // Agregar acciones al menú principal
    menu_agregar_accion(menu_principal, 'L', "Listar Pokémon", accion_listar_pokemon);
    menu_agregar_accion(menu_principal, 'B', "Buscar Pokémon", accion_buscar_pokemon);
    menu_agregar_accion(menu_principal, 'G', "Guardar Partida", accion_guardar_partida);
    menu_agregar_accion(menu_principal, 'R', "Cargar Partida", accion_cargar_partida);
    
    // Crear submenú de configuración
    menu_t *submenu_config = menu_crear_submenu(menu_principal, 'C', "CONFIGURACIÓN");
    if (submenu_config) {
        menu_agregar_accion(submenu_config, 'X', "Configurar Audio", accion_configurar_audio);
        menu_agregar_accion(submenu_config, 'V', "Configurar Video", accion_configurar_video);
    }
    
    // Agregar acción de salir al final
    menu_agregar_accion(menu_principal, 'S', "Salir del Juego", accion_salir_juego);
    
    return menu_principal;
}

// Test básico del navegador
void test_navegador_basico() {
    pa2m_nuevo_grupo("Pruebas básicas del navegador");
    
    menu_t *menu = crear_menu_prueba();
    pa2m_afirmar(menu != NULL, "Se crea el menú de prueba");
    
    menu_navegador_t *nav = menu_navegador_crear(menu, NULL);
    pa2m_afirmar(nav != NULL, "Se crea el navegador");
    
    // Verificar estado inicial
    menu_t *actual = menu_navegador_obtener_actual(nav);
    pa2m_afirmar(actual == menu, "El menú actual es el principal");
    pa2m_afirmar(!menu_navegador_esta_terminado(nav), "El navegador no está terminado inicialmente");
    
    menu_navegador_destruir(nav);
    menu_destruir_todo(menu);
}

// Ejecución interactiva del menú
void ejecutar_menu_interactivo(menu_t* menu) {
    printf("🎮 INICIANDO PRUEBA INTERACTIVA DEL MENÚ POKÉMON\n");
    printf("=============================================\n\n");

    // Usar el navegador directamente
    menu_navegador_t *nav = menu_navegador_crear(menu, NULL);
    if (!nav) {
        printf("❌ Error creando navegador\n");
        menu_destruir_todo(menu);
        return;
    }
    
    // Loop principal de navegación
    while (!menu_navegador_esta_terminado(nav)) {
        limpiar_pantalla();
        
        // Mostrar menú actual usando el navegador
        menu_navegador_mostrar(nav);
        
        // Obtener tecla del usuario
        //printf(""); // El "Opción: " ya lo pone menu_navegador_mostrar
        char tecla = obtener_tecla_segura();
        if (tecla == '\0') {
            printf("\n❌ Error leyendo tecla\n");
            break;
        }
        limpiar_buffer();
        
        // Procesar tecla con el navegador
        menu_navegacion_estado_t resultado = menu_navegador_procesar_tecla(nav, tecla);
        
        // Manejar resultado
        switch (resultado) {
            case MENU_NAVEGACION_CONTINUAR:
                // Continuar normalmente
                break;
                
            case MENU_NAVEGACION_TERMINAR:
                printf("\n🏁 Navegación terminada\n");
                break;
                
            case MENU_NAVEGACION_ERROR:
                printf("\n❌ Error en la navegación\n");
                esperar_enter();
                break;
        }
        
        // Pequeña pausa para que se vea el resultado de las acciones
        if (resultado == MENU_NAVEGACION_CONTINUAR) {
            
        }
    }
    
    printf("\n👋 Prueba del menú completada\n");
    
    menu_navegador_destruir(nav);
    menu_destruir_todo(menu);
}

// Test de navegación programática
void test_navegacion_automatica() {
    pa2m_nuevo_grupo("Prueba de navegación automática");
    
    menu_t *menu = crear_menu_prueba();
    menu_navegador_t *nav = menu_navegador_crear(menu, NULL);
    
    // Simular navegación: Ir a configuración
    menu_navegacion_estado_t res1 = menu_navegador_procesar_tecla(nav, 'C');
    pa2m_afirmar(res1 == MENU_NAVEGACION_CONTINUAR, "Navegación a submenu configuración");
    
    menu_t *submenu = menu_navegador_obtener_actual(nav);
    pa2m_afirmar(submenu != menu, "Se cambió al submenu de configuración");
    
    // Volver al menú principal
    menu_navegacion_estado_t res2 = menu_navegador_procesar_tecla(nav, 'A');
    pa2m_afirmar(res2 == MENU_NAVEGACION_CONTINUAR, "Volver al menú principal");
    
    menu_t *principal = menu_navegador_obtener_actual(nav);
    pa2m_afirmar(principal == menu, "Se volvió al menú principal (%p = %p)",principal,menu);
    
    // Probar cambio de estilo
    menu_navegacion_estado_t res3 = menu_navegador_procesar_tecla(nav, 'E');
    pa2m_afirmar(res3 == MENU_NAVEGACION_CONTINUAR, "Cambio de estilo exitoso");
    
    // Salir
    menu_navegacion_estado_t res4 = menu_navegador_procesar_tecla(nav, 'Q');
    pa2m_afirmar(res4 == MENU_NAVEGACION_TERMINAR, "Salida del menú");
    pa2m_afirmar(menu_navegador_esta_terminado(nav), "Navegador marcado como terminado");
    
    menu_navegador_destruir(nav);
    menu_destruir_todo(menu);
}

int main() {
    printf("🧪 TESTEO COMPLETO DEL SISTEMA DE MENÚS\n");
    printf("=======================================\n\n");
    
    // Ejecutar pruebas automáticas
    test_navegador_basico();
    test_navegacion_automatica();
    
    printf("\n");
    
    // Ejecutar prueba interactiva
    //ejecutar_menu_interactivo();
    
    return pa2m_mostrar_reporte();
}