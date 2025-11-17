#include "juego.h"
#include "ansi.h"

#include <stdio.h>
#include <unistd.h>

#define COLOR_JUGADOR_1 ANSI_COLOR_CYAN
#define COLOR_JUGADOR_2 ANSI_COLOR_YELLOW

#define COLOR_SELECCIONADO ANSI_COLOR_MAGENTA

#define COLOR_ACIERTO ANSI_COLOR_GREEN
#define COLOR_ERROR ANSI_COLOR_RED

typedef struct carta{
    struct pokemon *pokemon;
    bool visible;
    bool encontrada;
    int jugador_descubridor; 
}carta_t;

typedef struct mapa{
    size_t filas;
    size_t columnas;    
    carta_t *celdas; 
}mapa_t;

typedef struct movimiento {
    int jugador;        // 0 o 1 (índice del jugador)
    int carta1;         // índice de la primera carta (0-17)
    int carta2;         // índice de la segunda carta (0-17)
    bool acierto;       // true si fue acierto
} movimiento_t;

typedef struct jugador{
    int puntaje;
}jugador_t;

struct juego{
    tp1_t *pokedex; // o hash 
    mapa_t* cuadrilla;
    jugador_t *jugadores;
    int jugador_actual;
    lista_t *historial_jugadas;
    int semilla;
    bool terminado;
};

struct buscar_ctx {
    size_t objetivo;
    size_t actual;
    struct pokemon *encontrado;
};

bool buscar_callback(struct pokemon *p, void *extra) {
    struct buscar_ctx *ctx = extra;

    if (ctx->actual == ctx->objetivo) {
        ctx->encontrado = p;
        return false;
    }

    ctx->actual = ctx->actual + 1;
    return true;
}

struct pokemon *tp1_obtener_en_posicion(tp1_t *tp1, size_t pos) {
    struct buscar_ctx ctx;

    ctx.objetivo   = pos;
    ctx.actual     = 0;
    ctx.encontrado = NULL;

    tp1_con_cada_pokemon(tp1, buscar_callback, &ctx);

    return ctx.encontrado;
}

struct pokemon *tp1_random(tp1_t *tp1, size_t cantidad) {
    size_t r = (size_t)(rand() % (int)cantidad);
    return tp1_obtener_en_posicion(tp1, r);
}

bool ya_esta(struct pokemon *p, struct pokemon **vec, size_t n) {
    size_t i = 0;
    bool encontrado = false;

    while (i < n) {
        if (vec[i] == p) {
            encontrado = true;
            i = n;
        } else {
            i = i + 1;
        }
    }

    return encontrado;
}

void mezclar(carta_t *vec, size_t n) {
    size_t i = n - 1;

    while (i > 0) {
        size_t r = (size_t)(rand() % (int)(i + 1));

        struct pokemon *tmp = vec[i].pokemon;
        vec[i].pokemon = vec[r].pokemon;
        vec[r].pokemon = tmp;

        i = i - 1;
    }
}

bool llenar_celdas(tp1_t *pokedex, carta_t *celdas) {
    if (!pokedex || !celdas) return false;

    size_t total = tp1_cantidad(pokedex);
    if (total < PAREJAS) {
        printf("Error: Se necesitan al menos %d pokémones únicos, pero hay %zu\n", 
               PAREJAS, total);
        return false;
    }
    struct pokemon *elegidos[PAREJAS];
    size_t n = 0;
    
    while (n < PAREJAS) {
        struct pokemon *p = tp1_random(pokedex, total);
        if (!ya_esta(p, elegidos, n)) {
            elegidos[n] = p;
            n = n + 1;
        }
    }

    for (size_t i = 0; i < PAREJAS; i++) {
        celdas[i].pokemon = elegidos[i];
        celdas[i].jugador_descubridor = -1;
        celdas[i + PAREJAS].pokemon = elegidos[i];
        celdas[i + PAREJAS].jugador_descubridor = -1;
    }

    mezclar(celdas, TOTAL_CARTAS);
    return true;
}

void destruir_celdas(carta_t *celdas){
    if (!celdas) return;
    free(celdas);
}

void mapa_destruir(mapa_t* m){
    destruir_celdas(m->celdas);
    free(m);
}

mapa_t *inicializar_mapa(){
    mapa_t *m = calloc(1, sizeof(mapa_t));
    if (!m) return NULL;

    m->filas = FILAS;
    m->columnas = COLUMNAS;

    m->celdas = calloc(FILAS*COLUMNAS, sizeof(carta_t));
    if (!m->celdas) {
        printf("Error al inicializar celdas\n");
        free(m);
        return NULL;
    }

    return m;
}

jugador_t *inicializar_jugadores(juego_t* juego){
    jugador_t *j=calloc(2,sizeof(jugador_t));
    if (!j) return NULL;

    return j;
}

juego_t *juego_crear(const char* archivo){    
    juego_t *juego=calloc(1,sizeof(juego_t));
    if (!juego) return NULL;

    if(archivo){
        juego->pokedex=tp1_leer_archivo(archivo);
        if (!juego->pokedex){
            printf("Error al crear pokedex\n");
            return NULL;
        }
    }

    juego->cuadrilla=inicializar_mapa();
    if (!juego->cuadrilla){
        printf("Error al crear el mapa\n");
        tp1_destruir(juego->pokedex);
        free(juego);
        return NULL;
    }

    juego->jugadores=inicializar_jugadores(juego);
    if (!juego->jugadores){
        printf("Error al crear jugadores\n");
        mapa_destruir(juego->cuadrilla);
        tp1_destruir(juego->pokedex);
        free(juego);
        return NULL;
    }

    juego->historial_jugadas=lista_crear();
    if (!juego->historial_jugadas){
        printf("Error al crear historial\n");
        lista_destruir(juego->historial_jugadas);
        mapa_destruir(juego->cuadrilla);
        tp1_destruir(juego->pokedex);
        free(juego->jugadores);
        free(juego);
        return NULL;
    }

    return juego;
}

void mostrar_carta_descubierta(int indice_jugador_descubierto, char nombre_abreviado[4]) {
    const char *color = (indice_jugador_descubierto == 0) ? 
                       COLOR_JUGADOR_1 : COLOR_JUGADOR_2;
    printf("%s[%s %3s %s]%s ", 
           color, ANSI_COLOR_RESET, nombre_abreviado, color, ANSI_COLOR_RESET);
}

void mostrar_carta_visible(char nombre_abreviado[4]) {
    printf(COLOR_SELECCIONADO "[" ANSI_COLOR_RESET " %3s " COLOR_SELECCIONADO "]" ANSI_COLOR_RESET " ", 
           nombre_abreviado);
}

void mostrar_mapa_debug(mapa_t *mapa) {
    if (!mapa || !mapa->celdas) return;

    size_t columnas = mapa->columnas;

    for (size_t fila = 0; fila < mapa->filas; fila++) {
        for (size_t col = 0; col < mapa->columnas; col++) {
            
            size_t idx = fila * columnas + col;
            carta_t *carta = &mapa->celdas[idx];
            
            if (!carta->pokemon) {
                printf("[ --- ] ");
            } else {
                char nombre_abreviado[4];
                strncpy(nombre_abreviado, carta->pokemon->nombre, 3);
                nombre_abreviado[3] = '\0';

                if (carta->encontrada) {
                    mostrar_carta_descubierta(carta->jugador_descubridor, nombre_abreviado);
                } else if (carta->visible) {
                    mostrar_carta_visible(nombre_abreviado);
                } else {
                    printf("[ %3u ] ", carta->pokemon->id);
                }
            }
        }
        printf("\n");
    }
}

void mostrar_mapa(mapa_t *mapa) {
    if (!mapa || !mapa->celdas) return;

    size_t columnas = mapa->columnas;

    for (size_t fila = 0; fila < mapa->filas; fila++) {
        for (size_t col = 0; col < mapa->columnas; col++) {
            
            size_t idx = fila * columnas + col;
            carta_t *carta = &mapa->celdas[idx];
            
            if (!carta->pokemon) {
                printf("[ --- ] ");
            } else {
                char nombre_abreviado[4];
                strncpy(nombre_abreviado, carta->pokemon->nombre, 3);
                nombre_abreviado[3] = '\0';

                if (carta->encontrada) {
                    mostrar_carta_descubierta(carta->jugador_descubridor, nombre_abreviado);
                } else if (carta->visible) {
                    mostrar_carta_visible(nombre_abreviado);
                } else {
                    printf("[ %3zu ] ", idx + 1);
                }
            }
        }
        printf("\n");
    }
}

/*
    Pre: juego no debe ser NULL.
    Post: Reinicia el juego al estado inicial con la semilla especificada.
          Devuelve true si se reinició correctamente, false en caso de error.
*/
bool juego_reiniciar(juego_t *juego, unsigned int semilla) {
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        juego->cuadrilla->celdas[i].visible = false;
        juego->cuadrilla->celdas[i].encontrada = false;
    }

    juego->jugadores[0].puntaje = 0;
    juego->jugadores[1].puntaje = 0;
    juego->jugador_actual = 0;

    if (juego->historial_jugadas) {
        lista_destruir_todo(juego->historial_jugadas, free);
        juego->historial_jugadas = lista_crear();
        if (!juego->historial_jugadas) return false;
    }

    if (!llenar_celdas(juego->pokedex, juego->cuadrilla->celdas)) {
        return false;
    }
    
    juego->terminado = false;
    return true;
}

bool juego_preparar(juego_t *juego, unsigned int semilla) {
    srand(semilla);

    if (juego->terminado) {
        return juego_reiniciar(juego, semilla);
    }

    if (!llenar_celdas(juego->pokedex, juego->cuadrilla->celdas)) {
        return false;
    }

    juego->semilla = (int)semilla;
    return true;
}

bool es_acierto(juego_t* juego, int idx1, int idx2){
    return juego->cuadrilla->celdas[idx1].pokemon->id == juego->cuadrilla->celdas[idx2].pokemon->id;
}

int cambiar_indice_jugador(int indice_actual){
    if (indice_actual==0) return 1;
    else if (indice_actual==1) return 0;

    return -1;
}

bool validar_formato(const char *linea, int *num1, int *num2) {
    if (!parsear_dos_numeros(linea, num1, num2)) {
        printf("Error: Formato incorrecto. Use: numero espacio numero\n");
        return false;
    }
    return true;
}

bool validar_rangos(int num1, int num2) {
    if (num1 < 1 || num1 > TOTAL_CARTAS || num2 < 1 || num2 > TOTAL_CARTAS) {
        printf("Error: Rango permitido: 1 a %d\n", TOTAL_CARTAS);
        return false;
    }
    return true;
}

bool validar_diferentes(int num1, int num2) {
    if (num1 == num2) {
        printf("Error: No puede seleccionar la misma carta dos veces.\n");
        return false;
    }
    return true;
}

bool validar_no_descubiertas(juego_t *juego, int idx1, int idx2) {
    if (juego->cuadrilla->celdas[idx1].encontrada) {
        printf("Error: La carta %d ya fue descubierta.\n", idx1 + 1);
        return false;
    }
    if (juego->cuadrilla->celdas[idx2].encontrada) {
        printf("Error: La carta %d ya fue descubierta.\n", idx2 + 1);
        return false;
    }
    return true;
}

bool pedir_y_validar_par_cartas(juego_t *juego, int *idx1, int *idx2) {
    const char *color_jugador = (juego->jugador_actual == 0) ? COLOR_JUGADOR_1 : COLOR_JUGADOR_2;
    bool par_valido = false;
    
    while (!par_valido) {
        printf("%sJugador %i%s ingrese dos cartas separadas por espacio: ", 
               color_jugador, juego->jugador_actual + 1, ANSI_COLOR_RESET);
        fflush(stdout);
        
        char *linea = leer_linea_dinamica();
        if (!linea) return false;
        
        int num1, num2;
        bool formato_valido = validar_formato(linea, &num1, &num2);
        bool rangos_validos = formato_valido && validar_rangos(num1, num2);
        bool diferentes = rangos_validos && validar_diferentes(num1, num2);
        
        if (diferentes) {
            *idx1 = num1 - 1;
            *idx2 = num2 - 1;
            bool no_descubiertas = validar_no_descubiertas(juego, *idx1, *idx2);
            par_valido = no_descubiertas;
        }
        
        free(linea);
    }
    
    return true;
}

bool juego_terminado(juego_t *juego) {
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        if (!juego->cuadrilla->celdas[i].encontrada) {
            return false;
        }
    }

    juego->terminado=true;
    return true;
}

movimiento_t *crear_movimiento(int jugador, int carta1, int carta2, bool acierto) {
    if (jugador < 0 || carta1 < 0 || carta2 < 0) {
        return NULL;
    }

    movimiento_t *mov = malloc(sizeof(movimiento_t));
    if (mov == NULL) {
        return NULL;
    }

    mov->jugador = jugador;
    mov->carta1 = carta1;
    mov->carta2 = carta2;
    mov->acierto = acierto;
    
    return mov;
}

bool agregar_jugada_historial(juego_t *juego, int idx1, int idx2, bool acierto) {
    if (!juego || !juego->historial_jugadas) return false;
    
    movimiento_t *mov = crear_movimiento(juego->jugador_actual, idx1, idx2, acierto);
    if (!mov) return false;
     
    size_t cantidad = lista_cantidad(juego->historial_jugadas);
    bool agregado;
    
    if (cantidad == 0) {
        agregado = lista_agregar(juego->historial_jugadas, mov);
    } else {
        agregado = lista_insertar(juego->historial_jugadas, mov, 0);
    }
    
    if (!agregado) {
        printf("Error al agregar mov (cantidad=%zu)\n", cantidad);
        free(mov);
        return false;
    }
    
    return true;
}

bool mostrar_ultimas_jugadas(juego_t *juego) {
    if (!juego || !juego->historial_jugadas) {
        printf("No hay jugadas previas\n");
        return false;
    }
    
    if (lista_vacia(juego->historial_jugadas)) return true;

    size_t jugadas_a_mostrar = lista_cantidad(juego->historial_jugadas);
    if (jugadas_a_mostrar > 5) {
        jugadas_a_mostrar = 5;
    }
    
    lista_iterador_t *iter = lista_iterador_crear(juego->historial_jugadas);
    if (!iter) return false;
    
    size_t contador = 0;
    while (lista_iterador_hay_mas_elementos(iter) && contador < jugadas_a_mostrar) {
        movimiento_t *mov = lista_iterador_obtener_actual(iter);
        if (mov) {
            const char *color_jugador = (mov->jugador == 0) ? COLOR_JUGADOR_1 : COLOR_JUGADOR_2;
            char simbolo = mov->acierto ? 'A' : 'F';
            const char *color_resultado = mov->acierto ? COLOR_ACIERTO : COLOR_ERROR;

            printf("%sJugador %d%s: %d-%d %s%c%s\n", 
                   color_jugador, mov->jugador + 1, ANSI_COLOR_RESET,
                   mov->carta1 + 1, mov->carta2 + 1, 
                   color_resultado, simbolo, ANSI_COLOR_RESET);
            contador++;
        }
        lista_iterador_siguiente(iter);
    }
    
    lista_iterador_destruir(iter);
    return true;
}

bool mostrar_layout_completo(juego_t *juego) {
    limpiar_pantalla();
    //mostrar_mapa(juego->cuadrilla);
    mostrar_mapa_debug(juego->cuadrilla);
    printf("\n");

    printf("Ultimas jugadas:\n");
    if (!mostrar_ultimas_jugadas(juego)) return false;
    printf("\n");

    fflush(stdout);
    return true;
}

bool procesar_jugada(juego_t *juego) {
    if (!juego || !juego->cuadrilla || !juego->cuadrilla->celdas) {
        return false;
    }

    if (!mostrar_layout_completo(juego)) return false;
    
    int idx1, idx2;
    bool exito = pedir_y_validar_par_cartas(juego, &idx1, &idx2);
    if (!exito) {
        return false;
    }

    juego->cuadrilla->celdas[idx1].visible = true;
    juego->cuadrilla->celdas[idx2].visible = true;

    mostrar_layout_completo(juego);
    printf("Mostrando cartas seleccionadas...\n\n");
    sleep(2);
    
    bool acierto = es_acierto(juego, idx1, idx2);
    
    bool historial_ok = agregar_jugada_historial(juego, idx1, idx2, acierto);
    if (!historial_ok) {
        return false;
    }
    
    if (acierto) {
        juego->cuadrilla->celdas[idx1].encontrada = true;
        juego->cuadrilla->celdas[idx2].encontrada = true;
        juego->cuadrilla->celdas[idx1].jugador_descubridor = juego->jugador_actual;
        juego->cuadrilla->celdas[idx2].jugador_descubridor = juego->jugador_actual;   
        juego->jugadores[juego->jugador_actual].puntaje++;
        printf("¡Acierto! Punto para el jugador actual.\n");
    } else {
        juego->cuadrilla->celdas[idx1].visible = false;
        juego->cuadrilla->celdas[idx2].visible = false;
        juego->jugador_actual = cambiar_indice_jugador(juego->jugador_actual);
        printf("No es un acierto. Turno del siguiente jugador.\n");
    }
    
    return true;
}

bool juego_correr(juego_t *juego) {
    if (!juego) return false;

    bool terminado = false;
    bool error_memoria = false;
    bool juego_completado = false;

    while (!terminado && !error_memoria) {
        bool jugada_exitosa = procesar_jugada(juego);
        if (!jugada_exitosa)
            printf("error en jugada\n");
        
        if (!jugada_exitosa) {
            error_memoria = true;
        } else {
            juego_completado = juego_terminado(juego);
            if (juego_completado) {
                printf("¡Juego terminado!\n");
                terminado = true;
            }
        }

        if (!error_memoria && !juego_completado) {
            sleep(2);
        }
    }

    limpiar_pantalla();

    if (juego->jugadores[0].puntaje > juego->jugadores[1].puntaje){
        printf("\tGANO EL JUGADOR 1.\n");
    }else{
        printf("\tGANO EL JUGADOR 2.\n");
    }

    return !error_memoria;
}

bool juego_jugar(juego_t* juego, unsigned int semilla){
    if (!juego_preparar(juego,semilla)){
        printf("Error preparando el juego.\n");
        return false;
    }

    return juego_correr(juego);
}

void juego_destruir(juego_t* juego){
    tp1_destruir(juego->pokedex);
    free(juego->cuadrilla->celdas);
    free(juego->cuadrilla);
    free(juego->jugadores);

    if (juego->historial_jugadas) {
        lista_destruir_todo(juego->historial_jugadas, free);
    }
    
    free(juego);
}

tp1_t *juego_obtener_pokedex(juego_t *juego) {
    if (!juego) return NULL;
    return juego->pokedex;
}

bool juego_establecer_pokedex(juego_t *juego, tp1_t *nueva_pokedex) {
    if (!juego) return false;

    if (juego->pokedex) {
        tp1_destruir(juego->pokedex);
    }
    
    juego->pokedex = nueva_pokedex;
    return true;
}

bool juego_tiene_pokedex(juego_t *juego) {
    return juego && juego->pokedex;
}