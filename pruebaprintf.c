#include "src/lista.h"
#include <stdio.h>

#define ANSI_COLOR_BLACK "\x1b[30m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_COLOR_BOLD "\x1b[1m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define ANSI_BG_RED "\x1b[41m"
#define ANSI_BG_GREEN "\x1b[42m"
#define ANSI_BG_YELLOW "\x1b[43m"
#define ANSI_BG_BLUE "\x1b[44m"
#define ANSI_BG_MAGENTA "\x1b[45m"
#define ANSI_BG_CYAN "\x1b[46m"
#define ANSI_BG_WHITE "\x1b[47m"
#define ANSI_BG_RESET "\x1b[0m"

#define COLOR_JUGADOR_1 ANSI_COLOR_CYAN
#define COLOR_JUGADOR_2 ANSI_COLOR_YELLOW

#define COLOR_SELECCIONADO ANSI_COLOR_MAGENTA

#define COLOR_ACIERTO ANSI_COLOR_GREEN
#define COLOR_ERROR ANSI_COLOR_RED

typedef struct movimiento {
    int jugador;        // 0 o 1 (índice del jugador)
    int carta1;         // índice de la primera carta (0-17)
    int carta2;         // índice de la segunda carta (0-17)
    bool acierto;       // true si fue acierto
} movimiento_t;

size_t int_tam(int n)
{
	size_t len = 0;
	if (n == 0)
		return 1;

	while (n > 0) {
		len++;
		n /= 10;
	}
	return len;
}

void mostrar_movimientos(const char* titulo, lista_t* historial, int numero_jugador) {
    printf("%s%s%s\n", ANSI_COLOR_BOLD, titulo, ANSI_COLOR_RESET);

    lista_iterador_t* it = lista_iterador_crear(historial);
    size_t turno = 1;
    while (lista_iterador_hay_mas_elementos(it)) {
        movimiento_t* mov = lista_iterador_obtener_actual(it);

        int c1 = mov->carta1 + 1;
        int c2 = mov->carta2 + 1;
        char letra = mov->acierto ? 'A' : 'F';
        
        // Color para acierto/error
        const char* color_mov = mov->acierto ? COLOR_ACIERTO : COLOR_ERROR;

        printf("%s%zu)%s %d-%d%s%c%s\n", 
               ANSI_COLOR_WHITE, turno, ANSI_COLOR_RESET,
               c1, c2, color_mov, letra, ANSI_COLOR_RESET);

        turno++;
        lista_iterador_siguiente(it);
    }

    lista_iterador_destruir(it);
    printf("\n");
}

void mostrar_resultado_final(lista_t *h1, lista_t *h2) {    
    int puntaje_j1 = 5;
    int puntaje_j2 = 4;

    // Encabezado con colores
    printf("%s%s╔════════════════════════════╗%s\n", ANSI_BG_BLUE, ANSI_COLOR_WHITE, ANSI_COLOR_RESET);
    printf("%s%s║       RESULTADO FINAL      ║%s\n", ANSI_BG_BLUE, ANSI_COLOR_WHITE, ANSI_COLOR_RESET);
    printf("%s%s╚════════════════════════════╝%s\n\n", ANSI_BG_BLUE, ANSI_COLOR_WHITE, ANSI_COLOR_RESET);
    
    // Puntajes con colores de jugador
    printf("%sJugador 1:%s %s%d puntos%s\n", 
           COLOR_JUGADOR_1, ANSI_COLOR_RESET,
           ANSI_COLOR_BOLD, puntaje_j1, ANSI_COLOR_RESET);
    
    printf("%sJugador 2:%s %s%d puntos%s\n\n", 
           COLOR_JUGADOR_2, ANSI_COLOR_RESET,
           ANSI_COLOR_BOLD, puntaje_j2, ANSI_COLOR_RESET);

    // Historiales con colores
    mostrar_movimientos("🎯 HISTORIAL JUGADOR 1 🎯", h1, 0);
    mostrar_movimientos("🎯 HISTORIAL JUGADOR 2 🎯", h2, 1);

    // Línea separadora
    printf("%s%s──────────────────────────────%s\n\n", ANSI_COLOR_WHITE, ANSI_COLOR_BOLD, ANSI_COLOR_RESET);

    // Mensaje final con fondo colorido
    if (puntaje_j1 > puntaje_j2) {
        printf("%s%s%s🎉 ¡JUGADOR 1 GANA! 🎉%s\n", 
               ANSI_BG_CYAN, ANSI_COLOR_BLACK, ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
    } else if (puntaje_j2 > puntaje_j1) {
        printf("%s%s%s🎉 ¡JUGADOR 2 GANA! 🎉%s\n", 
               ANSI_BG_YELLOW, ANSI_COLOR_BLACK, ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
    } else {
        printf("%s%s%s🤝 ¡EMPATE! 🤝%s\n", 
               ANSI_BG_MAGENTA, ANSI_COLOR_WHITE, ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
    }
    
    printf("\n");
}


// Función auxiliar para crear un movimiento
movimiento_t* crear_movimiento(int jugador, int carta1, int carta2, bool acierto) {
    movimiento_t* mov = malloc(sizeof(movimiento_t));
    mov->jugador = jugador;
    mov->carta1 = carta1;
    mov->carta2 = carta2;
    mov->acierto = acierto;
    return mov;
}

int main() {
    // Crear listas para los historiales
    lista_t* historial_j1 = lista_crear();
    lista_t* historial_j2 = lista_crear();
    
    // Agregar movimientos de ejemplo para JUGADOR 1
    lista_agregar(historial_j1, crear_movimiento(0, 1, 5, false));  // 2-6F
    lista_agregar(historial_j1, crear_movimiento(0, 14, 15, true)); // 15-16A
    lista_agregar(historial_j1, crear_movimiento(0, 3, 13, true));  // 4-14A
    lista_agregar(historial_j1, crear_movimiento(0, 2, 8, true));   // 3-9A
    lista_agregar(historial_j1, crear_movimiento(0, 0, 4, true));   // 1-5A
    
    // Agregar movimientos de ejemplo para JUGADOR 2
    lista_agregar(historial_j2, crear_movimiento(1, 12, 16, true)); // 13-17A
    lista_agregar(historial_j2, crear_movimiento(1, 10, 7, true));  // 11-8A
    lista_agregar(historial_j2, crear_movimiento(1, 11, 9, true));  // 12-10A
    lista_agregar(historial_j2, crear_movimiento(1, 5, 6, true));   // 6-7A
    //lista_agregar(historial_j2, crear_movimiento(1, 1, 17, true));  // 2-18A
    
    // Llamar a la función de prueba
    printf("=== PRUEBA DE MOSTRAR RESULTADO ===\n\n");
    mostrar_resultado_final(historial_j1, historial_j2);
    
    // Limpiar memoria (en este caso la función ya destruye las listas)
    
    return 0;
}