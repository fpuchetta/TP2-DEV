#include "menu.h"
#include "lista.h"
#include "pila.h"
#include "hash.h"
#include "functions.h"
#include "ansi.h"
#include "ctype.h"

#include <stdio.h>
typedef enum {
    MENU_TIPO_RAIZ,
    MENU_TIPO_SUBMENU
} menu_tipo_t;

typedef enum {
    OPCION_ACCION,
    OPCION_SUBMENU
} opcion_tipo_t;

typedef struct estilo{
    size_t id;
    menu_mostrar_t estilo;
}estilo_t;

typedef struct opcion {
    char tecla;
    char *nombre;
    opcion_tipo_t tipo;
    menu_accion_t accion;
    struct menu *submenu;
} opcion_t;

struct menu {
    char *titulo;
    hash_t *opciones;
    menu_tipo_t tipo;
    estilo_t* estilos;
    size_t cant_estilos;
};

// Contexto de ejecución (oculto al usuario)
typedef struct {
    menu_t *menu_actual;
    pila_t *stack_menus;
    estilo_t estilo_actual;
    size_t cant_estilos;
    void *user_data;
    bool salir;
} menu_running_t;

typedef struct estilo_ctx{
    menu_mostrar_t estilo;
}estilo_ctx_t;

/*
    Pre: El nombre no debe ser NULL.
    
    Post: Imprime la opcion por pantalla.
*/
void mostrar_estilo_default(char tecla, char *nombre){
    printf ("%c - %s\n",tecla,nombre);
}

/*
    Pre: El titulo pasado por parametro no debe ser NULL.
    
    Post: Reserva y asigna en memoria los campos
          para un menu_t y lo devuelve.
          Devuelve NULL en caso de error.
*/
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
        m->estilos=calloc(1,sizeof(estilo_t));
        if (!m->estilos){
            free(m->titulo);
            free(m);
        }
        m->estilos[0].estilo=estilo ? estilo : mostrar_estilo_default;
        m->estilos[0].id=0;
        m->cant_estilos++;
    }

    return m;
}

menu_t *menu_crear_base(const char *titulo, menu_mostrar_t estilo){
    if (!titulo) return NULL;

    menu_t *m=menu_crear(titulo,MENU_TIPO_RAIZ,estilo);
    return m;
}

/*
    Pre: La opcion pasada por parametro no debe ser NULL.

    Post: Libera la memoria reservada para una opcion.
*/
void menu_destruir_opcion(void *_op){
    opcion_t *op=_op;

    free(op->nombre);
    free(op);
}

/*
    Pre: -

    Post: Libera toda la memoria reservada
          para un menu_t.
*/
void menu_destruir_submenu(menu_t *menu){
    if (!menu || menu->tipo != MENU_TIPO_SUBMENU){
        return;
    }

    if (menu->opciones)
        hash_destruir_todo(menu->opciones, menu_destruir_opcion);

    free(menu->titulo);
    free(menu);
}

/*
    Pre: -

    Post: Agrega una opcion al menu pasado por parametro.
          Devuelve true en caso de funcionar correctamente
          Devuelve false en caso de error.
*/
bool menu_agregar_opcion(menu_t *menu,opcion_t *op){
    if (!menu || !op) return false;
    char tecla[2]={op->tecla,'\0'};

    if (!menu->opciones){
        menu->opciones=hash_crear(5);
        if (!menu->opciones) return false;
    }

    return hash_insertar(menu->opciones,tecla,op,NULL);
}

/*
    Pre: -

    Post: Devuelve true si existe una opcion con la tecla pasada por parametro.
          Devuelve false en caso contrario.
*/
bool menu_existe_opcion(menu_t *m, char tecla){
    if (!m || !m->opciones || tecla == MENU_TECLA_SALIR || tecla == MENU_TECLA_VOLVER) return false;

    char tecla_s[2] = { tecla, '\0' };
    return hash_contiene(m->opciones, tecla_s);
}

/*
    Pre: -

    Post: Devuelve un puntero a opcion_t reservado en memoria con los campos asignados a los parametros correspondientes.
          Devuelve NULL en caso de error.
*/
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

    estilo_t *estilos = realloc(menu->estilos,
                                      nueva_cant * sizeof(estilo_t));
    if (!estilos) return false;

    menu->estilos = estilos;
    menu->estilos[menu->cant_estilos].estilo = estilo;
    menu->estilos[menu->cant_estilos].id=menu->cant_estilos;
    menu->cant_estilos = nueva_cant;

    return true;
}

/*
    Pre: El parametro "run" no debe ser NULL.

    Post: Actualiza el estilo actual a usar.
*/
void actualizar_estilo_actual(menu_running_t *run){
    menu_t *m=run->menu_actual;
    
    if (run->estilo_actual.id == run->cant_estilos-1){
        run->estilo_actual.id=0;
    }else{    
        run->estilo_actual.id++;
    }

    run->estilo_actual.estilo=m->estilos[run->estilo_actual.id].estilo;
}

/*
    Pre: El parametro "run" no debe ser NULL.

    Post: Devuelve true si se puso realizar la accion especial.
          Devuelve false en caso de error.
*/
bool manejar_tecla_especial(menu_running_t *run, char tecla){
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

/*
    Pre: -

    Post: Devuelve un puntero a la opcion correspondiente a la tecla pasada por parametro.
          Devuelve NULL en caso de error. 
*/
opcion_t *menu_buscar_opcion(menu_t *m, char tecla){
    if (!m) return NULL;

    char tecla_s[2] = { tecla, '\0' };
    opcion_t *op=hash_buscar(m->opciones,tecla_s);

    return op;
}

/*
    Pre: El parametro "run" no debe ser NULL.

    Post: Realiza la opcion relacionada a la tecla pasada por parametro.
*/
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

/*
    Pre: El ctx pasado por parametro no debe ser NULL.

    Post: Devuelve true si se pudo imprimir la opcion.
*/
bool mostrar_opcion_cb(char *clave, void *valor, void *_ctx){
    (void)clave;
    estilo_ctx_t *ctx=_ctx;

    opcion_t *op = valor;
    if (!op) return true;

    ctx->estilo(op->tecla,op->nombre);
    return true;
}

/*
    Pre: -

    Post: Muestra el menu del campo "menu_actual" del parametro.
*/
void menu_mostrar(menu_running_t *run){
    if (!run || !run->menu_actual) return;

    menu_t *m = run->menu_actual;
    menu_mostrar_t estilo_actual= run->estilo_actual.estilo;

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

/*
    Pre: El parametro "run" no debe ser NULL.

    Post: Ejecuta el menu por tiempo indefinido
          hasta que el usuario decida salir.
*/
void menu_correr(menu_running_t *run){
    while (!run->salir) {

        limpiar_pantalla();
        menu_mostrar(run);

        int c = getchar();
        limpiar_buffer();

        if (c == EOF) {
            run->salir = true;
            return;
        }

        char tecla = (char)c;
        
        bool manejada = false;
        if (!run->salir)
            manejada = manejar_tecla_especial(run, tecla);

        if (!run->salir && !manejada)
            manejar_opcion_normal(run, tecla);
    }
}

bool menu_ejecutar(menu_t *menu_base, void *user_data){
    if (!menu_base) return false;
    
    menu_running_t run = {
        .menu_actual = menu_base,
        .estilo_actual={.estilo=menu_base->estilos[0].estilo, .id=menu_base->estilos[0].id},
        .cant_estilos=menu_base->cant_estilos,
        .stack_menus = pila_crear(),
        .user_data   = user_data,
        .salir       = false
    };

    if (!run.stack_menus){
        // error creando la pila, no se puede correr el menú
        return false;
    }

    menu_correr(&run);

    pila_destruir(run.stack_menus);
    
    return true;
}

/*
    Pre: -

    Post: Libera toda la memoria reservada para un menu de tipo raiz.
*/
void destruir_menu_simple(menu_t *menu){
    if (!menu) return;

    if (menu->opciones){
        hash_destruir_todo(menu->opciones, menu_destruir_opcion);
    }

    free(menu->estilos);
    free(menu->titulo);
    free(menu);
}

void menu_destruir(menu_t *menu){
    destruir_menu_simple(menu);
}

/*
    Pre: El valor pasado por parametro no debe ser NULL.

    Post: Libera recursivamente la memoria de un menu y sus submenus
*/
bool destruir_submenus_cb(char *clave, void *valor, void *ctx){
    opcion_t *op = valor;

    // Si esta opción tiene un submenu, destruirlo recursivamente
    if (op->tipo == OPCION_SUBMENU && op->submenu){
        menu_destruir_todo(op->submenu);   // DFS
    }

    return true; // seguir iterando
}

void menu_destruir_todo(menu_t *menu_base){
    if (!menu_base) return;

    if (menu_base->opciones){
        hash_iterar(menu_base->opciones, destruir_submenus_cb, NULL);
    }

    destruir_menu_simple(menu_base);
}