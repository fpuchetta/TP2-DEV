#include "juego.h"

#include <stdio.h>

#define TOTAL_CARTAS 18
#define PAREJAS 9

carta_t **inicializar_celdas(){
    carta_t **celdas= calloc(FILAS, sizeof(carta_t *));
    if (!celdas) {
        return NULL;
    }

    bool ok = true;
    size_t i = 0;
    while (i < FILAS && ok) {
        celdas[i] = calloc(COLUMNAS, sizeof(carta_t));

        if (!celdas[i])
            ok = false;
        else
            i++;
    }

    if (!ok) {
        size_t j = 0;

        while (j < i) {
            free(celdas[j]);
            j++;
        }

        free(celdas);
        return NULL;
    }

    return celdas;
}

/* ------------- obtener pokémon en posición r ------------- */

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

void mezclar(struct pokemon **vec, size_t n) {
    size_t i = n - 1;

    while (i > 0) {
        size_t r = (size_t)(rand() % (int)(i + 1));

        struct pokemon *tmp = vec[i];
        vec[i] = vec[r];
        vec[r] = tmp;

        i = i - 1;
    }
}

bool llenar_celdas(tp1_t *pokedex, carta_t **celdas,unsigned int semilla){
    if (!pokedex || !celdas) return false;

    size_t total = tp1_cantidad(pokedex);
    if (total < PAREJAS) {
        return false;
    }

    srand((unsigned int)semilla);

    struct pokemon *elegidos[PAREJAS];
    struct pokemon *cartas[TOTAL_CARTAS];

    /* 1) elegir 9 pokémon distintos */

    size_t n = 0;
    while (n < PAREJAS) {
        struct pokemon *p = tp1_random(pokedex, total);

        if (!ya_esta(p, elegidos, n)) {
            elegidos[n] = p;
            n = n + 1;
        }
    }

    /* 2) duplicar: 9 → 18 */

    size_t i = 0;
    while (i < PAREJAS) {
        cartas[i]       = elegidos[i];
        cartas[i + 9]   = elegidos[i];
        i = i + 1;
    }

    /* 3) mezclar las 18 cartas */

    mezclar(cartas, TOTAL_CARTAS);

    /* 4) volcar en la matriz FILAS x COLUMNAS */

    size_t k = 0;
    size_t f = 0;

    while (f < FILAS) {
        size_t c = 0;

        while (c < COLUMNAS) {
            celdas[f][c].pokemon    = cartas[k];
            celdas[f][c].visible    = false;
            celdas[f][c].encontrada = false;

            k = k + 1;
            c = c + 1;
        }

        f = f + 1;
    }

    return true;
}

void destruir_celdas(carta_t **celdas){
    if (!celdas) return;

    int i=0;
    while (i < FILAS) {
        free(celdas[i]);
        i++;
    }
    
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

    m->celdas = inicializar_celdas();
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

    j[0].movimientos=lista_crear();
    if (!j[0].movimientos){
        free(j);
        return NULL;
    }

    j[1].movimientos=lista_crear();
    if (!j[1].movimientos){
        lista_destruir(j[0].movimientos);
        free(j);
        return NULL;
    }

    return j;
}

juego_t *juego_crear(const char* archivo){
    if (!archivo) return NULL;
    
    juego_t *juego=calloc(1,sizeof(juego_t));
    if (!juego) return NULL;

    juego->pokedex=tp1_leer_archivo(archivo);
    if (!juego->pokedex){
        printf("Error al crear pokedex\n");
        return NULL;
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

    juego->movimientos=cola_crear();
    if (!juego->movimientos){
        printf("Error al crear cola\n");
        cola_destruir(juego->movimientos);
        mapa_destruir(juego->cuadrilla);
        tp1_destruir(juego->pokedex);
        free(juego);
        return NULL;
    }// no creo que sea una cola, quizas una lista

    return juego;
}

void mostrar_mapa(mapa_t* mapa){
    if (!mapa) return;

    size_t f = 0;
    while (f < mapa->filas) {

        size_t c = 0;
        while (c < mapa->columnas) {

            struct pokemon *p = mapa->celdas[f][c].pokemon;

            if (p == NULL) {
                printf("[  --  ] ");   // celda vacía
            } else {
                // muestro el id (o podés usar el nombre)
                printf("[ %4d ] ", p->id);
            }

            c++;
        }

        printf("\n");
        f++;
    }
}

bool juego_preparar(juego_t *juego, unsigned int semilla){
    if (!llenar_celdas(juego->pokedex,juego->cuadrilla->celdas, semilla)){
        return false;
    }

    return true;
}

bool juego_correr(juego_t* juego){
    if (!juego) return false;

    int c = 0;
    char letra = '\0';

    
    while (letra != 'Q') {
        limpiar_pantalla();
        printf("Presione ENTER para refrescar, Q para salir.\n");
        mostrar_mapa(juego->cuadrilla);

        printf("\n> ");
        fflush(stdout);

        c = getchar();
        letra = (char)c;

        // consumir basura del buffer si no es '\n'
        if (c != '\n') {
            int basura = getchar();
            while (basura != '\n' && basura != EOF)
                basura = getchar();
        }

        // si el usuario toca enter sin querer,
        // volvemos al loop y volvemos a imprimir
    }

    return true;
}

bool juego_jugar(juego_t* juego, unsigned int semilla){
    if (!juego_preparar(juego,semilla)){
        printf("Error preparando el juego.\n");
        return false;
    }

    return juego_correr(juego);
}