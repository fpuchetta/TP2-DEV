#include "pa2m.h"
#include "juego.h"
#include "tp1.h"
#include <stdio.h>
#include <stdlib.h>

// Helper para crear pokedex mínima para testing
tp1_t* crear_pokedex_minima() {
    tp1_t* pokedex = tp1_leer_archivo("normal.csv");
    // Necesitamos al menos 9 pokémones únicos para las 9 parejas
    // (asumiendo que tu tp1 tiene funciones para agregar)
    return pokedex;
}

void test_creacion_y_destruccion() {
    pa2m_nuevo_grupo("Pruebas de Creación y Destrucción");
    
    // Test 1: Crear juego sin archivo
    juego_t* juego = juego_crear(NULL);
    pa2m_afirmar(juego != NULL, "Juego se crea correctamente sin archivo");
    pa2m_afirmar(!juego_tiene_pokedex(juego), "Juego sin archivo no tiene pokedex");
    juego_destruir(juego);
    
    // Test 2: Crear juego con archivo inexistente
    juego = juego_crear("archivo_inexistente.txt");
    pa2m_afirmar(juego == NULL, "Juego con archivo inexistente devuelve NULL");
    
    // Test 3: Crear y destruir múltiples veces
    juego = juego_crear(NULL);
    juego_t* juego2 = juego_crear(NULL);
    pa2m_afirmar(juego != NULL && juego2 != NULL, "Se pueden crear múltiples juegos");
    juego_destruir(juego);
    juego_destruir(juego2);
}

void test_preparacion_y_reinicio() {
    pa2m_nuevo_grupo("Pruebas de Preparación y Reinicio");
    
    juego_t* juego = juego_crear("normal.csv");
    
    // Test 1: Preparar juego con semilla
    bool preparado = juego_preparar(juego, 1234);
    pa2m_afirmar(preparado, "Juego se prepara correctamente con semilla");
    
    // Test 2: Verificar estado inicial después de preparar
    pa2m_afirmar(juego_obtener_jugador_actual(juego) == 0, "Jugador inicial es 0");
    pa2m_afirmar(juego_obtener_puntaje(juego, 0) == 0, "Puntaje inicial jugador 0 es 0");
    pa2m_afirmar(juego_obtener_puntaje(juego, 1) == 0, "Puntaje inicial jugador 1 es 0");
    pa2m_afirmar(!juego_terminado(juego), "Juego no está terminado al inicio");
    
    // Test 3: Reiniciar juego
    bool reiniciado = juego_reiniciar(juego, 5678);
    pa2m_afirmar(reiniciado, "Juego se reinicia correctamente");
    pa2m_afirmar(juego_obtener_jugador_actual(juego) == 0, "Jugador es 0 después de reinicio");
    pa2m_afirmar(juego_obtener_puntaje(juego, 0) == 0, "Puntaje jugador 0 es 0 después de reinicio");
    
    juego_destruir(juego);
}

void test_validacion_jugadas() {
    pa2m_nuevo_grupo("Pruebas de Validación de Jugadas");
    
    juego_t* juego = juego_crear(NULL);
    juego_preparar(juego, 42);
    
    // Test 1: Jugada válida
    estado_jugada_t resultado = juego_validar_jugada(juego, 0, 1);
    pa2m_afirmar(resultado == JUGADA_VALIDA, "Jugada válida (0,1) es aceptada");
    
    // Test 2: Misma carta
    resultado = juego_validar_jugada(juego, 5, 5);
    pa2m_afirmar(resultado == JUGADA_MISMA_CARTA, "Misma carta (5,5) es rechazada");
    
    // Test 3: Cartas fuera de rango
    resultado = juego_validar_jugada(juego, -1, 5);
    pa2m_afirmar(resultado == JUGADA_CARTA_INVALIDA, "Carta -1 es inválida");
    
    resultado = juego_validar_jugada(juego, 0, 18);
    pa2m_afirmar(resultado == JUGADA_CARTA_INVALIDA, "Carta 18 es inválida");
    
    resultado = juego_validar_jugada(juego, 100, 200);
    pa2m_afirmar(resultado == JUGADA_CARTA_INVALIDA, "Cartas 100,200 son inválidas");
    
    // Test 4: Juego NULL
    resultado = juego_validar_jugada(NULL, 0, 1);
    pa2m_afirmar(resultado == JUGADA_ERROR_MEMORIA, "Juego NULL devuelve inválido");
    
    juego_destruir(juego);
}

void test_ejecucion_jugadas() {
    pa2m_nuevo_grupo("Pruebas de Ejecución de Jugadas");
    
    juego_t* juego = juego_crear("normal.csv");
    juego_preparar(juego, 123); // Semilla fija para reproducibilidad
    
    // Test 1: Ejecutar jugada válida
    estado_jugada_t resultado = juego_ejecutar_jugada(juego, 0, 1);
    pa2m_afirmar(resultado == JUGADA_FORMO_PAR || resultado == JUGADA_NO_FORMO_PAR, 
                "Jugada válida se ejecuta correctamente");
    
    // Test 2: Verificar cambios de estado después de jugada
    if (resultado == JUGADA_FORMO_PAR) {
        pa2m_afirmar(juego_obtener_puntaje(juego, 0) == 1, 
                    "Puntaje aumenta cuando se forma par");
        pa2m_afirmar(juego_es_carta_descubierta(juego, 0) && 
                    juego_es_carta_descubierta(juego, 1),
                    "Cartas se marcan como descubiertas al formar par");
    } else {
        pa2m_afirmar(juego_obtener_puntaje(juego, 0) == 0,
                    "Puntaje no cambia cuando no se forma par");
        pa2m_afirmar(juego_obtener_jugador_actual(juego) == 1,
                    "Turno cambia cuando no se forma par");
    }
    
    // Test 3: Ejecutar múltiples jugadas
    int jugadas_realizadas = 0;
    for (int i = 2; i < TOTAL_CARTAS && jugadas_realizadas < 3; i++) {
        for (int j = i + 1; j < TOTAL_CARTAS && jugadas_realizadas < 3; j++) {
            if (!juego_es_carta_descubierta(juego, i) && 
                !juego_es_carta_descubierta(juego, j)) {
                resultado = juego_ejecutar_jugada(juego, i, j);
                pa2m_afirmar(resultado == JUGADA_FORMO_PAR || resultado == JUGADA_NO_FORMO_PAR,
                            "Jugada adicional se ejecuta correctamente");
                jugadas_realizadas++;
                break;
            }
        }
    }
    
    juego_destruir(juego);
}

void test_estado_cartas() {
    pa2m_nuevo_grupo("Pruebas de Estado de Cartas");
    
    juego_t* juego = juego_crear(NULL);
    juego_preparar(juego, 999);
    
    // Test 1: Estado inicial de cartas
    pa2m_afirmar(!juego_es_carta_descubierta(juego, 0), "Carta 0 no descubierta al inicio");
    pa2m_afirmar(!juego_es_carta_visible(juego, 0), "Carta 0 no visible al inicio");
    pa2m_afirmar(juego_obtener_pokemon_carta(juego, 0) != NULL, "Carta 0 tiene pokemon asociado");
    
    // Test 2: Estado después de descubrir carta
    // Forzar descubrimiento de una carta para testing
    //juego->cuadrilla->celdas[0].encontrada = true;
    pa2m_afirmar(juego_es_carta_descubierta(juego, 0), "Carta 0 marcada como descubierta");
    
    // Test 3: Cartas fuera de rango
    pa2m_afirmar(!juego_es_carta_descubierta(juego, -1), "Carta -1 no existe");
    pa2m_afirmar(!juego_es_carta_visible(juego, 100), "Carta 100 no existe");
    pa2m_afirmar(juego_obtener_pokemon_carta(juego, 50) == NULL, "Carta 50 no existe");
    
    juego_destruir(juego);
}

void test_pokedex() {
    pa2m_nuevo_grupo("Pruebas de Pokedex");
    
    // Test 1: Juego sin pokedex
    juego_t* juego = juego_crear(NULL);
    pa2m_afirmar(juego_obtener_pokedex(juego) == NULL, "Juego sin archivo tiene pokedex NULL");
    pa2m_afirmar(!juego_tiene_pokedex(juego), "juego_tiene_pokedex devuelve false sin pokedex");
    
    // Test 2: Establecer pokedex
    tp1_t* pokedex_test = tp1_leer_archivo("normal.csv");
    bool establecida = juego_establecer_pokedex(juego, pokedex_test);
    pa2m_afirmar(establecida, "Pokedex se establece correctamente");
    pa2m_afirmar(juego_tiene_pokedex(juego), "juego_tiene_pokedex devuelve true con pokedex");
    pa2m_afirmar(juego_obtener_pokedex(juego) == pokedex_test, "Pokedex obtenida es la correcta");
    
    // Test 3: Reemplazar pokedex
    tp1_t* nueva_pokedex = tp1_leer_archivo("normal.csv");
    establecida = juego_establecer_pokedex(juego, nueva_pokedex);
    pa2m_afirmar(establecida, "Pokedex se reemplaza correctamente");
    
    juego_destruir(juego);
    // No destruir pokedex_test ni nueva_pokedex porque el juego se encarga
}

void test_juego_completo_simulado() {
    pa2m_nuevo_grupo("Pruebas de Juego Completo Simulado");
    
    juego_t* juego = juego_crear("normal.csv");
    juego_preparar(juego, 12345);
    
    // Test 1: Simular juego completo mediante jugadas automáticas
    int jugadas_realizadas = 0;
    int max_jugadas = TOTAL_CARTAS * 2; // Límite para evitar loop infinito
    
    while (!juego_terminado(juego) && jugadas_realizadas < max_jugadas) {
        // Encontrar un par de cartas no descubiertas
        int carta1 = -1, carta2 = -1;
        bool par_encontrado = false;
        
        for (int i = 0; i < TOTAL_CARTAS && !par_encontrado; i++) {
            if (!juego_es_carta_descubierta(juego, i)) {
                for (int j = i + 1; j < TOTAL_CARTAS && !par_encontrado; j++) {
                    if (!juego_es_carta_descubierta(juego, j)) {
                        // Verificar si son par
                        struct pokemon* p1 = juego_obtener_pokemon_carta(juego, i);
                        struct pokemon* p2 = juego_obtener_pokemon_carta(juego, j);
                        
                        if (p1 && p2 && p1->id == p2->id) {
                            carta1 = i;
                            carta2 = j;
                            par_encontrado = true;
                        }
                    }
                }
            }
        }
        
        if (par_encontrado) {
            // Ejecutar la jugada que forma par
            estado_jugada_t resultado = juego_ejecutar_jugada(juego, carta1, carta2);
            pa2m_afirmar(resultado == JUGADA_FORMO_PAR, 
                        "Jugada (%d,%d) forma par correctamente", carta1, carta2);
            jugadas_realizadas++;
        } else {
            // Si no encontramos par, hacer una jugada aleatoria (puede fallar)
            for (int i = 0; i < TOTAL_CARTAS && carta1 == -1; i++) {
                if (!juego_es_carta_descubierta(juego, i)) {
                    for (int j = i + 1; j < TOTAL_CARTAS && carta2 == -1; j++) {
                        if (!juego_es_carta_descubierta(juego, j)) {
                            carta1 = i;
                            carta2 = j;
                        }
                    }
                }
            }
            
            if (carta1 != -1 && carta2 != -1) {
                estado_jugada_t resultado = juego_ejecutar_jugada(juego, carta1, carta2);
                pa2m_afirmar(resultado == JUGADA_NO_FORMO_PAR, 
                            "Jugada (%d,%d) no forma par", carta1, carta2);
                jugadas_realizadas++;
            }
        }
    }
    
    // Test 2: Verificar que el juego terminó
    pa2m_afirmar(juego_terminado(juego), "Juego termina después de jugadas simuladas");
    
    // Test 3: Verificar que todas las cartas están descubiertas
    int cartas_descubiertas = 0;
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        if (juego_es_carta_descubierta(juego, i)) {
            cartas_descubiertas++;
        }
    }
    pa2m_afirmar(cartas_descubiertas == TOTAL_CARTAS, 
                "Todas las cartas están descubiertas al finalizar");
    
    // Test 4: Puntajes válidos
    int puntaje_j1 = juego_obtener_puntaje(juego, 0);
    int puntaje_j2 = juego_obtener_puntaje(juego, 1);
    pa2m_afirmar(puntaje_j1 >= 0 && puntaje_j2 >= 0, "Puntajes son no negativos");
    pa2m_afirmar(puntaje_j1 + puntaje_j2 == PAREJAS, 
                "Suma de puntajes (%d + %d = %d) igual a total de pares (%d)", 
                puntaje_j1, puntaje_j2, puntaje_j1 + puntaje_j2, PAREJAS);
    
    juego_destruir(juego);
}

void test_robustez() {
    pa2m_nuevo_grupo("Pruebas de Robustez");
    
    // Test 1: Funciones con juego NULL
    pa2m_afirmar(!juego_preparar(NULL, 123), "preparar con juego NULL devuelve false");
    pa2m_afirmar(!juego_reiniciar(NULL, 123), "reiniciar con juego NULL devuelve false");
    pa2m_afirmar(juego_terminado(NULL), "terminado con juego NULL devuelve true");
    pa2m_afirmar(juego_obtener_pokedex(NULL) == NULL, "obtener_pokedex con juego NULL devuelve NULL");
    pa2m_afirmar(!juego_establecer_pokedex(NULL, NULL), "establecer_pokedex con juego NULL devuelve false");
    pa2m_afirmar(!juego_tiene_pokedex(NULL), "tiene_pokedex con juego NULL devuelve false");
    pa2m_afirmar(juego_obtener_jugador_actual(NULL) == -1, "obtener_jugador_actual con juego NULL devuelve -1");
    pa2m_afirmar(juego_obtener_puntaje(NULL, 0) == -1, "obtener_puntaje con juego NULL devuelve -1");
    pa2m_afirmar(!juego_es_carta_descubierta(NULL, 0), "es_carta_descubierta con juego NULL devuelve false");
    pa2m_afirmar(!juego_es_carta_visible(NULL, 0), "es_carta_visible con juego NULL devuelve false");
    pa2m_afirmar(juego_obtener_pokemon_carta(NULL, 0) == NULL, "obtener_pokemon_carta con juego NULL devuelve NULL");
    
    // Test 2: Destruir juego NULL (no debería fallar)
    juego_destruir(NULL);
    pa2m_afirmar(true, "Destruir juego NULL no causa errores");
}

int main() {
    test_creacion_y_destruccion();
    test_preparacion_y_reinicio();
    test_validacion_jugadas();
    test_ejecucion_jugadas();
    //test_estado_cartas();
    test_pokedex();
    test_juego_completo_simulado();
    //test_robustez();
    
    return pa2m_mostrar_reporte();
}