<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP2

## Alumno: Puchetta Federico - 112853 - fpuchetta@fi.uba.ar

---

## Compilacion:

- Para compilar junto a main:

```bash
make tp2
```

- Para compilar junto a pruebas de usuario:

```bash
make pruebas
```

- Para ejecutar:

```bash
./tp2 <archivo_csv>
```

- Para limpiar archivos:

```bash
make clean
```

- Para aplicar format:

```bash
make format
```

---
##  Eleccion de estructuras:

En el enunciado del TP se pedia:

- Implementar un **TDA menú** genérico.
- Implementar un **juego** que utilizara ese menú y los TDAs de la cátedra.

Para realizar el **TDA menu** opte por utilizar las siguientes estructuras definidas previamente en los demas TPS.

1. `hash_t`: Es utilizado para un rapido acceso (casi O(1)) a las opciones que obtendra el usuario. Elegi usar un hash en este campo debido a que el usuario accedera a las opciones muy regularmente, por lo cual cuanto mas rapido se pueda hacer sera mejor su rendimiento.
2. `cola_t`: Es implementado para encolar y poder seguir el orden de los distintos estilos de impresion proporcionados por el usuario al momento de imprimir el menu.
3. `pila_t`: Por ultimo, la pila fue utilizada para poder mantener un orden de ejecucion a la hora de navegar a distintos submenus. De esta forma, si un usuario quiere volver al menu anterior no se necesita crear una nueva instancia con la copia del menu anterior, simplemente se desapila el menu actual y se vuelve al menu anterior.
4. `distintos structs nuevos (estilo_t, opcion_t, etc)`: Explicados en la seccion TDA MENU mas abajo.

Para implementar el juego pedido elegi crear el tda `juego_t` dentro del cual elegi utilizar las siguientes estructuras.

1. `tp1_t`: Es la estructura encargada de manejar y contener toda la informacion de los pokemones a utilizar a lo largo de la utilizacion del menu y del juego.
2. `lista_t`: La lista esta implementada para mantener un orden del historial de jugadas realizadas durante la ejecucion del juego.
3. `distintos structs nuevos (jugador_t, mapa_t, movimiento_t, carta_t, etc)`: Explicados en la seccion TDA JUEGO mas abajo.


Por ultimo, para unificar estos dos tdas, defini una nueva estructura `tp2_t` la cual se encarga de unificarlos y permitir la correcta ejecucion del tp2 sin necesidad de tener que crear el menu o el juego cada vez que se quiera utilizarlo, esto es realizado simplemente llamando a la funcion `tp2_crear`.

---

## TDA Menu:

### Explicacion general

Como solicitaba el enunciado, el **tda menu** fue implementado de la manera mas generica posible para lograr una reutilizacion del mismo en cualquier aspecto que sea necesario, siempre y cuando se cumpla con las condiciones de las primitivas.

Para crear el menu, opte por una estructura `menu_t` la cual contiene los metadatos de un menu, junto a los distintos estilos que se pueden utilizar para imprimir el mismo. A su vez, el campo "opciones" es el encargado de manejar las distintas opciones proporcionadas por el usuario y manejar correctamente el actuar de la misma.

Para asegurar un correcto funcionamiento y una correcta ejecucion del menu, opte por diferenciar un **menu raiz** de un **menu submenu**. Esto fue elegido de esta forma para lograr un correcto manejo de la memoria dinamica y la impresion de datos, ya que los menu de tipo raiz son los unicos encargados de realizar un cambio de aspecto que luego se aplicara para el mismo y el resto de submenus.

Solo puede existir un menu raiz por menu pero distintos menu raiz pueden compartir los mismos submenus, lo cual aporta una flexibilidad para la utilizacion del tda al usuario.

### Decisiones de diseño

A la hora de crear el **TDA menu** tome algunas decisiones de diseño debido a la libertad al respecto en el enunciado.

Primero, opte que las opciones `Q`, `A` y `E` vengan por defecto en cada menu o submenu a crear, es decir, el usuario no debe encargarse de crearlas para salir, volver o cambiar el estilo en caso de estar en el menu raiz. Son proporcionadas por el TDA mismo.

Segundo, en caso de no proporcionar al menu raiz creado un estilo de muestra, el menu se creara correctamente y se le asignara una funcion de muestra basica y suficiente para garantizar la creacion del menu sin problemas.

Tercero, La navegacion de un menu viene proporcionada con el tda `menu_navegador_t`, lo que permite que el usuario maneje libremente la navegacion del menu.

### Explicacion structs implementados

Para manejar de una manera mas amena las distintas funciones del menu, opte por crear distintos structs los cuales algunos son proporcionados al usuario y otros solo son utilizados para la implementacion del codigo.

* El usuario tiene permitido acceder a los struct `menu_accion_t, menu_mostrar_opciones_t, menu_mostrar_titulo_t`

1. `menu_accion_t`: Es proporcionada al usuario como la funcion que le puede asignar a una opcion a la hora de crear un menu, posee el campo `user_data` para que el usuario elija como manejar la informacion adicional que le quiera proporcionar a la accion que el mismo creo.

2. `menu_mostrar_t`: Ambos structs para mostrar son proporcionadas al usuario para que sepa cual es la forma esperada que el usuario debe proporcionar a la hora de querer asignar un estilo al menu, de esta forma se garantiza una mejor utilizacion de las primitivas y un pequenio conocimiento de como se espera que funcione dicha funcion.

* Para la implementacion interna del codigo se crearon los structs `estilo_t, opcion_t, menu_tipo_t, opcion_tipo_t`

1. `estilo_t`: Este struct es el encargado de guardar las funciones de estilo que proporciona el usuario tanto para las opciones como para el titulo del menu. Fue creado para poder insertarlos en la cola de estilos del **tda menu**.
2. `opcion_t`: Este struct es el que sera guardado en las opciones del menu, es decir, el hash del **tda menu**. Contiene los metadatos de una opcion (nombre, tecla de accion, tipo), la accion correspondiente de tipo `menu_accion_t` y un puntero a menu_t el cual sera utilizado en caso de que la opcion creada sea de tipo `OPCION_SUBMENU`, lo cual indica que dicha opcion no realizara una accion sino que avanzara al submenu asociado.
3. `menu_tipo_t opcion_tipo_t`: Enums definidos para diferenciar entre tipos de opciones y tipos de menu. Por un lado, una opcion puede tomar **tipo accion** o **tipo submenu**, lo que diferencia entre si al llamarse a dicha opcion se utilizara el **campo accion** o el **campo submenu**. Por otro lado, un menu puede ser de **tipo submenu** o **tipo raiz**, lo que diferencia entre las caracteristicas de cada uno como por ejemplo la posecion de estilos y si la opcion Q saldra del menu o la opcion A volvera atras.

### Primitivas implementadas

Para garantizar una comoda experiencia al usuario, opte por implementar multiples funciones para la creacion y modificacion de menus.

Se permite:

* La creacion de menus o submenus.

* La implementacion de estilos.

* La creacion de acciones a un menu o submenu.

* Obtecion de datos basicos del menu y cada submenu (cantidades, titulos, teclas)

* Verificacion de existencia de instancias de menu o opciones ya creadas.

* Destruccion de acciones y submenus asociados a un menu raiz y la destruccion total de un menu.

* Navegacion de menus mediante `menu_navegador_t` (creacion, avance, destruccion, imprimir, procesar tecla).


### Navegacion de un menu

Para lograr la navegacion de un menu opte por crear el tda `menu_navegador_t` el cual es el encargado de realizar el manejo de apilacion de menus al momento de avanzar a un submenu o la correcta implementacion de las **opciones fundamentales** (salir, volver, estilos) y proporcionarle al usuario lo ocurrido al momento de realizar dicho avance. De alguna manera, funciona como un "iterador de menus", el cual el usuario elije como utilizar y de que manera realizar el bucle de navegacion.

La representacion de un menu en memoria con distintas instancias de submenu se podria ver de la siguiente forma:

<div align="center">
<img src="img/menu_memoria.svg">
</div>

---

## TDA Juego:

### Explicacion general

El juego creado consta de un **juego de memoria** como pedia el enunciado, el cual maneja correctamente las jugadas realizadas por cada usuario, procesa jugadas y las realiza mediante funcionalidades implementadas.

Opte por crear el TDA `juego_t` el cual contiene la pokedex con los pokemones del csv correspondiente, metadatos de cada usuario y datos del juego en si para un correcto manejo de su utilizacion, como por ejemplo un campo para la semilla y dos booleanos para determinar si el juego es **jugable** (preparado) o si ya **termino** (terminado).

Dentro de dicho TDA se implementaron distintas estructuras creadas para una mejor modularizacion de cada campo del juego.

### Decisiones de diseño

Al momento de implementar el juego, tome algunas decisiones de diseño debido a la libertad al respecto en el enunciado.

Primero, el mapa de pokemones, es decir, los pokemones utilizados para jugar, son representados mediante un vector lineal de 18 lugares. Esto es asi para facilitar la iteracion y seleccion de pokemones al jugar. De esta forma, si el jugador quiere jugar el pokemon 18, entonces simplemente accedo al vector en la pos 17.

Segundo, la randomizacion de posiciones fue hecha mediante el **algoritmo Fisher-Yates Shuffle** el cual garantiza una mezcla de los pokemones de manera uniforme entre las permutaciones.

Tercero, Al momento de descubrir una carta se muestran las primeras 3 letras iniciales del nombre del pokemon. Esto esta hecho de esta forma para garantizar que el mapa se muestre correctamente siempre sin importar el largo del nombre del pokemon de la carta correspondiente.

Cuarto, elegi proporcionar de forma publica un enum con los distintos tipos de estados al realizar una jugada. De esta forma, al realizar una jugada en el juego, se puede verificar que caso de error genero o que caso de exito genero dicha jugada y lograr testear de mejor manera o jugar de mejor manera.

### Explicacion structs implementados

Para manejar de una manera mas comoda las distintas funciones al crear el juego, opte por la creacion de distintas estructuras para la implementacion interna del mismo.

* Para la implementacion interna del codigo se crearon los structs `carta_t, mapa_t, jugador_t, movimiento_t`.

1. `carta_t`: Esta estructura esta pensada para manejar las cartas que se estaran utilizando en el juego. Posee un campo al pokemon asignado a dicha carta, y 3 metadatos para utilizar a la hora de imprimir por pantalla el juego. De esta forma se garantiza una mejor jugabilidad al usuario permitiendo saber por colores si la carta ya esta encontrada y por quien.
2. `mapa_t`: Esta estructura esta pensada para manejar el mapa o cuadrilla del juego, contiene un vector de 18 elementos el cual representa las cartas y la cantidad de filas y columnas correspondientes a dicho mapa. El vector de cartas fue implementado como un vector en vez de matriz para facilitar el acceso a los indices al momento de seleccionar una carta.
3. `jugador_t`: Jugador_t fue pensado para mantener encapsulado en una misma estructura los datos respectivamente de cada jugador. Ademas del puntaje de cada jugador, guarda en si una lista que contiene los movimientos de tipo `movimiento_t` de cada jugada realizada a lo largo del juego por cada jugador. A pesar de tener una lista con la cantidad total de movimientos en el struct `juego_t` opte por tener otra lista extra por cada jugador para lograr la impresion del historial completo de jugadas separadas por cada jugador, como pedia el enunciado.
4. `movimiento_t`: Por ultimo, esta estructura fue pensada para almacenar los datos de cada jugada realizada al momento de almacenarla en los historiales correspondientes. Contiene el indice del jugador que descubrio dicha carta, si fue acierto y los indices de dichas cartas.

### Primitivas implementadas

Para garantizar una comoda experiencia al usuario al momento de jugar, opte por escribir por separado la logica de cada paso de juego, asi el usuario puede elegir la forma de flujo que quiere al momento de utilizar el juego.

Se permite:

* La creacion del juego.

* La preparacion y reinicio del juego en caso de ya terminado.

* Funciones de ayuda para obtener estados de juego, como por ejemplo si posee pokedex o si esta terminado.

* Validacion de jugada a realizar.

* Ejecucion de jugada previamente validada.

* Impresion del mapa de juego y los historiales correspondientes.

* Destruccion total del juego.

### Ejecucion de un juego

Para conseguir una limpia y grata experiencia de juego, opte por separar la logica de los pasos del juego, en distintas funciones proveidas al usuario las cuales, en caso de usarse en un flujo constante y de la forma correcta, garantizan un juego completo y bien ejecutado.

Estas funciones (validar_jugada y realizar_jugada) en un principio, las habia realizado todas dentro de una misma funcion encapsulada llamada `juego_jugar`, pero de esta forma, permito que el usuario sea el encargado de realizar el flujo de codigo o simplemente realizar jugadas al azar y lograr un correcto testeo de jugabilidad.

La representacion de un juego en memoria con distintas jugadas realizadas se podria ver de la siguiente forma:

<div align="center">
<img src="img/juego_memoria.svg">
</div>

---

## Integracion en conjunto:

Finalmente, para lograr realizar el tp2 como se pide, se integran ambas estructuras explicadas previamente en un mismo tda `tp2_t`. En terminos ya conocidos, el menu se ejecuta en un bucle en `tp2_est.c` archivo el cual se encarga de la creacion del menu pedido y el bucle de ejecucion del mismo. Luego, el bucle del juego es creado en `tp2_acciones.c` archivo el cual contiene las acciones pedidas por enunciado para el menu del tp2. Para jugar, el menu llama a la accion `juego_jugar` o `juego_jugar_con_semilla` y pasa como **user_data** el juego guardado en el struct de `tp2_t`. Al momento de terminar de jugar, se termina de ejecutar dicha funcion y se vuelve al bucle de ejecucion del menu. En caso de querer salir, se toca la Q y se libera toda la memoria correspondiente creada hasta ese momento. En caso de error, en el tp2, tanto menu como juego se dejan de ejecutar inmediatamente y se libera la memoria asignada. 

---

## Respuestas a las preguntas teóricas

Explicar los siguientes puntos (no necesariamente en orden, pero por favor usando diagramas):

   - Explicar el TDA menú. Justifique las operaciones incluídas en el TDA.

El TDA menu fue explicado previamente, las operaciones incluidas son:

`menu_t *menu_crear_base(const char *titulo, menu_mostrar_opciones_t estilo_opciones, menu_mostrar_titulo_t estilo_titulo)`:

Esta funcion es implementada para lograr crear el "menu raiz", el cual permite asignarle un estilo de impresion o utilizar uno generico (se asigna solo).

`menu_t *menu_crear_submenu(menu_t *padre, char tecla, const char *nombre)`:

Esta funcion es implementada para poder lograr diferenciar un menu de un submenu, y asignarle a un menu ya creado, sus respectivos submenus. De esta forma, se permite ya diferenciar lo que seria el menu raiz de sus respectivos submenus asociados.

`bool menu_agregar_accion(menu_t *menu, char tecla, const char *nombre, menu_accion_t accion)`:

Esta funcion, al contrario de la anterior, no agrega un submenu, sino que agrega una accion, es decir, necesita una funcion creda por el usuario la cual realizara lo que sea que el usuario quiera que se haga al seleccionar dicha tecla. Es separada de la anterior para poder diferenciar el campo de `opcion_t` "tipo", de esta forma, se logra el correcto funcionamiento de submenus y llamadas a funciones.

`bool menu_agregar_estilo(menu_t* menu, menu_mostrar_opciones_t estilo_opciones, menu_mostrar_titulo_t estilo_titulo)`:

Esta operacion es proporcionada al usuario para poder permitirle agregar estilos al menu, una vez ya este creado, logrando que el menu pueda mantener en si mas de 1 estilo y cambiarlo cuando el usuario lo quiera.

`const char *menu_obtener_titulo(menu_t *menu)`:

Esta funcion es proporcionada para permitirle al usuario conocer el nombre o titulo del menu pasado por parametro, permitiendo que el usuario sepa cual sera el nombre que se mostrara por pantalla al momento de imprimir el menu sin saber su implementacion interna.

`size_t menu_cantidad_x(menu_t *menu) & bool menu_tiene_x(menu_t *menu)`:

Todas las funciones de este tipo, fueron pensadas para que el usuario pueda manejar el menu a su antojo y proporcionar mas libertad y conocimiento a la hora de crearlo. Por ejemplo, si el usuario quiere crear una forma interactiva de crear un menu, puede lograrlo con dichas funciones y consultar los estados de cantidad del menu segun le plazca.

`bool menu_existe_opcion(menu_t *menu, char tecla)`:

Facilita la obtencion y verificacion de opciones ya existentes. Permite al usuario saber si X tecla esta utilizada y lograr un mejor uso de las demas funciones del tda al momento de implementar mas opciones.

`const char *menu_obtener_nombre_opcion(const menu_t *menu, char tecla)`:

Al igual que `menu_obtener_titulo` permite que el usuario conozca el nombre o informacion de la opcion asociada a X tecla sin conocer la implementacion interna del menu.

`bool menu_sacar_accion(menu_t *menu, char tecla) & menu_t *menu_sacar_submenu(menu_t* padre, char tecla)`:

Ambas funciones fueron creadas para darle mas libertad al usuario para el manejo de creaciones o destrucciones de menus. Permite destrruir parcialmente un menu entero, o sacar distintas opciones las cuales ya no son necesarias. Esto ayuda a que el usuario en caso de querer cambiar un menu, no deba crearlo nuevamente o modificar el codigo de creacion, sino que simplemente saca la accion o submenu que quiera.

`void menu_destruir(menu_t *menu) & void menu_destruir_todo(menu_t *menu_base)`:

Ambas funciones estan pensadas para permitir que el usuario decida si quiere borrar todo un menu entero con sus submenus correspondientes o si solo quiere encargarse de liberar la memoria correspondiente a un menu en especifico.

`menu_navegador_t *menu_navegador_crear(menu_t *menu_base, void *user_data)`:

Esta creada para lograr un navegador por menus, el menu proporcionado debe ser de tipo raiz, lo que garantiza el correcto funcionamiento de la letra `Q` para salir del menu. El `user_data` sera la proporcionada por parametro para cada funcion.

`menu_navegacion_estado_t menu_navegador_procesar_tecla(menu_navegador_t *nav, char tecla)`:

Esta funcion es la encargada de procesar que la tecla pasada por parametro corresponda a una opcion del menu actual del navegador y realizar la accion o el cambio de menu correspondiente a dicha tecla.

`void menu_navegador_mostrar(const menu_navegador_t *nav)`:

En caso de que el usuario quiera ver por pantalla el menu en el que se encuentra actualmente, imprime por pantalla la informacion del menu actual del navegador y sus opciones en funcion del estilo seleccionado en el menu raiz.

`menu_t *menu_navegador_obtener_actual(const menu_navegador_t *nav)`:

Permite obtener el menu actual, para que el usuario maneje el menu de la forma que quiera o consulte las caracteristicas de dicho menu.

`bool menu_navegador_esta_terminado(const menu_navegador_t *nav)`:

Esta funcion verifica si el navegador ya se termino de usar, es decir, se salio del menu raiz.

`void menu_navegador_destruir(menu_navegador_t *nav)`:

Para lograr liberar toda la memoria reservada, se crea esta funcion encarga de destruir la pila de ejecucion en caso de tener menus apilados en ella y liberar el resto de memoria reservada.

---

   - Explique qué dificultades encontró al implementar el TP2 y cómo pudo solucionarlas.

Al momento de implementar el TP2, encontre algunas dificultades en los 2 tdas creados.

Por un lado, al crear el **tda menu_t** tuve inconvenientes sobre como lograr la navegacion correcta del menu. En un principio, no sabia si hacer que el struct menu en si maneje la pila de ejecucion, o si hacer que una accion q obtiene un submenu sea la encargada de realizar ese salto a dicho submenu. Esto me generaba una duda al respecto de como liberar la memoria, debido a que multiples submenus estarian interconectados entre si. Por eso cree el `menu_navegador_t` el cual se encarga de manejar correctamente la pila de ejecucion y lograr que el menu se navegue de la forma que el usuario crea correspondiente.

Luego, al momento de testear el tda, no sabia bien como testear una simulacion de navegacion por distintos submenus, debido a que las funciones de navegbacion estaban todas creadas implicitamente dentro de una funcion `menu_ejecutar` la cual se encargaba de pedirle al usuario inputs por teclado, lo que no me permitia simular una navegacion propia. Para solucionarlo, dividi en distintas funciones cada paso para la navegacion de un menu y las proporcione en el .h, lo que permite que el usuario pueda navegar, procesar teclas o destruir la navegacion y lograr una simulacion de navegacion apropiada para testear el funcionamiento en conjunto del tda.

Al principio resultó confuso definir la firma de la función de cada opción y cómo pasarle el contexto correcto (por ejemplo, el juego_t*). Luego, se me ocurrio definir el typedef generico, y dentro de cada funcion el usuario sabra que dato esta proporcionando y lo casteara como sea debido.

Por otro lado, al crear el **tda juego_t** habia muchas opciones de implementacion las cuales no estaban definidas, por lo cual opte por algunas de ellas que me generaron problemas mas adelante y que tuve que adaptar a lo largo de diseñar el tp2.

Una de estos problemas fue la impresion de una jugada del juego, es decir, como le mostraba al usuario que la jugada estaba siendo procesada, para esto implemente las funciones `validar_jugada` y `realizar_jugada`, las cuales si se utilizan en conjunto correctamente, al mostrar el layout de juego luego de cada jugada, se actualiza correspondientemente en caso de que sea una jugada correcta o no.

Relacionado a esto, encontre problemas al respecto de como pedirle al usuario un input de x posicion del mapa, mostrar el cambio que tuvo esa decision y leerlo de forma dinamica. Para esto, implemente las funciones `leer_linea_dinamica` y `esperar_enter`. `leer_linea_dinamica` me permite pedirle al usuario que ingrese X cosa, y en caso de que se sobrepase a lo pensado, se realoca la linea leida en funcion del tamanio del buffer de entrada. Luego, para lograr que el usuario vea que su jugada o ingreso tuvo un efecto en el juego, utilize `esperar_enter` lo cual genera una pausa en la ejecucion del juego y muestra por pantalla el estado actual del juego.

A su vez, tuve problemas con los inputs que el usuario podria llegar a ingresar en casos en los que no son necesarios, como por ejemplo al momento de pedir un enter el usuario podia ingresar un numero X del mapa y la siguiente jugada tomaria ese numero como el proximo a utilizar. Esto fue arreglado limpiando el buffer luego de cada enter solicitado con un bucle infinito y getchar, lo cual garantiza que al finalizar la ejecucion de `esperar_enter` el buffer quede "limpio".

Por ultimo, otro de los problemas que vi al realizar el juego, fue imprimir los movimientos segun pedia el enunciado, debido a que se pedia que los ultimos 5 movimientos fueran impresos de arriba hacia abajo en funcion del orden de llegada, y que luego para cada usuario se imprima sus movimientos totales. Para solucionar esto, opte por simplemente crear 3 historiales distintos. Un historial sera el global el cual mostrara los 5 ultimos movimientos realizados, y luego cada jugador tendra ese mismo movimiento por separado para mostrarse en el resumen final.
