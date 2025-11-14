#include "menu.h"
#include "lista.h"
#include "pila.h"
#include "hash.h"
#include "functions.h"
#include "ansi.h"
#include "ctype.h"

#define ANSI_CURSOR_HOME "\x1b[H"

#include <stdio.h>
typedef enum {
    MENU_TIPO_RAIZ,
    MENU_TIPO_SUBMENU
} menu_tipo_t;

typedef enum {
    OPCION_ACCION,
    OPCION_SUBMENU
} opcion_tipo_t;

typedef struct opcion {
    char tecla;
    char *nombre;
    opcion_tipo_t tipo;
    menu_accion_t accion;   // si tipo == ACCION
    struct menu *submenu;   // si tipo == SUBMENU
} opcion_t;

struct menu {
    char *titulo;
    hash_t *opciones;       // clave: char[2] -> valor: opcion_t*
    menu_tipo_t tipo;       // RAIZ o SUBMENU
    menu_mostrar_t* estilos;
    size_t cant_estilos;
};

// Contexto de ejecución (oculto al usuario)
typedef struct {
    menu_t *menu_actual;
    pila_t *stack_menus;
    menu_mostrar_t estilo_actual;
    size_t indice_estilo_actual;
    size_t cant_estilos;
    void *user_data;
    bool salir;
} menu_running_t;

typedef struct estilo_ctx{
    menu_mostrar_t estilo;
}estilo_ctx_t;

void mostrar_estilo_default(char tecla, char *nombre){
    printf ("%c - %s\n",tecla,nombre);
}

menu_t *menu_crear(const char *titulo, menu_tipo_t tipo, menu_mostrar_t estilo){
    menu_t* m=calloc(1,sizeof(menu_t));
    if (!m) return NULL;

    m->titulo=mi_strdup(titulo);
    if(!m->titulo){
        free(m);
        return NULL;
    }
    m->tipo=tipo;
    if (tipo == MENU_TIPO_RAIZ){
        m->estilos=calloc(1,sizeof(menu_mostrar_t));
        if (!m->estilos){
            free(m->titulo);
            free(m);
        }
        m->estilos[0]=estilo ? estilo : mostrar_estilo_default;
        m->cant_estilos++;
    }

    return m;
}

menu_t *menu_crear_base(const char *titulo, menu_mostrar_t estilo){
    if (!titulo) return NULL;

    menu_t *m=menu_crear(titulo,MENU_TIPO_RAIZ,estilo);
    return m;
}

void menu_destruir_opcion(void *_op){
    opcion_t *op=_op;

    free(op->nombre);
    free(op);
}

void menu_destruir_submenu(menu_t *menu){
    if (!menu || menu->tipo != MENU_TIPO_SUBMENU){
        return;
    }

    if (menu->opciones)
        hash_destruir_todo(menu->opciones, menu_destruir_opcion);

    free(menu->titulo);
    free(menu);
}

bool menu_agregar_opcion(menu_t *menu,opcion_t *op){
    if (!menu || !op) return false;
    char tecla[2]={op->tecla,'\0'};

    if (!menu->opciones){
        menu->opciones=hash_crear(5);
        if (!menu->opciones) return false;
    }

    return hash_insertar(menu->opciones,tecla,op,NULL);
}

bool menu_existe_opcion(menu_t *m, char tecla){
    if (!m || !m->opciones || tecla == MENU_TECLA_SALIR || tecla == MENU_TECLA_VOLVER) return false;

    char tecla_s[2] = { tecla, '\0' };
    return hash_contiene(m->opciones, tecla_s);
}

opcion_t *opcion_crear(const char *nombre, char tecla, opcion_tipo_t tipo, menu_accion_t accion, menu_t* submenu){
    if (!nombre) return NULL;

    // Validar coherencia
    if (tipo == OPCION_ACCION && !accion)
        return NULL;

    if (tipo == OPCION_SUBMENU && !submenu)
        return NULL;

    char tecla_mayus = (char) toupper((unsigned char) tecla);

    opcion_t *op = calloc(1, sizeof(opcion_t));
    if (!op) return NULL;

    op->nombre = mi_strdup(nombre);
    if (!op->nombre) {
        free(op);
        return NULL;
    }

    op->tecla = tecla_mayus;
    op->tipo  = tipo;
    op->accion = accion;
    op->submenu = submenu;

    return op;
}

menu_t *menu_crear_submenu(menu_t *padre, char tecla, const char *nombre){
    if (!padre || !nombre) return NULL;

    if (menu_existe_opcion(padre, tecla)){
        return NULL;
    }

    menu_t *submenu=menu_crear(nombre,MENU_TIPO_SUBMENU,NULL);
    if (!submenu) return NULL;

    opcion_t *op=opcion_crear(nombre,tecla,OPCION_SUBMENU,NULL,submenu);
    if (!op){
        menu_destruir_submenu(submenu);
        return NULL;
    }

    if(!menu_agregar_opcion(padre,op)){
        menu_destruir_submenu(submenu);
        menu_destruir_opcion(op);
        return NULL;
    }

    return submenu;
}

bool menu_agregar_accion(menu_t *menu, char tecla, const char *nombre, menu_accion_t accion){
    if (!menu || !nombre || !accion){
        return false;
    }

    if (menu_existe_opcion(menu, tecla)){
        return false;
    }

    opcion_t *op=opcion_crear(nombre,tecla,OPCION_ACCION,accion,NULL);
    if (!op) return false;

    if (!menu_agregar_opcion(menu,op)){
        menu_destruir_opcion(op);
        return false;
    }

    return true;
}

bool menu_agregar_estilo(menu_t* menu, menu_mostrar_t estilo){
    if (!menu || !estilo) return false;
    if (menu->tipo != MENU_TIPO_RAIZ) return false;

    size_t nueva_cant = menu->cant_estilos + 1;

    menu_mostrar_t *estilos = realloc(menu->estilos,
                                      nueva_cant * sizeof *estilos);
    if (!estilos) return false;

    menu->estilos = estilos;
    menu->estilos[menu->cant_estilos] = estilo;
    menu->cant_estilos = nueva_cant;

    return true;
}

void actualizar_estilo_actual(menu_running_t *run){
    menu_t *m=run->menu_actual;
    
    if (run->indice_estilo_actual == run->cant_estilos-1){
        run->indice_estilo_actual=0;
    }else{    
        run->indice_estilo_actual++;
    }

    run->estilo_actual=m->estilos[run->indice_estilo_actual];
}

static bool manejar_tecla_especial(menu_running_t *run, char tecla){
    menu_t *m = run->menu_actual;

    if (m->tipo == MENU_TIPO_RAIZ && tecla == MENU_TECLA_SALIR){
        run->salir = true;
        return true;             // ya manejada
    }

    if (m->tipo == MENU_TIPO_RAIZ && tecla == MENU_TECLA_ESTILOS){
        actualizar_estilo_actual(run);
    }

    if (m->tipo == MENU_TIPO_SUBMENU && tecla == MENU_TECLA_VOLVER){
        if (pila_cantidad(run->stack_menus) != 0){
            run->menu_actual = pila_desapilar(run->stack_menus);
        } else {
            printf("Ya estás en el menú raíz\n");
        }
        return true;             // ya manejada
    }

    return false;                // no era tecla especial
}

opcion_t *menu_buscar_opcion(menu_t *m, char tecla){
    if (!m) return NULL;

    char tecla_s[2] = { tecla, '\0' };
    opcion_t *op=hash_buscar(m->opciones,tecla_s);

    return op;
}

static void limpiar_pantalla(void){
    printf(ANSI_CLEAR_SCREEN ANSI_CURSOR_HOME);
    fflush(stdout);
}

static void esperar_enter(void){
    char aux[8];

    printf("\n\nPresione ENTER para volver al menú...");
    fflush(stdout);

    if (!fgets(aux, sizeof(aux), stdin)) {
        // EOF o error, no hacemos nada especial
    }
}

void manejar_opcion_normal(menu_running_t *run, char tecla){
    menu_t *m = run->menu_actual;
    opcion_t *op = menu_buscar_opcion(m, tecla);

    if (!op){
        printf("Opción inválida\n");
        return;
    }

    if (op->tipo == OPCION_ACCION){
        limpiar_pantalla();
        op->accion(run->user_data);
        esperar_enter();
    } else if (op->tipo == OPCION_SUBMENU){
        if (!pila_apilar(run->stack_menus, m)){
            run->salir=true;
        }else{
            run->menu_actual = op->submenu;
        }
    }
}

static bool mostrar_opcion_cb(char *clave, void *valor, void *_ctx){
    (void)clave;  // no la necesitamos
    estilo_ctx_t *ctx=_ctx;

    opcion_t *op = valor;
    if (!op) return true;  // seguir iterando igual

    ctx->estilo(op->tecla,op->nombre);
    return true;           // true = seguir iterando
}

static void menu_mostrar(menu_running_t *run){
    if (!run || !run->menu_actual) return;

    menu_t *m = run->menu_actual;
    menu_mostrar_t estilo_actual= run->estilo_actual;

    printf("\n=== %s ===\n", m->titulo);

    estilo_ctx_t ctx={.estilo=estilo_actual};
    if (m->opciones){
        hash_iterar(m->opciones, mostrar_opcion_cb, &ctx);
    }

    
    if (m->tipo == MENU_TIPO_RAIZ){
        estilo_actual(MENU_TECLA_ESTILOS,"Cambiar estilo");
        estilo_actual(MENU_TECLA_SALIR,"Salir");
    } else { // MENU_TIPO_SUBMENU
        estilo_actual(MENU_TECLA_VOLVER,"Volver");
    }

    printf("Opción: ");
    fflush(stdout);
}

static void menu_correr(menu_running_t *run){
    char linea[64];

    while (!run->salir){
        limpiar_pantalla();
        menu_mostrar(run);

        // Leer una línea completa del usuario
        if (!fgets(linea, sizeof(linea), stdin)) {
            // EOF o error de entrada: tomamos como "salir"
            run->salir = true;
        } else {
            bool hay_tecla = (linea[0] != '\n' && linea[0] != '\0');

            if (hay_tecla) {
                char tecla = linea[0];
                bool manejada = false;

                // 1) teclas especiales (salir/volver)
                if (!run->salir) {
                    manejada = manejar_tecla_especial(run, tecla);
                }

                // 2) si no fue especial, la tratamos como opción normal
                if (!run->salir && !manejada) {
                    manejar_opcion_normal(run, tecla);
                }
            }
        }
    }
}

/*
void menu_correr(menu_running_t *run){
    while (!run->salir){
        limpiar_pantalla();
        menu_mostrar(run);

        int c =getchar();

        bool hay_tecla = (c != '\n');
        if (hay_tecla){
            char tecla = (char)c;

            bool manejada = false;
            bool seguir_con_opcion = false;

            if (!run->salir){
                manejada = manejar_tecla_especial(run, tecla);
            }
            
            if (!run->salir && !manejada) seguir_con_opcion = true;

            if (seguir_con_opcion) manejar_opcion_normal(run, tecla);            
        }
    }
}
*/
void menu_ejecutar(menu_t *menu_base, void *user_data){
    if (!menu_base) return;
    
    menu_running_t run = {
        .menu_actual = menu_base,
        .estilo_actual=menu_base->estilos[0],
        .indice_estilo_actual=0,
        .cant_estilos=menu_base->cant_estilos,
        .stack_menus = pila_crear(),
        .user_data   = user_data,
        .salir       = false
    };

    if (!run.stack_menus){
        // error creando la pila, no se puede correr el menú
        return;
    }

    menu_correr(&run);

    pila_destruir(run.stack_menus);
}

void destruir_menu_simple(menu_t *menu){
    if (!menu) return;

    if (menu->opciones){
        // Destruye el hash y cada opcion_t
        hash_destruir_todo(menu->opciones, menu_destruir_opcion);
    }

    free(menu->estilos);
    free(menu->titulo);
    free(menu);
}

void menu_destruir(menu_t *menu){
    destruir_menu_simple(menu);
}

static bool destruir_submenus_cb(char *clave, void *valor, void *ctx){
    opcion_t *op = valor;

    // Si esta opción tiene un submenu, destruirlo recursivamente
    if (op->tipo == OPCION_SUBMENU && op->submenu){
        menu_destruir_todo(op->submenu);   // DFS
    }

    return true; // seguir iterando
}

void menu_destruir_todo(menu_t *menu_base){
    if (!menu_base) return;

    // 1. Destruir recursivamente todos los submenús
    if (menu_base->opciones){
        hash_iterar(menu_base->opciones, destruir_submenus_cb, NULL);
    }

    // 2. Destruir este menú (opciones + título + hash + estructura)
    destruir_menu_simple(menu_base);
}