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
    int jugador;
    int carta1;
    int carta2;
    bool acierto;
} movimiento_t;

typedef struct jugador{
    int puntaje;
    lista_t *historial_personal;
}jugador_t;

struct juego{
    tp1_t *pokedex;
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

typedef struct {
    size_t turno_actual;
    int numero_jugador;
    bool es_historial_personal;
    size_t maximo_a_mostrar;
    size_t contador_actual;
} contexto_movimientos_t;

bool callback_mostrar_movimientos(void* elemento, void* contexto) {
    movimiento_t* mov = (movimiento_t*)elemento;
    contexto_movimientos_t* ctx = (contexto_movimientos_t*)contexto;

    if (!ctx->es_historial_personal && ctx->contador_actual >= ctx->maximo_a_mostrar) {
        return false;
    }
    
    int c1 = mov->carta1 + 1;
    int c2 = mov->carta2 + 1;
    char letra = mov->acierto ? 'A' : 'F';
    const char* color_mov = mov->acierto ? COLOR_ACIERTO : COLOR_ERROR;

    if (ctx->es_historial_personal) {
        printf("%s%zu)%s %d-%d%s%c%s\n", 
               ANSI_COLOR_WHITE, ctx->turno_actual, ANSI_COLOR_RESET,
               c1, c2, color_mov, letra, ANSI_COLOR_RESET);
        ctx->turno_actual++;
    } else {
        const char *color_jugador = (mov->jugador == 0) ? COLOR_JUGADOR_1 : COLOR_JUGADOR_2;
        printf("%sJugador %d%s: %d-%d %s%c%s\n", 
               color_jugador, mov->jugador + 1, ANSI_COLOR_RESET,
               c1, c2, color_mov, letra, ANSI_COLOR_RESET);
        ctx->contador_actual++;
    }
    
    return true;
}

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

bool inicializar_historial_personal(jugador_t * j){
    j[0].historial_personal=lista_crear();
    if (!j[0].historial_personal){
        return false;
    }

    j[1].historial_personal=lista_crear();
    if(!j[1].historial_personal){
        lista_destruir(j[0].historial_personal);
        return false;
    }

    return true;
}

jugador_t *inicializar_jugadores(juego_t* juego){
    jugador_t *j=calloc(2,sizeof(jugador_t));
    if (!j) return NULL;

    if (!inicializar_historial_personal(j)){
        free(j);
        return NULL;
    }

    return j;
}

juego_t *juego_crear(const char* archivo){    
    juego_t *juego=calloc(1,sizeof(juego_t));
    if (!juego) return NULL;

    if(archivo){
        juego->pokedex=tp1_leer_archivo(archivo);
        if (!juego->pokedex){
            free(juego);
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

    lista_destruir(juego->jugadores[0].historial_personal);
    lista_destruir(juego->jugadores[1].historial_personal);

    if(!inicializar_historial_personal(juego->jugadores)){
        return false;
    }

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
    return parsear_dos_numeros(linea,num1,num2);
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
    if (jugador < 0 || carta1 < 0 || carta2 < 0) return NULL;

    movimiento_t *mov = malloc(sizeof(movimiento_t));
    if (mov == NULL)
        return NULL;

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
    if (cantidad == 0)
        agregado = lista_agregar(juego->historial_jugadas, mov);
    else
        agregado = lista_insertar(juego->historial_jugadas, mov, 0);
    
    lista_t* hist_personal_act=juego->jugadores[juego->jugador_actual].historial_personal;

    if (!agregado) {
        free(mov);
        return false;
    }

    if (!lista_agregar(hist_personal_act,mov)){
        free(mov);
        return false;
    }

    return true;
}

void mostrar_ultimas_jugadas(juego_t *juego) {
    if (!juego || !juego->historial_jugadas) return;
    if (lista_vacia(juego->historial_jugadas)) return;

    size_t jugadas_a_mostrar = lista_cantidad(juego->historial_jugadas);
    if (jugadas_a_mostrar > 5) {
        jugadas_a_mostrar = 5;
    }
    
    contexto_movimientos_t contexto = {
        .turno_actual = 0,
        .numero_jugador = -1,
        .es_historial_personal = false,
        .maximo_a_mostrar = jugadas_a_mostrar,
        .contador_actual = 0
    };

    lista_con_cada_elemento(juego->historial_jugadas, callback_mostrar_movimientos, &contexto);
}

void mostrar_layout_completo(juego_t *juego) {
    limpiar_pantalla();
    mostrar_mapa(juego->cuadrilla);
    //mostrar_mapa_debug(juego->cuadrilla);
    printf("\n");

    printf("Ultimas jugadas:\n");
    mostrar_ultimas_jugadas(juego);
    printf("\n");

    fflush(stdout);
}

bool pedir_y_validar_par_cartas(juego_t *juego, int *idx1, int *idx2) {
    const char *color_jugador = (juego->jugador_actual == 0) ? COLOR_JUGADOR_1 : COLOR_JUGADOR_2;
    printf("%sJugador %i%s ingrese dos cartas separadas por espacio: ", 
           color_jugador, juego->jugador_actual + 1, ANSI_COLOR_RESET);
    fflush(stdout);
    
    char *linea = leer_linea_dinamica();
    if (!linea)
        return false;
    
    int num1, num2;
    bool formato_valido = validar_formato(linea, &num1, &num2);
    if(!formato_valido){
        *idx1=-1;
        *idx2=-1;
        printf("Error: Formato incorrecto. Use: numero espacio numero\n");
    }else{
        *idx1 = num1 - 1;
        *idx2 = num2 - 1;
    }
    
    free(linea);
    return true;
}

void mostrar_mensaje_error(estado_jugada_t resultado) {
    switch (resultado) {
        case JUGADA_CARTA_YA_DESCUBIERTA:
            printf("Error: Una de las cartas ya fue descubierta. Intente nuevamente.\n");
            break;
        case JUGADA_MISMA_CARTA:
            printf("Error: No puede seleccionar la misma carta dos veces. Intente nuevamente.\n");
            break;
        case JUGADA_CARTA_INVALIDA:
        default:
            printf("Error: Por favor elija carta dentro de los limites.\n");
            break;
    }
}

estado_jugada_t juego_validar_jugada(juego_t *juego, int carta1, int carta2) {
    if (!juego) return JUGADA_ERROR_MEMORIA;

    if (carta1 < 0 || carta1 >= TOTAL_CARTAS || carta2 < 0 || carta2 >= TOTAL_CARTAS)
        return JUGADA_CARTA_INVALIDA;

    if (carta1 == carta2)
        return JUGADA_MISMA_CARTA;

    if (juego->cuadrilla->celdas[carta1].encontrada || 
        juego->cuadrilla->celdas[carta2].encontrada)
        return JUGADA_CARTA_YA_DESCUBIERTA;
    
    juego->cuadrilla->celdas[carta1].visible=true;
    juego->cuadrilla->celdas[carta2].visible=true;
    return JUGADA_VALIDA;
}

void juego_mostrar_cartas_temporalmente(juego_t *juego, int carta1, int carta2) {
    if (!juego) return;

    mostrar_layout_completo(juego);
    printf("Mostrando cartas seleccionadas...\n\n");

    juego->cuadrilla->celdas[carta1].visible = false;
    juego->cuadrilla->celdas[carta2].visible = false;
}

estado_jugada_t juego_ejecutar_jugada(juego_t *juego, int carta1, int carta2) {
    bool acierto = es_acierto(juego, carta1, carta2);
    
    bool historial_ok = agregar_jugada_historial(juego, carta1, carta2, acierto);
    if (!historial_ok)
        return JUGADA_ERROR_MEMORIA;
    
    if (acierto) {
        juego->cuadrilla->celdas[carta1].encontrada = true;
        juego->cuadrilla->celdas[carta2].encontrada = true;
        juego->cuadrilla->celdas[carta1].jugador_descubridor = juego->jugador_actual;
        juego->cuadrilla->celdas[carta2].jugador_descubridor = juego->jugador_actual;
        juego->jugadores[juego->jugador_actual].puntaje++;
    } else
        juego->jugador_actual = cambiar_indice_jugador(juego->jugador_actual);

    return (acierto) ? JUGADA_FORMO_PAR : JUGADA_NO_FORMO_PAR;
}

bool procesar_turno(juego_t *juego) {
    if (!juego) return false;

    mostrar_layout_completo(juego);

    estado_jugada_t resultado=JUGADA_VALIDA;
    bool jugada_valida = false;
    int idx1, idx2;
    
    while (!jugada_valida) {
        if (!pedir_y_validar_par_cartas(juego, &idx1, &idx2)) {
            return false;
        }
        resultado = juego_validar_jugada(juego, idx1, idx2);
        if (resultado != JUGADA_VALIDA) {
            mostrar_mensaje_error(resultado);
        }else{
            juego_mostrar_cartas_temporalmente(juego, idx1, idx2);
            sleep(2);

            resultado=juego_ejecutar_jugada(juego, idx1, idx2);
            if (resultado == JUGADA_FORMO_PAR || resultado == JUGADA_NO_FORMO_PAR)
                jugada_valida = true;
            else
                return false;
        }
    }
    
    mostrar_layout_completo(juego);

    if (resultado == JUGADA_FORMO_PAR)
        printf("¡Acierto! Punto para el jugador actual.\n");
    else
        printf("No es un acierto. Turno del siguiente jugador.\n");
    
    return true;
}

void mostrar_movimientos(const char* titulo, lista_t* historial, int numero_jugador) {
    printf("%s%s%s\n", ANSI_COLOR_BOLD, titulo, ANSI_COLOR_RESET);

    contexto_movimientos_t contexto = {
        .turno_actual = 1,
        .numero_jugador = numero_jugador,
        .es_historial_personal = true,
        .maximo_a_mostrar = 0,
        .contador_actual = 0
    };
    
    lista_con_cada_elemento(historial, callback_mostrar_movimientos, &contexto);
    printf("\n");
}

void mostrar_resultado_final(juego_t *juego) {   
    if(!juego->terminado) return;
    
    int puntaje_j1 = juego_obtener_puntaje(juego, 0);
    int puntaje_j2 = juego_obtener_puntaje(juego, 1);

    printf("%s%s╔════════════════════════════╗%s\n", ANSI_BG_BLUE, ANSI_COLOR_WHITE, ANSI_COLOR_RESET);
    printf("%s%s║       RESULTADO FINAL      ║%s\n", ANSI_BG_BLUE, ANSI_COLOR_WHITE, ANSI_COLOR_RESET);
    printf("%s%s╚════════════════════════════╝%s\n\n", ANSI_BG_BLUE, ANSI_COLOR_WHITE, ANSI_COLOR_RESET);

    printf("%sJugador 1:%s %s%d puntos%s\n", 
           COLOR_JUGADOR_1, ANSI_COLOR_RESET,
           ANSI_COLOR_BOLD, puntaje_j1, ANSI_COLOR_RESET);
    
    printf("%sJugador 2:%s %s%d puntos%s\n\n", 
           COLOR_JUGADOR_2, ANSI_COLOR_RESET,
           ANSI_COLOR_BOLD, puntaje_j2, ANSI_COLOR_RESET);

    mostrar_movimientos("🎯 HISTORIAL JUGADOR 1 🎯", juego->jugadores[0].historial_personal, 0);
    mostrar_movimientos("🎯 HISTORIAL JUGADOR 2 🎯", juego->jugadores[1].historial_personal, 1);

    printf("%s%s──────────────────────────────%s\n\n", ANSI_COLOR_WHITE, ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
    if (puntaje_j1 > puntaje_j2)
        printf("%s%s%s🎉 ¡JUGADOR 1 GANA! 🎉%s\n", 
               ANSI_BG_CYAN, ANSI_COLOR_BLACK, ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
    else
        printf("%s%s%s🎉 ¡JUGADOR 2 GANA! 🎉%s\n", 
               ANSI_BG_YELLOW, ANSI_COLOR_BLACK, ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
    printf("\n");
}

bool juego_interactivo(juego_t *juego) {
    if (!juego) return false;

    bool terminado = false;
    while (!terminado) {
        if (!procesar_turno(juego)) {
            return false;
        }

        if (juego_terminado(juego)){
            terminado=true;
            printf("¡Juego terminado!\n");
        }
        sleep(2);
    }

    limpiar_pantalla();
    mostrar_resultado_final(juego);
    return terminado;
}

bool juego_jugar(juego_t* juego, unsigned int semilla){
    if (!juego_preparar(juego,semilla)){
        printf("Error preparando el juego.\n");
        return false;
    }

    return juego_interactivo(juego);
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

int juego_obtener_jugador_actual(const juego_t *juego) {
    return juego ? juego->jugador_actual : -1;
}

int juego_obtener_puntaje(const juego_t *juego, int jugador) {
    if (!juego || jugador < 0 || jugador > 1) return -1;
    return juego->jugadores[jugador].puntaje;
}

bool juego_es_carta_descubierta(const juego_t *juego, int indice_carta) {
    if (!juego || indice_carta < 0 || indice_carta >= TOTAL_CARTAS) return false;
    return juego->cuadrilla->celdas[indice_carta].encontrada;
}

bool juego_es_carta_visible(const juego_t *juego, int indice_carta) {
    if (!juego || indice_carta < 0 || indice_carta >= TOTAL_CARTAS) return false;
    return juego->cuadrilla->celdas[indice_carta].visible;
}

struct pokemon* juego_obtener_pokemon_carta(const juego_t *juego, int indice_carta) {
    if (!juego || indice_carta < 0 || indice_carta >= TOTAL_CARTAS) return NULL;
    return juego->cuadrilla->celdas[indice_carta].pokemon;
}

lista_t* juego_obtener_historial_completo(juego_t* juego){
    if (!juego) return NULL;

    return juego->historial_jugadas;
}

lista_t* juego_obtener_historial_jugador(juego_t* juego, int jugador) {
    if (!juego || jugador < 0 || jugador > 1) {
        return NULL;
    }
    
    return juego->jugadores[jugador].historial_personal;
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