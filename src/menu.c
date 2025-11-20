#include "menu.h"
#include "lista.h"
#include "cola.h"
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
    menu_mostrar_opciones_t estilo_opciones;
    menu_mostrar_titulo_t estilo_titulo;
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
    cola_t* estilos;
    size_t cant_acciones;
    size_t cant_submenus;
};

struct menu_navegador {
    menu_t *menu_actual;
    pila_t *stack_menus;
    estilo_t *estilo_actual;
    void *user_data;
    bool salir;
};

typedef struct estilo_ctx{
    menu_mostrar_opciones_t estilo;
}estilo_ctx_t;

/*
    Pre: El nombre no debe ser NULL.
    
    Post: Imprime la opcion por pantalla.
*/
void mostrar_estilo_opcion_default(char tecla, char *nombre){
    printf ("%c - %s\n",tecla,nombre);
}

/*
    Pre: El titulo no debe ser NULL.
    
    Post: Imprime el titulo por pantalla.
*/
void mostrar_estilo_titulo_default(char *titulo){
    printf("\n=== %s ===\n", titulo);
}

estilo_t *estilo_crear(menu_mostrar_opciones_t eo, menu_mostrar_titulo_t et){
    estilo_t* e=calloc(1,sizeof(estilo_t));
    if (!e) return NULL;
    
    e->estilo_opciones=eo;
    e->estilo_titulo=et;

    return e;
}

bool estilos_inicializar(menu_t *menu, menu_mostrar_opciones_t estilo_opciones, menu_mostrar_titulo_t estilo_titulo){
    if (!menu) return false;

    menu->estilos=cola_crear();
    if (!menu->estilos) return false;

    estilo_t *estilo_inicial=estilo_crear(estilo_opciones,estilo_titulo);
    if (!estilo_inicial){
        cola_destruir(menu->estilos);
        return false;
    }

    if (!cola_encolar(menu->estilos,estilo_inicial)){
        free(estilo_inicial);
        cola_destruir(menu->estilos);
        return false;
    }

    return true;
}

/*
    Pre: El titulo pasado por parametro no debe ser NULL.
    
    Post: Reserva y asigna en memoria los campos
          para un menu_t y lo devuelve.
          Devuelve NULL en caso de error.
*/
menu_t *menu_crear(const char *titulo, menu_tipo_t tipo, menu_mostrar_opciones_t estilo_opciones, menu_mostrar_titulo_t estilo_titulo){
    menu_t* m=calloc(1,sizeof(menu_t));
    if (!m) return NULL;

    m->titulo=mi_strdup(titulo);
    if(!m->titulo){
        free(m);
        return NULL;
    }
    m->tipo=tipo;
    if (tipo == MENU_TIPO_RAIZ){
        menu_mostrar_titulo_t estilo_titulo_a_usar= (estilo_titulo) ? estilo_titulo : mostrar_estilo_titulo_default;
        menu_mostrar_opciones_t estilo_opciones_a_usar= (estilo_opciones) ? estilo_opciones : mostrar_estilo_opcion_default;
        if (!estilos_inicializar(m,estilo_opciones_a_usar,estilo_titulo_a_usar)){
            free(m->titulo);
            free(m);
            return NULL;
        }
    }

    return m;
}

menu_t *menu_crear_base(const char *titulo, menu_mostrar_opciones_t estilo_opciones, menu_mostrar_titulo_t estilo_titulo){
    if (!titulo) return NULL;

    menu_t *m=menu_crear(titulo,MENU_TIPO_RAIZ,estilo_opciones,estilo_titulo);
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

    Post: Devuelve true si la tecla esta reservad para estilos, salir o volver.
          Devuelve false en caso contrario.
*/
bool menu_es_tecla_reservada(char tecla) {
    return tecla == MENU_TECLA_SALIR || 
           tecla == MENU_TECLA_VOLVER || 
           tecla == MENU_TECLA_ESTILOS;
}

/*
    Pre: -

    Post: Devuelve true si existe una opcion con la tecla pasada por parametro.
          Devuelve false en caso contrario.
*/
bool menu_existe_opcion(menu_t *menu, char tecla){
    if (!menu || !menu->opciones) return false;

    char tecla_s[2] = { tecla, '\0' };
    return hash_contiene(menu->opciones, tecla_s);
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

/*
    Pre: -

    Post: Devuelve true si la tecla es distinta de algunos caracteres especiales.
          Devuelve false en caso contrario.
*/
bool es_tecla_usable(char tecla) {
    return (tecla != '\0' && tecla != '\n' && tecla != '\t' && tecla != '\r');
}

menu_t *menu_crear_submenu(menu_t *padre, char tecla, const char *nombre){
    if (!padre || !nombre || !es_tecla_usable(tecla) || strlen(nombre) == 0) return NULL;

    if (menu_es_tecla_reservada(tecla) || menu_existe_opcion(padre, tecla)){
        return NULL;
    }

    menu_t *submenu=menu_crear(nombre,MENU_TIPO_SUBMENU,NULL,NULL);
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

    padre->cant_submenus++;

    return submenu;
}

bool menu_agregar_accion(menu_t *menu, char tecla, const char *nombre, menu_accion_t accion){
    if (!menu || !nombre || !accion || !es_tecla_usable(tecla) || strlen(nombre) == 0){
        return false;
    }

    if (menu_es_tecla_reservada(tecla) || menu_existe_opcion(menu, tecla)){
        return false;
    }

    opcion_t *op=opcion_crear(nombre,tecla,OPCION_ACCION,accion,NULL);
    if (!op) return false;

    if (!menu_agregar_opcion(menu,op)){
        menu_destruir_opcion(op);
        return false;
    }

    menu->cant_acciones++;

    return true;
}

bool menu_agregar_estilo(menu_t* menu, menu_mostrar_opciones_t estilo_opciones, menu_mostrar_titulo_t estilo_titulo){
    if (!menu || !estilo_opciones || !estilo_titulo) return false;
    if (menu->tipo != MENU_TIPO_RAIZ) return false;

    estilo_t* e=estilo_crear(estilo_opciones,estilo_titulo);
    if (!e){
        return false;
    }

    if (!cola_encolar(menu->estilos,e)){
        free(e);
        return false;
    }

    return true;
}

const char *menu_obtener_titulo(menu_t *menu){
    if (!menu) return NULL;

    return menu->titulo;
}

size_t menu_cantidad_acciones(menu_t *menu){
    return (menu) ? menu->cant_acciones : 0;
}

bool menu_tiene_acciones(menu_t *menu){
    if (!menu) return false;

    return (menu_cantidad_acciones(menu) > 0) ? true : false;
}

size_t menu_cantidad_submenus(menu_t *menu){
    return (menu) ? menu->cant_submenus : 0;
}

bool menu_tiene_submenus(menu_t *menu){
    if (!menu) return false;

    return (menu_cantidad_submenus(menu) > 0) ? true : false;
}

size_t menu_cantidad_estilos(menu_t *menu){
    if (!menu || menu->tipo != MENU_TIPO_RAIZ) return 0;

    return cola_cantidad(menu->estilos);
}

bool menu_tiene_estilos(menu_t *menu){
    if (!menu || menu->tipo != MENU_TIPO_RAIZ) return false;

    return (menu_cantidad_estilos(menu) > 1) ? true : false;
}

const char *menu_obtener_nombre_opcion(const menu_t *menu, char tecla){
    if (!menu) return NULL;

    char tecla_s[2]={tecla, '\0'};
    opcion_t *buscado=hash_buscar(menu->opciones,tecla_s);
    return (buscado) ? buscado->nombre : NULL;
}

//////////////////////////////////
//
///*
//    Pre: El parametro "run" no debe ser NULL.
//
//    Post: Actualiza el estilo actual a usar.
//*/
//void actualizar_estilo_actual(menu_running_t *run){
//    menu_t *m=run->menu_actual;
//    
//    if (run->estilo_actual.id == run->cant_estilos-1){
//        run->estilo_actual.id=0;
//    }else{    
//        run->estilo_actual.id++;
//    }
//
//    run->estilo_actual.estilo=m->estilos[run->estilo_actual.id].estilo;
//}
//
///*
//    Pre: El parametro "run" no debe ser NULL.
//
//    Post: Devuelve true si se puso realizar la accion especial.
//          Devuelve false en caso de error.
//*/
//bool manejar_tecla_especial(menu_running_t *run, char tecla){
//    menu_t *m = run->menu_actual;
//
//    if (m->tipo == MENU_TIPO_RAIZ && tecla == MENU_TECLA_SALIR){
//        run->salir = true;
//        return true;             // ya manejada
//    }
//
//    if (m->tipo == MENU_TIPO_RAIZ && tecla == MENU_TECLA_ESTILOS){
//        actualizar_estilo_actual(run);
//    }
//
//    if (m->tipo == MENU_TIPO_SUBMENU && tecla == MENU_TECLA_VOLVER){
//        if (pila_cantidad(run->stack_menus) != 0){
//            run->menu_actual = pila_desapilar(run->stack_menus);
//        }
//        return true;             // ya manejada
//    }
//
//    return false;                // no era tecla especial
//}
//

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

///*
//    Pre: El parametro "run" no debe ser NULL.
//
//    Post: Realiza la opcion relacionada a la tecla pasada por parametro.
//*/
//bool manejar_opcion_normal(menu_running_t *run, char tecla){
//    menu_t *m = run->menu_actual;
//    opcion_t *op = menu_buscar_opcion(m, tecla);
//
//    if (!op) return true;
//
//    if (op->tipo == OPCION_ACCION){
//        limpiar_pantalla();
//        bool resultado = op->accion(run->user_data);
//        esperar_enter();
//        return resultado;
//    } else if (op->tipo == OPCION_SUBMENU){
//        if (!pila_apilar(run->stack_menus, m)){
//            run->salir=true;
//            return false;
//        }else{
//            run->menu_actual = op->submenu;
//        }
//    }
//    
//    return true;
//}

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

///*
//    Pre: -
//
//    Post: Muestra el menu del campo "menu_actual" del parametro.
//*/
//void menu_mostrar(menu_running_t *run){
//    if (!run || !run->menu_actual) return;
//
//    menu_t *m = run->menu_actual;
//    menu_mostrar_t estilo_actual= run->estilo_actual.estilo;
//
//    printf("\n=== %s ===\n", m->titulo);
//
//    estilo_ctx_t ctx={.estilo=estilo_actual};
//    if (m->opciones){
//        hash_iterar(m->opciones, mostrar_opcion_cb, &ctx);
//    }
//
//    
//    if (m->tipo == MENU_TIPO_RAIZ){
//        estilo_actual(MENU_TECLA_ESTILOS,"Cambiar estilo");
//        estilo_actual(MENU_TECLA_SALIR,"Salir");
//    } else { // MENU_TIPO_SUBMENU
//        estilo_actual(MENU_TECLA_VOLVER,"Volver");
//    }
//
//    printf("Opción: ");
//    fflush(stdout);
//}
//
///*
//    Pre: El parametro "run" no debe ser NULL.
//
//    Post: Ejecuta el menu por tiempo indefinido
//          hasta que el usuario decida salir.
//*/
//bool menu_correr(menu_running_t *run){
//    while (!run->salir) {
//
//        limpiar_pantalla();
//        menu_mostrar(run);
//
//        int c = getchar();
//        limpiar_buffer();
//
//        if (c == EOF) {
//            run->salir = true;
//            return true;
//        }
//
//        char tecla = (char)c;
//        
//        bool manejada = false;
//        if (!run->salir)
//            manejada = manejar_tecla_especial(run, tecla);
//
//        if (!run->salir && !manejada)
//            if (!manejar_opcion_normal(run, tecla)){
//                return false;
//            }
//    }
//
//    return true;
//}
//
//bool menu_ejecutar(menu_t *menu_base, void *user_data){
//    if (!menu_base) return false;
//    
//    menu_running_t run = {
//        .menu_actual = menu_base,
//        .estilo_actual={.estilo=menu_base->estilos[0].estilo, .id=menu_base->estilos[0].id},
//        .cant_estilos=menu_base->cant_estilos,
//        .stack_menus = pila_crear(),
//        .user_data   = user_data,
//        .salir       = false
//    };
//
//    if (!run.stack_menus){
//        return false;
//    }
//
//    if (!menu_correr(&run)){
//        pila_destruir(run.stack_menus);
//        return false;
//    }
//
//    pila_destruir(run.stack_menus);
//    
//    return true;
//}
//
////////////////////////////////////
//
bool menu_sacar_accion(menu_t *menu, char tecla) {
    if (!menu || !menu->opciones) return false;

    char tecla_s[2] = {tecla, '\0'};
    opcion_t *opcion = hash_buscar(menu->opciones, tecla_s);
    if (!opcion || opcion->tipo != OPCION_ACCION) {
        return false;
    }

    opcion_t *opcion_quitada = hash_quitar(menu->opciones, tecla_s);
    if (!opcion_quitada) return false;

    menu_destruir_opcion(opcion_quitada);
    
    menu->cant_acciones--;

    return true;
}

menu_t *menu_sacar_submenu(menu_t *padre, char tecla) {
    if (!padre || !padre->opciones) return NULL;

    char tecla_s[2] = {tecla, '\0'};
    opcion_t *opcion = hash_buscar(padre->opciones, tecla_s);
    if (!opcion || opcion->tipo != OPCION_SUBMENU) {
        return NULL;
    }

    opcion_t *opcion_quitada = hash_quitar(padre->opciones, tecla_s);
    if (!opcion_quitada) return NULL;

    menu_t *submenu = opcion_quitada->submenu;

    menu_destruir_opcion(opcion_quitada);

    padre->cant_submenus--;
    
    return submenu;  // Usuario decide si destruirlo o reusarlo
}

// Creación idéntica a tu inicialización
menu_navegador_t *menu_navegador_crear(menu_t *menu_base, void *user_data) {
    if (!menu_base) return NULL;
    
    menu_navegador_t *nav = calloc(1, sizeof(menu_navegador_t));
    if (!nav) return NULL;
    
    estilo_t *e_inicial=cola_ver_primero(menu_base->estilos);
    nav->menu_actual = menu_base;
    nav->estilo_actual = e_inicial;
    nav->stack_menus = pila_crear();
    nav->user_data = user_data;
    nav->salir = false;
    
    if (!nav->stack_menus) {
        free(nav);
        return NULL;
    }
    
    return nav;
}

// Réplica de tu actualizar_estilo_actual
bool menu_navegador_actualizar_estilo(menu_navegador_t *nav) {
    menu_t *m = nav->menu_actual;
    
    estilo_t *e_final=cola_desencolar(m->estilos);
    nav->estilo_actual=cola_ver_primero(m->estilos);

    if (!cola_encolar(m->estilos,e_final)){
        return false;
    }

    return true;
}

bool menu_navegador_manejar_tecla_especial(menu_navegador_t *nav, char tecla) {
    menu_t *m = nav->menu_actual;

    if (m->tipo == MENU_TIPO_RAIZ && tecla == MENU_TECLA_SALIR) {
        nav->salir = true;
        return MENU_NAVEGACION_CONTINUAR;
    }

    if (m->tipo == MENU_TIPO_RAIZ && tecla == MENU_TECLA_ESTILOS) {
        if (!menu_navegador_actualizar_estilo(nav)) {
            nav->salir = true;
            return MENU_NAVEGACION_ERROR;
        }
        return MENU_NAVEGACION_CONTINUAR;
    }

    if (m->tipo == MENU_TIPO_SUBMENU && tecla == MENU_TECLA_VOLVER) {
        if (pila_cantidad(nav->stack_menus) != 0) {
            nav->menu_actual = pila_desapilar(nav->stack_menus);
        }
        return MENU_NAVEGACION_CONTINUAR;
    }

    return MENU_NAVEGACION_CONTINUAR;
}

menu_navegacion_estado_t menu_navegador_manejar_opcion_normal(menu_navegador_t *nav, char tecla) {
    menu_t *m = nav->menu_actual;
    opcion_t *op = menu_buscar_opcion(m, tecla);

    if (!op) return MENU_NAVEGACION_CONTINUAR;

    if (op->tipo == OPCION_ACCION) {
        bool resultado = op->accion(nav->user_data);
        return resultado ? MENU_NAVEGACION_CONTINUAR : MENU_NAVEGACION_ERROR;
    } else if (op->tipo == OPCION_SUBMENU) {
        if (!pila_apilar(nav->stack_menus, m)) {
            nav->salir = true;
            return MENU_NAVEGACION_ERROR;
        } else {
            nav->menu_actual = op->submenu;
        }
    }
    
    return MENU_NAVEGACION_CONTINUAR;
}

// Función principal - procesa tecla como en tu código
menu_navegacion_estado_t menu_navegador_procesar_tecla(menu_navegador_t *nav, char tecla) {
    if (!nav) return MENU_NAVEGACION_ERROR;
    if (nav->salir) return MENU_NAVEGACION_TERMINAR;
    
    menu_navegacion_estado_t resultado_especial = menu_navegador_manejar_tecla_especial(nav, tecla);
    if (resultado_especial == MENU_NAVEGACION_ERROR) {
        return MENU_NAVEGACION_ERROR;
    }
    
    if (!nav->salir && resultado_especial == MENU_NAVEGACION_CONTINUAR) {
        menu_navegacion_estado_t resultado_normal = menu_navegador_manejar_opcion_normal(nav, tecla);
        if (resultado_normal == MENU_NAVEGACION_ERROR) {
            return MENU_NAVEGACION_ERROR;
        }
    }
    
    return nav->salir ? MENU_NAVEGACION_TERMINAR : MENU_NAVEGACION_CONTINUAR;
}

// Réplica de tu menu_mostrar
void menu_navegador_mostrar(const menu_navegador_t *nav) {
    if (!nav || !nav->menu_actual) return;

    menu_t *m = nav->menu_actual;
    estilo_t *estilo_actual = nav->estilo_actual;

    estilo_actual->estilo_titulo(m->titulo);

    estilo_ctx_t ctx = {.estilo = estilo_actual->estilo_opciones};
    if (m->opciones) {
        hash_iterar(m->opciones, mostrar_opcion_cb, &ctx);
    }

    if (m->tipo == MENU_TIPO_RAIZ) {
        estilo_actual->estilo_opciones(MENU_TECLA_ESTILOS, "Cambiar estilo");
        estilo_actual->estilo_opciones(MENU_TECLA_SALIR, "Salir");
    } else {
        estilo_actual->estilo_opciones(MENU_TECLA_VOLVER, "Volver");
    }

    printf("Opción: ");
    fflush(stdout);
}

menu_t *menu_navegador_obtener_actual(const menu_navegador_t *nav) {
    return nav ? nav->menu_actual : NULL;
}

bool menu_navegador_esta_terminado(const menu_navegador_t *nav) {
    return nav ? nav->salir : true;
}

void menu_navegador_destruir(menu_navegador_t *nav) {
    if (nav) {
        pila_destruir(nav->stack_menus);
        free(nav);
    }
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

    while (cola_cantidad(menu->estilos) != 0) {
        estilo_t *e = cola_desencolar(menu->estilos);
        free(e);
    }
    cola_destruir(menu->estilos);
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

    if (op->tipo == OPCION_SUBMENU && op->submenu){
        menu_destruir_todo(op->submenu);
    }

    return true;
}

void menu_destruir_todo(menu_t *menu_base){
    if (!menu_base) return;

    if (menu_base->opciones){
        hash_iterar(menu_base->opciones, destruir_submenus_cb, NULL);
    }

    destruir_menu_simple(menu_base);
}

///////////////////////////////////


